#include <cctype>
#include <cstdio>
#include <memory>

#include "parser.h"

using namespace std;

namespace CalcParser {
  BadInput::BadInput(string message = "") : message(message) {}

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

  class parser : public Parser{
  private:
    string _cur_line;
    unsigned int _cur_offset;
    bool _has_value;

    double get_double()
    {
      double d;
      int i;
      sscanf(_cur_line.c_str() + _cur_offset, "%lf%n", &d, &i);
      _cur_offset += i;
      return d;
    }

    Token get_token()
    {
      Token t;

      if (_cur_offset >= _cur_line.length()) {
        t.type = NIL;
        return t;
      }

      char c = _cur_line[_cur_offset++];

      switch (c) {
      case '.':
        if (_cur_offset > _cur_line.length() || !isdigit(_cur_line[_cur_offset]))
          throw BadInput("Incomplete number");
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
          --_cur_offset;
          t.type = NUM;
          int i;
          sscanf(_cur_line.c_str() + _cur_offset, "%lf%n", &(t.value), &i);
          _cur_offset += i;
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

        --_cur_offset;
        throw BadInput("Invalid character");
      }

      return t;
    }

    Token peek_token()
    {
      int offset = _cur_offset;
      Token t = get_token();
      _cur_offset = offset;
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
          if (t.type != RPAREN) throw BadInput("Right parenthesis ')' expected");
          return d;
        }
      case PLUS:
        return get_primary();
      case MINUS:
        return - get_primary();
      default:
        if (t.type != NIL) --_cur_offset;
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
          --_cur_offset;
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
          --_cur_offset;
          return val;
        }
      }

      return val;
    }

    bool done()
    {
      return (_cur_offset >= _cur_line.length());
    }

  public:
    double eval(string line)
    {
      _cur_line = line;
      _cur_offset = 0;
      if (done()) return 0; //TODO

      double d;
      while (! done()) {
        Token t = peek_token();;
        while (t.type == EXPR_END || t.type == NIL){
          _has_value=false;
          get_token();
          if (done()) break;
          t = peek_token();
        }
        if (done()) break;

        d = get_expression();
        _has_value = true;
        if (! done()) {
          Token t = peek_token();
          if (t.type != EXPR_END)
            throw BadInput("Junk after complete expression");
          get_token();
        }
      }

      return d;
    }

    string cur_line() {return _cur_line;}
    unsigned int cur_offset() {return _cur_offset;}
    bool has_value() {return _has_value;}
  };

  unique_ptr<Parser> new_Parser()
  {
    return unique_ptr<Parser>(new parser);
  }
}
