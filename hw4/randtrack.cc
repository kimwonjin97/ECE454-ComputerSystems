#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "defs.h"
#include "hash.h"

#define SAMPLES_TO_COLLECT   10000000
#define RAND_NUM_UPPER_BOUND   100000
#define NUM_SEED_STREAMS            4

/* 
 * ECE454 Students: 
 * Please fill in the following team struct 
 */
team_t team = {
    "TTGH",                  /* Team name */

    "Won-Jin Kim",                    /* Member full name */
    "1003611424",                 /* Member student number */
    "wonjin.kim@mail.utoronto.ca",                 /* Member email address */
};

unsigned num_threads;
unsigned samples_to_skip;

class sample;

class sample 
{
private:
  unsigned my_key;
public:
  sample *next;
  unsigned count;

  sample(unsigned the_key){my_key = the_key; count = 0;};
  unsigned key(){return my_key;}
  void print(FILE *f){printf("%d %d\n",my_key,count);}
};


// This instantiates an empty hash table
// it is a C++ template, which means we define the types for
// the element and key value here: element is "class sample" and
// key value is "unsigned".  
hash<sample,unsigned> h;

typedef struct
{
  int tid;
  int start;
  int end;
} range;

range* job_range;

// function depends on the build
void* function(void * job_range);

//////////////////////////////////////////////////////////////////////
////////////////////   THREAD FUNCTIONS //////////////////////////////
//////////////////////////////////////////////////////////////////////

#ifndef DEFAULT
pthread_t* threads;
void thread_initialization(unsigned int num_thread)
{
  threads = (pthread_t*)malloc(sizeof(pthread_t)*num_thread);
}

void thread_create(unsigned int num_thread)
{
  for(int i=0; i<num_thread; ++i)
  {
    pthread_create(threads+i, NULL, &function, (void*)(job_range+i));
  }
}

void thread_join(unsigned int num_thread)
{
  for(int i=0; i<num_thread; ++i)
  {
    pthread_join(*(threads+i), NULL);
  }
}
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

#ifdef ELEVEL_LOCK
  pthread_mutex_t mutex_lock_list[RAND_NUM_UPPER_BOUND];
#endif
#ifdef  GLOBAL_LOCK
  pthread_mutex_t mutex_lock;
#endif
#ifdef RAND_REDUCTION
  hash<sample,unsigned>* hash_list;
#endif

int  
main (int argc, char* argv[]){
  int i,j,k;
  int rnum;
  unsigned key;
  sample *s;

  // Print out team information
  printf( "Team Name: %s\n", team.team );
  printf( "\n" );
  printf( "Student 1 Name: %s\n", team.name1 );
  printf( "Student 1 Student Number: %s\n", team.number1 );
  printf( "Student 1 Email: %s\n", team.email1 );
  printf( "\n" );

#ifdef  GLOBAL_LOCK
  pthread_mutex_init(&mutex_lock, NULL);
#endif
#ifdef ELEVEL_LOCK
  for(int i=0; i<RAND_NUM_UPPER_BOUND; ++i)
  {
    pthread_mutex_init(&mutex_lock_list[i], NULL);
  }
#endif  

  // Parse program arguments
  if (argc != 3){
    printf("Usage: %s <num_threads> <samples_to_skip>\n", argv[0]);
    exit(1);  
  }
  sscanf(argv[1], " %d", &num_threads); // not used in this single-threaded version
  sscanf(argv[2], " %d", &samples_to_skip);

  // initialize a 16K-entry (2**14) hash of empty lists
  h.setup(14);


//////////////////////////////////////////////////////////////////////
////////////////////////  different locks ////////////////////////////
//////////////////////////////////////////////////////////////////////


#if defined(GLOBAL_LOCK) || defined(LLEVEL_LOCK) || defined(ELEVEL_LOCK) || defined(RAND_REDUCTION)
  thread_initialization(num_threads);
  if(num_threads == 0 || num_threads == 1)
  {
    job_range = (range*)malloc(sizeof(range));
    job_range->tid = job_range->start = 0;
    job_range->end = NUM_SEED_STREAMS;
  } else {
    int val = NUM_SEED_STREAMS/num_threads;
    job_range = (range*)malloc(sizeof(range)*num_threads);
    for(int idx=0; idx<num_threads; ++idx)
    {
      if(idx!=0)
      {
        (job_range+idx)->tid = idx;
        (job_range+idx)->start = (job_range+idx-1)->end;
        (job_range+idx)->end = (job_range+idx-1)->end + val;
      }
      else 
      {
        
        job_range->tid = job_range->start = idx;
        job_range->end = idx + val;
      }
    }
  }

  #ifdef RAND_REDUCTION
  hash_list = (hash<sample,unsigned> *)malloc(sizeof(hash<sample,unsigned>)* num_threads);
  for(int i = 0 ; i < num_threads; i++){
    (hash_list+i)->setup(14);
  }
  #endif
  thread_create(num_threads);
  thread_join(num_threads);
#endif

#ifdef RAND_REDUCTION
  for(int i=0; i<num_threads; ++i)
  {
    h.merge(&hash_list[i]);
  }
#endif
  h.print();

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


#ifdef DEFAULT
  // process streams starting with different initial numbers
  for (i=0; i<NUM_SEED_STREAMS; i++)
  {
    rnum = i;

    // collect a number of samples
    for (j=0; j<SAMPLES_TO_COLLECT; j++)
    {

      // skip a number of samples
      for (k=0; k<samples_to_skip; k++)
      {
	      rnum = rand_r((unsigned int*)&rnum);
      }

      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;

      // if this sample has not been counted before
      if (!(s = h.lookup(key)))
      {
        // insert a new element for it into the hash table
        s = new sample(key);
        h.insert(s);
      }

      // increment the count for the sample
      s->count++;
    }
  }
  // print a list of the frequency of all samples
  h.print();
#endif
}



//////////////////////////////////////////////////////////////////////
////////////////////////  FUNCTIONS //////////////////////////////////
//////////////////////////////////////////////////////////////////////
#ifdef GLOBAL_LOCK
void* function(void* job_range)
{
  int i,j,k;
  int rnum;
  unsigned key;
  sample *s;
  // job_r = (range*)job_range
// process streams starting with different initial numbers
  for (i=((range*)job_range)->start; i<((range*)job_range)->end; ++i)
  {
    rnum = i;

    // collect a number of samples
    for (j=0; j<SAMPLES_TO_COLLECT; j++)
    {

      // skip a number of samples
      for (k=0; k<samples_to_skip; k++)
      {
	      rnum = rand_r((unsigned int*)&rnum);
      }

      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;

      pthread_mutex_lock(&mutex_lock);
      // if this sample has not been counted before
      if (!(s = h.lookup(key)))
      {
        // insert a new element for it into the hash table
        s = new sample(key);
        h.insert(s);
      }

      // increment the count for the sample
      s->count++;

      pthread_mutex_unlock(&mutex_lock);
    }
  }
  // print a list of the frequency of all samples
  h.print();
  pthread_exit(NULL);
}
#endif


#ifdef LLEVEL_LOCK
void* function(void* job_range)
{
  int i,j,k;
  int rnum;
  unsigned key;
  sample *s;
  // job_r = (range*)job_range
// process streams starting with different initial numbers
  for (i=((range*)job_range)->start; i<((range*)job_range)->end; ++i)
  {
    rnum = i;

    // collect a number of samples
    for (j=0; j<SAMPLES_TO_COLLECT; j++)
    {

      // skip a number of samples
      for (k=0; k<samples_to_skip; k++)
      {
	      rnum = rand_r((unsigned int*)&rnum);
      }

      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;

      h.list_lock(key); 
      // if this sample has not been counted before
      if (!(s = h.lookup(key)))
      {
        // insert a new element for it into the hash table
        s = new sample(key);
        h.insert(s);
      }

      // increment the count for the sample
      s->count++;

      h.list_unlock(key);
    }
  }
  // print a list of the frequency of all samples
  h.print();
  pthread_exit(NULL);
}
#endif

#ifdef ELEVEL_LOCK
void* function(void* job_range)
{
  int i,j,k;
  int rnum;
  unsigned key;
  sample *s;
  // job_r = (range*)job_range
// process streams starting with different initial numbers
  for (i=((range*)job_range)->start; i<((range*)job_range)->end; ++i)
  {
    rnum = i;

    // collect a number of samples
    for (j=0; j<SAMPLES_TO_COLLECT; j++)
    {

      // skip a number of samples
      for (k=0; k<samples_to_skip; k++)
      {
	      rnum = rand_r((unsigned int*)&rnum);
      }

      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;

      pthread_mutex_lock(&mutex_lock_list[key]);
      // if this sample has not been counted before
      if (!(s = h.lookup(key)))
      {
        // insert a new element for it into the hash table
        s = new sample(key);
        h.insert(s);
      }

      // increment the count for the sample
      s->count++;

      pthread_mutex_unlock(&mutex_lock_list[key]);
    }
  }
  // print a list of the frequency of all samples
  h.print();
  pthread_exit(NULL);
}
#endif


#ifdef RAND_REDUCTION
void* function(void* job_range)
{
  int i,j,k;
  int rnum;
  unsigned key;
  sample *s;
  
  int idx = ((range*)job_range)->tid;
  hash<sample,unsigned>* cur_hash = &hash_list[idx];

// process streams starting with different initial numbers
  for (i=((range*)job_range)->start; i<((range*)job_range)->end; ++i)
  {
    rnum = i;

    // collect a number of samples
    for (j=0; j<SAMPLES_TO_COLLECT; j++)
    {

      // skip a number of samples
      for (k=0; k<samples_to_skip; k++)
      {
	      rnum = rand_r((unsigned int*)&rnum);
      }

      // force the sample to be within the range of 0..RAND_NUM_UPPER_BOUND-1
      key = rnum % RAND_NUM_UPPER_BOUND;

      // pthread_mutex_lock(&mutex_lock_list[key]);
      // if this sample has not been counted before
      if (!(s = cur_hash->lookup(key)))
      {
        // insert a new element for it into the hash table
        s = new sample(key);
        cur_hash->insert(s);
      }

      // increment the count for the sample
      s->count++;

      // pthread_mutex_unlock(&mutex_lock_list[key]);
    }
  }
  // print a list of the frequency of all samples
  pthread_exit(NULL);
}
#endif




//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
