#include <cctype>
#include <cstdio>
#include <memory>

#include "parser.h"

using std::string;

namespace CalculatorParser {
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

  class Parser : public ParserInterface{
  private:
    string cur_line;
    unsigned int cur_offset;
    bool has_value_;

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

    bool done()
    {
      return (cur_offset >= cur_line.length());
    }

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

  public:
    double eval(string line)
    {
      cur_line = line;
      cur_offset = 0;
      has_value_=false;

      if (done())
        return 0; // TODO: return a Value object

      double d;
      while (!done()) {
        Token t = peek_token();;
        while (t.type == EXPR_END || t.type == NIL){
          has_value_=false;
          get_token();

          if (done())
            break;

          t = peek_token();
        }

        if (done()) break;

        d = get_expression();
        has_value_ = true;

        if (!done()) {
          Token t = peek_token();
          if (t.type != EXPR_END)
            throw make_BadInput("Junk after complete expression");
          get_token();
        }
      }

      return d;
    }

    bool has_value() {return has_value_;}
  };

  std::unique_ptr<ParserInterface> new_Parser()
  {
    return std::unique_ptr<ParserInterface>(new Parser);
  }
}
