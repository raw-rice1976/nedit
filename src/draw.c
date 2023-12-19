#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "structs.h"

void draw(int maxx, int maxy, struct window *win, struct __string *__string)
{
  unsigned int i;
  //(void)write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
  append_string(__string, "\x1b[2J\x1b[H", 7);
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
        //write(STDOUT_FILENO, buffer, win->maxx);
        append_string(__string, buffer, win->maxx);
        //write(STDOUT_FILENO, "\r\n", 2); 
        append_string(__string, "\r\n", 2);

      }
      else 
      {
        //write(STDOUT_FILENO, win->linenode[i].string, win->linenode[i].length);
        append_string(__string, win->linenode[i].string, win->linenode[i].length);
        //write(STDOUT_FILENO, "\r\n", 2);
        append_string(__string, "\r\n", 2);
      }
    }
    else 
    {
      //write(STDOUT_FILENO, "-\r\n", 3);
      append_string(__string, "-\r\n", 3);
    }
  }

  write(STDOUT_FILENO, __string->string, __string->length);
  __string->length = 0;
  /*char test[10];
  sprintf(test, "%d;%d", win->cx, win->cy);
  write(STDOUT_FILENO, &test, strlen(test));*/

  char buf[32];
  (void)sprintf(buf, "\x1b[%d;%dH", win->cy, win->cx);
  (void)write(STDOUT_FILENO, &buf, strlen(buf));
}
