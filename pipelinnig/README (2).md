


# Pipeline Stall Detector/Simulator

This program is designed to read assembly language instructions  from a file, convert them into standard assembly language , and print the resulting assembly instructions to the console with NOP added where ever it is needed. It supports different instruction types and can handle R-type, I-type instructions.

## Prerequisites

Before you can use this program, make sure you have the following:

- A C compiler (e.g., GCC) installed on your system.
- A text file containing machine code instructions. The instructions can be in any machine supported instruction with one instruction per line.
-Name the file inputs.txt.

## How to Use

1. Place your machine code instructions in a text file. Each instruction should be on a separate line in assembly language eg
   ```c
    add t0, t1, t2

2. Update the `filename` variable in the `main` function of the code with the name of your instructions file:

   ```c
   const char *filename = "inputs.txt";
3. Compile the program using a C compiler. For example, using GCC:

     ```c
   gcc your_code.c -o your_program


4. Run the compiled program:

      ```c
     ./your_program


5. The program will read the instructions from the file, convert them to standard instructions, and print the results  to the console.



## Outputs
Your program will generate the following outputs:

-The converted input instructions will be saved to a file named convertedInputs.txt.

-The program will also display matrices coreesponding to no data forwarding as no forwarding and with data forwarding as with forwarding along with given file inputs,their converted versions into standard x type with temperoray added to name. 
-These  outputs are printed in  the terminal.



## Error Handling
Your program includes error handling for file operations. If there are any issues, error messages will be displayed on the terminal.

That's it! You should be able to compile and execute your C code using these instructions. If you encounter any issues or errors, please ensure that you have the necessary libraries installed and that the input files are in the correct location.

## Contributions
Contributions to this program are welcome. If you want to add support for more instruction types or enhance its functionality, feel free to make pull requests.

## License
This program is open-source .
