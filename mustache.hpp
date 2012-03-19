
#ifndef MUSTACHE_HPP
#define MUSTACHE_HPP

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "data.hpp"
#include "exception.hpp"
#include "node.hpp"
#include "renderer.hpp"
#include "template.hpp"
#include "tokenizer.hpp"
#include "utils.hpp"

namespace mustache {


/*! \class Mustache
    \brief Container class for a tokenizer and a renderer

    This class contains the tokenizer and renderer.
*/
class Mustache {
  private:
  public:
    typedef std::auto_ptr<Mustache> Ptr;
    
    //! Tokenizer
    Tokenizer tokenizer;
    
    //! Renderer
    Renderer renderer;
    
    //! Utility method for Tokenizer::setStartSequence()
    void setStartSequence(const std::string& start) {
      return tokenizer.setStartSequence(start);
    };
    
    //! Utility method for Tokenizer::setStartSequence()
    void setStartSequence(const char * start, long len = 0) {
      tokenizer.setStartSequence(start, len);
    };
    
    //! Utility method for Tokenizer::setStopSequence()
    void setStopSequence(const std::string& stop) {
      tokenizer.setStopSequence(stop);
    };
    
    //! Utility method for Tokenizer::setStopSequence()
    void setStopSequence(const char * stop, long len = 0) {
      tokenizer.setStopSequence(stop, len);
    };
    
    //! Utility method for Tokenizer::setEscapeByDefault()
    void setEscapeByDefault(bool flag) {
      tokenizer.setEscapeByDefault(flag);
    };
    
    //! Utility method for Tokenizer::getStartSequence()
    const std::string & getStartSequence() {
      return tokenizer.getStartSequence();
    }
    
    //! Utility method for Tokenizer::getStopSequence()
    const std::string & getStopSequence() {
      return tokenizer.getStopSequence();
    };
    
    //! Utility method for Tokenizer::getEscapeByDefault()
    bool getEscapeByDefault() {
      return tokenizer.getEscapeByDefault();
    };
    
    //! Utility method for Tokenizer::tokenize()
    void tokenize(Template & tmpl) {
      tokenizer.tokenize(tmpl);
    };
    
    //! Utility method for Renderer::init() and Renderer::render()
    void render(Template * tmpl, Data * data, Node::Partials * partials, std::string * output) {
      renderer.init(tmpl, data, partials, output);
      renderer.render();
    };
};


} // namespace Mustache

#endif
