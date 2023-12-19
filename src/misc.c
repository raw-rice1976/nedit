#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "structs.h"

void get_cursor_position(int *cx, int *cy)
{
  unsigned int i = 0;
  char buffer[16];
  (void)write(STDOUT_FILENO, "\x1b[6n", 4);
  while(i < sizeof(buffer) - 1)
  {
    (void)read(STDIN_FILENO, &buffer[i], 1);
    if(buffer[i] == 'R')
    {
      break;
    }
    i++;
  }
  buffer[i]= '\0';
  (void)sscanf(&buffer[2], "%d;%d", cy, cx);
}

void get_window_size(int *maxx, int *maxy)
{
  (void)write(STDOUT_FILENO, "\x1b[200C\x1b[200B", 12);
  get_cursor_position(maxx, maxy);
}

void append_string(struct __string *__string, char* _input_string, int _string_length)
{
  __string->string = realloc(__string->string, __string->length + _string_length);
  (void)memcpy(&__string->string[__string->length], _input_string, _string_length);
  __string->length += _string_length;
}
