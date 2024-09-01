#include <stdio.h>
#include <pthread.h>

int shared_resource = 0;

#define NUM_ITERS 1000000
#define NUM_THREADS 100

void lock();
void unlock();
int TAS(volatile int* target);
volatile int LOCK = 0;

void lock()
{
	while(TAS(&LOCK));

}

void unlock()
{
	LOCK = 0;
}

int TAS(volatile int* target)
{
	int rv= *target;
	//*target= 1;
	asm volatile("xchg %0, %1" :
		 	     "+m"(*target), "=r"(rv) : 
			 	 "1"(1) : 
				 "memory");
	return rv;
}

void* thread_func(void* arg) {
    int tid = *(int*)arg;
    
    lock();
    
        for(int i = 0; i < NUM_ITERS; i++)    shared_resource++;
    
    unlock();
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int tids[NUM_THREADS];
    
    for (int i = 0; i < NUM_THREADS; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("shared: %d\n", shared_resource);
    
    return 0;
}
