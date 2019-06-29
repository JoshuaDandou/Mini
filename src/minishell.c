#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "interactive_mode.c"

/*int read_file(char *file)
{
  return 0;
}*/

int main(int argc, char *argv[])
{
  int res = 0;
  if (argc < 2)
  {
    res = interact_mode();
    return res;
  }
  else
  {
    //gestion des arguments
    for (int i = 1; i < argc; i++)
    {
      //res = read_file(argv[i]);
      if (argv[i])
        return 0;
    }
    return 0;
  }
  return res;
}
