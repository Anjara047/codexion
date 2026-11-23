*This project has been created as part of the 42 curriculum by tsanjara.*

# CODEXION

## DESCRIPTION

Codexion is a concurrency project developed as part of the 42 Common Core curriculum using the C programming language.

It simulates a group of coders sharing a limited number of resources, called **dongles**. Each coder requires two adjacent dongles to perform a compilation.

The number of dongles is equal to the number of coders.

### Routine

Each coder follows the same routine:

- **Compile:** Compile their work using two adjacent dongles.
- **Debug:** Debug their code.
- **Refactor:** Refactor their code before starting another compilation cycle.

### Scheduling Algorithms

The project implements two scheduling algorithms:

- **FIFO (First In, First Out):** Coders are selected according to their arrival order in the scheduling queue. The first coder in the queue is selected before the next one.

- **EDF (Earliest Deadline First):** Coders are selected according to their earliest compilation deadline. The coder with the earliest deadline is selected first.

### GOAL

The main goal of this project is to manage multiple concurrent coders sharing limited resources while ensuring that they can complete their compilation cycles without reaching burnout.

**BURNOUT:** The state of a coder who fails to start a new compilation within the allotted time because they cannot access the required resources in time.

## INSTRUCTIONS

A `Makefile` is provided at the root of the repository to simplify compilation and execution.

The following commands are available:

- `make`: Compile the project and generate the executable.
- `make re`: Recompile the project from scratch.
- `make clean`: Remove object files (`.o`).
- `make fclean`: Remove object files and the executable.

This project requires eight argument, these must must be present during the execution so that it can never compile if one of them is missing
So after running the project via `make`, the project can be tested with:  
&emsp;&emsp; `./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler`

- `number_of_coders`: the numbers of coders needed during the compilation, it is really important to remind that the number of coders cannot be neither invalid integer, nor 0 nor 1
&emsp; 0 means no coder to compile
&emsp; 1 means that there won't be 2 dongles required for the compilation
- `time_to_burnout`: the maximum time(in millisiconds) a coder can wait before starting a new compilation. If the coder fails to start compiling within this time limit, they reach the `BURNOUT` state
- `time_to_compile`: the time to compile for each coders during compilation
- `time_to_debug`: the time required to debbugging for each coders
- `time_to_refactor`: the time that is needed to refactoring for each coder as well
- `time_to_compiles_required`: each coders can compile multiple times as much as the required number of compilation
- `dongle_cooldown`: the minimun time a dongle must remain unavailable after realeased by the coder before it can be acquired again by another coder
