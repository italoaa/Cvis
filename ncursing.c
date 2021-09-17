#include <ncurses.h> /* ncurses.h includes stdio.h */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void ccolumnclear(WINDOW *win, int Size,int y, int x, int color);
void visclear(WINDOW *win, int arr[], int ArrSize);
void cswap(WINDOW *win, int *x, int *y, int Xindex, int Yindex);
int getCordx(WINDOW *win, int index);
int getCordy(WINDOW *win, int index);
void vis(WINDOW *, int[], int, int);
void ccolumn(WINDOW *win, int Size, int y ,int x, int color);
int part(WINDOW *win, int arr[], int start, int end);
void quicksort(WINDOW *win, int arr[], int start, int end);

int main(int argc, char *argv[]) {
  int parent_x, parent_y, new_x, new_y;
  int key = ERR;

  initscr();
  start_color();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);

  // set up initial windows
  getmaxyx(stdscr, parent_y, parent_x);

  WINDOW *center = newwin(parent_y, parent_x, 0, 0);

  box(center, 0, 0);
  int test[100];

  int i;
  for (i = 0; i < 40; i++) {
    test[i] = rand() % 10;
  }
  // White
  init_pair(1,7,0);

  // Red
  init_pair(2,1,0);

  // yellow
  init_pair(3,3,0);

  // blue
  init_pair(4,4,0);

  //Magenta
  init_pair(5,5,0);

  //Green
  init_pair(6, 2, 0);

  int limit = new_x - 8;
  vis(center, test, 40, 1);
  quicksort(center, test, 0, 40);
  visclear(center, test, 40);
  vis(center, test, 40,1);
  
  while (key != 'q') {
    if (key == KEY_RESIZE) {
#ifdef PDCURSES
      resize_term(0, 0);
#endif
      getmaxyx(stdscr, new_y, new_x);

      if (new_y != parent_y || new_x != parent_x) {
        parent_x = new_x;
        parent_y = new_y;

        wresize(center, new_y, new_x);

        wclear(stdscr);
        wclear(center);

        box(center, 0, 0);
      }
    }
    // draw to our windows
    mvwprintw(center, 1, 1, "Sorted list - Press[q] to quit...");

    // refresh each window
    wrefresh(stdscr);
    wrefresh(center);

    key = getch();

  }

  endwin();
  return 0;
}

//color column , color is 1 2 or 3
void ccolumn(WINDOW *win, int Size,int y, int x, int color) {
  wattron(win, COLOR_PAIR(color));
  for (int i = 0; i < Size; i++){
    mvwprintw(win,y,x,"$");
    y--;
    wrefresh(stdscr);
    wrefresh(win);
  }
  wattroff(win,COLOR_PAIR(color));
}


void quicksort(WINDOW *win, int arr[], int start, int end) {
  if (start < end) {
    int Pindex = part(win,arr, start, end);
    quicksort(win ,arr, start, Pindex-1);
    quicksort(win ,arr, Pindex+1, end);
  }
}

int part(WINDOW *win, int arr[], int start, int end) {
  int Pindex = start;
  int pivot = arr[end];
  int endIndex = end-1;
  int i;
  for (i = start; i < end; i++){
    //Clear the  screen
    visclear(win, arr, 40);
    //Print the current list
    vis(win, arr, 40,1);
    //Color the item that is being checked
    ccolumn(win, arr[i], getCordy(win, i), getCordx(win, i+1), 2);
    //Color the Pivot
    ccolumn(win, arr[endIndex], getCordy(win, end), getCordx(win, end), 4);
    //Color the Pindex
    ccolumn(win, arr[Pindex-1], getCordy(win, Pindex), getCordx(win, Pindex), 5);
    unsigned int mSeconds = 3000;
    unsigned int swapTime = 10000;
    if (arr[i] < pivot) {
      //Color the item that passed the check
      ccolumn(win, arr[i], getCordy(win, i), getCordx(win, i+1), 6);
      //Color the pindex that is swaping
      ccolumn(win, arr[Pindex-1], getCordy(win, Pindex), getCordx(win, Pindex), 6);
      usleep(swapTime);
      cswap(win, &arr[i], &arr[Pindex], i, Pindex);
      Pindex++;
    }
    usleep(mSeconds);
  }
  cswap(win, &arr[end], &arr[Pindex], end ,Pindex);
  return Pindex;
}

void cswap(WINDOW *win, int *x, int *y, int Xindex, int Yindex) {
  int t = *x;
  *x = *y;
  *y = t;
}

int getCordx(WINDOW *win, int index) {
  int x,y;
  getmaxyx(win, y, x);
  x = 4;
  for (int i = 0; i < index; i++) {
    x = x + 2;
  }
  return x;
}

int getCordy(WINDOW *win, int index) {
  int x,y;
  getmaxyx(win, y, x);
  y = y - 4;
  return y;
}
//Prints a white visualizer
void vis(WINDOW *win, int arr[], int ArrSize,int color) {
  int i, j;
  int x, y;
  getmaxyx(win, y, x);
  y = y - 4;
  x = 4;

  for (i = 0; i < ArrSize; i++) {
    int nums = arr[i];
    x = x + 2;
    wattron(win,COLOR_PAIR(3));
    mvwprintw(win, y + 1, x, "%d", nums);
    wattroff(win,COLOR_PAIR(3));
    ccolumn(win,nums,y,x,color);
    y = getmaxy(win) - 4;
    sleep(0.70);
  }
}

void visclear(WINDOW *win, int arr[], int ArrSize) {
  int i, j;
  int x, y;
  getmaxyx(win, y, x);
  y = y - 4;
  x = 4;

  for (i = 0; i < ArrSize; i++) {
    int nums = arr[i];
    x = x + 2;
    wattron(win,COLOR_PAIR(3));
    mvwprintw(win, y + 1, x, "%d", nums);
    wattroff(win,COLOR_PAIR(3));
    ccolumnclear(win,nums,y,x,1);
    y = getmaxy(win) - 4;
  }
}

void ccolumnclear(WINDOW *win, int Size,int y, int x, int color) {
  Size = 40;
  wattron(win, COLOR_PAIR(color));
  for (int i = 0; i < Size; i++){
    mvwprintw(win,y,x," ");
    y--;
    wrefresh(stdscr);
    wrefresh(win);
  }
  wattroff(win,COLOR_PAIR(color));
}
