# MPI-QuickSort
To Sort given numbers by quick sort and parallelizing it by MPI.  
generator.c is used to generate N random numbers and write them in file random.txt. N is asked from user. It runs in one processor only.   
Code is parallelized in sort.c. Just for the sake of similarity, generator.c is also converted to a MPI code.   
To compile:
```
mpicc -o generator generator.c
```
To run:
```
mpiexec -n 1 ./generator
```
Where n is the number of processors. n can take any positive integer value.  
sort.c reads the data from random.txt and sorts it and writes back the sorted data in sorted.txt. Best results will come when n = 4 is given while running sort.c  
To compile:
```
mpicc -o sort sort.c
```
To run
```
mpiexec -n 4 ./sort
```
