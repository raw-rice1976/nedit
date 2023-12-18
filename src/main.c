#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include "structs.h"

struct termios original;
struct termios raw;

struct window wc;

#define ctrlkey(k) ((k) & 0x1f)

void disable_raw_mode()
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
}

void enable_raw_mode()
{
  tcgetattr(STDIN_FILENO, &original);
  raw = original;

  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  atexit(disable_raw_mode);
}

void init()
{
  enable_raw_mode();
  atexit(disable_raw_mode);
  wc.cx = 1;
  wc.cy = 1;
  wc.maxx = 0;
  wc.maxy = 0;
  wc.lines = 0;
  wc.row_offset = 0;
  get_window_size(&wc.maxx, &wc.maxy);
  write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
}

int main(int argc, char *argv[])
{
  if(argc == 1)
  { 
    printf("usage: nedit <filename> \r\n");
    exit(1);
  }
  init();
  wc.file_name = argv[1];
  load_file(&wc);
  write(STDOUT_FILENO, "\x1b[H", 3);
  draw(wc.maxx, wc.maxy, &wc);
  char c;
  while(1)
  {
    read(STDIN_FILENO, &c, 1);
    process_key(c, &wc);
    draw(wc.maxx, wc.maxy, &wc);
  }
}
