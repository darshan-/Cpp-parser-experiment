#include <cctype>
#include <cstdio>

#include "parser.h"

using std::string;

namespace Parser {
  enum TokenType {NIL, NUM, PLUS='+', MINUS='-', MUL='*', DIV='/',
                  LPAREN='(', RPAREN=')', EXPR_END=';'};

  class Token {
  public:
    TokenType type;
    double   value;

    explicit operator bool()
    {
      return (type != NIL);
    }
  };

  const Value NIL_VALUE = {false, 0};

  string cur_line;
  unsigned int cur_offset;

  BadInput make_BadInput(string message)
  {
    string short_message = "Error: " + message;
    string long_message = short_message + '\n' +
      "~ " + cur_line + '\n' +
      "~ ";
    for (unsigned int i = 0; i < cur_offset; ++i)
      long_message += ' ';
    long_message += '^';

    return {short_message, long_message};
  }

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
      if (cur_offset > cur_line.length() || !isdigit(cur_line[cur_offset]))
        throw make_BadInput("Incomplete number");
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
    case ';':
      t.type = TokenType(c);
      break;
    default:
      if (isspace(c))
        return get_token();

      --cur_offset;
      throw make_BadInput("Invalid character");
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
        if (t.type != RPAREN)
          throw make_BadInput("Right parenthesis ')' expected");

        return d;
      }
    case PLUS:
      return get_primary();
    case MINUS:
      return - get_primary();
    default:
      if (t.type != NIL)
        --cur_offset;

      throw make_BadInput("Primary expected");
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

  inline bool done()
  {
    return (cur_offset >= cur_line.length());
  }

  Value eval(string line) throw (BadInput)
  {
    cur_line = line.erase(line.find_last_not_of(" ")+1); // right trim
    cur_offset = 0;
    Value value = NIL_VALUE;

    while (!done()) {
      Token t = peek_token();;

      while (t.type == EXPR_END || t.type == NIL){
        value = NIL_VALUE;
        get_token();

        if (done())
          break;

        t = peek_token();
      }

      if (done())
        break;

      value = {true, get_expression()};

      if (!done()) {
        Token t = peek_token();
        if (t.type != EXPR_END)
          throw make_BadInput("Junk after complete expression");
        get_token();
      }
    }

    return value;
  }
}
