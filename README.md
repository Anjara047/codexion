*This project has been created as part of the 42 curriculum by tsanjara.*

# CODEXION

## DESCRIPTION

This project is part of 42 Curriculum  in the common core, it is based on C language that simulates a group of coders sharing limited dongles in each side of each coders, by means the numbers of coders is the same as the number of dongles

routine:  
Each coders follows the following routine:  
&emsp; * compiles  
&emsp; * debugg  
&emsp; * refactores  
It requires two implementation in general:  
&emsp;* FIFO (First In, First Out): The coders which is first in the heap will process the routine first, and the out so that  the next coders in the heap will compile and always like that untill the required parameters will have finished what they need.  
&emsp; * EDF(Earliest Deadline First): Each coders with the earliest deadline in the coders will process first, and after that out from the heap and the next coders follows the same routine.

### GOAL
&emsp; In general, the main of this project is to manage multiple coders that shared the same ressources(we say dongles here) while and ensures that each coders can compile their work without reaching the burnout

`BURNOUT`: The state of a coder who fails to start a new compilation within the allotted time because they cannot access the required resources in time

## INSTRUCTIONS

There is already Makefile in the root of the repository to facilatate all the work to compile the program, so just the user only needs to to run the following command to run the code.  

`make`: to compile the program at the very first the user want to check the program
`make re`: to recompile all the program and create the executable and remove all the files.o just newly created.
`make clean`: remove all the files.o
`make fclean`: remove the executable files that contains the binary

./codexion 5 800 200 100 100 1 0 fifo
./codexion 5 800 200 100 100 2 0 fifo
./codexion 5 800 200 100 100 3 0 fifo

./codexion 2 2000 200 100 100 2 0 fifo
./codexion 3 2000 200 100 100 2 0 fifo
./codexion 4 2000 200 100 100 2 0 fifo
./codexion 5 2000 200 100 100 2 0 fifo
./codexion 7 2000 200 100 100 2 0 fifo

./codexion 5 2000 200 100 100 2 50 fifo
./codexion 5 2000 200 100 100 2 200 fifo
./codexion 5 2000 200 100 100 2 500 fifo

./codexion 1 200 100 100 100 2 0 fifo
./codexion 5 100 200 100 100 2 0 fifo
./codexion 5 250 200 100 100 2 0 fifo
./codexion 5 800 200 100 100 2 0 fifo

./codexion 5 800 200 100 100 2 0 edf
./codexion 5 800 200 100 100 2 100 edf
./codexion 7 1200 200 100 100 3 50 edf