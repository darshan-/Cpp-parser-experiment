#ifndef PARSER_H
#define PARSER_H

#include <string>

namespace CalcParser {
  class BadInput {
  public:
    std::string message;
    BadInput(std::string message);
  };

  class Quit {};

  class Parser {
  public:
    virtual double eval(std::string line) = 0;
    virtual std::string cur_line() = 0;
    virtual unsigned int cur_offset() = 0;
    virtual bool has_value() = 0;
  };

  Parser* new_Parser();
}
#endif // PARSER_H
