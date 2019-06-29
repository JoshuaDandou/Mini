#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "interactive_mode.c"
#include "file_mode.c"

int main(int argc, char *argv[])
{
  int res = 0;
  if (argc < 2)
  {
    //mode interactif
    res = interact_mode();
  }
  else
  {
    //gestion des arguments
    for (int i = 1; i < argc; i++)
      res = read_file(argv[i]);
  }
  return res;
}
