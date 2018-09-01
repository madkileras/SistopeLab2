#include <uC++.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "imageControl.hpp"

using namespace std;

_Monitor Buffer{
private:
  int front, back, count, buffer1;
  ImageControl* elements;
  bool full, empty;
public:
  Buffer();
  Buffer(int buffer1);
  /*~Buffer(){
  	delete [] elements;
  };*/
  bool getFull();
  bool getEmpty();
  _Nomutex int query();
  void push(ImageControl element);
  ImageControl pull();
};