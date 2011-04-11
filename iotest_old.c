#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

//typedef long long off64_t;
//typedef off_t off64_t;
void process_handler(int sig);

char* buf __attribute__ ((aligned(4096))) = NULL;
char file_name[16];
long long file_size;
long long rec_size;
long long num_recs;
long long file_offset_half = 0;



long long current_nums = 0;
struct sigaction act;
struct timeval start;
struct timeval now;
double rate;

void fill_buffer(long long rec_size)
{
  //  int val = 0x12345678;
  //  long long nums = rec_size/sizeof(int);
  //  long long i;
  memset (buf, 'a', rec_size);
}

void write_test(int fd, long long file_size, long long rec_size, char* buf)
{
  //  int fd = open(file_name, O_CREAT | O_WRONLY | O_DIRECT, S_IRUSR | S_IWUSR);
 
  num_recs = file_size / rec_size;
  long long i;
  act.sa_handler = process_handler;
  act.sa_flags = SA_ONESHOT | SA_NOMASK;
  sigaction(SIGINT, &act, NULL);
  gettimeofday(&start, NULL);
  for (i = 0; i < num_recs; i++)
  {
    fill_buffer(rec_size);
    int rec = write(fd, buf, rec_size);
    if (rec == -1)
      perror("write error\n");
    current_nums = i + 1;
  }
  gettimeofday(&now, NULL);
  double diff = now.tv_sec - start.tv_sec +
    (double) (now.tv_usec - start.tv_usec) / 1000000;
  printf("I/O rate is %.3fKBytes/s\n", file_size / (diff * 1024));
  
}

void read_test(int fd, long long file_size, long long rec_size, char* buf)
{
  /*  int fd = open(file_name, O_RDONLY | O_DIRECT, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    perror("open error");
    if (errno == EEXIST)
    {
      fd = open(file_name, O_RDONLY | O_DIRECT, S_IRUSR | S_IWUSR);
    }
    }*/

  num_recs = file_size / rec_size;
  long long i;
  act.sa_handler = process_handler;
  act.sa_flags = SA_ONESHOT | SA_NOMASK;
  sigaction(SIGINT, &act, NULL);
  gettimeofday(&start, NULL);
  for (i = 0; i < num_recs; i++)
  {
    //fill_buffer(rec_size);
    int rec = read(fd, buf, rec_size);
    if (rec == -1)
      perror("read error\n");
    current_nums = i + 1;
  }
  gettimeofday(&now, NULL);
  double diff = now.tv_sec - start.tv_sec +
    (double) (now.tv_usec - start.tv_usec) / 1000000;
  printf("I/O rate is %.3fKBytes/s\n", file_size / (diff * 1024));
  
  
}

void random_write_test(int fd, long long file_size, long long rec_size, char* buf)
{
  /*int fd = open(file_name, O_WRONLY | O_DIRECT, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    perror("open error");
    if (errno == EEXIST)
    {
      fd = open(file_name, O_WRONLY | O_DIRECT, S_IRUSR | S_IWUSR);
    }
    }*/
  srand((int)time(0));
  num_recs = file_size / rec_size;
  long long i;
  act.sa_handler = process_handler;
  act.sa_flags = SA_ONESHOT | SA_NOMASK;
  sigaction(SIGINT, &act, NULL);
  gettimeofday(&start, NULL);
  for (i = 0; i < num_recs; i++)
  {
    long long ret;
    long long pos;
    fill_buffer(rec_size);
    pos = (long long)(rand() % num_recs) + file_offset_half * 2;
    ret = lseek(fd, pos * rec_size, SEEK_SET);
    if (ret == -1)
      perror("lseek error\n");
    //    printf("the pos is: %lld\n", ret);
    ret = write(fd, buf, rec_size);
    if (ret == -1)
      perror("random write error\n");
    current_nums = i + 1;
  }
  gettimeofday(&now, NULL);
  double diff = now.tv_sec - start.tv_sec +
    (double) (now.tv_usec - start.tv_usec) / 1000000;
  printf("I/O rate is %.3fKBytes/s\n", file_size / (diff * 1024));
  

}

void random_read_test(int fd, long long file_size, long long rec_size, char* buf)
{
  /*int fd = open(file_name, O_RDONLY | O_DIRECT, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    perror("open error");
    if (errno == EEXIST)
    {
      fd = open(file_name, O_RDONLY | O_DIRECT, S_IRUSR | S_IWUSR);
    }
    }*/
  srand((int)time(0));
   num_recs = file_size / rec_size;
  long long i;
  act.sa_handler = process_handler;
  act.sa_flags = SA_ONESHOT | SA_NOMASK;
  sigaction(SIGINT, &act, NULL);
  gettimeofday(&start, NULL);
  for (i = 0; i < num_recs; i++)
  {
    long long ret;
    long long pos;
    fill_buffer(rec_size);
    pos = (long long)(rand() % num_recs) + file_offset_half * 2;
    ret = lseek(fd, pos * rec_size, SEEK_SET);
    if (ret == -1)
      perror("lseek error\n");
    //    printf("the pos is: %lld\n", ret);
    ret = read(fd, buf, rec_size);
    if (ret == -1)
      perror("random read error\n");
    current_nums = i + 1;
  }
  gettimeofday(&now, NULL);
  double diff = now.tv_sec - start.tv_sec +
    (double) (now.tv_usec - start.tv_usec) / 1000000;
  printf("I/O rate is %.3fKBytes/s\n", file_size / (diff * 1024));
  
}

void main(int argc, char** argv)
{
  
  int func_idx;
 
  void (*func[])() = {
    write_test,
    read_test,
    random_write_test,
    random_read_test,
  };
  
  if (argc != 6)
  {
    printf("Usage:\n");
    printf("iotest file_name file_size(k/m/g) record_size(k/m/g) offset io_type\n");
    printf("For io_type:\n");
    printf("0: write\n");
    printf("1: read\n");
    printf("2: random write\n");
    printf("3: random read\n");
    exit(0);
  }

  strcpy(file_name, argv[1]);

  if (argv[2][strlen(argv[2]) - 1] == 'k' || argv[2][strlen(argv[2]) - 1] == 'K')
  {
    file_size = 1024 * atoll(argv[2]);
  }
  if (argv[2][strlen(argv[2]) - 1] == 'm' || argv[2][strlen(argv[2]) - 1] == 'M')
  {
    file_size = 1024 * 1024 * atoll(argv[2]);
  }
  if (argv[2][strlen(argv[2]) - 1] == 'g' || argv[2][strlen(argv[2]) - 1] == 'G')
  {
    file_size = 1024 * 1024 * 1024 * atoll(argv[2]);
  }
  //else
  //{
  // file_size = atoll(argv[2]);
  // }

  if (argv[3][strlen(argv[3]) - 1] == 'k' || argv[3][strlen(argv[3]) - 1] == 'K')
  {
    rec_size = 1024 * atoi(argv[3]);
  }
  if (argv[3][strlen(argv[3]) - 1] == 'm' || argv[3][strlen(argv[3]) - 1] == 'M')
  {
    rec_size = 1024 * 1024 * atoi(argv[3]);
  }
  if (argv[3][strlen(argv[3]) - 1] == 'g' || argv[3][strlen(argv[3]) - 1] == 'G')
  {
    rec_size = 1024 * 1024 * 1024 * atoi(argv[3]);
  }
  //else
  // {
  // rec_size = atoi(argv[3]);
  // }

  buf = valloc(sizeof(char) * rec_size);

  
  int fd = open(file_name, O_CREAT | O_RDWR | O_DIRECT, S_IRUSR | S_IWUSR);
  if (fd == -1)
  {
    perror("open error");
    exit(1);
    if (errno == EEXIST)
    {
      fd = open(file_name, O_RDWR | O_DIRECT, S_IRUSR | S_IWUSR);
    }
  }

  if (argv[4][strlen(argv[4]) - 1] == 'k' || argv[4][strlen(argv[4]) - 1] == 'K')
  {
    file_offset_half = 1024 * atol(argv[4]) / 2;
  }
  if (argv[4][strlen(argv[4]) - 1] == 'm' || argv[4][strlen(argv[4]) - 1] == 'M')
  {
    file_offset_half = 1024 * 1024 * atol(argv[4]) / 2;
  }
  if (argv[4][strlen(argv[4]) - 1] == 'g' || argv[4][strlen(argv[4]) - 1] == 'G')
  {
    file_offset_half = 1024 * 1024 *1024 * atol(argv[4]) / 2;
  }

  int ret  = lseek(fd, file_offset_half, SEEK_CUR);
  if (ret < 0)
  {
    perror("lseek 1");
    exit(1);
  }
  ret = lseek(fd, file_offset_half, SEEK_CUR);
  if (ret < 0)
  {
    perror("lseek 2");
    exit(1);
  }

  
  func_idx = atoi(argv[5]);
  
  
  func[func_idx](fd, file_size, rec_size, buf);
  
}


void process_handler(int sig)
{
   gettimeofday(&now, NULL);
   double diff = now.tv_sec - start.tv_sec +
     (double) (now.tv_usec - start.tv_usec) / 1000000;
   
   printf("\nI/O rate is %.3fKBytes/s\n", current_nums * rec_size / (diff * 1024));
   exit(0);
}
