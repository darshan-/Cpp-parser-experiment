// g++ -std=c++0x -Wall calc.cpp && ./a.out

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class BadInput {};
class Quit     {};

string cur_line;
unsigned int cur_offset;

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

double get_double()
{
  double d;
  istringstream iss(cur_line.substr(cur_offset, cur_line.length() - cur_offset));
  int n = iss.rdbuf()->in_avail();
  iss >> d;
  cur_offset += n - iss.rdbuf()->in_avail();
  return d;
 }

Token get_token()
{
  Token t;

  if (cur_offset >= cur_line.length()) {
    t.type = NIL;
    return t;
  }

  char c = cur_line[cur_offset++];

  switch (c) {
  case 0:
  case '\n':
    t.type = NIL;
    break;
  case '.':
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
    --cur_offset;
    t.type = NUMBER;
    t.value = get_double();
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
      --cur_offset;
      return val;
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
      stringbuf sb;
      cin.get(sb);
      if (cin.eof()) return 0;
      cur_line = sb.str();
      cur_offset = 0;
      cout << get_expression() << endl;
    } catch (BadInput &e) {
      cout << "Bad input!" << endl;
    } catch (Quit &e) {
      cout << endl;
      return 0;
    }

    cin.get(); // pop '\n';
  }

  return 0;
}
