// g++ -std=c++0x -Wall calc.cpp && ./a.out

#include <cstdio>
#include <iostream>
#include <string>

#include "parser.h"

using namespace std;
using namespace CalcParser;

int main()
{
  Parser* parser = new_Parser();
  while (true) {
    double d;

    cout << "> ";
    try {
      string line;
      getline(cin, line);
      if (cin.eof()) throw Quit();

      d = parser->eval(line);

    } catch (BadInput &e) {
      cout << "Error: " << e.message << endl;
      cout << "~ " << parser->cur_line() << endl;
      cout << "~ ";
      for (unsigned int i=0; i < parser->cur_offset(); ++i) cout << ' ';
      cout << '^' << endl;
    } catch (Quit &e) {
      cout << endl;
      return 0;
    }

    if (parser->has_value()) cout << d << endl;
  }

  return 0;
}
