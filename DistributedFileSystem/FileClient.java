/**
 * FileClient.java
 * Authors: Andrew VanKooten, Stephen Rawson
 * Due Date: June 8, 2016
 * 
 */

 import java.net.*;
 import java.net.UnknownHostException;
 import java.rmi.*;
 import java.io.*;
 import java.rmi.server.*;

 public class FileClient extends UnicastRemoteObject implements ClientInterface
 {
     private ServerInterface server = null;
     private BufferedReader reader = null;
     private CacheFile file = null;
     private String clientName = "youraccount";  // cached ~ file /tmp/youraccount.txt 
     private String clientIP = "";               
     private static final int invalid = 0;
     private static final int readShare = 1;
     private static final int writeOwned = 2;
     private static final int releaseOwnership = 3;

     public FileClient (String ip, int port) throws RemoteException
     {
         try {
             InetAddress inetAddress = InetAddress.getLocalHost();
             clientIP = inetAddress.getHostName();
             server = (ServerInterface) Naming.lookup("rmi://" + ip + ":" + port + "/fileserver");
             reader = new BufferedReader(new InputStreamReader(System.in));
             file = new CacheFile();
         } catch (NotBoundException nbe) {
             nbe.printStackTrace();
         } catch (MalformedURLException murle) {
             murle.printStackTrace();
         } catch (UnknownHostException uhe) {
             uhe.printStackTrace();
         }
     }

     public static void main(String[] args)
     {
         if (args.length != 2)
         {
             System.err.println("useage java FileClient serverIP port#" );
             System.exit(-1);
         }
         try {
             String serverIp = args[0];
             int port = Integer.parseInt(args[1]);
             FileClient fileClient = new FileClient(serverIp, port);
             Naming.rebind("rmi://localhost:" + port + "/fileclient", fileClient);
             fileClient.session();       // start Session

         } catch (Exception e) {
             e.printStackTrace();
             System.exit(-1);
         }
     }

     // OVERIDE INTERFACE FUNCTIONS
     public boolean invalidate() throws RemoteException
     {
         return file.invalidate();
     }
     // OVERRIDE INTERFACE FUNCTIONS
     public boolean writeback() throws RemoteException
     {
         return file.writeback();
     }

     // session is infinite loop that request's the account name, a single
     // cache file is kept in "memory at one time"
     public void session()
     {
       clientName = getPromptAccountName();
       while(true)
       {
           CacheThread cacheThread = new CacheThread();
           cacheThread.start();
           String fileName = getPromptFileName();
           if (fileName.equals("exit"))
           {
               if(file.getState() == writeOwned);
               {
                   file.upload();
               }
               System.out.println("Have a nice day!");
               System.exit(0);
           }
           else if (fileName.equals(""))
           {
               cacheThread.endThread();
               System.err.println("ERROR: FILE NAME REQUIRED");
               continue;
           }
           String mode = getPromptReadWrite();
           if (!mode.equals("r") && !mode.equals("w"))
           {
               cacheThread.endThread();
               System.err.println("ERROR: READ = r && WRITE = w");
               continue;
           }
           cacheThread.endThread();    // No more System.in
           if (!cacheFileStatus(fileName, mode))
           {
             if(file.getState() == writeOwned)
             {
               file.upload();
             }
             file.download(fileName, mode);
           }
           file.launchEditor(mode);
       }
     }
     
     // checks if the file is currently in the cache and its current state
     public synchronized boolean cacheFileStatus(String dName, String dMode)
     {
       System.out.println("Starting cachFileStatus.");
       if (!file.getFileName().equals(dName))
       {
         System.out.println("File: " + dName + " does not exist");
         return false;
       }
       if (file.getState() == releaseOwnership)
       {
         System.out.println("File: " + dName + " must release ownership & writeback");
         return false;
       }
       if (dMode.equals("w") && file.getState() == writeOwned)
       {
         System.out.println("File: " + dName + " is currently owned and available to be written");
         return true;
       }
       else if (dMode.equals("r") && file.getState() != invalid)
       {
         System.out.println("File: " + dName + " is avaialbe to be read");
         return true;
       }
       else
       {
         System.out.println("File: " + dName + " must be accessed with " + dMode);
         return false;
       }
     }
     
     // prompt user for file name
     private String getPromptFileName()
     {
         String result = null;
         try {
             System.out.println("FileClient: Next file to open (Option: Type 'exit' to quit)");
             System.out.println("File name: ");
             result = reader.readLine();
         } catch (IOException ioe) {
             System.err.println("ERROR: GET PROMPT FILE NAME");
             ioe.printStackTrace();
         }
         return result;
     }
     
     // prompt user for read or write
     private String getPromptReadWrite()
     {
         String result = null;
         try {
             System.out.println("How(r/w): ");
             result = reader.readLine();
         } catch (IOException ioe) {
             System.err.println("ERROR: GET PROMPT READ WRITE");
             ioe.printStackTrace();
         }
         return result;
     }

     // prompt user account name
     private String getPromptAccountName()
     {
         String result = null;
         try {
             System.out.println("Account name: ");
             result = reader.readLine();
         } catch (IOException ioe) {
             System.err.println("ERROR: GET PROMPT ACCOUNT NAME");
             ioe.printStackTrace();
         }
         return result;
     }
     
     // Maintains the cache file data, name, and state.
     private class CacheFile
     {
       private String name = "";
       private int state = 0;
       private byte[] data = null;

       public CacheFile()
       {
         state = invalid;
       }
       
       // executes Unix Commands based on input
       public boolean execCommand(String[] commandArr)
       {
         try {
           Runtime runtime = Runtime.getRuntime();
           Process process = runtime.exec(commandArr);
           process.waitFor();
         }
         catch (IOException ioe) {
           System.err.println("ERROR: execCommand => " + commandArr[0]);
           ioe.printStackTrace();
           return false;
         }
         catch (InterruptedException ie) {
           System.err.println("ERROR: execCommand IException");
           ie.printStackTrace();
           return false;
         }
         return true;
       }
       
       // sets permission to "/tmp/youraccount.txt" if read = '400' and write = '600'
       public boolean launchEditor(String dMode)
       {
         System.out.println("Launching editor");
         String[] command = { "chmod", "600", "/tmp/" + clientName + ".txt" }; 
         execCommand(command);
         if (!writeToFile())
         {
           System.out.println("Launch Editor: Write to file successful");
           return false;
         }
         String chmodValue = "";
         if (dMode.equals("r"))
         {
           chmodValue = "400";
         }
         else if (dMode.equals("w"))
         {
           chmodValue = "600";
         }
         String[] chmodCommandArr = {"chmod", chmodValue, "/tmp/avk1994.txt"};
         if (!execCommand(chmodCommandArr))
         {
           System.out.println("ERROR: luanchEditor unable to change permission " + dMode);
           return false;
         }
         String[] commandArr = {"emacs", "/tmp/" + clientName + ".txt"};
         if (!execCommand(commandArr))
         {
           System.out.println("ERROR: launchEditor unable to launch editor!");
           return false;
         }
         if (dMode.equals("w"))
         {
           System.out.println("luanchEditor reading from file");
           return readFromFile();
         }
         return true;
       }
       
       // Writes cached buffer into "/tmp/youraccount.txt"
       private boolean writeToFile()
       {
         try {
           System.out.println("CacheFile is writing to f");
           FileOutputStream fileOutStream = new FileOutputStream("/tmp/avk1994.txt");
           System.out.println("FileOutputStream open. About to write");
           fileOutStream.write(data);
           System.out.println("CacheFile has completed writing to file");
           fileOutStream.flush();
           fileOutStream.close();
         } catch (FileNotFoundException fnfe)
         {
           System.err.println("ERROR: writeToFile => File Not Found Exception");
           fnfe.printStackTrace();
           return false;
         } catch (IOException ioe)
         {
           System.err.println("ERROR: writeToFile => IOExcetion");
           ioe.printStackTrace();
           return false;
         }
         System.out.println("Success: writeToFile completed");
         return true;
       }
       
       // reads the cached file in "/tmp/youraccount.txt" into the cached file buffer
       private boolean readFromFile()
       {
         try {
           System.out.println("CacheFile is reading from file");
           FileInputStream fileInStream = new FileInputStream("/tmp/" + clientName + ".txt");
           data = new byte[fileInStream.available()];
           fileInStream.read(data);
           System.out.println("CachFile has completed reading from file");
           fileInStream.close();
         } catch (FileNotFoundException fnfe)
         {
           System.err.println("ERROR: readFromFile => File Not Found Exception");
           fnfe.printStackTrace();
           return false;
         } catch (IOException ioe)
         {
           System.err.println("ERROR: readFromFile => IOexception");
           ioe.printStackTrace();
           return false;
         }
         System.out.println("Success: readFromFile completed");
         return true;
       }

       // returns file's name
       public synchronized String getFileName()
       {
         return name;
       }
       
       // uploads cache file to server, if state is writeOwned => invalid & if
       // state is releaseOnwership => readShared
       public boolean upload()
       {
         System.out.println("Entering CacheFile Upload" + name);
         try {
           synchronized(this)
           {
             switch (state)
             {
               case writeOwned:
               {
                 state = invalid;
                 break;
               }
               case releaseOwnership:
               {
                 state = readShare;
                 System.err.println("Delayed until emacs session is completed");
                 //break; delayed until emacs session is completed
               }
             }
            }
           FileContents tmpFile = new FileContents(data);
           server.upload(clientIP, name, tmpFile);
           System.out.println("Uploaded " + name + " to server completed");
         } catch (RemoteException re)
         {
           System.err.println("ERROR: UNABLE TO UPLOAD FILE CONTENTS TO SERVER");
           re.printStackTrace();
           return false;
         }
         return true;
       }

       public boolean download(String dName, String dMode)
       {
         System.out.println("Starting download function");
         // Need to check prior
         try {
           synchronized (this)
           {
             switch (state)
             {
               case invalid:
               {
                 if (dMode.equals("r")) // invalid => read_shared
                 {
                   state = readShare;
                   break;
                 }
                 else if (dMode.equals("w")) // invalid => write_owned
                 {
                   state = writeOwned;
                   break;
                 }
               }
               case readShare:
               {
                 if (dMode.equals("w"))      // read_shared => write_owned
                 {
                   state = writeOwned;
                   break;
                 }
               }
             }
           }
           FileContents fileContents = server.download(clientIP, dName, dMode);
           System.out.println("Downloading " + dName + " as " + dMode + " mode");
           if (fileContents == null)
           {
             System.out.print("Server returned null for fileContents");
             return false;
           }
           System.out.println("Download was successful for " + dName);
           name = dName;   // set cache file name
           //mode = dMode;   // set mode of file for ownership purposes
           data = fileContents.get();
         } catch (RemoteException re)
         {
           System.err.println("ERROR: UNABLE TO DOWNLOAD FILECONTENTS FROM SERVER");
           re.printStackTrace();
           return false;
         }
         System.out.println("Success: Download funciton completed");
         return true;
       }

       // For read_shared files, server requests invalidates the cache
       public synchronized boolean invalidate()
       {
         if (state == readShare)
         {
           System.out.println("Server invalidated file " + name);
           state = invalid;
           return true;
         }
         return false;
       }

       // For write_owned files, server requests cached filed to be uploaded
       public synchronized boolean writeback()
       {
         if (state == writeOwned)
         {
           System.out.println("Server requests file " + name + " for upload");
           // Must wait if file is open in emacs --> handled in luanchEditor function
           state = releaseOwnership;
           return true;
         }
         return false;
       }
       // returns state
       public synchronized int getState()
       {
         return this.state;
       }
    }

     // LISTENING THREAD THAT REACTS TO SERVER REQUESTS TO UPLOAD THE CACHE FILE
     // WHEN RELEASE OWNERSHIP IS REQUIRED
     private class CacheThread extends Thread
     {
         boolean activeThread = false;

         public CacheThread()
         {
             activeThread = true;
         }
         
         // enables the run thread to activate it is infinite run loop
         private synchronized boolean isActiveThread()
         {
             return activeThread;
         }

         // RUNNING THREAD THAT RUNS WHILE SYSTEM.IN IS BLOCKING READ
         public void run()
         {
             while(isActiveThread())
             {
                 if (file.getState() == releaseOwnership)
                 {
                     file.upload();
                 }
                 else
                 {
                     continue;
                 }
             }
         }
         
         // ends the thread and joins the parent thread
         public synchronized void endThread() {
             try
             {
                 activeThread = false;
                 join();
             } catch (InterruptedException ie)
             {
                 ie.printStackTrace();
             }
         }
     }
 }
