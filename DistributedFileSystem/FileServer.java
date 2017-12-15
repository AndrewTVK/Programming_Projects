/**
 * FileServer.java
 * Authors: Andrew VanKooten, Stephen Rawson
 * Due Date: June 8, 2016
 * 
 */

import java.io.*;
import java.rmi.*;
import java.rmi.server.*;
import java.util.*;

public class FileServer extends UnicastRemoteObject implements ServerInterface {
    private ArrayList<CacheEntry> Cache;
    private String Port;

    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("usage: java FileServer <port>");
            System.exit(-1);
        }

        try {
            FileServer fileServer = new FileServer(args[0]);
            Naming.rebind("rmi://localhost:" + args[0] + "/fileserver", fileServer);
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(-1);
        }
    }

    public FileServer(String port) throws RemoteException {
        Port = port;
        Cache = new ArrayList<>();
    }

    public FileContents download(String client, String filename, String mode) throws RemoteException {
        CacheEntry file = null;

        synchronized ( Cache ) {
            for ( CacheEntry entry : Cache ) {
                if ( filename.equals( entry.getName() ) ) {
                    file = entry;
                    break;
                }
            }
            if ( file == null ) {
                file = new CacheEntry( filename, Port );
                Cache.add( file );
            }
        }

        return file.registerClient( client, mode );
    }

    public boolean upload(String ClientIP, String Filename, FileContents Contents) throws RemoteException {
        CacheEntry File = null;
        synchronized ( Cache ) {
            for ( CacheEntry entry : Cache ) {
                if ( Filename.equals( entry.getName() ) ) {
                    File = entry;
                    break;
                }
            }
        }
        if ( File == null )
            return false;
        return File.replaceFile( ClientIP, Contents );
    }
}

class CacheEntry {
    private String Filename;
    private ArrayList<String> ReadClients;
    private String Owner;
    private String Port;

    private State CurrentState;
    private enum State { Not_Shared, Read_Shared, Write_Shared, Ownership_Change }

    private FileContents File;

    CacheEntry( String filename, String port ) {
        Filename = filename;
        Port = port;
        ReadClients = new ArrayList<>();
        Owner = null;
        CurrentState = State.Not_Shared;
        File = readFile();
    }

    private FileContents readFile() {
        byte[] data;
        try {
            FileInputStream inFile = new FileInputStream( Filename );
            data = new byte[ inFile.available() ];
            inFile.read( data );
            inFile.close();
        } catch ( IOException e ) {
            e.printStackTrace();
            return null;
        }
        return new FileContents( data );
    }

    private boolean writeFile() {
        byte[] data = File.get();
        try {
            FileOutputStream outFile = new FileOutputStream( Filename );
            outFile.write( data );
            outFile.flush();
            outFile.close();
        } catch ( IOException e ) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    public synchronized FileContents registerClient( String ClientIP, String mode ) {
        if ( !( mode.equals( "r" ) || mode.equals( "w" ) || !ClientIP.equals( "" ) ) )
            return null;

        System.out.println( "Download " + Filename + ": Download Started" );

        State prevState = CurrentState;

        //<editor-fold desc="Ownership_Change">
        while ( CurrentState == State.Ownership_Change ) {
            try {
                System.out.println( ClientIP + " Waiting on " + Filename + " to Change Ownership" );
                this.wait();
                System.out.println( "Finished waiting" );
            } catch ( InterruptedException e ) {
                e.printStackTrace();
            }
        }
        //</editor-fold>

        switch ( CurrentState ) {
            //<editor-fold desc="Not_Shared">
            case Not_Shared:
                switch ( mode ) {
                    case "r":
                        CurrentState = State.Read_Shared;
                        ReadClients.add( ClientIP );
                        break;
                    case "w":
                        CurrentState = State.Write_Shared;
                        if ( Owner == null )
                            Owner = ClientIP;
                        else
                            return null;
                        break;
                }
                break;
            //</editor-fold>

            //<editor-fold desc="Read_Shared">
            case Read_Shared:

                switch ( mode ) {
                    case "r":
                        if ( !ReadClients.contains( ClientIP ) )
                            ReadClients.add( ClientIP );
                        break;
                    case "w":
                        CurrentState = State.Write_Shared;
                        if ( Owner == null )
                            Owner = ClientIP;
                        else
                            return null;
                        break;
                }
                break;
            //</editor-fold>

            //<editor-fold desc="Write_Shared">
            case Write_Shared:

                switch ( mode ) {
                    case "r":
                        ReadClients.add( ClientIP );
                        break;
                    case "w":
                        CurrentState = State.Ownership_Change;

                        // Create a ClientInterface object for the current owner of the file
                        ClientInterface Client;
                        try {
                            Client = (ClientInterface)Naming.lookup( "rmi://" + Owner + ":" + Port + "/fileclient" );
                        } catch ( Exception e ) {
                            e.printStackTrace();
                            return null;
                        }

                        // Call the writeback function for the current owner
                        if ( Client != null ) {
                            try {
                                Client.writeback();
                            } catch ( RemoteException e ) {
                                e.printStackTrace();
                                return null;
                            }
                        }
                        else
                            return null;

                        // Suspend the download function until the owner client calls the upload function
                        try {
                            System.out.println( "Download " + Filename + ": Waiting For Write Permissions From " + Owner );
                            this.wait();
                            System.out.println( "Download " + Filename + ": " + ClientIP + " now resuming" );
                        } catch ( InterruptedException e ) {
                            e.printStackTrace();
                            return null;
                        }

                        Owner = ClientIP;
                        break;
                }
                break;
            //</editor-fold>
        }

        System.out.println( "Download " + Filename + ": State Change: " + prevState + " --> " + CurrentState );
        System.out.println( "Download " + Filename + ": Readers:" );
        for ( String reader : ReadClients ) {
            System.out.println( "\t" + reader );
        }
        System.out.println( "Download " + Filename + ": Owner: " + Owner );

        if ( prevState == State.Write_Shared ) {
            System.out.println( "Download " + Filename + ": Notifying Threads Waiting on Ownership Change" );
            this.notifyAll();
        }
        return File;
    }

    public synchronized boolean replaceFile( String ClientIP, FileContents Contents ) {
        if ( CurrentState == State.Not_Shared || CurrentState == State.Read_Shared )
            return false;
	
        State prevState = CurrentState;

        // Invalidate the copy of this file cached in each of the read clients
        System.out.println( "Upload " + Filename + ": Invalidating Readers" );
        boolean error = false;
        for ( String Reader : ReadClients ) {
            // Create a ClientInterface object for each client in ReadClients
            ClientInterface Client = null;
            try {
                Client = ( ClientInterface ) Naming.lookup( "rmi://" + Reader + ":" + Port + "/fileclient" );
            } catch ( Exception e ) {
                e.printStackTrace();
                error = true;
            }

            // Call that client's invalidate function
            if ( Client != null ) {
                try {
                    Client.invalidate();
                } catch ( RemoteException e ) {
                    e.printStackTrace();
                    error = true;
                }
            }
        }

        ReadClients.clear();
        File = Contents;                                // Update the entry data with Contents
        switch ( CurrentState ) {
            case Write_Shared:
                CurrentState = State.Not_Shared;
                Owner = null;
                if ( !writeFile() )
                    error = true;
                break;
            case Ownership_Change:
                CurrentState = State.Write_Shared;
                Owner = ClientIP;
                System.out.println( "Upload " + Filename + ": Resuming one of the suspended download calls" );
                this.notify();                               // Resume one of the download() calls that has been suspended
                break;
        }

        System.out.println( "Upload " + Filename + ": State Change: " + prevState + " --> " + CurrentState );

        return !error;
    }

    public String getName() {
        return Filename;
    }
}
