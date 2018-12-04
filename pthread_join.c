#include <stdio.h>
#include <pthread.h>

void *do_sum(void *val);
int main()
{
	int status;
	pthread_t thread_id[3];
	int a = 4;
	int b = 5;
	int c = 6;
	status = pthread_create(&thread_id[0], NULL,do_sum, (void *)&a); //a값을 넘김
	status = pthread_create(&thread_id[1], NULL,do_sum, (void *)&b); //b값을 넘김
 	status = pthread_create(&thread_id[2], NULL,do_sum, (void *)&c); //c값을 넘김
	
	pthread_join(thread_id[0], (void**)&status);
	pthread_join(thread_id[1], (void**)&status);
	pthread_join(thread_id[2], (void**)&status);

	printf("main program end\n");
	return 0;
}

void *do_sum(void *val)
{
	int i;
	int sum = 0;
	int max = *((int *)val); //주의
	
	for(i=0; i<max; i++)
	{
		sum = sum + i;
		printf("%d - add %d \n", max , i);
		sleep(1);
	}

	printf("%d - sum is %d \n", max, sum);
}
