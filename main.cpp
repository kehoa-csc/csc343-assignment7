#include <iostream>
#include <pthread.h>
#include <mutex>
#include <unistd.h>

using namespace std;

/*
    Assignment 7 for CSC343. Written by Andrew Kehoe.
*/

mutex m;
int phnum = -1; //-1 so that first is 0.
//The 3 states of the philosophers
#define THINKING 0;
#define EATING 1;
int state[2];

bool test() { //see if forks are available
    for (int i = 0; i<2; i++) {
        if (state[i] == 1) {
            printf("fork is not available\n");
            return false; //fork is not available
        }
    }
    printf("fork is available\n");
    return true; //available
}

void take_fork(int phnum) {
    m.lock();
    if (test()) {
        state[phnum] = EATING;
        printf("Philosopher %d is taking fork      Eating....\n",phnum);
    } else {
        state[phnum] = THINKING;
        printf("Philosopher %d cannot take fork.   Thinking...\n",phnum);
    }
    sleep(rand()%3);
    m.unlock();
}

void release_fork(int phnum) {
    m.lock();
    if (state[phnum] == 1) {
        printf("Philosopher %d is releasing fork.  Thinking...\n",phnum);
    } else {
        printf("Philosopher %d does not have fork. Thinking...\n",phnum);
    }
    state[phnum] = THINKING;
    sleep(rand()%3);
    m.unlock();
}

void *runner(void *ptr) {
    //Set my philosopher number (separate from thread ID)
    m.lock();
        phnum++;
        int mynum=phnum;
    m.unlock();
    printf("I am philosopher %d and my ID is %d.\n",mynum,(int)pthread_self()); //Casting to avoid compiler error?
    while (true) {
        take_fork(mynum);
        release_fork(mynum);
    }
    return 0;
}

int main()
{
    pthread_t ph0, ph1;
    pthread_attr_t aph0, aph1;
    pthread_attr_init(&aph0);
    pthread_attr_init(&aph1);

    pthread_create(&ph0,&aph0,runner,NULL);
    pthread_create(&ph1,&aph1,runner,NULL);

    pthread_join(ph0,NULL);
    pthread_join(ph1,NULL);

    //cout << "Hello world!" << endl;
    return 0;
}
