// RUN: %libomp-compile-and-run
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "omp_testsuite.h"
#include "omp_my_sleep.h"

#define CFSMAX_SIZE 1000
#define MAX_TIME 0.01

#ifdef SLEEPTIME
#undef SLEEPTIME
#define SLEEPTIME 0.0005
#endif

#define VERBOSE 0

int test_omp_for_schedule_static_3()
{
  int threads;
  int i,lasttid;

  int * tids;
  int * tids2;
  int notout;
  int maxiter;
  int chunk_size;

  int counter = 0;
  int tmp_count=1;
  int lastthreadsstarttid = -1;
  int result = 1;
  chunk_size = 7;

  tids = (int *) malloc (sizeof (int) * (CFSMAX_SIZE + 1));
  notout = 1;
  maxiter = 0;

  #pragma omp parallel shared(tids,counter)
  {  /* begin of parallel*/
    #pragma omp single
    {
      threads = omp_get_num_threads ();
    }  /* end of single */
  }  /* end of parallel */

  /* Ensure that at least two threads are created */
  if (threads < 2) {
    omp_set_num_threads(2);
    threads = 2;
  }
  fprintf (stderr,"Using an internal count of %d\nUsing a"
    " specified chunksize of %d\n", CFSMAX_SIZE, chunk_size);
  tids[CFSMAX_SIZE] = -1;  /* setting endflag */

  #pragma omp parallel shared(tids)
  {  /* begin of parallel */
    double count;
    int tid;
    int j;

    tid = omp_get_thread_num ();

    #pragma omp for nowait schedule(static,chunk_size)
    for(j = 0; j < CFSMAX_SIZE; ++j) {
      count = 0.;
      #pragma omp flush(maxiter)
      if (j > maxiter) {
        #pragma omp critical
        {
          maxiter = j;
        }
      }
      /*printf ("thread %d sleeping\n", tid);*/
      while (notout && (count < MAX_TIME) && (maxiter == j)) {
        #pragma omp flush(maxiter,notout)
        my_sleep (SLEEPTIME);
        count += SLEEPTIME;
        printf(".");
      }
#ifdef VERBOSE
      if (count > 0.) printf(" waited %lf s\n", count);
#endif
      /*printf ("thread %d awake\n", tid);*/
      tids[j] = tid;
#ifdef VERBOSE
      printf("%d finished by %d\n",j,tid);
#endif
    } /* end of omp parallel for */

    notout = 0;
    #pragma omp flush(maxiter,notout)
  } /* end of parallel */

  /**** analysing the data in array tids ****/

  lasttid = tids[0];
  tmp_count = 0;

  for (i = 0; i < CFSMAX_SIZE + 1; ++i) {
    /* If the work  was done by the same thread
       increase tmp_count by one. */
    if (tids[i] == lasttid) {
      tmp_count++;
#ifdef VERBOSE
      fprintf (stderr, "%d: %d \n", i, tids[i]);
#endif
      continue;
    }

    /* Check if the next thread had has the right thread number.
     * When finding threadnumber -1 the end should be reached.
     */
    if (tids[i] == (lasttid + 1) % threads || tids[i] == -1) {
      /* checking for the right chunk size */
      if (tmp_count == chunk_size) {
        tmp_count = 1;
        lasttid = tids[i];
#ifdef VERBOSE
        fprintf (stderr, "OK\n");
#endif
      } else {
        /* If the chunk size was wrong, check if the end was reached */
        if (tids[i] == -1) {
          if (i == CFSMAX_SIZE) {
            fprintf (stderr, "Last thread had chunk size %d\n",
              tmp_count);
            break;
          } else {
            fprintf (stderr, "ERROR: Last thread (thread with"
              " number -1) was found before the end.\n");
            result = 0;
          }
        } else {
          fprintf (stderr, "ERROR: chunk size was %d. (assigned"
            " was %d)\n", tmp_count, chunk_size);
          result = 0;
        }
      }
    } else {
      fprintf(stderr, "ERROR: Found thread with number %d (should be"
        " inbetween 0 and %d).", tids[i], threads - 1);
      result = 0;
    }
#ifdef VERBOSE
    fprintf (stderr, "%d: %d \n", i, tids[i]);
#endif
  }

  /* Now we check if several loop regions in one parallel region have the
   * same logical assignement of chunks to threads. We use the nowait
   * clause to increase the probability to get an error. */

  /* First we allocate some more memmory */
  free (tids);
  tids = (int *) malloc (sizeof (int) * LOOPCOUNT);
  tids2 = (int *) malloc (sizeof (int) * LOOPCOUNT);

  #pragma omp parallel
  {
    {
      int n;
      #pragma omp for schedule(static) nowait
      for (n = 0; n < LOOPCOUNT; n++) {
        if (LOOPCOUNT == n + 1 )
          my_sleep(SLEEPTIME);

        tids[n] = omp_get_thread_num();
      }
    }
    {
      int m;
      #pragma omp for schedule(static) nowait
      for (m = 1; m <= LOOPCOUNT; m++) {
        tids2[m-1] = omp_get_thread_num();
      }
    }
  }

  for (i = 0; i < LOOPCOUNT; i++)
  if (tids[i] != tids2[i]) {
    fprintf (stderr, "Chunk no. %d was assigned once to thread %d and"
      " later to thread %d.\n", i, tids[i],tids2[i]);
    result = 0;
  }

  free (tids);
  free (tids2);
  return result;
}

int main()
{
  int i;
  int num_failed=0;

  for (i = 0; i < REPETITIONS; i++) {
    if(!test_omp_for_schedule_static_3()) {
      num_failed++;
    }
  }
  return num_failed;
}
