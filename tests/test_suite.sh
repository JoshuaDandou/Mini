#!/bin/sh
make
mv minishell tests/
cd tests

GREEN='\033[32m'
NEUTRE='\033[0m'
RED='\033[31m'

echo Test. > test_1.txt
./minishell files/test.sh > test_1_res_1.txt
diff test_1.txt test_1_res_1.txt
if [ "$?" -eq 0 ]; then
  echo -e "./minishell test.sh : ${GREEN}ok${NEUTRE}"
else
  echo -e "./minishell test.sh : ${RED}ko${NEUTRE}"
fi
rm test_1_res_1.txt

echo "echo Test." | ./minishell > test_1_res.txt
diff test_1.txt test_1_res.txt
if [ "$?" -eq 0 ]; then
  echo -e "echo echo Test. | ./minishell : ${GREEN}ok${NEUTRE}"
else
  echo -e "echo echo Test. | ./minishell : ${RED}ko${NEUTRE}"
fi
rm test_1_res.txt
rm test_1.txt
rm minishell
cd ..
make clean
