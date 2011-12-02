// g++ -std=c++0x -Wall -lreadline *.cpp && ./a.out

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include <iostream>
#include <memory>
#include <string>

#include "parser.h"

using std::cout;
using std::endl;
using namespace CalculatorParser;

int main()
{
  std::unique_ptr<ParserInterface> parser = new_Parser();
  while (true) {
    double d;

    try {
      char* line_cstr = readline("> ");
      if (! line_cstr) throw Quit();

      std::string line(line_cstr);
      add_history(line_cstr);
      free(line_cstr);

      d = parser->eval(line);

    } catch (BadInput &e) {
      cout << e.long_message << endl;
      continue;
    } catch (Quit &e) {
      cout << endl;
      return 0;
    }

    if (parser->has_value()) cout << d << endl;
  }

  return 0;
}
