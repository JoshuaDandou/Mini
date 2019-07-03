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


echo -n moapo fnei e > test_2.txt
./minishell files/test_echo-n.sh > test_2_res.txt
diff test_2.txt test_2_res.txt
if [ "$?" -eq 0 ]; then
  echo -e "./minishell echo -n moapo fnei e : ${GREEN}ok${NEUTRE}"
else
  echo -e "./minishell echo -n moapo fnei e : ${RED}ko${NEUTRE}"
fi
rm test_2_res.txt

echo "echo -n moapo fnei e" | ./minishell > test_2_res_2.txt
diff test_2.txt test_2_res_2.txt
if [ "$?" -eq 0 ]; then
  echo -e "echo -n moapo fnei e | ./minishell : ${GREEN}ok${NEUTRE}"
else
  echo -e "echo -n moapo fnei e | ./minishell : ${RED}ko${NEUTRE}"
fi
rm test_2_res_2.txt
rm test_2.txt

(echo poi; qwepo cie asieu; popo; echo -n porte) > test_3.txt
./minishell files/test_instructions.sh > test_3_res.txt
diff test_3.txt test_3_res.txt
if [ "$?" -eq 0 ]; then
  echo -e "./minishell echo poi; qwepo cie asieu;
popo; echo -n porte : ${GREEN}ok${NEUTRE}"
else
  echo -e "./minishell echo poi; qwepo cie asieu;
popo; echo -n porte : ${RED}ko${NEUTRE}"
fi
rm test_3_res.txt

echo "echo poi; qwepo cie asieu; popo; echo -n porte" | ./minishell > test_3_res_3.txt
diff test_3.txt test_3_res_3.txt
if [ "$?" -eq 0 ]; then
  echo -e "echo poi; qwepo cie asieu;
popo; echo -n porte | ./minishell : ${GREEN}ok${NEUTRE}"
else
  echo -e "./minishell echo poi; qwepo cie asieu;
popo; echo -n porte | ./minishell : ${RED}ko${NEUTRE}"
fi
rm test_3_res_3.txt
rm test_3.txt

 > test_4.txt
./minishell files/test_empty.sh > test_4_res.txt
diff test_4.txt test_4_res.txt
if [ "$?" -eq 0 ]; then
  echo -e "./minishell () : ${GREEN}ok${NEUTRE}"
else
  echo -e "./minishell () : ${RED}ko${NEUTRE}"
fi
rm test_4_res.txt

echo | ./minishell > test_4_res_4.txt
if [ "$?" -eq 0 ]; then
  echo -e "() | ./minishell : ${GREEN}ok${NEUTRE}"
else
  echo -e "() | ./minishell : ${RED}ko${NEUTRE}"
fi
rm test_4_res_4.txt
rm test_4.txt

rm minishell
cd ..
make clean
