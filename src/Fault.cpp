#include "Fault.hpp"

Fault::Fault(){
  this->base = false;
  this->signaled = false;
  this->suppressed = false;
}

void Fault::signal(){
  this-> signaled = true;
  if(!this->suppressed){
    this->base = true;
  }
}

void Fault::release(){
  this->signaled = false;
  this->base = false;
}

void Fault::suppress(){
  this->suppressed = true;
  this->base = false;
}

void Fault::unsuppress(){
  this->suppressed = false;
  this->base = this->signaled;
}