
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
    typedef std::vector<std::string> Fragments;
    typedef std::vector<Node> Nodes;
    typedef std::map<std::string,Template> Partials;
    
    //! Constants
    static const int defaultTemplateSize = 1000;
    static const int defaultFragmentsSize = 400;
    static const int defaultNodesSize = 200;
    
    //! The template string value
    std::string str;
    
    //! The template fragments
    Template::Fragments fragments;
    
    //! The template node list
    Template::Nodes nodes;
    
    //! The template root node
    Node * root;
    
    //! The internal template partials
    Template::Partials partials;
    
    //! Constructor
    Template(const std::string& tmpl) : 
        str(tmpl),
        fragments(defaultFragmentsSize), 
        nodes(defaultNodesSize), 
        root(NULL) {};
    Template(const char * tmpl, int len) : 
        str(tmpl, len), 
        fragments(defaultFragmentsSize), 
        nodes(defaultNodesSize), 
        root(NULL) {};
    
    //! Gets the template string
    const std::string& getString() {
      return str;
    };
};


} // namespace Mustache

#endif

