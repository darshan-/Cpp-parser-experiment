// g++ -std=c++0x -Wall calc.cpp && ./a.out

#include <iostream>
#include <cctype>

using namespace std;

class BadInput {};
class Quit     {};

enum TokenType {NIL, NUMBER, PLUS='+', MINUS='-', MUL='*', DIV='/'};

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

  if (cin.eof()) throw Quit();

  switch (c) {
  case 0:
  case '\n':
    t.type = NIL;
    break;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    cin.unget();
    t.type = NUMBER;
    cin >> t.value;
    break;
  case '+':
  case '-':
  case '*':
  case '/':
    t.type = TokenType(c);
    break;
  default:
    if (isspace(c)) return get_token();
    else throw BadInput();
  }

  return t;
}

double get_primary()
{
  Token t = get_token();
  if (t.type != NUMBER) throw BadInput();

  return t.value;
}

double get_term()
{
  return get_primary();
  double val = get_primary();

  Token t;
  while ((t = get_token())) {
    switch (t.type) {
    case MUL:
      val *= get_primary();
      break;
    case DIV:
      val /= get_primary();
      break;
    default:
      throw BadInput();
    }
  }

  return val;
}

double get_expression()
{
  double val = get_term();

  Token t;
  while ((t = get_token())) {
    switch (t.type) {
    case PLUS:
      val += get_term();
      break;
    case MINUS:
      val -= get_term();
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
