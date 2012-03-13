
#ifndef MUSTACHE_NODE_HPP
#define MUSTACHE_NODE_HPP

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace mustache {


class Node {
  public:
    typedef std::auto_ptr<Node> Ptr;
    typedef std::vector<Node *> Children;
    typedef std::stack<Node *> Stack;
    typedef std::map<std::string,std::string> RawPartials;
    typedef std::map<std::string,Node::Ptr> Partials;
    typedef std::pair<std::string,Node::Ptr> PartialPair;
    enum Type {
      TypeNone = 0,
      TypeRoot = 1,
      TypeOutput = 2,
      TypeTag = 3
    };
    enum Flag { 
      FlagNone = 0,
      FlagEscape = 1,
      FlagNegate = 2,
      FlagSection = 4,
      FlagStop = 8,
      FlagComment = 16,
      FlagPartial = 32,
      FlagInlinePartial = 64,
      
      FlagHasChildren = Node::FlagNegate | Node::FlagSection | Node::FlagPartial
    };
    
    Node::Type type;
    int flags;
    std::string * data;
    Node::Children children;
    
    ~Node();
};


} // namespace Mustache

#endif


