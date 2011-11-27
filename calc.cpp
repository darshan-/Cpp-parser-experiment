// g++ -std=c++0x -Wall calc.cpp && ./a.out

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

#include "parser.h"

using namespace std;
using namespace CalcParser;

int main()
{
  unique_ptr<Parser> parser = new_Parser();
  while (true) {
    double d;

    cout << "> ";
    try {
      string line;
      getline(cin, line);
      if (cin.eof()) throw Quit();

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
