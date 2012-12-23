#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<semaphore.h>
#define N 10

int stack[N];
int index = -1;
pthread_mutex_t mutex;
sem_t sem_full;
sem_t sem_empty;

void push_buffer(int element)
{
	index++;
	stack[index] = element;
}

int pop_buffer()
{
	int temp = stack[index];
	stack[index] = 0;
	index--;
	return temp;
}

void print_buffer(void)
{
	int i;
	for (i = 0; i < N; i++)
	{
		if (stack[i] == 0)
			printf("- ");
		else
			printf("%d ", stack[i]);
	}
}
void * producer1()
{
	while(1)
	{
		sem_wait(&sem_full);
		pthread_mutex_lock(&mutex);
		push_buffer( (rand() %9) + 1);
		print_buffer();
		printf("P1: \n");
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_empty);		
	}
}

void * producer2()
{
	while(1)
	{
		sem_wait(&sem_full);
		pthread_mutex_lock(&mutex);
		push_buffer( (rand() %9) + 1);
		
		print_buffer();
		
		printf("P2: \n");

		pthread_mutex_unlock(&mutex);
		sem_post(&sem_empty);
	}
}

void * consumer1()
{
	while(1)
	{
		sem_wait(&sem_empty);
		pthread_mutex_lock(&mutex);
		
		print_buffer();
		printf("C1: %d\n",pop_buffer());
		
		pthread_mutex_unlock(&mutex);\
		sem_post(&sem_full);
	}
}

void * consumer2()
{
	while(1)
	{
		sem_wait(&sem_empty);
		pthread_mutex_lock(&mutex);
		print_buffer();
		printf("C2: %d\n", pop_buffer());
		
		pthread_mutex_unlock(&mutex);
		sem_post(&sem_full);
	}
}

int main(void)
{
	int i;
	srand(time(NULL));
	pthread_t p1, p2, c1, c2;
	pthread_mutex_init(&mutex, NULL);
	sem_init(&sem_full, 0, N);
	sem_init(&sem_empty, 0, 0);	

	pthread_create(&p1, NULL, producer1, NULL);
	pthread_create(&p2, NULL, producer2, NULL);
	pthread_create(&c1, NULL, consumer1, NULL);
	pthread_create(&c2, NULL, consumer2, NULL);

	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_join(c1, NULL);
	pthread_join(c2, NULL);


	return 0;
}


