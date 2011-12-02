// g++ -std=c++0x -Os -Wall -lreadline *.cpp && ./a.out

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
  const char* prompt = "> ";

  while (true) {
    Value value;

    try {
      char* line_cstr = readline(prompt);
      if (! line_cstr) { /* User sent EOF; quit */
        cout << endl;
        return 0;
      }

      std::string line(line_cstr);
      add_history(line_cstr);
      free(line_cstr);

      value = parser->eval(line);
    } catch (BadInput e) {
      cout << e.long_message << endl;
      continue;
    }

    if (value.non_empty)
      cout << value.value << endl;
  }

  return 0;
}
