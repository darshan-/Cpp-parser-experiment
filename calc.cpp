// g++ -std=c++0x -Wall calc.cpp && ./a.out

#include <iostream>
#include <cctype>

using namespace std;

class BadInput {};
class Quit     {};

enum TokenType {NIL, NUMBER, PLUS, MINUS};

class Token {
public:
  TokenType type;
  double   value;

  explicit operator bool()
  {
    return (type != NIL);
  }
};

Token get_token()
{
  Token t;
  char c = 0;
  cin.get(c);

  if (cin.eof()) {
    throw Quit();
  } else if (c == 0 || c == '\n') {
    t.type = NIL;
  } else if (c >= '0' && c <= '9') {
    cin.unget();
    t.type = NUMBER;
    cin >> t.value;
  } else if (c == '+') {
    t.type = PLUS;
  } else if (c == '-') {
    t.type = MINUS;
  } else if (isspace(c)) {
    return get_token();
  } else {
    throw BadInput();
  }

  return t;
}

void explain(Token t)
{
  switch (t.type) {
  case NUMBER:
    cout << "The number " << t.value << endl;
    break;
  case PLUS:
    cout << "PLUS" << endl;
    break;
  case MINUS:
    cout << "MINUS" << endl;
    break;
  default:
    cout << "WTF?" << endl;
  }
}

double get_primary()
{
  Token t = get_token();
  if (t.type != NUMBER) throw BadInput();

  return t.value;
}

double get_expression()
{
  double val = get_primary();

  Token t;
  while ((t = get_token())) {
    switch (t.type) {
    case PLUS:
      val += get_primary();
      break;
    case MINUS:
      val -= get_primary();
      break;
    default:
      throw BadInput();
    }
  }

  return val;
}

int main()
{
  while (true) {
    cout << "> ";
    try {
      cout << get_expression() << endl;;
    } catch (BadInput &e) {
      cout << "Bad input!" << endl;
    } catch (Quit &e) {
      cout << endl;
      return 0;
    }
  }

  return 0;
}
