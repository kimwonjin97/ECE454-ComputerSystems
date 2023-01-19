/*****************************************************************************
 * life.c
 * Parallelized and optimized implementation of the game of life resides here
 ****************************************************************************/
#define _GNU_SOURCE
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "life.h"
#include "util.h"

#pragma GCC diagnostic ignored "-Wchar-subscripts" 

#define SWAP_BOARDS( b1, b2 )  do { \
  char* temp = b1; \
  b1 = b2; \
  b2 = temp; \
} while(0)


pthread_barrier_t barrier;

const int total_threads = 16;


/*****************************************************************************
 * Helper function definitions
 ****************************************************************************/
typedef struct {
  int nrows;
  int ncols;
  int starting_row;
  int ending_row;
  int thread_id;
  int gens_max;
  char *inboard;
  char *outboard;
} ci_t __attribute__((aligned(32)));


void* threads_function(void* _info)
{
	ci_t* info = (ci_t*)_info;

	int nrows = info->nrows;
	int ncols = info->ncols;
	int starting_row = info->starting_row;
    int ending_row = info->ending_row;
	int thread_id = info->thread_id;
	int gens_max = info->gens_max;
	char *inboard = info->inboard;
	char *outboard = info->outboard;
    

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(thread_id, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    int curgen, row, col;
    for (curgen = 0; curgen < gens_max; ++curgen) 
    {
        memset(outboard + starting_row*ncols, 0, (ending_row-starting_row)*ncols);
        pthread_barrier_wait(&barrier); 

        for (row = starting_row; row < ending_row; ++row) 
        {
            for (col = 0; col < ncols; ++col) 
            {
                if (inboard[row * nrows + col]==5 || 
                    inboard[row * nrows + col]==6 || 
                    inboard[row * nrows + col]==7) 
                {
                    if (row == 0) 
                    {
                        if (col == 0) 
                        {
                            outboard[nrows*ncols-1] += 2;
                            outboard[nrows*(ncols-1)] += 2;
                            outboard[nrows*(ncols-1) + 1] += 2;
                            outboard[ncols-1] += 2;
                            outboard[col+1] += 2;
                            outboard[nrows + (ncols-1)] += 2;
                            outboard[nrows] += 2;
                            outboard[nrows + 1] += 2;
                        } else if (col == (ncols-1)) {
                            outboard[(nrows*ncols-1)-1] += 2;
                            outboard[(nrows*ncols-1)] += 2;
                            outboard[nrows*(ncols-1)] += 2;
                            outboard[(ncols-1) - 1] += 2;
                            outboard[0] += 2;
                            outboard[nrows + (ncols-1) - 1] += 2;
                            outboard[nrows + (ncols-1)] += 2;
                            outboard[nrows] += 2;
                        } else {
                            outboard[nrows*(ncols-1) + col - 1] += 2;
                            outboard[nrows*(ncols-1) + col] += 2;
                            outboard[nrows*(ncols-1) + col + 1] += 2;
                            outboard[col-1] += 2;
                            outboard[col+1] += 2;
                            outboard[nrows + col-1] += 2;
                            outboard[nrows + col] += 2;
                            outboard[nrows + col+1] += 2;
                        }
                    } else if (row == (nrows-1)) {
                        if (col == 0) 
                        {
                            outboard[(nrows - 2) * nrows + (ncols - 1)] += 2;
                            outboard[(nrows - 2) * nrows + 0] += 2;
                            outboard[(nrows - 2) * nrows + 1] += 2;
                            outboard[nrows*ncols-1] += 2;
                            outboard[nrows*(ncols-1) + 1] += 2;
                            outboard[(ncols - 1)] += 2;
                            outboard[0] += 2;
                            outboard[1] += 2;
                        } else if (col == (ncols - 1)) {
                            outboard[(nrows - 2) * nrows + (ncols - 1) - 1] += 2;
                            outboard[(nrows - 2) * nrows + (ncols - 1)] += 2;
                            outboard[(nrows - 2) * nrows + 0] += 2;
                            outboard[nrows*ncols-1 - 1] += 2;
                            outboard[nrows*(ncols-1) + 0] += 2;
                            outboard[(ncols - 1) - 1] += 2;
                            outboard[(ncols - 1)] += 2;
                            outboard[0] += 2;
                        } else {
                            outboard[(nrows - 2) * nrows + col - 1] += 2;
                            outboard[(nrows - 2) * nrows + col] += 2;
                            outboard[(nrows - 2) * nrows + col + 1] += 2;
                            outboard[(nrows-1) * nrows + col - 1] += 2;
                            outboard[(nrows-1) * nrows + col + 1] += 2;
                            outboard[col - 1] += 2;
                            outboard[col] += 2;
                            outboard[col + 1] += 2;
                        }
                    } else if (col == 0) {
                        outboard[(row-1) * nrows + (ncols - 1)] += 2;
                        outboard[(row-1) * nrows] += 2;
                        outboard[(row-1) * nrows + 1] += 2;
                        outboard[row * nrows + (ncols - 1)] += 2;
                        outboard[row * nrows + 1] += 2;
                        outboard[(row+1) * nrows + (ncols - 1)] += 2;
                        outboard[(row+1) * nrows] += 2;
                        outboard[(row+1) * nrows + 1] += 2;
                    } else if (col == (ncols - 1)) {
                        outboard[(row-1) * nrows + (ncols - 1)-1] += 2;
                        outboard[(row-1) * nrows + (ncols - 1)] += 2;
                        outboard[(row-1) * nrows] += 2;
                        outboard[row * nrows + (ncols - 1)-1] += 2;
                        outboard[row * nrows] += 2;
                        outboard[(row+1) * nrows + (ncols - 1)-1] += 2;
                        outboard[(row+1) * nrows + (ncols - 1)] += 2;
                        outboard[(row+1) * nrows] += 2;
                    } else {
                        outboard[(row-1) * nrows + col-1] += 2;
                        outboard[(row-1) * nrows + col] += 2;
                        outboard[(row-1) * nrows + col+1] += 2;
                        outboard[row * nrows + col-1] += 2;
                        outboard[row * nrows + col+1] += 2;
                        outboard[(row+1) * nrows + col-1] += 2;
                        outboard[(row+1) * nrows + col] += 2;
                        outboard[(row+1) * nrows + col+1] += 2;
                    }
                    outboard[row * nrows + col] |= 0x1;
                }
            }
        }
        pthread_barrier_wait(&barrier);
        SWAP_BOARDS(outboard, inboard);
    }
    return NULL;
}

/*****************************************************************************
 * Game of life implementation
 ****************************************************************************/
char*
game_of_life (char* outboard, 
	      char* inboard,
	      const int nrows,
	      const int ncols,
	      const int gens_max)
{
  return customized_game_of_life(outboard, inboard, nrows, ncols, gens_max);
}

void change_inboard(char *board, const int nrows, const int ncols) 
{
  int neighborCount = 0;

  int upper_roffset = ncols*((nrows - 1) - 1);
  int lower_roffset = 0;
  int roffset = -ncols;

  int rcol, lcol;
  for (int r = 0; r < nrows; ++r) 
  {
    upper_roffset += ncols;
    lower_roffset += ncols;
    roffset += ncols;
    
    lcol = (ncols - 1) - 1;
    rcol = 0;
    if (r == 1) 
    {
        upper_roffset = 0;
    } else if (r == (nrows - 1)) 
    {
        lower_roffset = 0;
    }
    for (int c = 0; c < ncols; ++c) 
    {
        ++lcol;
        ++rcol;
        if (c == 1) 
        {
            lcol = 0;
        } else if (c == (ncols - 1)) {
            rcol = 0;
        }
        neighborCount = (board[upper_roffset + lcol] & 0x1) << 1;
        neighborCount = neighborCount + ((board[upper_roffset + c] & 0x1) << 1 )
                                      + ((board[upper_roffset + rcol] & 0x1) << 1)
                                      + ((board[lower_roffset + lcol] & 0x1) << 1)
                                      + ((board[lower_roffset + c] & 0x1) << 1)
                                      + ((board[lower_roffset + rcol] & 0x1) << 1)
                                      + ((board[roffset + lcol] & 0x1) << 1)
                                      + ((board[roffset + rcol] & 0x1) << 1);
        if (neighborCount) board[roffset + c] |= neighborCount;
    }
  }
}


char* customized_game_of_life (char* outboard, 
                              char* inboard,
                              const int nrows,
                              const int ncols,
                              const int gens_max)
{
	pthread_t threads[total_threads-1];
    ci_t* core_info = (ci_t*)malloc(sizeof(ci_t*) * total_threads);
    
	if(pthread_barrier_init(&barrier, NULL, total_threads) != 0) {
    	return NULL;
    }
    change_inboard(inboard, nrows, ncols);
    

    for(int i=0; i<total_threads; ++i)
    {
        core_info[i].nrows = nrows;
        core_info[i].ncols = ncols;
        core_info[i].starting_row = (i*nrows)/total_threads;
        core_info[i].ending_row = ((i+1)*nrows)/total_threads;
		core_info[i].gens_max = gens_max;
		core_info[i].thread_id = i;
		core_info[i].inboard = inboard;
		core_info[i].outboard = outboard;


		if(i!=(total_threads-1))
		{
			if (pthread_create(&threads[i], NULL, threads_function, (void *)&core_info[i]) != 0) {
				return NULL;
			}
        }
		else
		{
			// do not have to create thread for the last element
			threads_function(&core_info[i]);
		}
    }

    free(core_info);
  	pthread_barrier_destroy(&barrier);
    return (gens_max % 2 == 1) ? outboard : inboard;
}


