#include<stdio.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

//Definitions

#define MaxPhil 5
#define hungry 0
#define thinking 1
#define eats 2
#define left (phil_no+4)%MaxPhil
#define right (phil_no+1)%MaxPhil

//Semaphores
sem_t mutex;
sem_t phil[MaxPhil];

//Declaring functions

void * philosopher(void * num);
void take_fork(int);
void put_fork(int);
void test(int);

//--

int CurrentState[MaxPhil];
int phil_num[MaxPhil]={0,1,2,3,4};

//main

int main() {

  int i;
  pthread_t thread_id[MaxPhil];
  sem_init(&mutex, 0, 1);

  for(i=0; i<MaxPhil; i++) {
    sem_init(&phil[i], 0, 0);
  }
  for(i=0; i<MaxPhil; i++) {
    pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);

    printf("Philosopher %d is thinking\n", i+1);
  }

  for (i = 0; i < MaxPhil; i++) {
    pthread_join(thread_id[i], NULL);
  }
}

void * philosopher(void *num) {
  while(1) {
    int * i = num;  //Points to philosopher we are using
    sleep(1);
    take_fork(*i);  //Sends philosopher to take fork
    sleep(0);       //Makes sure other threads get to run, before progressing
                    //and if no other threads are running, it runs to the end.
    put_fork(*i);   //Puts fork back on eable
  }
}

void take_fork(int phil_no) {
  sem_wait(&mutex); //locks Mutex to current philosopher
  CurrentState[phil_no] = hungry;
  printf("Philosopher %d is hungry\n", phil_no+1);
  test(phil_no);    //sends philosopher to test
  sem_post(&mutex); //Frees up Mutex
  sem_wait(&phil[phil_no]);  //Waits for signal to be used again later
  sleep(1);         //Gives time for threads to finish
}

void put_fork(int phil_no) {
  sem_wait(&mutex); //wait for mutex
  CurrentState[phil_no] = thinking;  //Resets philosopher
  printf("Philosopher %d putting fork %d and %d down\n", phil_no+1, left+1, phil_no+1);
  printf("Philosopher %d is thinking\n", phil_no+1);
  test(left);          //Check status of philosophers
  test(right);         //Check status of philosophers
  sem_post(&mutex);    //Frees up mutex
}

void test(int phil_no) {
  if(CurrentState[phil_no] == hungry && CurrentState[left] != eats && CurrentState[right] != eats) {
    CurrentState[phil_no] = eats;   //Updates state
    sleep(2);
    printf("Philosopher %d takes fork %d and %d\n", phil_no+1, left+1, phil_no+1);
    printf("Philosopher %d is eating\n", phil_no+1);
    sem_post(&phil[phil_no]);       //Waits for signal to be reused later
  }
}
