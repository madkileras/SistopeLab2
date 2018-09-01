#include "buffer.hpp"

Buffer::Buffer(){
  this->front = 0;
  this->back = 0;
  this->count = 0;
  this->buffer1=0;
  this->full= false;
  this->empty=true;
  //this->elements = NULL;
}

Buffer::Buffer(int buffer1){
  this->front = 0;
  this->back = 0;
  this->count = 0;
  this->full= false;
  this->empty=true;
  this->buffer1=buffer1;
  //this->elements = new imageControl[this->buffer1];
}

_Nomutex int Buffer::query(){
  return count;
}

bool Buffer::getFull(){
  return this->full;
}

bool Buffer::getEmpty(){
  return this->empty;
}

void Buffer::push(ImageControl element){
  if(this->count==this->buffer1){
    this->full=true;
    //cout<<"Se llena buffer"<<endl;
    _Accept(pull);
  }
  this->empty=false;
  //cout<<element<<endl;
  this->elements[this->back]=element;
  this->back = (this->back+1)%this->buffer1;
  this->count+=1;

}

ImageControl Buffer::pull(){
  if(this->count==0){
    this->empty=true;
    _Accept(push);
  }
  ImageControl element = this->elements[this->front];
  this->front=(this->front+1)%this->buffer1;
  this->count -= 1;
  return element;
  //return "Cambiar element";
}
