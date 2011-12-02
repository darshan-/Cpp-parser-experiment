#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>

namespace CalculatorParser {
  struct BadInput {
    const std::string short_message;
    const std::string long_message;
  };

  class ParserInterface {
  public:
    virtual double eval(std::string line) = 0;
    virtual bool has_value() = 0;
  };

  std::unique_ptr<ParserInterface> new_Parser();
}
#endif // PARSER_H
