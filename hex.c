#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define BUF_SIZE 16
int total = 0;
void writ(char character) {
  if(write(STDOUT_FILENO, &character, 1) == -1)
    perror("write");
}

void hex(int h) {
  char hex[16] = "0123456789ABCDEF";
  writ(hex[(h/16)%16]);
  writ(hex[h%16]);
}

void processBuf(unsigned char *buf, int r) {
  int spacer = 1;
  for (int i = 0; i < r; i++) { 
    hex(buf[i]);
    if (!(i == BUF_SIZE-1))
     writ(' ');
    if (spacer % 8 == 0)
      writ(' ');
    spacer++;
  }
  if (r < 16) {
    for (int i = r; i < 16; i++) 
      for (int j = 0; j < 3; j++)
        writ(' ');
  }
  writ(':');
  writ(' ');
}

void printBuf(unsigned char *buf, int r) {
  for (int i = 0; i < r; i++) {
    if (isprint(buf[i]))
      writ(buf[i]);
    else
      writ('.');
  }
  writ('\n');
}

void offset(int r) {
  total += r;
  unsigned int offset = 0x8;
  char hex[16] = "0123456789ABCDEF";
  writ(hex[(total/16)%16]);
  writ(hex[total%16]);
}

int main(int argc, char *argv[]) {
  int fd;
  int r;
  unsigned char buf[BUF_SIZE];
  if (argv[1] != NULL) {
    fd = open(argv[1], O_RDONLY, 0666);
    while ((r = read(fd, buf, BUF_SIZE)) > 0) {
      offset(r);
      writ(':');
      writ(' ');
      processBuf(buf, r);
      printBuf(buf, r);
    }
    close(fd);

  } else {
    while ((r = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
      offset(r);
      writ(':');
      writ(' ');
      processBuf(buf, r);
      printBuf(buf, r);
    }
  
  }

  return 0;
}
