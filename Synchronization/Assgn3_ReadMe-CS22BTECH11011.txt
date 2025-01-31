Instructions to Run the Program :

-> Follow these steps to run the parallel matrix multiplication program in C++:

Prerequisites :
-> Ensure you have a C++ compiler installed on your system.

Input format :
-> Input is given through a text file "inp.txt".
-> It will consist of three parameters: N, K, RowInc and the matrix.

Output format :
-> there will be an output generated on execution of each source file.
-> The output file will contain the resultant square matrix and the time taken by the respective algorithm to compute it.
-> "out_TAS.txt" is the output file generated by "Assgn3_Src-CS22BTECH11011_TAS.cpp".
-> "out_CAS.txt" is the output file generated by "Assgn3_Src-CS22BTECH11011_CAS.cpp".
-> "out_bounded_CAS.txt" is the output file generated by "Assgn3_Src-CS22BTECH11011_bounded_CAS.cpp".
-> "out_atomic_inc.txt" is the output file generated by "Assgn3_Src-CS22BTECH11011_atomic_inc.cpp".

Execution Steps :
-> Open a terminal or command prompt.
-> Navigate to the directory containing the source code "Assgn3_Src-CS22BTECH11011.cpp".
-> Compile the code using the following commands
 "g++ -o TAS Assgn3_Src-CS22BTECH11011_TAS.cpp -pthread"
 "g++ -o CAS Assgn3_Src-CS22BTECH11011_CAS.cpp -pthread"
 "g++ -o bounded_CAS Assgn3_Src-CS22BTECH11011_bounded_CAS.cpp -pthread"
 "g++ -o atomic_inc Assgn3_Src-CS22BTECH11011_atomic_inc.cpp -pthread"

 -> These commands compiles the codes and links the pthread library for multi-threading.

Run the Program :
-> Execute the compiled binary using the following commands
 "./TAS"
 "./CAS"    
 "./bounded_CAS"    
 "./atomic_inc"    

-> And the respective output files will be created.
   
Note :
-> Ensure that the input file `inp.txt` is in the same directory as the compiled binary.
-> Verify that you have the necessary permissions to execute the compiled binary.
-> If any issues arise during execution, ensure that the required dependencies are installed.
