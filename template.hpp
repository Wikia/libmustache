
#ifndef MUSTACHE_TEMPLATE_HPP
#define MUSTACHE_TEMPLATE_HPP

#include <map>
#include <memory>
#include <stack>
#include <string>
//#include <unordered_map>
#include <vector>

#include "node.hpp"

namespace mustache {


class Template {
  public:
    //! Typedefs
    typedef std::auto_ptr<Template> Ptr;
    typedef std::map<std::string,Template*> Partials;
    
    //! Enum of states
    enum State {
      StateEmpty = 0,
      StateAssigned = 1,
      StateTokenized = 2
    };
    
  private:
    //! The template state
    Template::State _state;
    
    //! The template string value
    const std::string _str;
    
    //! The template root node
    Node _root;
    
    //! The internal template partials
    Template::Partials _partials;
    
    //! Internal constructor
    Template() : 
        _state(Template::StateEmpty) {};
    Template(Node node) : 
        _state(Template::StateTokenized),
        _root(node) {};
    
  public:
    //! Constructor
    Template(const std::string& tmpl) : 
        _state(Template::StateAssigned),
        _str(tmpl) {};
    Template(const char * tmpl, int len) : 
        _state(Template::StateAssigned),
        _str(tmpl, len) {};
    
    //! Destructor
    ~Template();
    
    //! Gets the template state
    const Template::State getState() {
      return _state;
    };
    
    //! Gets the template string
    const std::string& getString() {
      return _str;
    };
    
    //! Tokenizer is a friend
    friend class Tokenizer;
    friend class Renderer;
};


} // namespace Mustache

#endif

