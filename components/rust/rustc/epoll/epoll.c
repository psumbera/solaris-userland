#include <fcntl.h>
#include <inttypes.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stropts.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/devpoll.h>

#define MAX_MY_UDATA 1024

struct my_udata {
  int epfd;
  int fd;
  uint64_t u64;
};

struct my_udata my_udata[MAX_MY_UDATA];

int my_udata_index = 0;

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_DEL 2
#define EPOLL_CTL_MOD 3

typedef union epoll_data {
  void    *ptr;
  int      fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event {
  uint32_t     events;    /* Epoll events */
  epoll_data_t data;      /* User data variable */
};

int epoll_create1(int flags)
{
  int epfd;

  if ((epfd = open("/dev/poll", O_RDWR)) < 0) {
                return epfd;
  }

  return epfd;
}

int epoll_create(int size)
{
  return epoll_create1(0);
}

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)
{
  struct pollfd pollfd;
  pollfd.fd = fd;
  int j;

#if DEBUG
  fprintf(stderr, "XXX epoll_ctl %d %d %d %"PRIu64"\n", epfd, op, fd, event->data.u64);
#endif

  if (op == EPOLL_CTL_ADD) {
    my_udata[my_udata_index].epfd = epfd;
    my_udata[my_udata_index].fd = fd;
    my_udata[my_udata_index].u64 = event->data.u64;
    my_udata_index++;
    pollfd.events = POLLIN;
  } else if (op == EPOLL_CTL_MOD)
     for (j = 0; j < my_udata_index; j++) {
       if (my_udata[j].epfd == epfd && my_udata[j].fd == fd) {
         my_udata[j].u64 = event->data.u64;
         break;
       }
      return 0;
  } else {
     for (j = 0; j < my_udata_index; j++) {
       if (my_udata[j].epfd == epfd && my_udata[j].fd == fd) {
         my_udata[j].epfd = 0;
         my_udata[j].fd = 0;
         my_udata[j].u64 = 0;
         break;
       }
     }
    pollfd.events = POLLREMOVE;
  }
  pollfd.revents = 0;

  if (write(epfd, &pollfd, sizeof(struct pollfd)) != sizeof(struct pollfd)) {
                perror("failed to write all pollfds");
                close (epfd);
                exit(-1);
  }


  return 0;
}

int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
{
  struct dvpoll dopoll;
  struct pollfd *pollfd;
  int result;
  int i, j;

#if DEBUG
  fprintf(stderr, "XXX epoll_wait epfd:%d maxevents:%d timeout:%d\n", epfd, maxevents, timeout);
#endif

  pollfd = malloc(sizeof(struct pollfd) * maxevents);
  if (pollfd == NULL) {
    perror("malloc(sizeof(struct pollfd) * maxevents) failed");
    close (epfd);
    exit(-1);
  }
  dopoll.dp_timeout = timeout;
  dopoll.dp_nfds = maxevents;
  dopoll.dp_fds = pollfd;
  result = ioctl(epfd, DP_POLL, &dopoll);
  if (result < 0) {
                perror("/dev/poll ioctl DP_POLL failed");
                close (epfd);
                free(pollfd);
                exit(-1);
  }
  for (i = 0; i < result; i++) {
     events[i].events = pollfd[i].events;
     events[i].data.u64 = 0;
     for (j = 0; j < my_udata_index; j++) {
       if (my_udata[j].epfd == epfd && my_udata[j].fd == pollfd[i].fd) {
         events[i].data.u64 = my_udata[j].u64;
         break;
       }
     }
  }
  free(pollfd);
  return result;
}

