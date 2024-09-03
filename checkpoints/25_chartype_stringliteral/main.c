#include "chibicc.h"

//8バイト単位になるようにバッファ確保　例えば、9バイトなら16バイト確保 7バイトなら8バイト確保
int align_to(int n, int align) {
  return (n + align - 1) & ~(align - 1);
}

int main(int argc, char **argv) {
  if (argc != 2)
    error("%s: invalid number of arguments", argv[0]);

  // Tokenize and parse.
  user_input = argv[1];
  token = tokenize();
  Program *prog = program();
  add_type(prog);

  // Assign offsets to local variables.
  for (Function *fn = prog->fns; fn; fn = fn->next) {
    int offset = 0;
    for (VarList *vl = fn->locals; vl; vl = vl->next) {
      Var *var = vl->var;
      offset += size_of(var->ty); //intなら8　charなら1　配列であれば左記値＊配列数で返却
      var->offset = offset;
    }
    fn->stack_size = align_to(offset, 8);
  }

  // Traverse the AST to emit assembly.
  codegen(prog);

  return 0;
}