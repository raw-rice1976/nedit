#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.h"

void get_cursor_position(int *cx, int *cy)
{
  unsigned int i = 0;
  char buffer[16];
  write(STDOUT_FILENO, "\x1b[6n", 4);
  while(i < sizeof(buffer) - 1)
  {
    read(STDIN_FILENO, &buffer[i], 1);
    if(buffer[i] == 'R')
    {
      break;
    }
    i++;
  }
  buffer[i]= '\0';
  sscanf(&buffer[2], "%d;%d", cy, cx);
}

void get_window_size(int *maxx, int *maxy)
{
  write(STDOUT_FILENO, "\x1b[200C\x1b[200B", 12);
  get_cursor_position(maxx, maxy);
}
