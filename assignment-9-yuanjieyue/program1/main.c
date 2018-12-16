#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_OF_COMMANDLINE_ARGUMENT 5
#define LENGTH_OF_NUM 4

/* Struct that stores the data of a worker thread */
typedef struct Thread_Data
{
  int thread_index;
  int start;
  int stop;
} Thread_Data;

/* Global Variable */
int* result; // array that will allocate based on the given input size
int* nums_from_file; // array store nums that read from the file
int num_to_search; // the number to search in the file

/* Mutex for entire program */
pthread_mutex_t lock;

/* Thread Barrier */
pthread_barrier_t barr;

/* 
   Former declare of the Function 
   that getting the line number of the given number 
*/
void *get_line_number(void *td);

int main(int argc, char* argv[]) {
  if (argc < NUM_OF_COMMANDLINE_ARGUMENT) {
    puts("Four arguments are needed\n");
    exit(1);
  } 
  else if (argc > NUM_OF_COMMANDLINE_ARGUMENT) {
    puts("Too many arguments supplied\n");
    exit(1);
  }
  else {
    /* Initialize the variables based on the command line arguments */
    char* file_name = argv[1];
    num_to_search = atoi(argv[2]);
    int num_of_threads = atoi(argv[3]);
    int input_size = (int)atof(argv[4]) + 1; 

    /* allocate memory for the two global arrays on the heap */
    result = (int*)malloc(input_size * (1024 * 1024));
    nums_from_file = (int*)malloc(input_size*(1024*1024));
              
    int ret_val = 0;
    pthread_t ptids[num_of_threads];
    Thread_Data datas[num_of_threads];
   
    /* Initialize the mutex */
    if (pthread_mutex_init(&lock, NULL) != 0) {
      puts("\n mutx int failed\n");
      return 1;
    }
    /* 
       Barrier initialization, set it to num of threads + 1, 
       so all worker threads and main threads will all wait on the barrier
    */
  
    if (pthread_barrier_init(&barr, NULL, num_of_threads + 1) != 0) {
      puts("barrier int failed\n");
      return 1;
    }
   
    /* Create threads */
    int i;
    for (i = 0; i < num_of_threads; i++) {
      ret_val = pthread_create(&ptids[i], NULL, get_line_number, &datas[i]);
      if (0 != ret_val) {
	puts("Can't create thread\n");
      } else {
	puts("A thread is created successfully\n");
      }
    } 

    /* Here is the part that handles the input file */
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
      puts("Error in opening file");
      exit(1);
    }  
    /* Read the nums in the file to the allocated array
       which is the total workload for all threads
    */
    char str[LENGTH_OF_NUM];
    int line_count = 0;   
    while (fgets(str, LENGTH_OF_NUM, fp) != NULL) {
      nums_from_file[line_count] = atoi(str);
      line_count++;
    }
    printf("The total lines in the file is %d\n", line_count);
    fclose(fp);

    /* Distribute the workload to each thread, initiallize the thread datas */ 
    int workload_per_thread = (line_count + num_of_threads - 1) / num_of_threads;
    for (i = 0; i < num_of_threads; i++) {
      datas[i].thread_index = i+1;
      datas[i].start = i * workload_per_thread;
      datas[i].stop = (i+1) * workload_per_thread;
    }
    datas[num_of_threads-1].stop = line_count;

    printf("the main thread is ready now\n");
    /* a barrier that will wait all the other worker thread to come here a
       and synchronize here, then proceed
    */
    int err = pthread_barrier_wait(&barr);
    if (err != 0 && err != PTHREAD_BARRIER_SERIAL_THREAD) {
      puts("Could not wait on barrier\n");
      exit(1);
    } else {
      puts("Main thread is now past the barrier\n");
    }
    printf("the main thread is on going\n");
    /* Wait for threads to finish */
    void* res;
    for (i = 0; i < num_of_threads; i++) {
      printf("%d thread join!\n", i);
      pthread_join(ptids[i], &res);
    }
     
    /* Copy the results to a smaller array */
    int size = 0;
    for (i = 0; i < line_count; i++) {
      if (result[i] != 0) {
        size++;
      }
    }

    int res_arr[size];
    int j = 0;
    i = 0;
    while (i < line_count) {
      if (result[i] != 0) {
        res_arr[j] = result[i];
        j++;
      }    
      i++; 
    }
    // Print out the results
    for (i = 0; i < size; i++) {
      printf("line number of the %dth [%d] is %d\n", i, num_to_search, res_arr[i]);
    }

    /* Destroy barrier */
    if (pthread_barrier_destroy(&barr) != 0) {
      puts("Barrier destroy failed\n");
      return 1;
    }

    /* Destroy mutex */
    pthread_mutex_destroy(&lock);

    /* Free the allocated memory */
    free(result);
    free(nums_from_file);
    printf("\nThe program successfully completed!\n");
  }
  return 0;
}


void *get_line_number(void* td) { 
  printf("Worker thread is ready now!\n");
  /* 
     A barrier here to wait the main thread to read the file out
     and initialize the arguments to passed to the worker thread
  */  
  int err = pthread_barrier_wait(&barr);
  if (err != 0 && err != PTHREAD_BARRIER_SERIAL_THREAD) {
    puts("Could not wait on barrier\n");
    exit(1);
  } else {
    printf("Worker thread is now past the barrier\n");
  }

  Thread_Data* data = (Thread_Data*)td; 
  int thread_index = data->thread_index;
  int start = data->start;
  int stop = data->stop;  
  int i;  
  for (i = start; i < stop; i++) {
    if (i == start || i == stop - 1) {
      printf("Now, the %d th elem has been checked\n", i);
    }
    if (nums_from_file[i] == num_to_search) {
      /* Get the lock */
      pthread_mutex_lock(&lock); 
      /* 
	 Set value at the result array to the line number 
	 that the num appear in the file     
      */
      result[i] = i + 1;
      /* Release the lock */
      pthread_mutex_unlock(&lock);
    }
  }
  printf("\n%d th thread is done!\n\n", thread_index);
  return NULL;
}




