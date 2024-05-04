
#include "zhizhicc.h"

//
// Generación de código
//

static FILE *output_file;
static int depth;
static char *argreg[] = {"a0", "a1", "a2", "a3", "a4", "a5" , "a6", "a7"};
static Obj *current_fn;

static void gen_expr(Node *node);
static void gen_stmt(Node *node);

static void println(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(output_file, fmt, ap);
  va_end(ap);
  fprintf(output_file, "\n");
}

static int count(void) {
  static int i = 1;
  return i++;
}

// Empuja el contenido del registro a0 en la pila
static void push(void) {
  println("  addi sp, sp, -4");
  println("  sw a0, 0(sp)");
  depth++;
}

// Pop del contenido de la pila al registro de argumento
static void pop(char *arg) {
  println("  lw %s, 0(sp)", arg);
  println("  addi sp, sp, 4");
  depth--;
}

// Agrega 'n' al múltiplo entero más cercano de 'align'.
// Por ejemplo: align_to(5, 8)  => 8
//              align_to(11, 8) => 16
// Se utiliza para alineación de memoria
int align_to(int n, int align) {
  return (n + align - 1) / align * align;
}

// Calcula la dirección absoluta del nodo dado en la memoria
static void gen_addr(Node *node) {
  switch (node->kind) {
  case ND_VAR:
    if (node->var->is_local) {
      println("  addi a0, fp, %d", node->var->offset);
    } else {
      // Carga la dirección de la variable global en el registro a0
      println("  la a0, %s", node->var->name);
    }
    return;
  case ND_DEREF:
    gen_expr(node->lhs);
    return;
  case ND_COMMA:
    gen_expr(node->lhs);
    gen_addr(node->rhs);
    return;
  case ND_MEMBER:
    gen_addr(node->lhs);
    println("  addi a0, a0, %d", node->member->offset);
    return;
  }

  error_tok(node->tok, "No es un lvalue");
}

static void load(Type *ty) {
  if (ty->kind == TY_ARRAY ||
      ty->kind == TY_STRUCT ||
      ty->kind == TY_UNION) {
    return;
  }
  if (ty->size == 1)
    println("  lbu a0, 0(a0)");
  else if (ty->size == 2)
    println("  lhu a0, 0(a0)");
  else if (ty->size == 4)
    println("  lw a0, 0(a0)");
  else
    println("  ld a0, 0(a0)");
}

static void store(Type *ty) {
  pop("a1");

  if (ty->kind == TY_STRUCT || ty->kind == TY_UNION) {
    for (int i = 0; i < ty->size; i++) {
      println("  lb a4, %d(a0)", i);
      println("  sb a4, %d(a1)", i);
    }
    return;
  }

  if (ty->size == 1)
    println("  sb a0, 0(a1)");
  else if (ty->size == 2)
    println("  sh a0, 0(a1)");
  else if (ty->size == 4)
    println("  sw a0, 0(a1)");
  else
    println("  sd a0, 0(a1)");
}

enum { I8, I16, I32, I64 };

static int getTypeId(Type *ty) {
  switch (ty->kind) {
  case TY_CHAR:
    return I8;
  case TY_SHORT:
    return I16;
  case TY_INT:
    return I32;
  }
  return I64;
}

// La tabla para las conversiones de tipo
//+static char i32i8[] = "movsbl %al, %eax";
//+static char i32i16[] = "movswl %ax, %eax";
//+static char i32i64[] = "movsxd %eax, %rax";
static char i32i8[] = "addi sp, sp, -4\n  sw a0, 0(sp)\n  lw a0, 0(sp)\n  andi a0, a0, 0xff\n  addi sp, sp, 4";
static char i32i16[] = "addi sp, sp, -4\n  sh a0, 0(sp)\n  lw a0, 0(sp)\n  addi sp, sp, 4";
static char i32i64[] = "addi sp, sp, -4\n  sw a0, 0(sp)\n  lw a0, 0(sp)\n  addi sp, sp, 4";

static char *cast_table[][10] = {
  {NULL,  NULL,   NULL, i32i64},    // i8
  {i32i8, NULL,   NULL, i32i64},    // i16
  {i32i8, i32i16, NULL, i32i64},    // i32
  {i32i8, i32i16, NULL, NULL},      // i64
};

static void cast(Type *from, Type *to) {
  if (to->kind == TY_VOID)
    return;

  if (to->kind == TY_BOOL) {
    println("  sub a0, a0, zero");
    println("  snez a0, a0");
    return;
  }

  int t1 = getTypeId(from);
  int t2 = getTypeId(to);
  if (cast_table[t1][t2])
    println("  %s", cast_table[t1][t2]);
}

static void gen_expr(Node *node) {
  //println("  .loc 1 %d", node->tok->line_no);

  switch (node->kind) {
  case ND_NUM:
    println("  li a0, %ld", node->val);
    return;
  case ND_NEG:
    gen_expr(node->lhs);
    println("  sub a0, zero, a0");
    return;
  case ND_VAR:
  case ND_MEMBER:
    gen_addr(node);
    load(node->ty);
    return;
  case ND_DEREF:
    gen_expr(node->lhs);
    load(node->ty);
    return;
  case ND_ADDR:
    gen_addr(node->lhs);
    return;
  case ND_ASSIGN:
    gen_addr(node->lhs);
    push();
    gen_expr

(node->rhs);
    store(node->ty);
    return;
  case ND_STMT_EXPR:
    for (Node *n = node->body; n; n = n->next)
      gen_stmt(n);
    return;
  case ND_COMMA:
    gen_expr(node->lhs);
    gen_expr(node->rhs);
    return;
  case ND_CAST:
    gen_expr(node->lhs);
    cast(node->lhs->ty, node->ty);
    return;
  case ND_NOT:
    gen_expr(node->lhs);
    //println("  sext.w a0, a0");
    println("  sub a0, a0, zero");
    println("  seqz a0, a0");
    println("  andi a0,a0,0xff");
    return;
  case ND_BITNOT:
    gen_expr(node->lhs);
    println("  not a0, a0");
    return;
  case ND_LOGAND: {
    int c = count();
    gen_expr(node->lhs);
    println("  beq a0, zero, .L.false.%d", c);
    gen_expr(node->rhs);
    println("  beq a0, zero, .L.false.%d", c);
    println("  li a0, 1");
    println("  j .L.end.%d", c);
    println(".L.false.%d:", c);
    println("  li a0, 0");
    println(".L.end.%d:", c);
    return;
  }
  case ND_LOGOR: {
    int c = count();
    gen_expr(node->lhs);
    println("  bne a0, zero, .L.true.%d", c);
    gen_expr(node->rhs);
    println("  bne a0, zero, .L.true.%d", c);
    println("  li a0, 0");
    println("  j .L.end.%d", c);
    println(".L.true.%d:", c);
    println("  li a0, 1");
    println(".L.end.%d:", c);
    return;
  }
  case ND_FUNCALL: {
    int nargs = 0;
    for (Node *arg = node->args; arg; arg = arg->next) {
      gen_expr(arg);
      push();
      nargs++;
    }

    for (int i = nargs - 1; i >= 0; i--)
      pop(argreg[i]);


   

    if(!strcmp(node->funcname,"printf")){
        println("  csrw 0x402,a0");
    }else if(!strcmp(node->funcname,"putc")){
       println("  csrw 0x402,a0");
    }else if(!strcmp(node->funcname,"puti")){
       println("  csrw 0x136,a0");
    }else{
      println("  call %s", node->funcname);
    }
       
    return;
  }
  }

  gen_expr(node->lhs);
  // Resultado del cálculo de lhs empujado a la pila.
  push();
  gen_expr(node->rhs);
  // Resultado del cálculo de lhs pop en registro a1.
  // Resultado del cálculo de rhs en registro a0.
  pop("a1");

  switch (node->kind) {
  case ND_ADD:
    if (node->lhs->ty->kind == TY_LONG || node->lhs->ty->base)
      println("  add a0, a0, a1");
    else
      println("  add a0, a0, a1");
    return;
  case ND_SUB:
    if (node->lhs->ty->kind == TY_LONG || node->lhs->ty->base)
      println("  sub a0, a1, a0");
    else
      println("  sub a0, a1, a0");
    return;
  case ND_MUL:
    if (node->lhs->ty->kind == TY_LONG || node->lhs->ty->base)
      println("  mul a0, a0, a1");
    else
      println("  mul a0, a0, a1");
    return;
  case ND_DIV:
    if (node->lhs->ty->kind == TY_LONG || node->lhs->ty->base)
      println("  div a0, a1, a0");
    else
      println("  div a0, a1, a0");
    return;
  case ND_MOD:
    println("  rem a0, a1, a0");
    return;
  case ND_BITAND:
    println("  and a0, a1, a0");
    return;
  case ND_BITOR:
    println("  or a0, a1, a0");
    return;
  case ND_BITXOR:
    println("  xor a0, a1, a0");
    return;
  case ND_EQ:
    println("  sub a0, a1, a0");
    println("  seqz a0, a0");
    println("  andi a0,a0,0xff");
    return;
  case ND_NE:
    println("  sub a0, a1, a0");
    println("  snez a0, a0");
    println("  andi a0,a0,0xff");
    return;
  case ND_LT:
    println("  slt a0, a1, a0");
    println("  andi a0,a0,0xff");
    return;
  case ND_LE:
    println("  sgt a0, a1, a0");
    println("  xori a0, a0, 1");
    println("  andi a0,a0,0xff");
    return;
  }

  error_tok(node->tok, "Expresión no válida");
}

static void gen_stmt(Node *node) {
  //println("  .loc 1 %d", node->tok->line_no);

  switch (node->kind) {
  case ND_IF: {
    int c = count();
    gen_expr(node->cond);
    println("  ble a0, zero, .L.else.%d", c);
    gen_stmt(node->then);
    println("  j .L.end.%d", c);
    println(".L.else.%d:", c);
    if (node->els)
      gen_stmt(node->els);
    println(".L.end.%d:", c);
    return;
  }
  case ND_FOR: {
    int c = count();
    if (node->init)
      gen_stmt(node->init);
    println(".L.begin.%d:", c);
    if (node->cond) {
      gen_expr(node->cond);
      println("  ble a0, zero, %s", node->brk_label);
    }
    gen_stmt(node->then);
    println("%s:", node->cont_label);
    if (node->inc) {
      gen_expr(node->inc);
    }
    println("  j .L.begin.%d", c);
    println("%s:", node->brk_label);
    return;
  }
  case ND_SWITCH:
    gen_expr(node->cond);

    for (Node *n = node->case_next; n; n = n->case_next) {
      println("  li a1, %ld", n->val);
      println("  beq a0, a1, %s", n->label);
    }

    if (node->default_case)
      println("  j %s", node->default_case->label);

    println("  j %s", node->brk_label);
    gen_stmt(node->then);
    println("%s:", node->brk_label);
    return;


  case ND_CASE:
    println("%s:", node->label);
    gen_stmt(node->lhs);
    return;
  case ND_BLOCK:
    for (Node *n = node->body; n; n = n->next)
      gen_stmt(n);
    return;
  case ND_GOTO:
    println("  j %s", node->unique_label);
    return;
  case ND_LABEL:
    println("%s:", node->unique_label);
    gen_stmt(node->lhs);
    return;
  case ND_RETURN:
    gen_expr(node->lhs);
    println("  j .L.return.%s", current_fn->name);
    return;
  case ND_EXPR_STMT:
    gen_expr(node->lhs);
    return;
  }

  error_tok(node->tok, "Sentencia no válida");
}

// Asigna los desplazamientos de las variables locales
static void assign_lvar_offsets(Obj *prog) {
  for (Obj *fn = prog; fn; fn = fn->next) {
    if (!fn->is_function)
      continue;

    int offset = 0;
    for (Obj *var = fn->locals; var; var = var->next) {
      offset += var->ty->size;
      offset = align_to(offset, var->ty->align);
      var->offset = -offset;
    }
    fn->stack_size = align_to(offset, 16);
  }
}

// Genera el contenido del segmento de datos, generalmente utilizado para almacenar datos como variables globales.
static void emit_data(Obj *prog) {
  for (Obj *var = prog; var; var = var->next) {
    if (var->is_function)
      continue;

    println("  .data");
    println("  .global %s", var->name);
    println("%s:", var->name);
    
    if (var->init_data) {
      for (int i = 0; i < var->ty->size; i++)
        println("  .byte %d", var->init_data[i]);
    } else {
      println("  .zero %d", var->ty->size);
    }
  }
}

static void store_gp(int r, int offset, int sz) {
  switch (sz) {
  case 1:
    println("  sb %s, %d(fp)", argreg[r], offset);
    return;
  case 2:
    println("  sh %s, %d(fp)", argreg[r], offset);
    return;
  case 4:
    println("  sw %s, %d(fp)", argreg[r], offset);
    return;
  case 8:
    println("  sd %s, %d(fp)", argreg[r], offset);
    return;
  }
  unreachable();
}

// Genera el contenido del segmento de texto, almacenando el programa
static void emit_text(Obj *prog) {
  for (Obj *fn = prog; fn; fn = fn->next) {
    if (!fn->is_function || !fn->is_definition)
      continue;

    if (fn->is_static)
      println("  .local %s", fn->name);
    else
      println("  .globl %s", fn->name);

    println(".text");
    println("%s:", fn->name);
    current_fn = fn;

    // Prologue
    println("  addi sp, sp, -8");
    println("  sw ra, 4(sp)"); // Guarda la dirección de retorno
    println("  sw fp, 0(sp)"); // Guarda el puntero de marco
    println("  mv fp, sp");
    println("  addi sp, sp, %d", -fn->stack_size);

    // Empuja los parámetros pasados desde los registros en la pila
    int i = 0;
    for (Obj *var = fn->params; var; var = var->next)
      store_gp(i++, var->offset, var->ty->size);

    gen_stmt(fn->body);
    assert(depth == 0);

    // Epilogue
    println(".L.return.%s:", fn->name);
    println("  mv sp, fp");
    println("  lw ra, 4(sp)"); // Restaura la dirección de retorno
    println("  lw fp, 0(sp)"); // Restaura el puntero de marco
    println("  addi sp, sp, 8");
    println("  ret");
  }
}

 

void codegen(Obj *prog, FILE *out) {
  output_file = out;
  
  assign_lvar_offsets(prog);


  println("  .section .text");
  println("  .global _start");
  println("  _start:");
  println("  la gp, __global_pointer$");
  println("  la sp, _sstack   ");   
  println("  mv  ra,zero");
  println("  jal zero, main");

  
  emit_text(prog);

  emit_data(prog);
}
 