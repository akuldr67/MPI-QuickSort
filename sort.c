#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void main(int argc, char *argv[]){
	long n = 0;
	char c;
	FILE *fp;
	double elapsed_time;
	int numprocs, myid,Root=0;
	long remaining_first,local_n,my_first,remaining_length;
	MPI_Status status; 

	void swap(long*, long*);
	long partition(long[], long, long);
	void quickSort(long[], long, long);


	/*....MPI Initialisation....*/
	MPI_Init(&argc, &argv);
	MPI_Barrier(MPI_COMM_WORLD);
	elapsed_time = - MPI_Wtime();
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);


	if(myid == Root){
		fp = fopen("random.txt","r");
		//counting number of lines present (and hence no of numbers)
		if(fp!=NULL){
			for (c = getc(fp); c != EOF; c = getc(fp)){
	        	if (c == '\n')
	            	n = n + 1; 
	    	}
		}
		fclose(fp);
	}


	MPI_Bcast(&n, 1, MPI_LONG, Root, MPI_COMM_WORLD);

	long array[n+1];
	long local_a[(n/numprocs)];
	long arrayGather[numprocs][n/numprocs];

	local_n = n/numprocs;
	my_first = myid * n/numprocs;
	remaining_first = local_n*numprocs;
	remaining_length = n-remaining_first;
	long remained[remaining_length];

	if(myid == Root){
		fp = fopen("random.txt","r");

		//scanning numbers and storing them in a array
		if(fp!=NULL){
			for(long i=0;i<n;i++){
				fscanf(fp,"%ld",&array[i]);
			}
		}
		fclose(fp);

		//printing inital unsorted array
		// printf("Initial array: \n");
		// for(int i =0;i<n;i++){
		// 	printf("%d ", array[i]);
		// }
		// printf("\n\n");
	}

	MPI_Scatter(array,local_n,MPI_LONG,local_a,local_n, MPI_LONG,Root,MPI_COMM_WORLD);
	// printf("array scattered to processors %d \n",myid);

	//calling recursive function to sort array
	quickSort(local_a,0,local_n-1);
	// printf("local array sorted in processor %d\n",myid);


	if(myid==Root){
		for(long i=remaining_first,j=0;i<n;i++,j++){
			remained[j]=array[i];
		}
		quickSort(remained,0,remaining_length-1);
		// printf("remaining array sorted in processor %d\n",myid);
	}

	// MPI_Gather(&arrayGather, local_n, MPI_LONG, &local_a, local_n, MPI_LONG, Root, MPI_COMM_WORLD);
	// printf("array gathered! , processors %d \n",myid);

	// if(myid == Root){
	// 	for(int i=0;i<numprocs;i++){
	// 		for(long j=0;j<local_n;j++)
	// 			printf("%d ", arrayGather[i][j]);
	// 		printf("\n");
	// 	}
	// 	printf("\n");
	// }



	if(myid != Root){
		MPI_Send(&local_a, local_n, MPI_LONG, Root, myid, MPI_COMM_WORLD); 
	}
	if(myid == Root){
		for(long i=0;i<local_n;i++){
			arrayGather[0][i] = local_a[i];
		}

		long temp[local_n];
		for(int i=1;i<numprocs;i++){
			MPI_Recv(temp, local_n, MPI_LONG, i, i, MPI_COMM_WORLD, &status);
			for(long j=0;j<local_n;j++){
				arrayGather[i][j] = temp[j];
			}
		}
	}


	if(myid == Root){
		long iterators[numprocs+1];
		for(int i=0;i<numprocs+1;i++)
			iterators[i]=0;

		long count = 0;
		while(count<n){
			long minVal = 99999999,procNo = 0;
			for(int i=0;i<numprocs;i++){
				if(iterators[i]<local_n && minVal>=arrayGather[i][iterators[i]]){
					minVal = arrayGather[i][iterators[i]];
					procNo = i;
				}
			}
			if(iterators[numprocs]<remaining_length && remained[iterators[numprocs]]<minVal){
				array[count] = remained[iterators[numprocs]];
				iterators[numprocs]++;
			}
			else{
				array[count] = minVal;
				iterators[procNo]++;
			}
			count++;
		}
	}

	//printing the sorted array
	if(myid == Root){
		// printf("Sorted array: \n");
		// for(long i =0;i<n;i++){
		// 	printf("%d ", array[i]);
		// }
		// printf("\n\n");

		//writing the sorted array in a file named sorted.txt
		fp = fopen("sorted.txt","w+");
		for(long i=0;i<n;i++){
			fprintf(fp, "%ld\n", array[i]);
		}
		fclose(fp);

		printf("done\n");
		elapsed_time+=MPI_Wtime();
		printf ("Time taken = %f\n", elapsed_time);
	}
	MPI_Finalize();
}


void swap(long* a, long* b) 
{ 
    long t = *a; 
    *a = *b; 
    *b = t; 
} 
  
 //always last element of the given array is picked as a pivot
 //after calling partition, elements before the returned index is less than pivot, and after are greater than pivot 
long partition (long arr[], long low, long high) 
{ 
    long pivot = arr[high];    // pivot 
    long i = (low - 1);  // Index of smaller element 
  
    for (long j = low; j <= high- 1; j++) 
    { 
    	//intra loop dependency here
        if (arr[j] < pivot) // If current element is smaller than the pivot 
        { 
            i++;    // increment index of smaller element 
            swap(&arr[i], &arr[j]); 
        } 
    } 
    swap(&arr[i + 1], &arr[high]); 
    return (i + 1); 
} 
  
void quickSort(long arr[], long low, long high) 
{ 
    if (low < high) 
    {      
       long pi = partition(arr, low, high); // pi is partitioning index, arr[p] is now at right place

        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
} 

