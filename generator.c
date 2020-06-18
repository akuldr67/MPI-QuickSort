#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <mpi.h>

void main(int argc, char *argv[]){
	long n = 10;
	double elapsed_time;
	int numprocs, myid,Root=0;


	/*....MPI Initialisation....*/
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	elapsed_time = - MPI_Wtime();

	//taking input from user in Root
	if(myid == Root){
		//taking input from user
		printf("enter number of random numbers to generate: ");
		scanf("%ld",&n);

		FILE *fp;
		fp = fopen("random.txt","w+");

		//generating a random seed, so that each time different random numbers are generated
		srand(time(0));

		//generating and printing n random numbers between 0 to 999, this is done so that we can analyse result easily
		//no loop level dependency here.. can run parallely
		for(long i=0;i<n;i++){
			fprintf(fp, "%d\n", rand()%10000);
		}
		fclose(fp);

		printf("Done!\n");
		elapsed_time+=MPI_Wtime();
		printf ("Time taken = %f\n", elapsed_time);
	}
	MPI_Finalize();
}