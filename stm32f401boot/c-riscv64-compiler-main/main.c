
#include "zhizhicc.h"

static char *opt_o;

static char *input_path;

static void usage(int status) {
  fprintf(stderr, "zhizhicc [ -o <ruta> ] <archivo>\n");
  exit(status);
}

static void parse_args(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--help"))
      usage(0);

    if (!strcmp(argv[i], "-o")) {
      if (!argv[++i])
        usage(1);
      opt_o = argv[i];
      continue;
    }

    if (!strncmp(argv[i], "-o", 2)) {
      opt_o = argv[i] + 2;
      continue;
    }

    if (argv[i][0] == '-' && argv[i][1] != '\0')
      error("Parámetro desconocido: %s", argv[i]);

    input_path = argv[i];
  }

  if (!input_path)
    error("No se proporcionó un archivo de entrada");
}

static FILE *open_file(char *path) {
  if (!path || strcmp(path, "-") == 0)
    return stdout;

  FILE *out = fopen(path, "w");
  if (!out)
    error("No se pudo abrir el archivo de salida: %s: %s", path, strerror(errno));
  return out;
}

int main(int argc, char **argv) {
  parse_args(argc, argv);

  // Análisis léxico y sintáctico
  Token *tok = tokenize_file(input_path);
  Obj *prog = parse(tok);

  // Recorre el árbol sintáctico e imprime el código ensamblador
  FILE *out = open_file(opt_o);
  //fprintf(out, ".file 1 \"%s\"\n", input_path);
  codegen(prog, out);
  return 0;
}
