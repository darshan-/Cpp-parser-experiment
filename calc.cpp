// g++ -std=c++0x -Wall calc.cpp && ./a.out

#include <cctype>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class BadInput {
public:
  string message;

  BadInput(string message = "") : message(message) {}
};

class Quit     {};

string cur_line;
unsigned int cur_offset;

enum TokenType {NIL, NUM, PLUS='+', MINUS='-', MUL='*', DIV='/', LPAREN = '(', RPAREN = ')'};

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
  int i;
  sscanf(cur_line.c_str() + cur_offset, "%lf%n", &d, &i);
  cur_offset += i;
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
    t.type = NUM;
    t.value = get_double();
    break;
  case '+':
  case '-':
  case '*':
  case '/':
  case '(':
  case ')':
    t.type = TokenType(c);
    break;
  default:
    if (isspace(c)) return get_token();

    --cur_offset;
    throw BadInput("Bad character");
  }

  return t;
}

double get_expression();

double get_primary()
{
  Token t = get_token();

  switch (t.type) {
  case NUM:
    return t.value;
  case LPAREN:
    {
      double d = get_expression();
      t = get_token();
      if (t.type != RPAREN) throw BadInput("Right parenthesis ')' expected");
      return d;
    }
  case PLUS:
    return get_primary();
  case MINUS:
    return - get_primary();
  default:
    if (t.type != NIL) --cur_offset;
    throw BadInput("Primary expected");
  }
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
      --cur_offset;
      return val;
    }
  }

  return val;
}

int main()
{
  while (true) {
    cout << "> ";
    try {
      char c = cin.get();
      if (c == '\n') continue;
      cin.putback(c);

      stringbuf sb;
      cin.get(sb);
      cin.get(); // pop '\n';
      if (cin.eof()) throw Quit();

      cur_line = sb.str();
      cur_offset = 0;

      double d = get_expression();
      if (cur_offset != cur_line.length()) throw BadInput("Junk after complete expression");

      cout << d << endl;
    } catch (BadInput &e) {
      cout << "Error: " << e.message << endl;
      cout << "~ " << cur_line << endl;
      cout << "~ ";
      for (unsigned int i=0; i<cur_offset; ++i) cout << ' ';
      cout << '^' << endl;
    } catch (Quit &e) {
      cout << endl;
      return 0;
    }
  }

  return 0;
}
