
#ifndef MUSTACHE_TEMPLATE_HPP
#define MUSTACHE_TEMPLATE_HPP

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "node.hpp"

namespace mustache {


class Template {
  public:
    std::string tmpl;
    Node node;
}


} // namespace Mustache

#endif

