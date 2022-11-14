#ifndef FUNCTIONS_C
#define FUNCTIONS_C

#include <stdio.h>  // printf()
#include <stdlib.h> // rand()
#include <time.h>   // time()
#include <errno.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void) //Überprüft ob zeichen im Tastaturpuffer
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  // newt.c_lflag &= ~(ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

#endif