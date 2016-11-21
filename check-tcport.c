#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define PROC_NET_TCP "/proc/net/tcp"

enum {
  TCP_ESTABLISHED = 1,
  TCP_SYN_SENT,
  TCP_SYN_RECV,
  TCP_FIN_WAIT1,
  TCP_FIN_WAIT2,
  TCP_TIME_WAIT,
  TCP_CLOSE,
  TCP_CLOSE_WAIT,
  TCP_LAST_ACK,
  TCP_LISTEN,
  TCP_CLOSING /* now a valid state */
};

void usage(char **argv)
{
  fprintf(stderr, "%s ipaddr port uid\n", argv[0]);
}

static int search_tcp_port(FILE *procinfo, char *check_addr, int check_port, int check_uid)
{
  do {
    char buffer[8192];

    if (fgets(buffer, sizeof(buffer), procinfo)) {
      unsigned long rxq, txq, time_len, retr, inode;
      int local_port, rem_port, d, state, uid, timer_run, timeout;
      char rem_addr[128], local_addr[128], more[512];
      struct sockaddr_in localaddr;
      char *localaddr_str;

      sscanf(buffer, "%d: %64[0-9A-Fa-f]:%X %64[0-9A-Fa-f]:%X %X %lX:%lX %X:%lX "
                     "%lX %d %d %ld %512s\n",
             &d, local_addr, &local_port, rem_addr, &rem_port, &state, &txq, &rxq, &timer_run, &time_len, &retr, &uid,
             &timeout, &inode, more);

      sscanf(local_addr, "%X", &((struct sockaddr_in *)&localaddr)->sin_addr.s_addr);
      ((struct sockaddr *)&localaddr)->sa_family = AF_INET;
      localaddr_str = inet_ntoa(localaddr.sin_addr);

      if ((state == TCP_LISTEN) && (check_port == local_port) && (uid == check_uid) &&
          (strcmp(localaddr_str, check_addr) == 0)) {
        printf("state=TCP_LISTEN ipaddr=%s port=%d inode=%ld uid=%d\n", inet_ntoa(localaddr.sin_addr), local_port,
               inode, uid);
        return 0;
      }
    }

  } while (!feof(procinfo));

  return 1;
}

int main(int argc, char *argv[])
{
  FILE *procinfo;
  int check_port, check_uid;
  char *check_addr;

  if (!argv[1]) {
    usage(argv);
    exit(1);
  } else {
    check_addr = argv[1];
  }

  if (!argv[2]) {
    usage(argv);
    exit(1);
  } else {
    check_port = atoi(argv[2]);
  }

  if (!argv[3]) {
    usage(argv);
    exit(1);
  } else {
    check_uid = atoi(argv[3]);
  }

  /*
   * /proc/net/tcp
   *
   * sl  local_address rem_address   st tx_queue rx_queue tr tm->when retrnsmt
   * uid  timeout inode
   *  4: 00000000:1F90 00000000:0000 0A 00000000:00000000 00:00000000 00000000
   * 808000        0 3760955 1 ffff88048b9d8780 100 0 0 10 0
   *
   * 00000000:1F90 <=> 0.0.0.0:8080
   */

  if ((procinfo = fopen(PROC_NET_TCP, "r")) == NULL) {
    perror(PROC_NET_TCP);
    exit(errno);
  }

  return search_tcp_port(procinfo, check_addr, check_port, check_uid);
}
