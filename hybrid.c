#include "mpi.h"
#include <omp.h>
#include <stdio.h>
#include<stdlib.h>

/* UNIT 14 - Combining MPI and OpenMP 
 * Example showing how to combine MPI with OpenMP in the same program.
 * Note - MPI_Init() has been replaced with a call to MPI_Init_thread(). See 
 * the details in the code as to what this actually means and why we do it
 */

int main(int argc , char* argv[]) 
{
	int nprocs, myrank;
       
/* ===============================================================================
 * When combining MPI with OpenMP then we should make use of the MPI
 * initialisation function MPI_Init_thread() rather than MPI_Init().
 * The MPI_Init_thread() initialises MPI in the same way as MPI_Init()
 * and in addition it initialises the thread environment. 
 *
 * The prototype for this function is as follows:
 *
 * int MPI_Init_thread (int *argc, char ***argv, int required, int *provided);
 *
 * The additional arguments
 * int required -  is used to specify the desired level of thread support.
 *                 The possible values are
 *                 MPI_THREAD_SINGLE -  Only one thread will execute.
 *                 MPI_THREAD_FUNNELED -  The process may be multi-threaded, 
 *                                        but the application must ensure that 
 *                                        only the main thread makes MPI calls 
 *                                       (THIS IS WHAT WE WANT FOR THIS PROGRAM)
 *                 MPI_THREAD_SERIALIZED The process may be multi-threaded, and 
 *                                       multiple threads may make MPI calls, 
 *                                       but only one at a time: MPI calls are 
 *                                       not made concurrently from two distinct
 *                                       threads i.e. all MPI calls are serialised.
 *                 MPI_THREAD_MULTIPLE -  Multiple threads may call MPI, with 
 *                                        no restrictions.
 *
 * int *provided  - contains the information about the level of thread support
 *                  that will be provided by MPI. It can be one of the 4 values
 *                  above
 *  NOTE: 
 * These values are monotonic; i.e., MPI_THREAD_SINGLE < MPI_THREAD_FUNNELED <
 * MPI_THREAD_SERIALIZED < MPI_THREAD_MULTIPLE.
 *
 * ==============================================================================*/

        /* The level of MPI thread support this version of MPI supports */
        int actual_thread_support; 

        /* Setup the THREADED MPI Environment */
	MPI_Init_thread(&argc,&argv, MPI_THREAD_FUNNELED, &actual_thread_support);

        /* Work out my MPI Rank */
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

        /* Work out the number of MPI tasks  */
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
        
        /* Demonstrate the level of thread supported by this MPI Implementation */
        if (myrank == 0){
                switch (actual_thread_support){
                case MPI_THREAD_SINGLE:
                  printf ("Support provided by this version of MPI = MPI_THREAD_SINGLE\n");
                   break;
                case MPI_THREAD_FUNNELED:
                  printf ("Support provided by this version of MPI = MPI_THREAD_FUNNELED\n");
                   break;
                case MPI_THREAD_SERIALIZED:
                  printf ("Support provided by this version of MPI = MPI_THREAD_SERIALIZED\n");
                   break;
                case MPI_THREAD_MULTIPLE:
                  printf ("Support provided by this version of MPI = MPI_THREAD_MULTIPLE\n");
                   break;
                default: 
                   printf ("Error no MPI Threaded support\nAborting...\n");
                   MPI_Abort(MPI_COMM_WORLD,-1);
                   break;
                }
        }    


	printf("P%d:Hello I am MPI process with a rank of %d of %d\n",myrank,myrank,nprocs);
	printf("P%d:Starting my OpenMP threads\n", myrank);

	int tid = 0;  
	int n_of_threads = 1;
	#pragma omp parallel firstprivate(myrank) private(tid, n_of_threads) 
	{
                n_of_threads = omp_get_num_threads();
	        tid = omp_get_thread_num();
 
               
		printf("P%d: I am thread#%d of %d threads belonging to MPI Process with a rank of  %d\n", myrank,
                        tid, n_of_threads, myrank);
                #pragma omp master
                {
		printf("P%d: I am  the master thread, thread#%d\n", myrank,
                        tid );
                }

               

	}
 

        /* Now back to just the MPI process running */


	MPI_Finalize();

	return 0;
}
