#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

void rand_permute(int *index, int len)
{
	int tmp, j, i;

	srand (time(NULL));
	for(i=len - 1; i > 1; i--) {
		/* uniformly 0  j  i */
		j = rand() % (i + 1);
		tmp = index[j];
		index[j] = index[i];
		index[i] = tmp;
	}
}

int file_size = 1500 * 1024 * 1024;
int record_size = 4 * 1024;

int main(int argc, char *argv[])
{
	int size = file_size / record_size;
	int *index = malloc (sizeof (int) * size);
	for (int i = 0; i < size; i++) {
		index[i] = i;
	}

	rand_permute(index, size);
	
	int fd = open (argv[1], O_RDWR);
	if (fd < 0) {
		perror ("open");
		exit (1);
	}

	char* buf = valloc(record_size);
	struct timeval start_time, end_time;
	gettimeofday (&start_time, NULL);
	for (int i = 0; i < size; i++) {
		int ret = lseek (fd, index[i] * record_size, SEEK_SET);
		if (ret < 0) {
			perror ("fseek");
			exit (1);
		}
		int rw_flag = atoi(argv[2]);
		if (rw_flag == 0)
		{	
			ret = read (fd, buf, record_size);
		}
		if (rw_flag == 1)
		{
			 ret = write (fd, buf, record_size);
			 if (ret < 0) {
			   perror ("write");
			   exit (1);
			 }
		
		}
	}
	gettimeofday (&end_time, NULL);
	printf ("%d: %ldMB/s\n", time(NULL), file_size / ((end_time.tv_sec - start_time.tv_sec) * 1000
			+ (end_time.tv_usec - start_time.tv_usec) / 1000) * 1000 / 1024 / 1024);
}
