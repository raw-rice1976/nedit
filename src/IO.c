#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "structs.h"

#define ctrlkey(k) ((k) & 0x1f)

void move_cursor(char c, struct window *win)
{
  switch(c) 
  {
    // a - up b - down c - left d - right
    case 'A':
      {
        if(win->cy != 1)
        {
          win->cy--;
        }
        else if(win->row_offset != 0)
        {
          win->row_offset--;
        }
      } break;
    case 'B':
      {
        if(win->maxy > (win->cy))
        {
          win->cy++;
        }
        else if((win->row_offset + win->maxy) < (win->lines + 1))
        {
          win->row_offset++;
        }
      } break;
    case 'C':
      {
        if(win->maxx >= win->cx)
        win->cx++;
      } break;
    case 'D':
      {
  if(win->cx != 1)
        {
          win->cx--;
        }
      }
  }
}

void save_file(struct window *win)
{
  int fd = open(win->file_name, O_RDWR | O_CREAT, 0664);
  if(fd == -1)
  {
    (void)exit(0);
  }
int length = 0;

  for(int i = 0; i < win->lines; i++)
  {
    length += win->linenode[i].length + 1;
  }
  char* buffer = malloc(length);
  char* p = buffer;
  for(int i = 0; i < win->lines; i++)
  {
    (void)memcpy(p, win->linenode[i].string, win->linenode[i].length);
    p += win->linenode[i].length;
    *p = '\n';
    p++;
    /*printf("   %s   ", win->linenode[i].string);
    exit(0);*/
  }

  (void)ftruncate(fd, length);
  (void)write(fd, buffer, length);
  (void)close(fd);
  (void)free(buffer);
}

int find_not_space(struct window *win, int at)
{
  for(int i = 0; i < win->linenode[at].length; i++)
  {
    if(win->linenode[at].string[i] != ' ')
    {
      return i;
    }
    else if (win->linenode[at].length == i) {
      return win->linenode[at].length;
    }
  }
  return 0;
}

/*
 * insert_character
 * inserts a character into a line
*/

void insert_character(char c, struct window *win)
{
  int cur_row = win->cy + win->row_offset - 1;
  //check if we arent out of bounds of the line node or in a row that doesnt exist
  if(win->cx < win->linenode[cur_row].length + 2 && cur_row < win->lines)
  {
    // increase the actual size of the string 
    win->linenode[cur_row].string = realloc(win->linenode[cur_row].string, //current string
                                            win->linenode[cur_row].length + 2); //target size of the string

    //make space between characters for the new one
    (void)memmove(&win->linenode[cur_row].string[win->cx], 
                  &win->linenode[cur_row].string[win->cx -1], 
                  win->linenode[cur_row].length - win->cx + 1);

    //increase the length of the string (of linenode struct)
    win->linenode[cur_row].length++;

    //insert the character at cursor position x - 1
    win->linenode[cur_row].string[win->cx - 1] = c;

    //increment the cursor position
    win->cx++;
  }
}

void process_key(char c, struct window *win)
{
  switch(c) 
  {
    case ctrlkey('q'):
      {
        exit(0);
      } break;
    case ctrlkey('s'):
      {
        save_file(win);
      }break;
    default:
      {
        insert_character(c, win);
      } break;
    case 127:
      {
        int at = win->cy+win->row_offset - 1;
        if(((win->cx - 1) < win->linenode[at].length) && win->cx != 1 && at < win->lines)
        {
          char end;
          end = win->linenode[at].string[win->linenode[at].length - 1];
          (void)memmove(&win->linenode[at].string[win->cx - 2], &win->linenode[at].string[win->cx - 1], win->linenode[at].length - win->cx);
          win->linenode[at].length--;
          win->linenode[at].string[win->linenode[at].length - 1] = end;
          win->cx--;
        }
        else if(((win->cx - 1) == win->linenode[at].length) && win->cx != 1 && at < win->lines)
        {
          win->linenode[at].length--;
          win->cx--;
        }
        else if((win->cx == 1) && at != 0 && at < win->lines)
        {
          int lengths = win->linenode[at - 1].length + win->linenode[at].length;
          win->linenode[at - 1].string = realloc(win->linenode[at - 1].string, (win->linenode[at - 1].length + win->linenode[at].length));
          (void)memmove(&win->linenode[at - 1].string[win->linenode[at - 1].length], &win->linenode[at].string[0], win->linenode[at].length);
          (void)memmove(&win->linenode[at], &win->linenode[at + 1], sizeof(struct line_node) * (win->lines - at));
          win->linenode[at - 1].length = lengths;
          win->lines--;
          if(win->cy != 0)
          {
            win->cy--;
            win->cx = lengths + 1;
          }
          else{
            win->row_offset--;
          }
        }
      } break;
    case 13:
      {
        if(win->cx - 1  <= win->linenode[(win->cy + win->row_offset) - 1].length && (win->cy + win->row_offset - 1) < win->lines)
        {
          int length = win->linenode[win->cy + win->row_offset - 1].length;
          int at = win->cy + win->row_offset - 1;
          int index = 0;

          win->linenode = realloc(win->linenode, sizeof(struct line_node) * (win->lines + 1));
          (void)memmove(&win->linenode[at+1], &win->linenode[at], sizeof(struct line_node) * ((win->lines) - at));


          index = find_not_space(win, at);

          //the next nodeline
          win->linenode[at+1].string = malloc((win->linenode[at].length + index) - win->cx);
          if(index > 0)
          {
            (void)memset(win->linenode[at+1].string, ' ', index);
          }
          (void)memmove(&win->linenode[at+1].string[index], &win->linenode[at].string[win->cx - 1], (win->linenode[at].length - win->cx) + 1);
          win->linenode[at+1].length = length - win->cx + index + 1;

          //first nodeline
          win->linenode[at].length = win->cx-1;

          if(win->cy == win->maxy)
          {
            win->row_offset++;
            win->cx = index;
          }
          else {
            win->cy++;
            win->cx = index+1;
          }
          win->lines++;
        }
      } break;
    case '\x1b':
      {
        char buffer[3];
        if(read(STDIN_FILENO, &buffer[0], 1) != 1)
        {
          ;;
        }
        if(read(STDIN_FILENO, &buffer[1], 1) != 1)
        {
          ;;
        }
        if(buffer[0] == '[')
        {
          switch(buffer[1])
          {
            case 'A':
            case 'B':
            case 'C':
            case 'D':
              {
                 move_cursor(buffer[1], win);
              } break;
          }
        }
      }
  }
}

void load_file(struct window *win)
{
  int file = open(win->file_name, O_RDWR | O_CREAT, 0664);
  FILE *fd = fopen(win->file_name, "rw");

  char *buffer;
  size_t bufsize = 0;
  size_t length = 0;
  win->lines = 0;
  while ((length = getline(&buffer, &bufsize, fd)) != -1)
  {
    while(length > 0 && (buffer[length - 1] == '\n' || buffer[length - 1] == '\r'))
      length--;
    win->linenode = realloc(win->linenode, sizeof(struct line_node) * (win->lines + 1));
    win->linenode[win->lines].string = malloc(bufsize + 1);
    win->linenode[win->lines].length = length;
    (void)memcpy(win->linenode[win->lines].string, buffer, length + 1);
    win->linenode[win->lines].string[length] = '\0';
    win->lines++;
  }
  if(win->lines == 0)
  {
    (void)write(file, " \n", 2);
    load_file(win);
  }
  (void)close(file);
  (void)fclose(fd);
  (void)free(buffer);
}
