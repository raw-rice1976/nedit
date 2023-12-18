#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "structs.h"

void draw(int maxx, int maxy, struct window *win)
{
  unsigned int i;
  write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
  for(i = win->row_offset; i < (maxy + win->row_offset - 1); i++)
  {
    if(win->lines > i)
    {
      if(win->linenode[i].length >= win->maxx)
      {
        char *buffer;
        buffer = malloc(win->maxx + 1);
        memmove(buffer, &win->linenode[i].string[0], win->maxx);
        buffer[win->maxx + 1] = '\0';
        write(STDOUT_FILENO, buffer, win->maxx);
        write(STDOUT_FILENO, "\r\n", 2);
      }
      else 
      {
        write(STDOUT_FILENO, win->linenode[i].string, win->linenode[i].length);
        write(STDOUT_FILENO, "\r\n", 2);
      }
    }
    else 
    {
      write(STDOUT_FILENO, "-\r\n", 3);
    }
  }
  /*char test[10];
  sprintf(test, "%d;%d", win->cx, win->cy);
  write(STDOUT_FILENO, &test, strlen(test));*/

  char buf[32];
  sprintf(buf, "\x1b[%d;%dH", win->cy, win->cx);
  write(STDOUT_FILENO, &buf, strlen(buf));
}
