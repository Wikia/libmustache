
#include "node.hpp"

#include "exception.hpp"
#include "utils.hpp"

namespace mustache {


Node::~Node()
{
  // Helper
  if( helper != NULL ) {
    delete helper;
  }
  
  // Data
  if( data != NULL ) {
    delete data;
  }
  
  // Data parts
  if( dataParts != NULL ) {
    delete dataParts;
  }
  
  // Children
  if( children.size() > 0 ) {
    Node::Children::iterator it;
    for ( it = children.begin() ; it != children.end(); it++ ) {
      delete *it;
    }
  }
  children.clear();
  
  // Child should not be freed
}

void Node::setData(const std::string& data)
{
  this->data = new std::string(data);
  
  if( this->type & Node::TypeHasDot ) {
    size_t found = data.find(".");
    if( found != std::string::npos ) {
      dataParts = new std::vector<std::string>;
      explode(".", *(this->data), dataParts);
    }
  }
}

std::string Node::toString() {
  std::stringstream ret;
  ret << "[type=";
  switch( type ) {
    case Node::TypeNone: ret << "None"; break;
    case Node::TypeRoot: ret << "Root"; break;
    case Node::TypeOutput: ret << "Output"; break;
    case Node::TypeTag: ret << "Tag"; break;
    case Node::TypeContainer: ret << "Container"; break;
    case Node::TypeVariable: ret << "Variable"; break;
    case Node::TypeNegate: ret << "Negate"; break;
    case Node::TypeSection: ret << "Section"; break;
    case Node::TypeStop: ret << "Stop"; break;
    case Node::TypeComment: ret << "Comment"; break;
    case Node::TypePartial: ret << "Partial"; break;
    case Node::TypeInlinePartial: ret << "InlinePartial"; break;
    case Node::TypeHelper: ret << "Helper"; break;
    case Node::TypeBlockHelper: ret << "BlockHelper"; break;
    default: ret << "Unknown"; break;
  }
  if( data != NULL ) {
    ret << ",data=" << *this->data;
  }
  if( helper != NULL ) {
    ret << ",helper=" << *this->helper;
  }
  ret << ",children=" << children.size();
  ret << "]";
  return ret.str();
}



void NodeStack::push_back(Node * node)
{
  if( _size < 0 || _size >= NodeStack::MAXSIZE ) {
    throw Exception("Reached max stack size");
  }
  _stack[_size] = node;
  _size++;
}

void NodeStack::pop_back()
{
  if( _size > 0 ) {
    _size--;
    _stack[_size] = NULL;
  }
}

Node * NodeStack::back()
{
  if( _size <= 0 ) {
    throw Exception("Reached bottom of stack");
  } else {
    return _stack[_size - 1];
  }
}

Node ** NodeStack::begin()
{
  return _stack;
}

Node ** NodeStack::end()
{
  return (_stack + _size - 1);
}


} // namespace Mustache
