#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define THREAD_CNT	2
#define COUNT		10000000

static volatile int count;
static volatile int start;

static void* do_count(void *data)
{
	int i;
	int id = (unsigned long)data;

	while (!start)
	      usleep(500);

	for (i = 0; i < COUNT; i++) {
		count++;
	}
	printf("thread-%d: Add %d to count\n",
		id, i);
	return NULL;
}

int main(void)
{
	int ret;
	int i, j, threads_ok;
	pthread_t threads[THREAD_CNT] = {};

	threads_ok = 0;
	for (i = 0; i < THREAD_CNT; i++) {
		ret = pthread_create(threads+i, NULL, do_count,
					(void*)(unsigned long)i);
		if (ret == -1) {
			printf("create thread-%d failed: %s\n",
				i, strerror(errno));
			goto out_thread;
		}
	}
	threads_ok = 1;
	/**
	 * Wait all threads online.
	 * This is not a good method, it is only
	 * used here for program brevity.
	 */
	sleep(2);

out_thread:
	/* Let all threads go ahead. */
	start = 1;
	for (j = 0; j < i; j++) {
		pthread_join(threads[j], NULL);
	}
	if (threads_ok) {
		printf("count=%d, expected value=%d\n",
			count, THREAD_CNT * COUNT);
	}
	return 0;
}
