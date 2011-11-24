#ifndef PARSER_H
#define PARSER_H

#include <string>

namespace Parser {
  class BadInput {
  public:
    std::string message;
    BadInput(std::string message);
  };

  class Quit {};

  class Parser {
  public:
    Parser();
    double eval(std::string line);
    std::string cur_line();
    unsigned int cur_offset();
    bool has_value();
  private:
    void* p;
  };
}
#endif // PARSER_H
