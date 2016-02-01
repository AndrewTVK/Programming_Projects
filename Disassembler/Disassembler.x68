*-------------------------------------------------------------------------------------------------
* Title      : Disassembler
* Written by : Eric Hargitt and Andrew VanKooten
* Date       : May 2015
* Description: Disassembles s68 files. 
*
* LIST OF PERMENANT REGISTERS
* A4 - Iterator for main loop pause count
* D7 - Holds the current OPCODE that we are decoding
* A5 - Start/current address pointer
* A6 - Holds end address
* A1 - Holds the text buffer
*-------------------------------------------------------------------------------------------------
    
*-----------System Equates------------------------------------------------------------------------
*-----------Masks---------------------------------------------------------------------------------
*-These will isolate certain bits in a word-------------------------------------------------------
bitsFEDC    EQU         $F000
bitsDC      EQU         $3000
bits876     EQU         $01C0
bitsBA98    EQU         $0F00
bitsBA9     EQU         $0E00
bitsBA9876  EQU         $0FC0
bitsB987    EQU         $0B80
bits76      EQU         $00C0
bits8       EQU         $0100
bits210     EQU         $0007
bits3210    EQU         $000F
bits543     EQU         $0038

CR          EQU         $0D
LF          EQU         $0A
*-----------Program Code--------------------------------------------------------------------------
    ORG    $1000

*-----------Input Section-------------------------------------------------------------------------
*-Gets the user input for the start and end address, and does a check to see if they are valid
IO_START    LEA         PROMPT_STRT,A1              Load PROMPT_STRT into A1               
            JSR         GET_INPUT                   Call GET_INPUT
            JSR         INPUT_CHECK                 Call CHECK_INPUT
            MOVE.L      A1,STRT_ADDR                Copy the start address to STRT_ADDR
            
            LEA         PROMPT_END,A1               Load PROMPT_END into A1
            JSR         GET_INPUT                   Call GET_INPUT
            JSR         INPUT_CHECK                 Call CHECK_INPUT
            MOVE.L      A1,END_ADDR                 Copy the end address to END_ADDR

            MOVEA.L     STRT_ADDR,A5                Store the starting addess into A5
            MOVEA.L     END_ADDR,A6                 Store the ending address into A6

            CMPA.L      A6,A5                       If (A5 >= A6) {
            BGE         IO_ERR                          Branch to Done }
            
*-----------Loop Setup----------------------------------------------------------------------------
*-Initialize the two output strings and stores the next instruction in D7
MAIN_LOOP   LEA         MEM_ADDR,A1                 Store the address for the address string in A1
            JSR         ADDR_TO_STR                 Call ADDR_TO_STR
            LEA         OUT_BUF,A1                  Store the address for the output string in A1
            MOVE.W      (A5)+,D7                    Store the next instruction in D7 
           
*-----------Nibble Finder-------------------------------------------------------------------------
*-Figures out what the most significant nibble is by shifting out the last 12 bits----------------
            CLR.W       D5                          Zero out D5
            MOVE.W      D7,D5                       Copy D7 into D5
            MOVEQ       #$C,D6                      Copy $C into D6
            LSR.W       D6,D5                       Cut off the last 12 bits      
      
*-----------Nibble Selector-----------------------------------------------------------------------
*-This section will find the value of that nibble and branch to the appropiate function-----------
*-This section is optimized to allow for faster selection by cutting the list in half twice-------
            CMPI        #$8,D5                      If (D5 >= $8) {
            BGE         GE8                             Branch to GE8 }
            CMPI        #$4,D5                      If (D5 >= $4) {
            BGE         GE4                             Branch to GE4 }

*           Nibbles greater than 0
GE0         CMPI        #$0,D5                      If (D5 == $0) {
            BEQ         NIBBLE0                         Branch to NIBBLE0 }
            CMPI        #$3,D5                      If (D5 <= $3) {
            BLE         NIBBLE1                         Branch to NIBBLE1 }
            
*           Nibbles greater than 4            
GE4         CMPI        #$4,D5                      If (D5 == $4) {
            BEQ         NIBBLE4                         Branch to NIBBLE4 }
            CMPI        #$5,D5                      Else If (D5 == $5) {
            BEQ         QUICK                           Branch to SUBQ }
            CMPI        #$6,D5                      Else If (D5 == $6) {
            BEQ         BRANCH                          Branch to BRANCH }
            CMPI        #$7,D5                      Else If (D5 == $7) {
            BEQ         DATA                            Branch to DATA }
            
*           Nibbles greater than 8            
GE8         CMPI        #$C,D5                      If (D5 >= $C) {
            BGE         GEC                             Branch to GEC }

            CMPI        #$8,D5                      If (D5 == $8) {
            BEQ         NIBBLE8                         Branch to NIBBLE8 }
            CMPI        #$9,D5                      Else If (D5 == $9) {
            BEQ         SUB                             Branch to SUB }
            CMPI        #$A,D5                      Else If (D5 == $A) {
            BEQ         DATA                            Branch to DATA }
            CMPI        #$B,D5                      Else if (D5 == $B) {
            BEQ         NIBBLEB                         Branch to NIBBLEB }
   
*           Nibbles greater than C         
GEC         CMPI        #$C,D5                      If (D5 == $C) {
            BEQ         NIBBLEC                         Branch to NIBBLEC }
            CMPI        #$D,D5                      Else If (D5 == $D) {
            BEQ         ADD                             Branch to ADD }
            CMPI        #$E,D5                      Else If (D5 == $E) {
            BEQ         SHIFTS                          Branch to SHIFTS }
            BRA         DATA                        Else Branch to DATA 


*-----------Nibble Functions----------------------------------------------------------------------
*-These functions will decide what the instruction is based on the most significant nibble--------
NIBBLE0     MOVE.W      D7,D6                       Copy D7 into D6
            LSR.W       #8,D6
            ANDI.W      #1,D6                       Isolate bits 8,7,6 in D6
            CMPI        #1,D6                       If (D5 == 1) { 
            BEQ         BIT_FUNC                        Branch to BIT_FUNC }
            BRA         IMM_HELPER                  Branch to DATA
            
NIBBLE1     MOVE.W      D7,D6                       Copy D7 into D6
            ANDI.W      #bits876,D6                 Isolate bits 8,7,6 in D6
            CMPI        #$0040,D6                   If (D6 == $0040) {
            BEQ         MOVEA                           Branch to MOVEA }
            BRA         MOVE                        Branch to MOVE
            
NIBBLE4     MOVE.W      D7,D6                       Copy D7 into D6
            ANDI.W      #bits876,D6                 Isolate bits 8,7,6
            CMPI        #$01C0,D6                   If (D6 == $01C0) {
            BEQ         LEA                             Branch to LEA }
            MOVE.W      D7,D6                       Copy D7 to D6
            ANDI.W      #bitsB987,D6                Isolate bits B,9,8,7
            CMPI        #$0880,D6                   If (D6 == $0880) {       
            BEQ         MOVEM                           Branch to MOVEM }
            BRA         NOTS                        Branch to NOTS                                  

NIBBLE8     MOVE.W      D7,D6                       Copy D7 into D6
            ANDI.W      #bits876,D6                 Isolate bits 8,7,6 in D6
            CMPI        #$01C0,D6                   If (D5 == $01C0) {
            BEQ         DIVS                            Branch to DIVS }
            BRA         OR                          Else { Branch to OR }

NIBBLEB     MOVE.W      D7,D6                       Copy D7 into D6
            LSR.W       #8,D6
            ANDI.W      #bits210,D6                 Ioslate bits 8,7,6
            CMPI        #$3,D6                      If (D6 <= 3) {  
            BLE         CMP                             Branch to CMP }
            CMPI        #$6,D6                      Else if (D6 <= 6) {
            BLE         EOR                             Branch to EOR }
            BRA         CMP                         Else { Branch to CMP }
            
NIBBLEC     MOVE.W      D7,D6                       Copy D7 into D6
            ANDI.W      #bits876,D6                 Isolate bits 8,7,6
            CMPI        #$01C0,D6                   If (D6 == $01C0) {
            BEQ         MULS                            Branch to MULS }
            BRA         AND                         Else { Branch to AND }
            
*-----------Supported Commands Functions---------------------------------------------
*-----------Start ARITH--------------------------------------------------------------
*           Adds 'ADD' to the output string and branches to ARITH_HELP
ADD         MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            BRA         ARITH_HELP          Branch to ARITH_HELP

*           Adds 'AND' to the output string and branches to ARITH_HELP
AND         MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'N',(A1)+          Add 'N' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            BRA         ARITH_HELP          Branch to ARITH_HELP

*           Adds 'CMP' to the output string and branches to ARITH_HELP
CMP         MOVE.B      #'C',(A1)+          Add 'C' to the output string
            MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.B      #'P',(A1)+          Add 'P' to the output string
            BRA         ARITH_HELP          Branch to ARITH_HELP

*           Adds 'EOR' to the output string and branches to ARITH_HELP
EOR         MOVE.B      #'E',(A1)+          Add 'E' to the output string
            MOVE.B      #'O',(A1)+          Add 'O' to the output string
            MOVE.B      #'R',(A1)+          Add 'R' to the output string
            BRA         ARITH_HELP          Branch to ARITH_HELP

*           Adds 'OR' to the output string and branches to ARITH_HELP
OR          MOVE.B      #'O',(A1)+          Add 'O' to the output string
            MOVE.B      #'R',(A1)+          Add 'R' to the output string
            BRA         ARITH_HELP          Branch to ARITH_HELP
    
*           Adds 'SUB' to the output string and branches to ARITH_HELP
SUB         MOVE.B      #'S',(A1)+          Add 'S' to the output string
            MOVE.B      #'U',(A1)+          Add 'U' to the output string
            MOVE.B      #'B',(A1)+          Add 'B' to the output string
            BRA         ARITH_HELP          Branch to ARITH_HELP
            
*           Adds the size and the operators to the output string            
ARITH_HELP  MOVE.B      D7,D5               Copy the current word to D5
            LSR.B       #6,D5               Shift the size bits to the end
            CMPI        #3,D5               If (D5 == 3) {
            BEQ         ARITH_ADDR              Branch to ARITH_ADDR }
            JSR         GETSIZE             Call GETSIZE
            MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #8,D6               Shift the first opcode bit to the end
            ANDI.W      #1,D6               Isolate the first opcode bit
            CMPI.B      #0,D6               If (D6 == 0) {
            BEQ         EA_DN                   Branch to EA_DN }
            BRA         DN_EA               Else Branch to DN_EA
            
*           Adds the effective address and then the register to the output string            
EA_DN       MOVE.W      D7,D4               Copy the current word to D4            
            ANDI.W      #bits210,D4         Isolate EA Reg Bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate EA mode bits
            LSR.W       #3,D3               Shift the mode bits to the end
            JSR         EA                  Call the EA func
            MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bitsBA9,D4         Isolate the register bits
            LSR.W       #7,D4               Shift the register bits to the end
            LSR.W       #2,D4               Shift the register bits to the end
            JSR         PICK_REG            Call PICK_REG
            BRA         DISPLAY             Branch to DISPLAY
      
*           Adds the register and then the effective address to the output string      
DN_EA       MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bitsBA9,D4         Isolate register bits
            LSR.W       #7,D4               Shift the register bits to the end
            LSR.W       #2,D4               Shift the register bits to the end
            JSR         PICK_REG            Call PICK_REG
            MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.W      D7,D4               Copy the current word to D4            
            ANDI.W      #bits210,D4         Isolate EA Reg Bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate EA mode bits
            LSR.W       #3,D3               Shift the mode bits to the end
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY     

*           Adds 'A.', the size, and the operands to the output string if the destination is an addrss register
ARITH_ADDR  MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'.',(A1)+          Add '.' to the output string
            MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #8,D6               Shift the size bit to the end
            ANDI.W      #1,D6               Isolate the size bit
            CMPI        #1,D6               If (D6 == 1) {
            BEQ         ADDRL                   Branch to ADDRL }
            MOVE.B      #'W',(A1)+          Add 'W' to the output string
            MOVE.B      #0,D5               Store 0 in D5
            BRA         ADDR_OP             Branch to ADDR_OP
ADDRL       MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #1,D5               Store 1 in D5
ADDR_OP     MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.W      D7,D4               Load up register bits to D4            
            ANDI.W      #bits210,D4         Isolate EA Reg Bits
            MOVE.W      D7,D3               Load up mode bits into D3
            ANDI.W      #bits543,D3         Isolate EA mode bits
            LSR.W       #3,D3               Shift the mode bits over
            JSR         EA                  Call the EA func
            MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bitsBA9,D4         Isolate the register bits
            LSR.W       #7,D4               Shift D4 to the right by 7 bits
            LSR.W       #2,D4               Shift D4 to the right by 2 bits
            JSR         PICK_REG            Call PICK_REG
            BRA         DISPLAY             Branch to DISPLAY
*-----------End ADDA-----------------------------------------------------------------

*-----------Start IMM_HELPER---------------------------------------------------------
*-Section that handles the immidiate instructions by looking at the second most significant nibble
*           Decodes the second most significant nibble and chooses an opcode based on that
IMM_HELPER  MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #8,D6               Shift D6 to the right by 8 bits
            ANDI.W      #$000F,D6           Isolate the last 4 bits
            CMPI        #$0,D6              If (D6 == $0) {
            BEQ         ORI                     Branch to ORI }
            CMPI        #$2,D6              Else if (D6 == $2) {
            BEQ         ANDI                    Branch to ANDI }
            CMPI        #$4,D6              Else if (D6 == $4) {
            BEQ         SUBI                    Branch to SUBI }
            CMPI        #$6,D6              Else if (D6 == $6) {
            BEQ         ADDI                    Branch to ADDI }
            CMPI        #$8,D6              Else If (D6 == $8) {
            BEQ         BIT_FUNC                Branch to BCLR }
            CMPI        #$A,D6              Else if (D6 == $A) {
            BEQ         EORI                    Branch to EORI }
            CMPI        #$C,D6              Else if (D6 == $C) {
            BEQ         CMPI                    Branch to CMPI }
            BRA         DATA                Else { Branch to DATA }
            
*           Adds 'ORI' to the ouput string 
ORI         MOVE.B      #'O',(A1)+          Add 'O' to the output string
            MOVE.B      #'R',(A1)+          Add 'R' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            BRA         DECODE_IMM          Branch to DECODE_IMM

*           Adds 'ANDI' to the output string
ANDI        MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'N',(A1)+          Add 'N' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            BRA         DECODE_IMM          Branch to DECODE_IMM
            
*           Adds 'ADDI' to the output string
ADDI        MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            BRA         DECODE_IMM          Branch to DECODE_IMM

*           Adds 'SUBI' to the output string
SUBI        MOVE.B      #'S',(A1)+          Add 'S' to the output string
            MOVE.B      #'U',(A1)+          Add 'U' to the output string
            MOVE.B      #'B',(A1)+          Add 'B' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            BRA         DECODE_IMM          Branch to DECODE_IMM
 
*           Adds 'EORI' to the output string
EORI        MOVE.B      #'E',(A1)+          Add 'E' to the output string
            MOVE.B      #'O',(A1)+          Add 'O' to the output string
            MOVE.B      #'R',(A1)+          Add 'R' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            BRA         DECODE_IMM          Branch to DECODE_IMM
            
*           Adds 'CMPI' to the output string
CMPI        MOVE.B      #'C',(A1)+          Add 'C' to the output string
            MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.B      #'P',(A1)+          Add 'P' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            BRA         DECODE_IMM          Branch to DECODE_IMM

*           Decodes the size and adds it to the output string           
DECODE_IMM  MOVE.W      D7,D5               Copy the current word to D5
            ANDI.W      #bits76,D5          Isolate bits 7,6 in D5
            LSR.W       #$6,D5              Shift D5 to the right by 6 bits
            JSR         GETSIZE             Call GETSIZE
            MOVE.B      #'#',(A1)+          Add '#' to the output string
            MOVE.B      #'$',(A1)+          Add '$' to the output string
            CMPI.W      #$0,D5              If (D5 == 0) {
            BEQ         IMM_W                   Branch to IMM_W }
            BRA         IMM_L               Else { Branch to IMM_L }

*           Get the next word from memory and add it to the output string   
IMM_W       MOVE.W      (A5)+,D5            Get the next word from A5
            JSR         REG_TO_STR          Call REG_TO_STR
            BRA         IMM_DONE            Branch to CMPI_DONE
            
*           Get the next two words from memory and add them to the output string
IMM_L       MOVE.W      (A5)+,D5            Get the next word from A5
            JSR         REG_TO_STR          Call REG_TO_STR
            MOVE.W      (A5)+,D5            Get the next word from A5
            JSR         REG_TO_STR          Call REG_TO_STR
            BRA         IMM_DONE            Branch to CMPI_DONE

*           Decode the EA register and mode, and them to the output string
IMM_DONE    MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the register bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the mode bits
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            CMPI        #1,D3               If (D3 == 1) {
            BEQ         DATA                    Branch to DATA }
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY
*-----------End IMM_HELPER--------------------------------------------------------          

*-----------Start BRANCH-----------------------------------------------------------
*-Decides the Bcc used based on the second most significant nibble 
*   and adds the address being branched to, to the output string
*           Isolates the second most significant nibble and branches to correct 
*               condition code
*           Optimized by cutting the list in half twice
BRANCH      CLR         D6                  Zero out D5
            MOVE.B      #'B',(A1)+          Add 'B' to the output string
            MOVE.W      D7,D6               Copy D7 to D5
            LSR.W       #8,D6               Shift D5 8 bits to the right
            ANDI.W      #bits3210,D6        Get the last four bits
            CMPI        #$8,D6              If (D6 >= $8) {
            BGE         BGE8                    Branch to BGE8 }
            CMPI        #$4,D6              If (D6 >= $4) {
            BGE         BGE4                    Branch to BGE4 }
            
BGE0        CMPI        #$0,D6              If (D5 == $0) {
            BEQ         BRA                     Branch to BRA }
            CMPI        #$2,D6              Else If (D6 == $2) {
            BEQ         BHI                     Branch to BHI }
            CMPI        #$3,D6              Else If (D6 == $3) {
            BEQ         BLS                     Branch to BLS }
            
BGE4        CMPI        #$4,D6              Else If (D6 == $4) {
            BEQ         BCC                     Branch to BCC }
            CMPI        #$5,D6              Else If (D5 == $5) {
            BEQ         BCS                     Branch to BCS }
            CMPI        #$6,D6              Else If (D6 == $6) {
            BEQ         BNE                     Branch to BNE }
            CMPI        #$7,D6              Else If (D6 == $7) {
            BEQ         BEQ                     Branch to BEQ }
            
BGE8        CMPI        #$C,D6              If (D6 >= $C) {
            BGE         BGEC                    Branch to BGEC }
            
            CMPI        #$8,D6              Else If (D5 == $8) {
            BEQ         BVC                     Branch to BVC }
            CMPI        #$9,D6              Else If (D5 == $9) {
            BEQ         BVS                     Branch to BVS }
            CMPI        #$A,D6              Else If (D5 == $A) {
            BEQ         BPL                     Branch to BPL }
            CMPI        #$B,D6              Else If (D6 == $B) {
            BEQ         BMI                     Branch to BMI }
            
BGEC        CMPI        #$C,D6              Else If (D5 == $C) {
            BEQ         BGE                     Branch to BGE }
            CMPI        #$D,D6              Else If (D5 == $D) {
            BEQ         BLT                     Branch to BLT }
            CMPI        #$E,D6              Else If (D6 == $E) {
            BEQ         BGT                     Branch to BGT }
            CMPI        #$F,D6              Else If (D6 == $F) {
            BEQ         BLE                     Branch to BLE }
            BRA         DATA                Else Branch to DATA

*           Adds the condition code to the output string and branches to END_BRANCH
BRA         MOVE.B      #'R',(A1)+          Add 'R' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH

BHI         MOVE.B      #'H',(A1)+          Add 'H' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH

BLS         MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH

BCC         MOVE.B      #'C',(A1)+          Add 'C' to the output string
            MOVE.B      #'C',(A1)+          Add 'C' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BCS         MOVE.B      #'C',(A1)+          Add 'C' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH

BNE         MOVE.B      #'N',(A1)+          Add 'N' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BEQ         MOVE.B      #'E',(A1)+          Add 'E' to the output string
            MOVE.B      #'Q',(A1)+          Add 'Q' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BVC         MOVE.B      #'V',(A1)+          Add 'V' to the output string
            MOVE.B      #'C',(A1)+          Add 'C' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BVS         MOVE.B      #'V',(A1)+          Add 'V' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BPL         MOVE.B      #'P',(A1)+          Add 'P' to the output string
            MOVE.B      #'L',(A1)+          Add 'L' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BMI         MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BGE         MOVE.B      #'G',(A1)+          Add 'G' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BLT         MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #'T',(A1)+          Add 'T' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH
            
BGT         MOVE.B      #'G',(A1)+          Add 'G' to the output string
            MOVE.B      #'T',(A1)+          Add 'T' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH  

BLE         MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string
            BRA         END_BRANCH          Branch to END_BRANCH          

*           Adds the address being branched to, to the output string                
END_BRANCH  MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.W      A5,D5               Copy A5 to D5
            ADD.W       (A5)+,D5            Get the next word in memory and store it in D6
            JSR         REG_TO_STR          Call REG_TO_STR function
            BRA         DISPLAY             Branch to DISPLAY
*-----------End BRANCH--------------------------------------------

*-----------Start BIT_FUNC------------------------------------------------------------
*           Adds 'B' to the output string and determines the rest
BIT_FUNC    MOVE.B      #'B',(A1)+          Add 'B' to the output string
            MOVE.B      D7,D5               Copy the current word to D5
            LSR.B       #6,D5               Shift bit 6 to the end
            ANDI.B      #3,D5               Isolate the last two bits
            CMPI        #0,D5               If (D5 == 0) {
            BEQ         BTST                    Branch to BTST }
            CMPI        #1,D5               Else if (D5 == 1) {
            BEQ         BCHG                    Branch to BCHG }
            CMPI        #2,D5               Else if (D5 == 2) {
            BEQ         BCLR                    Branch to BCLR }
            BRA         BSET                Else Branch to BSET

*           Adds 'TST' to the output string and branches to TEST_IMM
BTST        MOVE.B      #'T',(A1)+          Add 'T' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            MOVE.B      #'T',(A1)+          Add 'T' to the output string
            BRA         TEST_IMM            Branch to TEST_IMM
            
*           Adds 'CHG' to the output string and branches to TEST_IMM            
BCHG        MOVE.B      #'C',(A1)+          Add 'C' to the output string
            MOVE.B      #'H',(A1)+          Add 'H' to the output string
            MOVE.B      #'G',(A1)+          Add 'G' to the output string
            BRA         TEST_IMM            Branch to TEST_IMM
            
*           Adds 'CLR' to the output string and branches to TEST_IMM
BCLR        MOVE.B      #'C',(A1)+          Add 'C' to the output string
            MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #'R',(A1)+          Add 'R' to the output string
            BRA         TEST_IMM            Branch to TEST_IMM
            
*           Adds 'SET' to the output string and branches to TEST_IMM            
BSET        MOVE.B      #'S',(A1)+          Add 'S' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string 
            MOVE.B      #'T',(A1)+          Add 'T' to the output string
            BRA         TEST_IMM            Branch to TEST_IMM
            
*           Determines if it is an immidiate or an address                    
TEST_IMM    MOVE.W      D7,D6               Copy the current word to D6
            ANDI.W      #bitsBA98,D6        Isolate bits B,A,9,8,7,6
            LSR.W       #$8,D6              Shift D6 to the right by 6 bits
            CMPI.W      #$8,D6              If (D6 == $22) {
            BEQ         BIT_IMM                 Branch to BCLR_IMM }
            BRA         BIT_DN              Else Branch to BCLR_DN

*           Stores the immidiate data and the effective address to the output string
BIT_IMM     MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the EA register
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the EA mode
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            CMPI        #1,D3               If (D3 == 1) {
            BEQ         DATA                    Branch to DATA }
            MOVE.W      (A5)+,D5            Get the next word in memory 
            MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.B      #'#',(A1)+          Add '#' to the output string
            MOVE.B      #'$',(A1)+          Add '$' to the output string
            JSR         REG_TO_STR          Call REG_TO_STR
            MOVE.B      #',',(A1)+          Add ',' to the output string
            JSR         EA                  Call EA 
            BRA         DISPLAY             Branch to DISPLAY

*           Stores the register and the effective address to the output string
BIT_DN      MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the EA register
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the EA mode
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            CMPI        #1,D3               If (D3 == 1) {
            BEQ         DATA                    Branch to DATA }
            MOVE.W      D7,D6               Copy the current word to D6
            ANDI.W      #bitsBA9,D6         Isolate the register number
            LSR.W       #$8,D6              Shift D6 to the right by 8 bits
            LSR.W       #$1,D6              Shift D6 to the right by 1 bit
            MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            ADDI.B      #$30,D6             Add $30 to D6
            MOVE.B      D6,(A1)+            Add the byte in D6 to the output string
            MOVE.B      #',',(A1)+          Add ',' to the output string
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY
*-----------End BIT_FUNC-------------------------------------------------------------

*-----------Start DATA---------------------------------------------------------------
*           Clears all the data in the output string and adds 'Data' and the current
*               instruction to the output string
DATA        MOVE.B      #' ',(A1)+          Add ' ' to the output string
            JSR         CLR_BUF             Call CLR_BUF
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'T',(A1)+          Add 'T' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.W      D7,D5               Copy D7 to D5
            JSR         REG_TO_STR          Call REG_TO_STR
            BRA         DISPLAY             Branch to DISPLAY
*-----------End DATA-----------------------------------------------------------------

*-----------Start DIVS MULS LEA--------------------------------------------
*-DIVS, MULS, and LEA have been grouped together due to their similiar structure 
*           Adds 'DIVS.W' to the output string and branches to DML_HELPER
DIVS        MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.B      #'I',(A1)+          Add 'I' to the output string
            MOVE.B      #'V',(A1)+          Add 'V' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            MOVE.B      #'.',(A1)+          Add '.' to the output string
            MOVE.B      #'W',(A1)+          Add 'W' to the output string
            MOVE.W      #1,D5               Copy 1 to D5
            BRA         DML_HELPER          Branch to DML_HELPER

*           Adds 'MULS.W' to the output string and branches to DML_HELPER
MULS        MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.B      #'U',(A1)+          Add 'U' to the output string
            MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            MOVE.B      #'.',(A1)+          Add '.' to the output string
            MOVE.B      #'W',(A1)+          Add 'W' to the output string
            MOVE.W      #1,D5               Copy 1 to D5
            BRA         DML_HELPER          Branch to DML_HELPER

*           Adds 'LEA' to the output string and branches to DML_HELPER
LEA         MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.W      #0,D5               Copy 0 yo D5
            BRA         DML_HELPER          Branch to DML_HELPER

*           Adds the operands to DIVS, MULS, and LEA 
DML_HELPER  MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the EA register bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the EA mode bits
            LSR.W       #$3,D3              Shift D3 to the right by three bits
            JSR         EA                  Call EA
            MOVE.B      #',',(A1)+          Add ',' to the current string
            LEA         AD_CHOOSE,A7        Store the address of AD_CHOOSE in A7
            ADD         D5,A7               Add D5 to A7
            MOVE.B      (A7),(A1)+          Add whatever is in A7 to the output string
            MOVEA.W     #0,A7               Clear out A7
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bitsBA9,D4         Isolate bits B,A,9
            LSR.W       #$6,D4              Shift D4 to the right by 6 bits
            LSR.W       #$3,D4              Shift D4 to the right by 3 bits
            JSR         PICK_REG            Call PICK_REG
            BRA         DISPLAY             Branch to DISPLAY
*-----------End DIVS MULS LEA------------------------------------------------

*-----------Start MOVE-----------------------------------------------------------
*           Adds 'MOVE.' to the output string and determines the size
MOVE        MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.B      #'O',(A1)+          Add 'O' to the output string
            MOVE.B      #'V',(A1)+          Add 'V' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string
            MOVE.B      #'.',(A1)+          Add '.' to the output string
            MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #$6,D6              Shift D6 to the right by 6 bits
            LSR.W       #$6,D6              Shift D6 to the right by 6 bits
            CMPI        #$1,D6              If (D6 == $1) {
            BEQ         MOVE_B                  Branch to MOVE_B }
            CMPI        #$3,D6              Else If (D6 == $3) {
            BEQ         MOVE_W                  Branch to MOVE_W }
            CMPI        #$2,D6              Else If (D6 == $2) {
            BEQ         MOVE_L                  Branch to MOVE_L }
            BRA         DATA                Else Branch to DATA

*           Adds the size to the output string and branches to END_MOVE
MOVE_B      MOVE.B      #'B',(A1)+          Add 'B' to the output string
            MOVE.B      #$0,D5              Stores 0 to D5
            BRA         END_MOVE            Branch to END_MOVE
MOVE_W      MOVE.B      #'W',(A1)+          Add 'W' to the output string
            MOVE.B      #$0,D5              Stores 0 to D5
            BRA         END_MOVE            Branch to END_MOVE
MOVE_L      MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #$1,D5              Stores 1 to D5
            BRA         END_MOVE            Branch to END_MOVE

*           Add the source and destination registers to the output string
END_MOVE    MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.W      D7,D6               Copy the current word to D6
            MOVE.W      D6,D4               Copy D6 to D4
            ANDI        #bits210,D4         Isolate the source register
            LSR.W       #3,D6               Shift D6 to the right by 3 bits
            MOVE.W      D6,D3               Copy D6 to D3
            ANDI        #bits210,D3         Isolate the source mode
            JSR         EA                  Call EA
            MOVE.B      #',',(A1)+          Add ',' to the output string
            LSR.W       #3,D6               Shift D6 to the right by 3 bits
            MOVE.W      D6,D3               Copy D6 to D3
            ANDI        #bits210,D3         Isolate the destination mode
            CMPI        #1,D3               If (D3 == 1) {
            BEQ         DATA                    Branch to DATA }
            LSR.W       #3,D6               Shift D6 to the right by 3 bits
            MOVE.W      D6,D4               Copy D6 to D4
            ANDI        #bits210,D4         Isolate the destination register
            CMPI        #4,D4               If (D4 == 4) {
            BEQ         DATA                    Branch to DATA }
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY
*-----------End MOVE-----------------------------------------------------------

*-----------Start MOVEA--------------------------------------------------------
*           Adds 'MOVEA.' to the output string and determines the size
MOVEA       MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.B      #'O',(A1)+          Add 'O' to the output string
            MOVE.B      #'V',(A1)+          Add 'V' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'.',(A1)+          Add '.' to the output string
            MOVE.W      D7,D6               Copy the current word to D6
            ANDI        #bitsDC,D6          Isolate the size bits
            LSR.W       #8,D6               Shift D6 to the right by 8 bits
            LSR.W       #4,D6               Shift D6 to the right by 4 bits
            CMPI        #$3,D6              If (D6 == $3) {
            BEQ         MOVEA_W                 Branch to MOVEA_W }
            CMPI        #$2,D6              Else If (D6 == $2) {
            BEQ         MOVEA_L                 Branch to MOVEA_L }
            BRA         DATA                Else Branch to DATA

*           Adds the size to the output string and branch to END_MOVEA
MOVEA_W     MOVE.B      #'W',(A1)+          Add 'W' to the output string
            BRA         END_MOVEA           Branch to END_MOVEA
MOVEA_L     MOVE.B      #'L',(A1)+          Add 'L' to the output string
            BRA         END_MOVEA           Branch to END_MOVEA

*           Adds the operands to the output string
END_MOVEA   MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.B      #$1,D5              Store 1 in D5
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the source register
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the source mode
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            JSR         EA                  Call EA
            MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bitsBA9,D4         Isolate the register number
            LSR.W       #$8,D4              Shift D4 to the right by 8 bits
            LSR.W       #$1,D4              Shift D4 to the right by 1 bit
            JSR         PICK_REG            Call PICK_REG
            BRA         DISPLAY             Branch to DISPLAY
*-----------End MOVEA-----------------------------------------------------------

*-----------Start MOVEM-------------------------------------------------------
*           Add 'MOVEM' to the ouput string and figure out the size            
MOVEM       MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.B      #'O',(A1)+          Add 'O' to the output string
            MOVE.B      #'V',(A1)+          Add 'V' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string
            MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.W      D7,D6               Copy the current word to D6
            ANDI.W      #$40,D6             Isolate the size bit
            LSR.W       #$6,D6              Shift size bit to the end
            CMPI        #$0,D6              If (D6 == $0) {
            BEQ         MOVEM_W                 Branch to MOVEM_W }
            BRA         MOVEM_L             Else Branch to MOVEM_L

*           Add the size to the ouput string and branch to MOVEM_P2
MOVEM_W     MOVE.B      #'.',(A1)+          Add '.' to the output string
            MOVE.B      #'W',(A1)+          Add 'W' to the output string
            BRA         MOVEM_P2            Branch to MOVEM_P2
MOVEM_L     MOVE.B      #'.',(A1)+          Add '.' to the output string
            MOVE.B      #'L',(A1)+          Add 'L' to the output string
            BRA         MOVEM_P2            Branch to MOVEM_P2

*           Figure out if it is register to memory or memory to register
MOVEM_P2    MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.W      D7,D6               Copy the current word to D6
            ANDI.W      #$400,D6            Isolate the dr bit (bit A)
            LSR.W       #$8,D6              Shift D6 to the right by 8 bits
            LSR.W       #$2,D6              Shift D6 to the right by 2 bits
            CMPI        #$0,D6              If (D6 == $0) {
            BEQ         R_TO_M                  Branch to R_TO_M }
            BRA         M_TO_R              Else Branch to M_TO_R
            
*           Print register list first, then EA           
R_TO_M      JSR         PRINT_REGS          Call PRINT_REGS
            MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the EA register bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the EA mode bits
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY

*           Print the EA first, then the register list
M_TO_R      MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the EA register bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the EA mode bits
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            JSR         EA                  Call EA
            MOVE.B      #',',(A1)+          Add ',' to the output string
            JSR         PRINT_REGS          Call PRINT_REGS
            BRA         DISPLAY             Branch to DISPLAY

*           Decides whether to use forward print or reverse print
PRINT_REGS  MOVE.W      D7,D6               Copy the current word to D6
            ANDI.W      #bits543,D6         Isolate bits 5,4,3 
            LSR.W       #$3,D6              Shift D6 to the right by 3 bits
            CMPI        #$4,D6              If (D6 == $4) {
            BEQ         REVERSE_P               Branch to REVERSE_P }
            BRA         FORWARD_P           Else Branch to FORWARD_P

*           Loads the correct register array to A2 and branches to MOVEM_LOOP       
REVERSE_P   LEA         MOVEM_REV,A2        Store MOVEM_REV to A2
            BRA         MOVEM_LOOP          Branch to MOVEM_LOOP
FORWARD_P   LEA         MOVEM_FOR,A2        Store MOVEM)FOR to A2
            BRA         MOVEM_LOOP          Branch to MOVEM_LOOP

*           Adds the registers being moved to the output string
MOVEM_LOOP  CLR.W       D2                  Clear out the iterator              
            MOVE.W      (A5)+,D6            Store the next word in memory to D6
LOOP3       MOVE.W      D6,D5               Copy D6 to D5
            ANDI.W      #$1,D5              Isolate the last bit in D5
            CMPI        #$1,D5              If (D5 == $1) {
            BEQ         PUSH_REG                Branch to PUSH_REG }
LOOP3_CHECK ROR.W       #$1,D6              Rotate D6 once to the right
            ADDQ.B      #$1,D2              Add one to the iterator
            CMPI        #$10,D2             If (D2 == $10) {
            BEQ         END_LOOP3               Branch to END_LOOP3 )
            BRA         LOOP3               Else Branch to LOOP3
END_LOOP3   CLR         D2                  Clear out the iterator
            RTS                             Return

*           Pushes a register stored in array in A3 to the output string
PUSH_REG    MOVEA.L     A2,A3               Copy the address in A2 to A3
            MOVE.W      D2,D3               Copy D2 to D3
            LSL.W       #$1,D3              Shift D3 to the left by 1
            ADDA.W      D3,A3               Add D3 to A3
            MOVE.B      (A3),(A1)+          Add the byte stored in A3 to the output string
            ADDA        #1,A3               Add 1 to A3
            MOVE.B      (A3),(A1)+          Add the byte stored in A3 to the output string
            BRA         LOOP3_CHECK         Branch to LOOP3_CHECK
*-----------End MOVEM------------------------------------------------------------

*-----------Start NOTS and JUMP----------------------------------------------------------
*           Determines what instruction D7 is by looking at the complete word 
*               and then by looking at the second most significant nibble            
NOTS        CMPI        #$4E71,D7           If (D7 == $4E71) {
            BEQ         NOP                     Branch to NOP }
            CMPI        #$4E75,D7           Else If (D7 == $4E75) {
            BEQ         RTS                     Branch to RTS }
            MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #8,D6               Shift D6 to the right by 8 bits
            ANDI.W      #$000F,D6           Isolate the last four bits
            CMPI        #$2,D6              If (D6 == $2) {
            BEQ         CLR                     Branch to CLR }
            CMPI        #$4,D6              Else If (D6 == $4) {
            BEQ         NEG                     Branch to NEG }
            CMPI        #$6,D6              Else If (D6 == $6) {
            BEQ         NOT                     Branch to NOT }
            CMPI        #$E,D6              Else If (D6 == $E) {
            BEQ         JUMP                    Branch to JUMP }
            BRA         DATA                Else Branch to DATA
            
*           Adds 'CLR' to the output string and branch to NOTS_HELPER
CLR         MOVE.B      #'C',(A1)+          Add 'C' to the output string
            MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #'R',(A1)+          Add 'R' to the output string
            BRA         NOTS_HELPER         Branch to NOTS_HELPER            

*           Adds 'NEG' to the output string and branch to NOTS_HELPER
NEG         MOVE.B      #'N',(A1)+          Add 'N' to the output string
            MOVE.B      #'E',(A1)+          Add 'E' to the output string
            MOVE.B      #'G',(A1)+          Add 'G' to the output string
            BRA         NOTS_HELPER         Branch to NOTS_HELPER

*           Adds 'NOT' to the output string and branch to NOTS_HELPER
NOT         MOVE.B      #'N',(A1)+          Adds 'N' to the output string
            MOVE.B      #'O',(A1)+          Adds 'O' to the output string
            MOVE.B      #'T',(A1)+          Adds 'T' to the output string
            BRA         NOTS_HELPER         Branch to NOTS_HELPER
            
*           Adds the size and the operands to NEG and NOT           
NOTS_HELPER MOVE.W      D7,D5               Copy the current word to D5
            ANDI.W      #bits76,D5          Isolate the size bits
            LSR.W       #$6,D5              Shift D5 to the right by 6 bits
            JSR         GETSIZE             Call GETSIZE
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the register bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the mode bits
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            CMPI        #1,D3               If (D3 == 1) {
            BEQ         DATA                    Branch to DATA }
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY

*           Determins if the instruction is RTS, JSR, or JMP 
JUMP        MOVE.W      D7,D6               Copy the instruction to D6
            LSR.W       #$6,D6              Shift D6 to the right by 6 bits
            ANDI.W      #$0003,D6           Isolate the last two bits
            CMPI        #$2,D6              Else If (D6 == $2) {
            BEQ         JSR                     Branch to JSR }
            CMPI        #$3,D6              Else If (D6 == $3) {
            BEQ         JMP                     Branch to JMP }
            BRA         DATA                Else Branch to DATA

*           Adds 'JSR' to the output string and branches to JUMP_HELPER
JSR         MOVE.B      #'J',(A1)+          Add 'J' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            MOVE.B      #'R',(A1)+          Add 'R' to the output string
            BRA         JUMP_HELPER         Branch to JUMP_HELPER
            
*           Adds 'JMP' to the output string and branches to JUMP_HELPER     
JMP         MOVE.B      #'J',(A1)+          Add 'J' to the output string
            MOVE.B      #'M',(A1)+          Add 'M' to the output string
            MOVE.B      #'P',(A1)+          Add 'P' to the output string
            BRA         JUMP_HELPER         Branch to JUMP_HELPER
        
*           Adds the effective address to the output string and branches to DISPLAY       
JUMP_HELPER MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the register bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the mode bits
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY
        
*           Adds 'RTS' to the output string and branches to DISPLAY
RTS         MOVE.B      #'R',(A1)+          Add 'R' to the output string
            MOVE.B      #'T',(A1)+          Add 'T' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            BRA         DISPLAY             Branch to DISPLAY

*           Adds 'NOP' to the output string and branches to DISPLAY            
NOP         MOVE.B      #'N',(A1)+          Add 'N' to the output string
            MOVE.B      #'O',(A1)+          Add 'O' to the output string
            MOVE.B      #'P',(A1)+          Add 'P' to the output string
            BRA         DISPLAY
*-----------End NOTS and JUMP----------------------------------------------------------   

*-----------Start SHIFTS----------------------------------------------------------------
*-Decides on the shift, the direction, the size, and the operands-----------------------            
SHIFTS      MOVE.W      D7,D6               Copy the current word to D6
            LSR.L       #$6,D6              Shift D6 to the right by 6 bits
            ANDI.W      #$0003,D6           Isolate the last two bits
            CMPI        #3,D6               If (D6 == $3) {
            BEQ         MEM_PICK                Branch to MEM_PICK }

*           Decides on the shift
            MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #$3,D6              Shift D6 to the right by 3 bits
            ANDI.W      #$0003,D6           Isolate the last two bits
            CMPI        #$0000,D6           If (D6 == $0) {
            BEQ         AS_SHIFT                Branch to AS_SHIFT }
            CMPI        #$0001,D6           Else If (D6 == $1) {
            BEQ         LS_SHIFT                Branch to LS_SHIFT }
            CMPI        #$0003,D6           Else If (D6 == $3) {
            BEQ         RO_SHIFT                Branch to RO_SHIFT }
            BRA         DATA                Else Branch to DATA

*           Decides on the type of shift if bits 7 and 6 were 11
MEM_PICK    MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #8,D6               Shift D6 to the right by 8 bits
            LSR.W       #1,D6               Shift D6 to the right by 1 bit
            ANDI.W      #bits210,D6         Isolate the last 3 bits
            CMPI        #$0,D6              If (D6 == $0) {
            BEQ         AS_SHIFT                Branch to AS_SHIFT }
            CMPI        #$1,D6              Else If (D6 == $1) {
            BEQ         LS_SHIFT                Branch to LS_SHIFT }
            CMPI        #$3,D6              Else If (D6 == $3) {
            BEQ         RO_SHIFT                Branch to RO_SHIFT }
            BRA         DATA                Else Branch to DATA

*           Adds the type of shift to the output string and branches to GET_DIR            
AS_SHIFT    MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            BRA         GET_DIR             Branch to GET_DIR
LS_SHIFT    MOVE.B      #'L',(A1)+          Add 'L' to the output string
            MOVE.B      #'S',(A1)+          Add 'S' to the output string
            BRA         GET_DIR             Branch to GET_DIR
RO_SHIFT    MOVE.B      #'R',(A1)+          Add 'R' to the output string
            MOVE.B      #'O',(A1)+          Add 'O' to the output string
            BRA         GET_DIR             Branch to GET_DIR

*           Decides the direction of the shift            
GET_DIR     MOVE.W      D7,D6               Copy the current word to D6
            ANDI.W      #bits8,D6           Isolate the 8th bit
            CMPI        #$0000,D6           If (D6 == $0) {
            BEQ         SHIFT_RIGHT             Branch to SHIFT_RIGHT }
            BRA         SHIFT_LEFT          Else Branch to SHIFT_LEFT

*           Adds the direction to the output string and branches to SHIFT_SIZE             
SHIFT_RIGHT MOVE.B      #'R',(A1)+          Add 'R' to the output string
            BRA         SHIFT_SIZE          Branch to SHIFT_SIZE
SHIFT_LEFT  MOVE.B      #'L',(A1)+          Add 'L' to the output string
            
*           Determines the size of the shift            
SHIFT_SIZE  MOVE.B      D7,D5               Copy the current word to D5
            LSR.B       #$6,D5              Shift D5 to the right by 6 bits
            CMPI        #$3,D5              If (D5 == $3) {
            BEQ         MEM_SHIFT               Branch to MEM_SHIFT }
            JSR         GETSIZE             Call GETSIZE
            MOVE.W      D7,D6               Copy the current word to D6
            BRA         REG_SHIFT           Branch to REG_SHIFT

*           Decides if the shift contains a count or the count is stored in a register
REG_SHIFT   MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #$5,D6              Shift D6 to the right by 5 bits
            ANDI.W      #$1,D6              Isolate the last bit
            CMPI        #$0,D6              If (D6 == $0) {
            BEQ         IR_IMM                  Branch to IR_IMM }
            BRA         IR_REG              Branch to IR_REG

*           If the count was an immidiate
IR_IMM      MOVE.B      #'#',(A1)+          Add '#' to the output string
            MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #$8,D6              Shift D6 to the right by 8 bits
            LSR.W       #$1,D6              Shift D6 to the right by 1 bit
            ANDI.W      #bits210,D6         Isolate the last 3 bits
            LEA         SHIFT_COUNT,A2      Store the address of SHIFT_COUNT in A2
            ADDA.W      D6,A2               Add D6 to A2
            MOVE.B      (A2),(A1)+          Get the character in A2 and put it in the output string
            MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the last 3 bits
            JSR         PICK_REG            Call PICK_REG
            BRA         DISPLAY             Branch to DISPLAY

*           If the count was stored in a register           
IR_REG      MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            LSR.W       #$8,D4              Shift D4 by 8 bits
            LSR.W       #$1,D4              Shift D4 by 1 bit
            ANDI.W      #bits210,D4         Isolate the last 3 bits
            JSR         PICK_REG            Call PICK_REG
            MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the last 3 bits
            JSR         PICK_REG            Call PICK_REG
            BRA         DISPLAY             Branch to DISPLAY
            
*           Add the effective address to the output string
MEM_SHIFT   MOVE.B      #'.',(A1)+          Add '.' to the output string
            MOVE.B      #'W',(A1)+          Add 'W' to the output string
            MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the register bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the mode bits
            LSR.W       #3,D3               Shift D3 to the right by three bits            
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY
*-----------End SHIFTS------------------------------------------------------------------

*-----------Start QUICK-----------------------------------------------------------
*           Adds either 'ADDQ' or 'SUBQ' to the output string         
QUICK       MOVE.W      D7,D6               Copy the current word to D6
            LSR.W       #8,D6               Shift D6 to the right by 8 bits
            ANDI.W      #1,D6               Isolate the last bit
            CMPI        #1,D6               If (D6 == 1) {
            BEQ         SUBQ                    Branch to SUBQ }
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.B      #'D',(A1)+          Add 'D' to the output string
            MOVE.B      #'Q',(A1)+          Add 'Q' to the output string
            BRA         QCK_OP              Branch to QCK_OP
SUBQ        MOVE.B      #'S',(A1)+          Add 'S' to the output string
            MOVE.B      #'U',(A1)+          Add 'U' to the output string
            MOVE.B      #'B',(A1)+          Add 'B' to the output string
            MOVE.B      #'Q',(A1)+          Add 'Q' to the output string

*           Adds the size and the operands to the output string
QCK_OP      MOVE.B      D7,D5               Copy the current word to D5
            ANDI.W      #bits76,D5          Isolate the size bits
            LSR.W       #$6,D5              Shift D5 to the right by 6 bits
            JSR         GETSIZE             Call GETSIZE
            MOVE.B      #'#',(A1)+          Add '#' to the output string
            MOVE.B      #'$',(A1)+          Add '#' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bitsBA9,D4         Isolate the register bits
            LSR.W       #$8,D4              Shift D4 to the right by 8 bits
            LSR.W       #$1,D4              Shift D4 to the right by 1 bit
            JSR         PICK_REG            Call PICK_REG
            MOVE.B      #',',(A1)+          Add ',' to the output string
            MOVE.W      D7,D4               Copy the current word to D4
            ANDI.W      #bits210,D4         Isolate the register bits
            MOVE.W      D7,D3               Copy the current word to D3
            ANDI.W      #bits543,D3         Isolate the mode bits
            LSR.W       #$3,D3              Shift D3 to the right by 3 bits
            JSR         EA                  Call EA
            BRA         DISPLAY             Branch to DISPLAY
*-----------End QUICK---------------------------------------------------------------        
  
*-----------Start DISPLAY------------------------------------------------------------------------
*-Display the address, the instruction, and the operands; checks the end address with the current one
DISPLAY     MOVE.B      #0,(A1)+            NULL terminate the OUT_BUF
            
*           Display the string holding the memory address
            MOVEA       A1,A2               Store A1 into a temp address
            LEA         MEM_ADDR,A1         Load the starting address of MEM_ADDR into A1
            MOVE.B      #14,D0              Copy 14 to D0
            TRAP        #15                 Display the string in A1
            
*           Display the string holding the instruction                     
            LEA         OUT_BUF,A1          Load the starting address of OUT_BUF into A1
            MOVE.B      #13,D0              Copy 13 to D0
            TRAP        #15                 Display the string in A1

*           Clear the output string    
            MOVEA       A2,A1               Get A1 back from the the temp register
            JSR         CLR_BUF             Call CLR_BUF

*           Increase the page iterator
            ADDA.L      #$1,A4              Add one to A4
            CMPA.L      #10,A4              If (A4 == 10) {
            BEQ         DISP_PAUSE              Branch to DISP_PAUSE }
            BRA         DISP_CONT           Else Branch to DISP_CONT

*           *Ask the user for the next page
DISP_PAUSE  LEA         PROMPT_CONT,A1      Store PROMPT_CONT in A1
            MOVE.B      #14,D0              Copy 14 to D0
            TRAP        #15                 Display the string at A1
            MOVE.B      #5,D0               Copy 2 to D0
            TRAP        #15                 Get user input
            MOVEA.L     #$0,A4              Clear A4
            BRA         DISP_CONT           Branch to DISP_CONT
            
            *Check the A5 with the end address
DISP_CONT   MOVEA.L     END_ADDR,A6         Copy END_ADDR to A6
            CMPA.L      A6,A5               If (A5 <= A6) {
            BLE         MAIN_LOOP               Branch to MAIN_LOOP } 
            BRA         DONE                Else { Branch to DONE }
*-----------End DISPLAY------------------------------------------------------------------------

*-----------Start GET_INPUT-------------------------------------------------------------
*-Description:      Displays the string stored in A1, accepts a number from the user, 
*                       converts that number into a valid hex number, and stores that 
*                       number in A1
*-Preconditions:    A1 is preloaded with a null terminated string
*-Postconditions:   The number inputed is stored into A1
*-Used Registers:   A1 - Holds a null terminated string to display and returns the 
*                       user input
*                   D2 - Temp register
*                   D3 - Stores the user input
GET_INPUT   MOVE.B      #14,D0              Copy 14 to D0
            TRAP        #15                 Display the string at A1

            LEA         TEMP_ADDR,A1
            MOVE.B      #2,D0               Copy 2 to D0
            TRAP        #15                 Get user input
            
LOOP2       MOVE.B      (A1)+,D2            Copy the byte in A1 and store it in D2
            JSR         CHECK_VALID         Call CHECK_VALID
            SUBI.B      #$30,D2             Subtract $30 from D2
            ADD.L       D2,D3               Add whatever is in D2 to D3
            CMPI.B      #$0,(A1)            If ((A1) == 0) {
            BEQ         END_LOOP2               Branch to END_LOOP2 }
            LSL.L       #$4,D3              Shift D3 to the left by 4 bits
            BRA         LOOP2               Branch to LOOP2
  
END_LOOP2   MOVEA.L     D3,A1               Copy D3 to A1
            CLR.L       D3                  Set D3 to 0
            CLR.L       D2                  Set D3 to 0
            RTS                             Return

CHECK_VALID CMPI        #$41,D2             If (D2 >= 41) {
            BGE         INPUT_GE                Branch to INPUT_GE }
            RTS                             Return
INPUT_GE    SUBI.B      #$7,D2              Subtract 7 from D2
            RTS                             Return
*-----------End GET_INPUT---------------------------------------------------------------

*-----------Start GETSIZE---------------------------------------------------------------
*-Description:      Checks bits 7,6; determines the size based on those bits; and adds it to the output string
*                       Puts a one in D5 if it is a long else make it a zero
*-Preconditions:    D5 is preloaded with the size 
*-Postconditions:   D5 is a 1 if it was a long, 0 if it was a word
*                       The size is added to the output string
*-Used Registers:   D5 - Used to hold the size                                          
*                   A1 - Size is added to it                                            
GETSIZE     MOVE.B      #'.',(A1)+                  Add a period to A1
                                                                                            
            CMPI        #$0,D5              If (D5 == #$0000) {                 
            BEQ         SIZEB                   Branch to SIZE00 }              
            CMPI        #$1,D5              Else If (D5 == #$0001) {            
            BEQ         SIZEW                   Branch to SIZE01 }              
            CMPI        #$2,D5              Else If (D5 == #$0002) {            
            BEQ         SIZEL                   Branch to SIZE10 }              
            BRA         DATA                Else { Branch to DATA }             
                                                                                            
SIZEB       MOVE.B      #'B',(A1)+          Add 'B' to A1 and increment it     
            MOVE.B      #$0,D5              Store a 0 in D5
            BRA         SIZEDONE            Return to where GETSIZE was called  
SIZEW       MOVE.B      #'W',(A1)+          Add 'W' to A1 and increment it 
            MOVE.B      #$0,D5              Store a 0 in D5
            BRA         SIZEDONE            Return to where GETSIZE was called 
SIZEL       MOVE.B      #'L',(A1)+          Add 'L' to A1 and increment it
            MOVE.B      #$1,D5              Store a 1 in D5
SIZEDONE    MOVE.B      #' ',(A1)+          Add a space to A1
            RTS
*-----------End GETSIZE-----------------------------------------------------------------       
            
*-----------Start CLR_BUF---------------------------------------------------------------          
*-Description:      Clears the buffer and resets A1 to OUT_BUF--------------------------
*-Preconditions:    The address in A1 is more than OUT_BUF
*-Postconditions:   The output string is cleared
*-Used Registers:   A1 - The location of the output string 
CLR_BUF     MOVE.B      #$FF,-(A1)          Store FF into A1
            CMPA.L      #OUT_BUF,A1         If (A1 != OUT_BUF) {
            BNE         CLR_BUF                 Branch to CLR_BUF }
            RTS                             Else RTS
*-----------End CLR_BUF-----------------------------------------------------------------

*-----------Start EA--------------------------------------------------------------------
*-Preconditions:    D3 is loaded with EA mode, D4 is loaded with EA register, can't use D6 in here
*-Postconditions:   The register and the mode is added to the output string
*-Used Registers:   D3 - Preloaded with the EA mode
*                   D4 - Preloaded with the EA register
*                   D5 - Preloaded with the size of the function; 1 for long, 0 for word or byte
*                   A1 - The address of the output string
*           Branches to a function based on the mode in D3                    
EA          CMP.B       #$0,D3              If (D3 == 0) {
            BLE         DATA_DIR                Branch to DATA_DIR }
            CMP.B       #$1,D3              Else If (D3 == 1) {
            BLE         ADDY_DIR                Branch to ADDY_DIR }
            CMP.B       #$2,D3              Else If (D3 == 2) {
            BLE         ADDY_INDIR              Branch to ADDY_INDIR }
            CMP.B       #$3,D3              Else If (D3 == 3) {
            BLE         ADDY_INC                Branch to ADDY_INC }
            CMP.B       #$4,D3              Else If (D3 == 4) {
            BLE         ADDY_DEC                Branch to ADDY_DEC }
            CMP.B       #$7,D3              Else If (D3 == 7) {
            BLE         DATA_MODES              Branch to DATA_MODES }
            BRA         DATA                Else Branch to DATA

*           Adds the data register to the output string
DATA_DIR    MOVE.B      #'D',(A1)+          Add 'D' to the output string
            JSR         PICK_REG            Call PICK_REG
            CLR.L       D3                  Zero out D3
            RTS                             Return

*           Adds the address register to the output string             
ADDY_DIR    MOVE.B      #'A',(A1)+          Add 'A' to the output string
            JSR         PICK_REG            Call PICK_REG
            CLR.L       D3                  Zero out D3
            RTS                             Return

*           Adds the indirect address register to the output string
ADDY_INDIR  MOVE.B      #'(',(A1)+          Add '(' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            JSR         PICK_REG            Call PICK_REG
            MOVE.B      #')',(A1)+          Add ')' to the output string
            CLR.L       D3                  Zero out D3
            RTS                             Return

*           Add the indirect address with post increment to the output string
ADDY_INC    MOVE.B      #'(',(A1)+          Add '(' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            JSR         PICK_REG            Call PICK_REG
            MOVE.B      #')',(A1)+          Add ')' to the output string    
            MOVE.B      #'+',(A1)+          Add '+' to tht output string
            CLR.L       D3                  Zero out D3
            RTS                             Return

*           Add the indirect address with pre decrement to the output string
ADDY_DEC    MOVE.B      #'-',(A1)+          Add '-' to the output string
            MOVE.B      #'(',(A1)+          Add '(' to the output string
            MOVE.B      #'A',(A1)+          Add 'A' to the output string
            JSR         PICK_REG            Call PICK_REG
            MOVE.B      #')',(A1)+          Add ')' to the output string
            CLR.L       D3                  Zero out D3
            RTS                             Return 

*           Adds the immidiate data to the output string
DATA_MODES  CLR.L       D3                  Zero our D3
            CMPI        #$0,D4              If (D4 == 0) {
            BEQ         ABS_W                   Branch to ABS_W }
            CMPI        #$1,D4              Else If (D4 == 1) {
            BEQ         ABS_L                   Branch to ABS_L }
            CMPI        #$4,D4              Else If (D4 == 4) {
            BEQ         IMM_DATA                Branch to IMM_DATA }
            BRA         DATA                Else Branch to DATA

*           Adds the nect word in memory to the output string
ABS_W       MOVE.B      #'#',(A1)+          Add '#' to the output string
            MOVE.B      #'$',(A1)+          Add '$' to the output string
            MOVE.W      (A5)+,D5            Get the next word from memory
            JSR         REG_TO_STR          Call REG_TO_STR
            RTS                             RETURN

*           Adds the next two words in memory to the output string
ABS_L       MOVE.B      #'#',(A1)+          Add '#' to the output string
            MOVE.B      #'$',(A1)+          Add '$' to the output string
            MOVE.L      (A5)+,D4            Get the next long in memory
            SWAP        D4                  Swap the words in D4
            MOVE.W      D4,D5               Copy D4 to D5
            JSR         REG_TO_STR          Call REG_TO_STR
            SWAP        D4                  Swap the words in D4
            MOVE.W      D4,D5               Copy D4 to D5
            JSR         REG_TO_STR          Call REG_TO_STR
            CLR.L       D4                  Zero out D4
            RTS                             Return

*           Adds the immediate data to the output string
IMM_DATA    MOVE.B      #'#',(A1)+          Add '#' to the output string
            MOVE.B      #'$',(A1)+          Add '$' to the output string
            MOVE.B      D5,D4               Copy D5 to D4
            MOVE.W      (A5)+,D5            Get the next word in memory
            JSR         REG_TO_STR          Call REG_TO_STR
            CMPI        #$0,D4              If (D4 == 0) {
            BEQ         IMM_RETURN              Branch to IMM_RETURN }
            MOVE.W      (A5)+,D5            Get the next word in memory
            JSR         REG_TO_STR          Call REG_TO_STR
IMM_RETURN  RTS                             Return
*-----------End EA--------------------------------------------------------------------

*-----------Start PICK_REG------------------------------------------------------------
*-Description:      Pushes a number of a register to the output string
*-Preconditions:    D4 is loaded with a number from 0-7
*-Postconditions:   The number in D4 is added to the output string
*-Used Registers:   A1 - Current address of the output string
*                   D4 - Register that holds the number to push
PICK_REG    ADD.B       #$30,D4             Add D4 with $30
            MOVE.B      D4,(A1)+            Add the contents of D4 to the output string     
            RTS                             Return 
           
            
*-----------Start REG_TO_STR------------------------------------------------------------
*-Description:      Takes a word in D5 and puts it in a string at A1
*-Preconditions:    D3 must not be used by the calling function
*-Postconditions:   The contents of D5 will now be on the output string at A1 
*-Used Registers:   D5 - The value to put in the string is stored here
*                   D3 - Iterator for the loop 
*                   D2 - Temp register
REG_TO_STR  CLR         D3                  Set D3 to 0

START_LOOP0 CMPI        #4,D3               If (D3 == 4) {
            BEQ         DONE_LOOP0              Branch to DONE_LOOP0 }
            
            ROL.W       #$4,D5              Rotate the nibble to push to the last nibble
            MOVE.W      D5,D2               Copy D5 to D2
            ANDI.W      #bits3210,D2        Isolate the last nibble in D2
            JSR         PUSH_CHAR           Call PUSH_CHAR
            ADDI        #1,D3               Increase the iterator by 1
            BRA         START_LOOP0         Branch to START_LOOP0
            
DONE_LOOP0  CLR         D2                  Set D2 to 0
            CLR         D3                  Set D3 to 0
            CLR         D5                  Set D5 to 0
            RTS                             Return
*-----------End REG_TO_STR--------------------------------------------------------------
            
*-----------Start ADDR_TO_STR-----------------------------------------------------------
*-Description:      Takes the address stored in A5 and stores it in a string at A1
*-Preconditions:    D5 must be loaded with the current address
*                   D3 must not be used by the calling function
*-Postconditions:   The current address will be put on th output string
*                   D3 will be zeroed out
*-Used Registers:   A5 - Not changed
*                   A1 - Incremented with each character added to it
*                   D3 - Iterator
*                   D2 - Temp register
*                   D6 - The value to put in the string
ADDR_TO_STR MOVE.L      A5,D6               Copy the address in A5 to D6
            CLR         D3                  Set D3 to 0

START_LOOP1 CMPI        #8,D3               If (D3 == 8) {
            BEQ         DONE_LOOP1              Branhc to DONE_LOOP1 }
            
            ROL.L       #$4,D6              Rotate the nibble to push to the last nibble
            MOVE.L      D6,D2               Copy D6 to D2
            ANDI.L      #$0000000F,D2       Isolate the last nibble in D2
            JSR         PUSH_CHAR           Call PUSH_CHAR
            ADDI        #1,D3               Increase the iterator by 1
            BRA         START_LOOP1         Branch to START_LOOP1
         
DONE_LOOP1  MOVE.B      #' ',(A1)+          Add ' ' to the output string
            MOVE.B      #0,(A1)+            Add null to the output string
            CLR         D2                  Set D2 to 0
            CLR         D3                  Set D3 to 0
            CLR         D6                  Set D6 to 0
            RTS                             Return
*-----------End REG_TO_STR--------------------------------------------------------------

*-----------Start PUSH_CHAR-------------------------------------------------------------
*-Description:      Takes a byte in D2 and pushes it onto the output string in ASCII
*-Preconditions:    The value in D2 must be lower than 16, otherwise it will RTS
*-Postcinditions:   The hex equivalant of D2 will be pushed to (A1)+
*-Used Regesters:   D2 - The value to be translated and pushed
*                   A0 - The address of the array holding CHAR_TABLE
*                   A1 - Location of the current output string
PUSH_CHAR   CMPI        #$10,D2                 If (D2 >= $10) {
            BGE         DONE_PUSH                   Branch to DONE_PUSH }
            LEA         CHAR_TABLE,A0           Store the address of CHAR_TABLE to A0
            ADDA.W      D2,A0                   Add the value in D2 to the CHAR_TABLE
            MOVE.B      (A0),(A1)+              Move the value at that index to (A1)+ 
DONE_PUSH   RTS                                 Return 
*-----------End PUSH_CHAR---------------------------------------------------------------  

*-----------Start IO_ERR----------------------------------------------------------------
*-Descriptions:     Displays the error message and branches back to the beginning
*-Preconditions:    None
*-Postconditions:   Program will go back to the beginning
*-Used Registers:   A1 - Holds the address of the string being displayed
*                   D0 - Hold the trap task to be executed
IO_ERR      LEA         ERR_MSG,A1          Load the address of ERR_MSG to A1
            MOVE.B      #14,D0              Store 14 in D0
            TRAP        #15                 Display the string at A1
            BRA         IO_START            Branch to IO_START
*-----------End IO_ERR------------------------------------------------------------------

*-----------Start INPUT_CHECK-----------------------------------------------------------
*-Description:      Checks A1 to see if it is a valid input, if not branch to IO_ERR
*-Preconditions:    The input must be loaded into A1
*-Postconditions:   None
*-Used Registers:   A1 - The input that is checked
*                   D0 - Temp register
INPUT_CHECK 
*           Checks if the input is negative
            CMPA        #0,A1                   If (A1 < 0) {
            BLT         IO_ERR                      Branch to IO_ERR }
            
*           Checks if the input is greater than $00FFFFFF
            CMPA.L      #$FFFFFF,A1             If (A1 >= $FFFFFF) {
            BGE         IO_ERR                      Branch to IO_ERR }
            
*           Checks if the input is odd            
            MOVE.L      A1,D0                   Copy the input to D0
            ANDI.L      #1,D0                   Isolate the last bit
            CMPI        #1,D0                   If (D0 == 1) {
            BEQ         IO_ERR                      Branch to IO_ERR }
            
            RTS                                 Return
*-----------End INPUT_CHECK-------------------------------------------------------------

*-----------Start DONE------------------------------------------------------------------       
DONE        LEA     END_MSG,A1
            MOVE.B  #14,D0
            TRAP    #15
*-----------End DONE--------------------------------------------------------------------
*-----------Variables and Constants-----------------------------------------------------
STRT_ADDR   DS.L        1
END_ADDR    DS.L        1
TEMP_ADDR   DS.L        1
OUT_BUF     DS.B        80
MEM_ADDR    DS.B        12

SHIFT_COUNT DC.B        '8','1','2','3','4','5','6','7'
CHAR_TABLE  DC.B        '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
AD_CHOOSE   DC.B        'A','D'
MOVEM_REV   DC.W        'A7','A6','A5','A4','A3','A2','A1','A0','D7','D6','D5','D4','D3','D2','D1','D0'
MOVEM_FOR   DC.W        'D0','D1','D2','D3','D4','D5','D6','D7','A0','A1','A2','A3','A4','A5','A6','A7'
END_MSG     DC.B        'Finished disassembling. Have a nice day.',0
ERR_MSG     DC.B        'An I/O error has occured. Please check your inputs.',CR,LF,0
PROMPT_STRT DC.B        'Enter the starting address in hexadecimal: ',CR,LF,0
PROMPT_END  DC.B        'Enter the ending address in hexadecimal: ',CR,LF,0
PROMPT_CONT DC.B        'Press enter to continue',CR,LF,0
    END    $1000





*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~4~
