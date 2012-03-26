
#include "data.hpp"

namespace mustache {


Data::~Data()
{
  switch( this->type ) {
    case Data::TypeString:
      delete val;
      break;
    case Data::TypeMap:
      if( data.size() > 0 ) {
        Data::Map::iterator dataIt;
        for ( dataIt = data.begin() ; dataIt != data.end(); dataIt++ ) {
          delete (*dataIt).second;
        }
        data.clear();
      }
      break;
    case Data::TypeArray:
      delete[] array;
      break;
  }
}

void Data::init(Data::Type type, int size) {
  this->type = type;
  this->length = size;
  switch( type ) {
    case Data::TypeString:
      val = new std::string();
      val->reserve(size);
      break;
    case Data::TypeMap:
      // Do nothing
      break;
    case Data::TypeArray:
      this->array = new Data[size];
      break;
  }
};

int Data::isEmpty()
{
  int ret = 0;
  switch( type ) {
    default:
    case Data::TypeNone:
      ret = 1;
      break;
    case Data::TypeString:
      if( val == NULL || val->length() <= 0 ) {
        ret = 1;
      }
      break;
    case Data::TypeList:
      if( children.size() <= 0 ) {
        ret = 1;
      }
      break;
    case Data::TypeMap:
      if( data.size() <= 0 ) {
        ret = 1;
      }
      break;
    case Data::TypeArray:
      if( length <= 0 ) {
        ret = 1;
      }
      break;
  }
  return ret;
}


void Stack::push(Data * data)
{
  if( this->size < 0 || this->size >= Stack::MAXSIZE ) {
    throw Exception("Reached max stack size");
  }
  this->stack[this->size] = data;
  this->size++;
}

void Stack::pop()
{
  if( this->size > 0 ) {
    this->size--;
    this->stack[this->size] = NULL;
  }
}

Data * Stack::top()
{
  if( this->size <= 0 ) {
    throw Exception("Reached bottom of stack");
  } else {
    return this->stack[this->size - 1];
  }
}

Data ** Stack::begin()
{
  return stack;
}

Data ** Stack::end()
{
  return (stack + size - 1);
}


} // namespace Mustache
