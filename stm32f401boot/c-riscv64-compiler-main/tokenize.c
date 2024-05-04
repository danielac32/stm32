
// Archivo de inclusión de "zhizhicc.h"
#include "zhizhicc.h"

// Nombre de archivo actual
static char *current_filename;

// Entrada actual
static char *current_input;

// Informa un error y sale
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Informa un mensaje de error en el siguiente formato.
//
// foo.c:10: x = y + 1;
//               ^ <mensaje de error aquí>
static void verror_at(int line_no, char *loc, char *fmt, va_list ap) {
  // Encuentra una línea que contenga `loc`.
  char *line = loc;
  while (current_input < line && line[-1] != '\n')
    line--;

  char *end = loc;
  while (*end != '\n')
    end++;

  // Imprime la línea.
  int indent = fprintf(stderr, "%s:%d: ", current_filename, line_no);
  fprintf(stderr, "%.*s\n", (int)(end - line), line);

  // Muestra el mensaje de error.
  int pos = loc - line + indent;

  fprintf(stderr, "%*s", pos, ""); // imprime pos espacios.
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
}

void error_at(char *loc, char *fmt, ...) {
  int line_no = 1;
  for (char *p = current_input; p < loc; p++)
    if (*p == '\n')
      line_no++;

  va_list ap;
  va_start(ap, fmt);
  verror_at(line_no, loc, fmt, ap);
  exit(1);
}

void error_tok(Token *tok, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  verror_at(tok->line_no, tok->loc, fmt, ap);
  exit(1);
}

// Si el token actual coincide con `s`, entonces consume.
bool equal(Token *tok, char *op) {
  return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

// Asegura que el token actual sea `s`, luego avanza.
Token *skip(Token *tok, char *s) {
  if (!equal(tok, s)) error_tok(tok, "Se esperaba: '%s'", s);
  return tok->next;
}

bool consume(Token **rest, Token *tok, char *str) {
  if (equal(tok, str)) {
    *rest = tok->next;
    return true;
  }
  *rest = tok;
  return false;
}

// Crea un nuevo token
static Token *new_token(TokenKind kind, char *start, char *end) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->loc = start;
  tok->len = end - start;
  return tok;
}

static bool startswith(char *p, char *q) {
  return strncmp(p, q, strlen(q)) == 0;
}

// Determina si un carácter puede ser el primer carácter de un identificador
static bool is_ident1(char c) {
  return ('a' <= c && c <= 'z') ||
         ('A' <= c && c <= 'Z') ||
         c == '_';
}

// Determina si un carácter puede ser el carácter intermedio de un identificador
static bool is_ident2(char c) {
  return is_ident1(c) || ('0' <= c && c <= '9');
}

static int from_hex(char c) {
  if ('0' <= c && c <= '9')
    return c - '0';
  if ('a' <= c && c <= 'f')
    return c - 'a' + 10;
  return c - 'A' + 10;
}

static int read_punct(char *p) {
  static char *kw[] = {
    "==", "!=", "<=", ">=", "->", "+=", "-=", "*=", "/=", "++", "--", "%=",
    "&=", "|=", "^=", "&&", "||",
  };

  for (int i = 0; i < sizeof(kw) / sizeof(*kw); i++)
    if (startswith(p, kw[i]))
      return strlen(kw[i]);

  return ispunct(*p) ? 1 : 0;
}

static bool is_keyword(Token *tok) {
  static char *kw[] = {"return", "if", "else", "for", "while", "int", "sizeof", "char", "struct", "union", "short", "long", "void", "typedef", "_Bool", "enum", "static", "goto", "break", "continue", "switch", "case", "default"};

  for (int i = 0; i < sizeof(kw) / sizeof(*kw); i++)
    if (equal(tok, kw[i]))
      return true;
  return false;
}

static int read_escaped_char(char **new_pos, char *p) {
  if ('0' <= *p && *p <= '7') {
    // Leer un número octal.
    int c = *p++ - '0';
    if ('0' <= *p && *p <= '7') {
      c = (c << 3) + (*p++ - '0');
      if ('0' <= *p && *p <= '7')
        c = (c << 3) + (*p++ - '0');
    }
    *new_pos = p;
    return c;
  }

  if (*p == 'x') {
    // Leer un número hexadecimal
    p++;
    if (!isxdigit(*p))
      error_at(p, "Secuencia de escape hexadecimal inválida");

    int c = 0;
    for (; isxdigit(*p); p++)
      c = (c << 4) + from_hex(*p);
    *new_pos = p;
    return c;
  }

  *new_pos = p + 1;

  // Las secuencias de escape se definen utilizando ellas mismas aquí. Por ejemplo,
  // '\n' se implementa usando '\n'. Esta definición tautológica funciona porque el compilador
  // que compila nuestro compilador sabe lo que es '\n' realmente. En otras palabras, "heredamos"
  // el código ASCII de '\n' del compilador que compila nuestro compilador, por lo que no
  // tenemos que enseñar el código real aquí.
  //
  // Este hecho tiene grandes implicaciones no solo para la corrección del compilador, sino también
  // para la seguridad del código generado.
  // Para obtener más información, lea "Reflexiones sobre confiar en Trust" de Ken Thompson.
  // https://github.com/rui314/chibicc/wiki/thompson1984.pdf


  switch (*p) {
  case 'a': return '\a';
  case 'b': return '\b';
  case 't': return '\t';
  case 'n': return '\n';
  case 'v': return '\v';
  case 'f': return '\f';
  case 'r': return '\r';
  // [GNU] \e para el carácter de escape ASCII es una extensión de GNU C.
  case 'e': return 27;
  default: return *p;
  }
}

// Encuentra una comilla doble de cierre.
static char *string_literal_end(char *p) {
  char *start = p;
  for (; *p != '"'; p++) {
    if (*p == '\n' || *p == '\0')
      error_at(start, "Literal de cadena no cerrado");
    if (*p == '\\')
      p++;
  }
  return p;
}

static Token *read_string_literal(char *start) {
  char *end = string_literal_end(start + 1);
  char *buf = calloc(1, end - start);
  int len = 0;

  for (char *p = start + 1; p < end;) {
    if (*p == '\\')
      buf[len++] = read_escaped_char(&p, p + 1);
    else
      buf[len++] = *p++;
  }

  Token *tok = new_token(TK_STR, start, end + 1);
  tok->ty = array_of(ty_char, len + 1);
  tok->str = buf;
  return tok;
}

static Token *read_char_literal(char *start) {
  char *p = start + 1;
  if (*p == '\0')
    error_at(start, "Literal de carácter no cerrado");

  char c;
  if (*p == '\\')
    c = read_escaped_char(&p, p + 1);
  else
    c = *p++;

  char *end = strchr(p, '\'');
  if (!end)
    error_at(p, "Literal de carácter no cerrado");

  Token *tok = new_token(TK_NUM, start, end + 1);
  tok->val = c;
  return tok;
}

static Token *read_int_literal(char *start) {
  char *p = start;

  int base = 10;
  if (!strncasecmp(p, "0x", 2) && isalnum(p[2])) {
    p += 2;
    base = 16;
  } else if (!strncasecmp(p, "0b", 2) && isalnum(p[2])) {
    p += 2;
    base = 2;
  } else if (*p == '0') {
    base = 8;
  }

  long val = strtoul(p, &p, base);
  if (isalnum(*p))
    error_at(p, "Número inválido");

  Token *tok = new_token(TK_NUM, start, p);
  tok->val = val;
  return tok;
}

static void convert_keywords(Token *tok) {
  for (Token *t = tok; t->kind != TK_EOF; t = t->next)
    if (is_keyword(t))
      t->kind = TK_KEYWORD;
}

// Inicializa la información de línea para todos los tokens.
static void add_line_numbers(Token *tok) {
  char *p = current_input;
  int n = 1;

  do {
    if (p == tok->loc) {
      tok->line_no = n;
      tok = tok->next;
    }
    if (*p == '\n')
      n++;
  } while (*p++);
}

// Realiza el análisis léxico de la cadena apuntada por `p`, luego devuelve una nueva secuencia de tokens.
Token *tokenize(char *filename, char *p) {
  current_filename = filename;
  current_input = p;
  Token head = {};
  Token *cur = &head;

  while (*p) {
    // Saltar comentarios de línea
    if (startswith(p, "//")) {
      p += 2;
      while (*p != '\n') p++;
      continue;
    }

    // Saltar comentarios de bloque
    if (startswith(p, "/*")) {
      char *q = strstr(p + 2, "*/");
      if (!q)
        error_at(p, "Comentario de bloque no cerrado");
      p = q + 2;
      continue;
    }
    
    // Saltar caracteres en blanco
    if (isspace(*p)) {
      p++;
      continue;
    }

    // Literales numéricos
    if (isdigit(*p)) {
      cur = cur->next = read_int_literal(p);
      p += cur->len;
      continue;
    }

    // Literales de cadena
    if (*p == '"') {
      cur = cur->next = read_string_literal(p);
      p += cur->len;
      continue;
    }

    // Literales de caracteres
    if (*p == '\'') {
      cur = cur->next = read_char_literal(p);
      p += cur->len;
      continue;
    }

    // Identificadores o palabras clave
    if (is_ident1(*p)) {
      char *start = p;
      do {
        p++;
      } while (is_ident2(*p));
      cur = cur->next = new_token(TK_IDENT, start, p);
      continue;
    }

    // Separadores
    int punct_len = read_punct(p);
    if (punct_len) {
      cur = cur->next = new_token(TK_PUNCT, p, p + punct_len);
      p += cur->len;
      continue;
    }

    error_at(p, "Token inválido");
  }

  cur = cur->next = new_token(TK_EOF, p, p);
  add_line_numbers(head.next);
  convert_keywords(head.next);
  return head.next;
}

// Devuelve el contenido de un archivo dado.
static char *read_file(char *path) {
  FILE *fp;

  if (strcmp(path, "-") == 0) {
    // Por convención, leer desde stdin si el nombre de archivo dado es "-".
    fp = stdin;
  } else {
    fp = fopen(path, "r");
    if (!fp)
      error("no se puede abrir %s: %s", path, strerror(errno));
  }

  char *buf;
  size_t buflen;
  FILE *out = open_memstream(&buf, &buflen);

  // Leer todo el archivo.
  for (;;) {
    char buf2[4096];
    int n = fread(buf2, 1, sizeof(buf2), fp);
    if (n == 0)
      break;
    fwrite(buf2, 1, n, out);
  }

  if (fp != stdin)
    fclose(fp);

  // Asegurarse de que la última línea esté correctamente terminada con '\n'.
  fflush(out);
  if (buflen == 0 || buf[buflen - 1] != '\n')
    fputc('\n', out);
  fputc('\0', out);
  fclose(out

);
  return buf;
}

Token *tokenize_file(char *path) {
  return tokenize(path, read_file(path));
}
