// g++ -std=c++0x -Wall calc.cpp && ./a.out

#include <cctype>
#include <cstdio>
#include <iostream>
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

enum TokenType {NIL, NUM, PLUS='+', MINUS='-', MUL='*', DIV='/', LPAREN='(', RPAREN=')', EXPR_END=';'};

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
    // TODO: '1.2.', '1.2.3'
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
    {
      --cur_offset;
      t.type = NUM;
      int i;
      sscanf(cur_line.c_str() + cur_offset, "%lf%n", &(t.value), &i);
      cur_offset += i;
      break;
    }
  case '+':
  case '-':
  case '*':
  case '/':
  case '(':
  case ')':
  case ';':
    t.type = TokenType(c);
    break;
  default:
    if (isspace(c)) return get_token();

    --cur_offset;
    throw BadInput("Invalid character");
  }

  return t;
}

Token peek_token()
{
  int offset = cur_offset;
  Token t = get_token();
  cur_offset = offset;
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
    double d;
    bool have_value=false;
    cout << "> ";

    try {
      cur_offset = 0;
      getline(cin, cur_line);
      if (cin.eof()) throw Quit();

      if (cur_line.length() == 0) continue;

      while (cur_offset != cur_line.length()) {
        Token t = peek_token();;
        while (t.type == EXPR_END || t.type == NIL){
          have_value=false;
          get_token();
          if (cur_offset == cur_line.length()) break;
          t = peek_token();
        }
        if (cur_offset == cur_line.length()) break;

        d = get_expression();
        have_value = true;
        if (cur_offset != cur_line.length()) {
          Token t = peek_token();
          if (t.type != EXPR_END)
            throw BadInput("Junk after complete expression");
          get_token();
        }
      }
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

    if (have_value) cout << d << endl;
  }

  return 0;
}
