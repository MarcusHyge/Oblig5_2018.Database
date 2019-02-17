#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

sem_t sem[6];

struct threadargs {
    int id;         /* thread number */
    int sec;        /* how many seconds to sleep */
    int signal[6];  /* which threads to signal when done */
};


void *tfunc(void *arg) {
    struct threadargs *targs = arg;
    sem_wait(&sem[targs->id]);
    printf("Tråd %d har startet.",targs->id);
    sleep(targs->sec);
    printf("Tråd %d kjørte i %d sekunder.",targs->id, targs->sec);

    for(int i = 0; i<=5;i++){
        if(targs->signal[i] == 1){
            sem_post(&sem[i]);
        }
    }
    return 0;
}


    int main(void)
    {
        int i,j;
        struct threadargs *targs[6];
        pthread_t joinQueue[6];
        for (i=0;i<6;i++) {
            targs[i] = (struct threadargs*) malloc(sizeof(struct threadargs));
            for (j=0;j<6;j++) {
                targs[i]->signal[j]=0;
            }


        }

        targs[0]->id = 0;
        targs[0]->sec = 1;
        targs[0]->signal[1] = 1;
        targs[0]->signal[4] = 1;
        sem_init(&sem[0], 0, 1);

        pthread_create(&joinQueue[0], NULL, tfunc, targs[0]);


        targs[1]->id = 1;
        targs[1]->sec = 2;
	targs[1]->signal[3] = 1;
        sem_init(&sem[1], 0, 0);

        pthread_create(&joinQueue[1], NULL, tfunc, targs[1]);

        targs[2]->id = 2;
        targs[2]->sec = 3;
        sem_init(&sem[2], 0, 1);

        pthread_create(&joinQueue[2], NULL, tfunc, targs[2]);

        targs[3]->id = 3;
        targs[3]->sec = 2;
        sem_init(&sem[3],0, 0);

        pthread_create(&joinQueue[3], NULL, tfunc, targs[3]);

        targs[4]->id = 4;
        targs[4]->sec = 3;
        targs[4]->signal[5] = 1;
        sem_init(&sem[4], 0, 0);

        pthread_create(&joinQueue[4], NULL, tfunc, targs[4]);

        targs[5]->id = 5;
        targs[5]->sec = 3;
        sem_init(&sem[5], 0, 0);

        pthread_create(&joinQueue[5], NULL, tfunc, targs[5]);

//     Setting a queue for the threads
        pthread_join(joinQueue[0], NULL);

        pthread_join(joinQueue[1], NULL);

        pthread_join(joinQueue[2], NULL);

        pthread_join(joinQueue[3], NULL);

        pthread_join(joinQueue[4], NULL);

        pthread_join(joinQueue[5], NULL);


        return 0;
    }
