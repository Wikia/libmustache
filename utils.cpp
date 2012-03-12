
#include "utils.hpp"

namespace mustache {



void trimRight(std::string& str, const std::string& trimChars)
{
  std::string::size_type pos = str.find_last_not_of(trimChars);
  str.erase(pos + 1);    
}

void trimLeft(std::string& str, const std::string& trimChars)
{
  std::string::size_type pos = str.find_first_not_of(trimChars);
  str.erase(0, pos);
}

void trim(std::string& str, const std::string& trimChars)
{
  trimRight(str, trimChars);
  trimLeft(str, trimChars);
}

void htmlspecialchars(std::string * str)
{
  std::string tmp;
  int pos = 0;
  int len = str->length();
  char * chr = (char *) str->c_str();
  tmp.reserve(len * 2);
  for( pos = 0; pos < len; pos++, chr++ ) {
    switch( *chr ) {
      case '&':
        tmp.append("&amp;");
        break;
      case '"':
        tmp.append("&quot;");
        break;
      case '\'':
        tmp.append("&#039;");
        break;
      case '<':
        tmp.append("&lt;");
        break;
      case '>':
        tmp.append("&gt;");
        break;
      default:
        tmp.append(1, *chr);
        break;
    }
  }
  str->swap(tmp);
}

void explode(const std::string &delimiter, const std::string &str, std::vector<std::string> * arr)
{
  std::vector<std::string> * arr = new std::vector<std::string>;

  int strleng = str.length();
  int delleng = delimiter.length();
  if( delleng == 0 ) {
    // no change
    return arr;
  }

  int i = 0;
  int k = 0;
  while( i < strleng ) {
      int j = 0;
      while( i + j < strleng && j < delleng && str[i+j] == delimiter[j] ) {
        j++;
      }
      if( j == delleng ) {
        // found delimiter
        arr->push_back(str.substr(k, i-k));
        i += delleng;
        k = i;
      } else {
        i++;
      }
  }
  arr->push_back(str.substr(k, i-k));
  return arr;
}

void stringTok(const std::string &str, const std::string &delimiters, std::vector<std::string> * tokens)
{
  // Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  
  // Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while( std::string::npos != pos || std::string::npos != lastPos ) {
    // Found a token, add it to the vector.
    tokens->push_back(str.substr(lastPos, pos - lastPos));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
  
  return tokens;
}



} // namespace Mustache
