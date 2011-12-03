#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>

namespace CalculatorParser {
  struct BadInput {
    const std::string short_message;
    const std::string long_message;
  };

  struct Value {
    bool non_empty;
    double value;
  };

  class ParserInterface {
  public:
    virtual Value eval(std::string line) throw (BadInput) = 0;
  };

  std::unique_ptr<ParserInterface> new_Parser();
}
#endif // PARSER_H
