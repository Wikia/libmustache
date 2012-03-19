
#include "template.hpp"

namespace mustache {


Template::~Template() {
  // Delete partials
  if( _partials.size() > 0 ) {
    Template::Partials::iterator it;
    for( it = _partials.begin() ; it != _partials.end(); it++ ) {
      delete it->second;
    }
  }
}


} // namespace Mustache
