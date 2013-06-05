
#include "test_spec.hpp"

std::list<MustacheSpecTest *> tests; 

int main( int argc, char * argv[] )
{
  if( argc < 2 ) {
    std::cerr << "Requires at least one argument\n";
    return 1;
  }
  
  DIR * dir;
  struct dirent * ent;
  char * directory = argv[1];
  if( (dir = opendir(directory)) == NULL ) {
    std::cerr << "Unable to open directory\n";
    return 1;
  }
  
  int nFiles = 0;
  while( (ent = readdir(dir)) != NULL ) {
    if( ent->d_name[0] == '.' ) continue;
    if( strlen(ent->d_name) < 5 ) continue;
    if( strcmp(ent->d_name + strlen(ent->d_name) - 4, ".yml") != 0 ) continue;
    if( *(ent->d_name) == '~' ) continue; // Ignore lambdas
    if( nFiles >= MAX_TEST_FILES ) continue;
    
    // Make filename
    char * file = ent->d_name;
    std::string fileName;
    fileName += directory;
    fileName += '/';
    fileName += file;
    
    std::ifstream pFile(fileName.c_str());
    if( !pFile.is_open() ) {
      std::cerr << "Unable to open file: " << fileName;
      continue;
    }
    
    // get length of file:
    pFile.seekg (0, pFile.end);
    int length = pFile.tellg();
    pFile.seekg (0, pFile.beg);
    
    // read file data
    char * fileData = new char[length];
    pFile.read(fileData, length);
    pFile.close();
    
    // parse the file
    std::cout << fileName << "\n";
    parse_file(fileData, length);
  }
  closedir(dir);
  
  
  // Summarize
  std::list<MustacheSpecTest *>::iterator it = tests.begin();
  int nPassed = 0;
  int nFailed = 0;
  for( ; it != tests.end(); ++it ) {
    if( (*it)->passed() ) {
      nPassed++;
    } else {
      nFailed++;
    }
  }
  int total = nPassed + nFailed;
  std::cout << nPassed << " passed, "
            << nFailed << " failed of "
            << total << " tests\n";
  return (nFailed > 0 ? 1 : 0);
}

void parse_file(char * fileData, int length)
{
  // start yaml parser
  yaml_parser_t parser;
  yaml_document_t document;
  yaml_parser_initialize(&parser);

  const unsigned char * input = reinterpret_cast<const unsigned char *>(fileData);

  yaml_parser_set_input_string(&parser, input, length);
  yaml_parser_load(&parser, &document);
  
  mustache_spec_parse_document(&document);
  
  yaml_document_delete(&document);
  yaml_parser_delete(&parser);
}


void mustache_spec_parse_document(yaml_document_t * document)
{
  yaml_node_t * node = yaml_document_get_root_node(document);
  if( node->type != YAML_MAPPING_NODE ) {
    return;
  }
  
  yaml_node_pair_t * pair;
  for( pair = node->data.mapping.pairs.start; pair < node->data.mapping.pairs.top; pair++ ) {
    yaml_node_t * keyNode = yaml_document_get_node(document, pair->key);
    yaml_node_t * valueNode = yaml_document_get_node(document, pair->value);
    char * keyValue = reinterpret_cast<char *>(keyNode->data.scalar.value);
    if( strcmp(keyValue, "tests") == 0 &&
        valueNode->type == YAML_SEQUENCE_NODE ) {
      mustache_spec_parse_tests(document, valueNode);
    }
  }
}

void mustache_spec_parse_tests(yaml_document_t * document, yaml_node_t * node)
{
  if( node->type != YAML_SEQUENCE_NODE ) {
    return;
  }
  
  yaml_node_item_t *item;
  for( item = node->data.sequence.items.start; item < node->data.sequence.items.top; item ++) {
    yaml_node_t * valueNode = yaml_document_get_node(document, *item);
    if( valueNode->type == YAML_MAPPING_NODE ) {
      mustache_spec_parse_test(document, valueNode);
    }
  }
}

void mustache_spec_parse_test(yaml_document_t * document, yaml_node_t * node)
{
  if( node->type != YAML_MAPPING_NODE ) {
    return;
  }
  
  MustacheSpecTest * test = new MustacheSpecTest;
  
  yaml_node_pair_t * pair;
  for( pair = node->data.mapping.pairs.start; pair < node->data.mapping.pairs.top; pair++ ) {
    yaml_node_t * keyNode = yaml_document_get_node(document, pair->key);
    yaml_node_t * valueNode = yaml_document_get_node(document, pair->value);
    char * keyValue = reinterpret_cast<char *>(keyNode->data.scalar.value);
    
    if( valueNode->type == YAML_SCALAR_NODE ) {
      char * valueValue = reinterpret_cast<char *>(valueNode->data.scalar.value);
      if( strcmp(keyValue, "name") == 0 ) {
        test->name.assign(valueValue);
      } else if( strcmp(keyValue, "desc") == 0 ) {
        test->desc.assign(valueValue);
      } else if( strcmp(keyValue, "template") == 0 ) {
        test->tmpl.assign(valueValue);
      } else if( strcmp(keyValue, "expected") == 0 ) {
        test->expected.assign(valueValue);
      }
    } else if( valueNode->type == YAML_MAPPING_NODE ) {
      if( strcmp(keyValue, "data") == 0 ) {
        mustache_spec_parse_data(document, valueNode, &test->data);
      } else if( strcmp(keyValue, "partials") == 0 ) {
        mustache_spec_parse_partials(document, valueNode, &test->partials);
      }
    }
  }
  
  // Execute the test
  try {
    mustache::Mustache mustache;
    mustache::Node root;
    mustache.tokenize(&test->tmpl, &root);
    mustache.render(&root, &test->data, &test->partials, &test->output);
  } catch( mustache::Exception e ) {
    std::cerr << e.what() << "\n";
  } catch( std::exception e ) {
    std::cerr << e.what() << "\n";
  }
  
  // Output result
  test->print();
  tests.push_back(test);
}

void mustache_spec_parse_data(yaml_document_t * document, yaml_node_t * node, mustache::Data * data)
{
  if( node->type == YAML_MAPPING_NODE ) {
    yaml_node_pair_t * pair;
    
    data->init(mustache::Data::TypeMap, 0);
    
    for( pair = node->data.mapping.pairs.start; pair < node->data.mapping.pairs.top; pair++ ) {
      yaml_node_t * keyNode = yaml_document_get_node(document, pair->key);
      yaml_node_t * valueNode = yaml_document_get_node(document, pair->value);
      char * keyValue = reinterpret_cast<char *>(keyNode->data.scalar.value);
      mustache::Data * child = new mustache::Data();
      mustache_spec_parse_data(document, valueNode, child);
      data->data.insert(std::pair<std::string,mustache::Data*>(keyValue,child));
    }
  } else if( node->type == YAML_SEQUENCE_NODE ) {
    yaml_node_item_t * item;
    int nItems = node->data.sequence.items.top - node->data.sequence.items.start;
    data->init(mustache::Data::TypeArray, nItems);
    for( item = node->data.sequence.items.start; item < node->data.sequence.items.top; item ++) {
      yaml_node_t * valueNode = yaml_document_get_node(document, *item);
      mustache_spec_parse_data(document, valueNode, &data->array[item - node->data.sequence.items.start]);
    }
  } else if( node->type == YAML_SCALAR_NODE ) {
    char * keyValue = reinterpret_cast<char *>(node->data.scalar.value);
    if( strcmp(keyValue, "0") == 0 ||
        strcmp(keyValue, "false") == 0 ) {
      data->init(mustache::Data::TypeString, 0);
    } else {
      data->init(mustache::Data::TypeString, node->data.scalar.length);
      data->val->assign(keyValue);
      mustache::trimDecimal(*(data->val));
    }
  }
}

void mustache_spec_parse_partials(yaml_document_t * document, yaml_node_t * node, mustache::Node::Partials * partials)
{
  if( node->type != YAML_MAPPING_NODE ) {
    return;
  }
  
  mustache::Mustache mustache;
  std::string ckey;
  yaml_node_pair_t * pair;

  for( pair = node->data.mapping.pairs.start; pair < node->data.mapping.pairs.top; pair++ ) {
    yaml_node_t * keyNode = yaml_document_get_node(document, pair->key);
    yaml_node_t * valueNode = yaml_document_get_node(document, pair->value);
    char * keyValue = reinterpret_cast<char *>(keyNode->data.scalar.value);
    char * valueValue = reinterpret_cast<char *>(valueNode->data.scalar.value);
    
    std::string ckey(keyValue);
    std::string tmpl(valueValue);
    mustache::Node node;

    partials->insert(std::make_pair(ckey, node));
    mustache.tokenize(&tmpl, &(*partials)[ckey]);
  }
}
