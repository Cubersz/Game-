#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <thread>
using namespace std;

//[1,2,3,4,5] int*
int keypress() {
  system("/bin/stty raw");
  int c;
  system("/bin/stty -echo");
  c = getc(stdin);
  system("/bin/stty echo");
  system("/bin/stty cooked");
  return c;
}
int keypressASCIIImplementation(){
  char c;
  cin >> c;
  cout << "\033[2K";
  return c;
}
void start(int length, string **display){
    for(int i = 0; i < length; i++){
  display[i][0] = "| ";
  display[i][length - 1] = " |";
      }
    for(int j = 0; j < length; j++){
  display[0][j] = ". ";
  display[length - 1][j] = ". ";
     }
}
//cut down on cout calls to increase performance
void screen(int length, string **display) {
  static int i = 0;
  string toPrint = "";
  for (int i = 0; i < length; i++) {
    toPrint += display[i][0];
    for (int j = 1; j < length; j++) {
      toPrint += display[i][j];
    }
    toPrint += "\n";
  }
  cout << toPrint << endl << endl;
  //display[10][10] = to_string(i++) + " ";
}

// &x has an integer x as a parameter passed by reference. Changes made to x inside "update" are made to the x outside of "update". Similar to pointers, but you lose some freedom.
void update(int length, string **display, int &x, int &y, int &nx, int &ny) {
int key = keypress();
bool left = false;
bool up = false;
bool right = false;
bool down = false;
if(key == 68){
  left = true;
}
else{
  left = false;
}
if(key == 65){
  up = true;
}
else{
   up = false; 
  }
if(key == 67){
  right = true;
}
else{
  right = false;
}
if(key == 66){
  down = true;
}
else{
  down = false;
}
//key == (65 up, 68 left, 67 right, 66 down)
   display[y][nx] = "  ";
   display[ny][x] = "  ";
   display[y][x] = "O ";
  /*why dosen't this delete the line before, where the player was
trying to make movement. I see the issue, let me try something. what does &x/&y mean
  Let me try something*/
if(left){
  nx = x;
  x--;
  /*... is that it WTF a single space
  You were on the right track, your only mistake was not actually setting nx = to anything. nx - 2 doesnt do anything on its own unless you say nx = nx - 2, or nx -=2. Though in this case a more elegant solution exists in nx = x; x--; I did to that below where you defined x
  You want to make sure you do it everytime you update. By doing it in the update loop, you always maintain that nx = the last position of x
  that makes a lot of sense*/
}
if(right){
  nx = x;
  x++;
}
if(down){
  ny = y;
  y++;
}
if(up){
  ny = y;
  y--;
}
if(y == 0 || y == length){
  display[0][x] = ". ";
  display[19][x] = ". ";
  y = length / 2;
  x = length / 2;
  }
if(x == 0 || x == length){
  display[y][1] = "  ";
  display[y][18] = "  ";
  x = length / 2;
  y = length / 2;
  }
}
void blank(int lines) {
  for (int i = 0; i < lines; i++) {
    cout << endl;
  }
}

//The only faster way to do this would be using an OS specific api to access lowerlevel information. Calling multiple system commands and getting input from the console is an expensive operation

//works in theory but relies on enter key, way to avoid that? Promissing but does not help speed(might actually slow down)


int main() {
  int length = 20;
  string **display = new string *[length];
  for (int i = 0; i < length; i++) {
    display[i] = new string[length];
    for (int j = 0; j < length; j++) {
      display[i][j] = "  ";
    }
    
  }
  //store players position
  int x = length / 2;
  int y = length / 2;
  int nx = x;
  int ny = y;

  start(length, display);

  do { // works now but still get laggy mind if I take over? not at all. I gotta go, but I will continue looking into this later
    //clear console
    
    cout << "\033[3J\033[0;0H";
    screen(length, display);
    update(length, display,x,y,nx,ny);
    //this_thread::sleep_for(std::chrono::milliseconds(400));

    //try avoiding system calls to see if it helps performance
     //std::cout << key << "\n";
  } while (true);

  for (int i = 0; i < length; i++) {
    delete[] display[i];
  }
  delete[] display;
}
