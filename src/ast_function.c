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
  if ((strcmp(str, "||") == 0) ||
      (strcmp(str, "&&") == 0) ||
      (strcmp(str, ">") == 0) ||
      (strcmp(str, "<") == 0) ||
      (strcmp(str, ">>") == 0))
  {
    res->value = str;
    res->left = ast;
  }
  else
  {
    struct ast *tmp = ast;
    while (tmp->right)
      tmp = tmp->right;
    if (str[0] == '$')
    {
      char *tmp_str = str + 1;
      if ((tmp_str = getenv(tmp_str)) != NULL)
      {
        free(res->value);
        size_t x = 0;
        while (tmp_str[x])
          x++;
        res->value = calloc(x + 1, sizeof(char));
        res->value = strcat(res->value, tmp_str);
      }
      else
        res->value = str;
    }
    else
      res->value = str;
    tmp->right = res;
    return ast;
  }
  return res;
}

void free_ast(struct ast *ast)
{
  if (ast->right == NULL && ast->left == NULL)
  {
    free(ast->value);
    free(ast);
    return ;
  }

  if (ast->left != NULL)
    free_ast(ast->left);
  if (ast->right != NULL)
    free_ast(ast->right);
  free(ast->value);
  free(ast);
}

struct ret_instr *exec_ast(struct ast *ast, FILE *file_out, FILE *file_in)
{
  struct ret_instr *ret = calloc(1, sizeof(struct ret_instr));
  ret->msg = "";
  ret->code = 0;

  if (strcmp(ast->value, "||") == 0)
  {
    struct ret_instr *tmp = exec_ast(ast->left, file_out, file_in);
    if (tmp->code == 0)
    {
      ret->code = 0;
      ret->msg = tmp->msg;
      free(tmp);
      return ret;
    }
    free(tmp);

    struct ret_instr *tmp_2 = exec_ast(ast->right, file_out, file_in);
    ret->code = tmp_2->code;
    ret->msg = tmp_2->msg;
    free(tmp_2);
    return ret;
  }

  if (strcmp(ast->value, "&&") == 0)
  {
    struct ret_instr *tmp = exec_ast(ast->left, file_out, file_in);
    if (tmp->code != 0)
    {
      ret->code = tmp->code;
      ret->msg = tmp->msg;
      free(tmp);
      return ret;
    }
    free(tmp);

    struct ret_instr *tmp_2 = exec_ast(ast->right, file_out, file_in);
    ret->code = tmp_2->code;
    ret->msg = tmp_2->msg;
    free(tmp_2);
    return ret;
  }

  if (strcmp(ast->value, ">") == 0)
  {
    FILE *fd = load_file(ast->right, "w+");

    fflush(stdout);
    int save = dup(fileno(stdout));
    dup2(fileno(fd), STDOUT_FILENO);

    struct ret_instr *tmp = exec_ast(ast->left, fd, file_in);
    ret->code = tmp->code;
    ret->msg = tmp->msg;

    fclose(fd);
    dup2(save, STDOUT_FILENO);

    free(tmp);
    return ret;
  }

  if (strcmp(ast->value, "<") == 0)
  {
    FILE *fd = load_file(ast->right, "r+");

    int save = dup(fileno(stdin));
    dup2(fileno(fd), STDIN_FILENO);

    struct ret_instr *tmp = exec_ast(ast->left, file_out, fd);
    ret->code = tmp->code;
    ret->msg = tmp->msg;

    fclose(fd);
    dup2(save, STDIN_FILENO);

    free(tmp);
    return ret;
  }

  if (strcmp(ast->value, ">>") == 0)
  {
    FILE *fd = load_file(ast->right, "a");
    int save = dup(fileno(stdout));
    dup2(fileno(fd), STDOUT_FILENO);
    struct ret_instr *tmp = exec_ast(ast->left, fd, file_in);
    ret->code = tmp->code;
    ret->msg = tmp->msg;

    fclose(fd);
    dup2(save, STDOUT_FILENO);

    free(tmp);
    return ret;
  }

  if (strcmp(ast->value, "echo") == 0)
  {
    echo_exec(ast->right, file_out);
    return ret;
  }

  if (strcmp(ast->value, "exit") == 0)
  {
    int res = exit_exec(ast->right);
    ret->msg = calloc(5, sizeof(char));
    ret->msg = strcat(ret->msg, "exit");
    ret->code = res;
    return ret;
  }

  if (strcmp(ast->value, "kill") == 0)
  {
    ret->code = kill_exec(ast->right);
    return ret;
  }

  if (strcmp(ast->value, "cd") == 0)
  {
    ret->code = cd_exec(ast->right);
    return ret;
  }

  else
  {
    ret->code = execution_forked(ast->value, ast->right);
    return ret;
  }

  fprintf(stderr, "error on tree");
  ret->code = 1;
  return ret;
}
