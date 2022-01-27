# MOS-Phase2
## Design Multiprogramming Operating System using C++

Input file - consists jobs and treated it as card reader

Output file - consists output of the job

Jobs may have program errors

Main memory = 300 words (30 blocks)

Paging introduced, page table stored in real memory

Program pages allocated one of 30 memory block using random number generator 

Load and run one program at a time


### NOTATION

M: memory

IR: Instruction Register (4 bytes)

IR [1, 2]: Bytes 1, 2 of IR/Operation Code

IR [3, 4]: Bytes 3, 4 of IR/Operand Address 

M[&]: Content of memory location &

IC: Instruction Counter Register (2 bytes)

R: General Purpose Register (4 bytes)

C: Toggle (1 byte)

PTR: Page Table Register (4 bytes)

PCB: Process Control Block (data structure)

VA: Virtual Address

RA: Real Address

TTC: Total Time Counter

LLC: Line Limit Counter

TTL: Total Time Limit

TLL: Total Line Limit

EM: Error Message


### INTERRUPT VALUES

SI = 1 on GD

   = 2 on PD
   
   = 3 on H
   
TI = 2 on Time Limit Exceeded

PI = 1 Operation Error

   = 2 Operand Error
   
   = 3 Page Fault
   
   
### Error Message Coding:

0 No Error

1 Out of Data

2 Line Limit Exceeded

3 Time Limit Exceeded

4 Operation Code Error

5 Operand Error

6 Invalid Page Fault 


### MOS (MASTER MODE)
#### Case TI and SI of 
TI SI Action

0  1  READ

0  2  WRITE

0  3  TERMINATE (0)

2  1  TERMINATE (3)

2  2  WRITE, THEN TERMINATE (3)

2  3  TERMINATE (0)


#### Case TI and PI of 
TI PI Action

0  1  TERMINATE (4)

0  2  TERMINATE (5)

0  3  If Page Fault Valid, ALLOCATE, update page Table, Adjust IC if necessary, 
      EXECUTE USER PROGRAM OTHERWISE TERMINATE (6)
       
2  1  TERMINATE (3,4)

2  2  TERMINATE (3,5)

2  3  TERMINATE (3)
