#pragma once

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>

struct line_node 
{
  char* string;
  int length;
};

struct window 
{
  int cx;
  int cy;
  int row_offset;
  int maxx;
  int maxy;
  int lines;
  struct line_node* linenode;
  char* file_name;
};

struct __string 
{
  char* string;
  int length;
};

/*
 * editor setup (cursor and window pos) + random shit
*/

void get_cursor_position(int *cx, int *cy);
void get_window_size(int *maxx, int *maxy);
int find_not_space(struct window *win, int at);
void append_string(struct __string *__string, char* _input_string, int _string_length);


/*
 * drawing to the screen
*/

void draw(int maxx, int maxy, struct window *win, struct __string *__string);

/*
 * FILE IO
*/

void save_file(struct window *win);
void load_file(struct window *win);

/*
 * USER IO
*/

void process_key(char c, struct window *win);
