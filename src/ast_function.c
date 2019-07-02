#include "ast_struct.h"

struct ast *new_ast()
{
  struct ast *new = calloc(1, sizeof(struct ast));
  new->value = NULL;
  new->right = NULL;
  new->left = NULL;
  return new;
}

struct ast *add_ast(struct ast *ast, char *str)
{
  if (ast == NULL)
  {
    struct ast *res = new_ast();
    res->value = str;
    return res;
  }
 
  struct ast *res = new_ast();
  if (strcmp(str, "||") == 0)
  {
    res->value = str;
    res->left = ast;
  }
  else if (strcmp(str, "&&") == 0)
  {
    res->value = str;
    res->left = ast;
  }
  else
  {
    struct ast *tmp = ast;
    while (tmp->right)
      tmp = tmp->right;
    res->value = str;
    tmp->right = res;
    return ast;
  }
  return res;
}

void echo_exec(struct ast *param)
{
  bool n_mode = false;
  if (param && (strcmp(param->value, "-n") == 0))
  {
    n_mode = true;
    param = param->right;
  }

  int fst_wrd = 0;
  while (param)
  {
    if (fst_wrd == 0)
      fst_wrd++;
    else
      write(1, " ", 1);

    char *tmp = param->value;
    while (*tmp != '\0')
    {
      write(1, tmp, 1);
      tmp += 1;
    }
    param = param->right;
  }
  if (!n_mode)
    write(1, "\n", 1);
}

int exec_ast(struct ast *ast)
{
  if (ast->right == NULL && ast->left == NULL)
    return 1;

  if (strcmp(ast->value, "||") == 0)
  {
    if (exec_ast(ast->left) == 0)
      return 0;
    return exec_ast(ast->right);
  }

  if (strcmp(ast->value, "&&") == 0)
  {
    if (exec_ast(ast->left) != 0)
      return 1;
    return exec_ast(ast->right);
  }

  if (strcmp(ast->value, "echo") == 0)
  {
    echo_exec(ast->right);
    return 0;
  }

  fprintf(stderr, "error on tree");
  return 1;
}
