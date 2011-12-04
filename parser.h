#ifndef PARSER_H
#define PARSER_H

#include <string>

namespace Parser {
  struct BadInput {
    const std::string short_message;
    const std::string long_message;
  };

  struct Value {
    bool non_empty;
    double value;
  };

  Value eval(std::string line) throw (BadInput);
}
#endif // PARSER_H
