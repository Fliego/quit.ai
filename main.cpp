using namespace llvm
//===----------------------------------------------------------------------===//
// Lexer (Лексический анализатор)
//===----------------------------------------------------------------------===//

// Лексический анализатор возвращает токены [0-255], если это неизвестны,
// иначе одну из известных единиц кода
enum Token {
  tok_eof = -1,

  // команды (ключевые слова)
  tok_def = -2, tok_extern = -3,

  // операнды (первичные выражения: идентификаторы, числа)
  tok_identifier = -4, tok_number = -5,

  // управляющие конструкции
  tok_if = -6, tok_then = -7, tok_else = -8,
  tok_for = -9, tok_in = -10
};

static std::string IdentifierStr;  // Заполняется, если tok_identifier
static double NumVal;              // Заполняется, если tok_number

/// gettok - Возвращает следующий токен из стандартного потока ввода.
static int gettok() {
  static int LastChar = ' ';

  // Пропускаем пробелы.
  while (isspace(LastChar))
    LastChar = getchar();

  if (isalpha(LastChar) || LastChar == '.') {   // Число: [0-9.]+
    IdentifierStr = LastChar;
    while (isalnum((LastChar = getchar())))
      IdentifierStr += LastChar;

    if (IdentifierStr == "def") return tok_def;
    if (IdentifierStr == "extern") return tok_extern;
    if (IdentifierStr == "if") return tok_if;
    if (IdentifierStr == "then") return tok_then;
    if (IdentifierStr == "else") return tok_else;
    if (IdentifierStr == "for") return tok_for;
    if (IdentifierStr == "in") return tok_in;
    return tok_identifier;
  }

  if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
    std::string NumStr;
    do {
      NumStr += LastChar;
      LastChar = getchar();
    } while (isdigit(LastChar) || LastChar == '.');

    NumVal = strtod(NumStr.c_str(), 0);
    return tok_number;
  }

  if (LastChar == '#') {
    // Комментарий до конца строки
    do LastChar = getchar();
    while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

    if (LastChar != EOF)
      return gettok();
  }

  // Проверка конца файла.
  if (LastChar == EOF)
    return tok_eof;

  // В противном случае просто возвращаем символ как значение ASCII
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}
