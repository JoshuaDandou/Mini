#include "ast_struct.h"
#include "return_struct.h"
#include "execution_functions.c"

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

struct ret_instr *exec_ast(struct ast *ast)
{
  struct ret_instr *ret = calloc(1, sizeof(struct ret_instr));
  ret->msg = "";
  ret->code = 0;

  if (strcmp(ast->value, "||") == 0)
  {
    if (exec_ast(ast->left) == 0)
    {
      ret->code = 0;
      return ret;
    }
    struct ret_instr *tmp = exec_ast(ast->right);
    ret->code = tmp->code;
    free(tmp);
    return ret;
  }

  if (strcmp(ast->value, "&&") == 0)
  {
    if (exec_ast(ast->left) != 0)
    {
      ret->code = 1;
      return ret;
    }
    struct ret_instr *tmp = exec_ast(ast->right);
    ret->code = tmp->code;
    free(tmp);
    return ret;
  }

  if (strcmp(ast->value, "echo") == 0)
  {
    echo_exec(ast->right);
    return ret;
  }

  if (strncmp(ast->value, "exit", 4) == 0)
  {
    int res = exit_exec(ast->right);
    ret->msg = calloc(5, sizeof(char));
    ret->msg = strcat(ret->msg, "exit");
    ret->code = res;
    return ret;
  }

  //if (ast->right == NULL && ast->left == NULL)
  else
  {
    //int res = execution_forked(ast->value, ast->right);
    //if (res < 0)
    //{
      ret->msg = calloc(1, sizeof(ast->value) + 21);
      ret->msg = strcpy(ret->msg, ast->value);
      ret->msg = strcat(ret->msg, ": command not found");
      ret->code = 127;
    /*}
    else
    {
      ret->code = res;
    }*/
    return ret;
  }

  fprintf(stderr, "error on tree");
  ret->code = 1;
  return ret;
}
