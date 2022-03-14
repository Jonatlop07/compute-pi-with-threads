#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define THREAD_NUM 100
#define ITERATIONS 1e07

#define LEIBNIZ_FACTOR 4

struct thread_data {
  int thread_number;
};

pthread_mutex_t mutex;

double computed = 0;

void * compute_pi(void *data) {
  struct thread_data *current_thread_data = data;
  int thread_number = current_thread_data->thread_number;
  int n = thread_number;
  int current_iteration = 0;
  double local_computed = 0;

  for (; current_iteration < ITERATIONS; ++current_iteration, n += THREAD_NUM) {
    local_computed += pow(-1, n) / (2 * n + 1);
  }

  pthread_mutex_lock(&mutex);
  
  computed += local_computed;

  pthread_mutex_unlock(&mutex);
}

int main() {
  pthread_t threads[THREAD_NUM];
  struct thread_data threads_data[THREAD_NUM];
  int current_thread = 0, r, *rh0;

  for (; current_thread < THREAD_NUM; ++current_thread) {
    threads_data[current_thread].thread_number = current_thread;
  }

  pthread_mutex_init(&mutex, NULL);

  for (current_thread = 0; current_thread < THREAD_NUM; ++current_thread) {
    r = pthread_create(
      &threads[current_thread],
      NULL,
      (void *) compute_pi,
      (void *) &threads_data[current_thread]
    );
    
    if (r != 0) {
      perror("\n-->pthread_create error: ");
      exit(-1);
    }
  }

  for (current_thread = 0; current_thread < THREAD_NUM; ++current_thread) {
    r = pthread_join(threads[current_thread], (void **) &rh0);

    if (r != 0) {
      perror("\n-->pthread_join error: ");
      exit(-1);
    }
  }

  pthread_mutex_destroy(&mutex);

  printf("\nCALCULATED VALUE OF PI = %.20f", computed * LEIBNIZ_FACTOR);

  return 0;
}


