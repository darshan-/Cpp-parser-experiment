#include <iostream>

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
    return (type == NIL);
  }
};

Token getToken()
{
  Token t;
  char c = 0;
  cin.get(c);

  if (cin.eof()) {
    throw Quit();
  } else if (c == 0) {
    t.type = NIL;
  } else if (c >= '0' && c <= '9') {
    cin.unget();
    t.type = NUMBER;
    cin >> t.value;
  } else if (c == '+') {
    t.type = PLUS;
  } else if (c == '-') {
    t.type = MINUS;
  } else {
    throw BadInput();
  }

  return t;
}

double expression()
{
  double first;
  Token t = getToken();

  if (t.type != NUMBER) throw BadInput();
  first = t.value;

  while ((t = getToken())) {
    cout << t.value << endl;
  }

  return first;
}

void ignore_rest_of_line()
{
  //cin.ignore(numeric_limits<int>::max(), '\n');
  char c = 0;
  while (c != '\n') {
    c = cin.get();
    if (cin.eof()) {
      cin.clear();
      break;
    }
  }
}

int main()
{
  Token t;

  while (true) {
    cout << "> ";
    try {
      t = getToken();
    } catch (BadInput &e) {
      cout << "Bad input!" << endl;
      goto loop_end;
    } catch (Quit &e) {
      cout << endl;
      return 0;
    }

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

  loop_end:
    ignore_rest_of_line();
  }

  return 0;
}
