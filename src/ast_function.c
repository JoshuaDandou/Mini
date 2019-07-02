#include "ast_struct.h"

struct ast *new_ast()
{
  struct ast *new = calloc(sizeof(struct ast));
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
 
  struct tree *res = new_ast();
  switch (str)
  {
    case "||":
      res->value = str;
      res->left = ast;
      break;
    case "&&":
      res->value = str;
      res->left = ast;
      break;
    default:
      res->value = str;
      ast->right = res;
      break;
  }
  return res;
}

int exec_ast(struct ast *ast)
{
  if (ast->right == NULL && ast->left == NULL)
    //execution function

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

  fprintf(stderr, "error on tree");
  return 1;
}
