# Operating-Systems-Practicum
Implementation of a few Page Replacement Algorithms in C++, namely FIFO, LRU, LFU and Second Chance.
Outputs GNU plots for comparison of peformance of the above algorithms for increasing No. of Frames. Precisely, outputs No. of Page Faults occurring v/s No. of Frames in a plot. 
##Compile Instructions
1. MakeFile
    -	$make
2.  Run the executable
    -	$./a.out
##Input
1. Input is read from the file "input" and has the following format:
  N = Size of the Matrices
  D = No. of integers accommodated by a single page in the memory
  Followed by start addresses for 3 Matrices of the order NxN. 

Reference string for paging is generated using standard matrix multiplications and memory accesses.
##Output
GNU Plots for the selected algorithm on STDOUT.
##Instructions
Maximum No. of Frames used for experimentation can be altered from main.cpp, default value 10.
##Note
If you encounter any error like "**** stack smashing detected ****", please re-run the code. It happens probably because of array indices being crossed beyond the boundaries while plotting.
