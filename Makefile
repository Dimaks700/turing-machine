CC=gcc
flags=-Wall -Werror -Wextra


all: compile_and_run

alld: compile_debug check

task:
	$(CC) $(flags) turing_machine.c funcs.c -o turing_machine.bin -fsanitize=address
	@printf "\nEXE 1: Приписать слева к слову P символ b (P → bP). \n\n"
	./turing_machine.bin tasks/task1_word1.txt tasks/task1_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task1_word2.txt tasks/task1_commands.txt # &>test2.txt
	@printf "\nEXE 2: Приписать справа к слову P символы bc (P → Pbc). \n\n"
	./turing_machine.bin tasks/task2_word1.txt tasks/task2_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task2_word2.txt tasks/task2_commands.txt # &>test1.txt
	@printf "\nEXE 3: Определить, является ли слово P записью числа в двоичной системе счисления (непустым словом, состоящем только из цифр 0 и 1). Ответ: слово 1 (да) или слово 0\n\n"
	./turing_machine.bin tasks/task3_word1.txt tasks/task3_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task3_word2.txt tasks/task3_commands.txt # &>test1.txt
	@printf "\nEXE 4: Считая непустое слово P записью числа в четверичной системе счисления, определить, является оно чётным числом или нет. Ответ: 1 (да) или 0.\n\n"
	./turing_machine.bin tasks/task4_word1.txt tasks/task4_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task4_word2.txt tasks/task4_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task4_word3.txt tasks/task4_commands.txt # &>test1.txt
	@printf "\nEXE 4: Заменить в P каждое вхождение a на bb.  \n\n"
	./turing_machine.bin tasks/task5_word1.txt tasks/task5_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task5_word2.txt tasks/task5_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task5_word3.txt tasks/task5_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task5_word4.txt tasks/task5_commands.txt # &>test1.txt
	@printf "\nEXE 5: Удвоить каждый символ слова P (например: bab → bbaabb.  \n\n"
	./turing_machine.bin tasks/task6_word1.txt tasks/task6_commands.txt #&>test2.txt
	./turing_machine.bin tasks/task6_word2.txt tasks/task6_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task6_word3.txt tasks/task6_commands.txt # &>test1.txt
	./turing_machine.bin tasks/task6_word4.txt tasks/task6_commands.txt # &>test2.txt
	@printf "\nEXE 6: Считая непустое слово P записью двоичного числа, получить это же число, но в четверичной системе. (Замечание: учесть, что в двоичном числе может быть нечётное количество цифр.).  \n\n"
	./turing_machine.bin tasks/task7_word1.txt tasks/task7_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task7_word2.txt tasks/task7_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task7_word3.txt tasks/task7_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task7_word4.txt tasks/task7_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task7_word5.txt tasks/task7_commands.txt  #&>test1.txt
	@printf "\nEXE 7: Определить, сбалансировано ли слово P по круглым скобкам. Ответ: Д (да) или Н (нет).  \n\n"
	./turing_machine.bin tasks/task8_word1.txt tasks/task8_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task8_word2.txt tasks/task8_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task8_word3.txt tasks/task8_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task8_word4.txt tasks/task8_commands.txt  #&>test1.txt
	@printf "\nEXE 7: Если в P символов a больше, чем символов b, то выдать ответ a, если символов a меньше символов b, то выдать ответ b, а иначе в качестве ответа выдать пустое слово  \n\n"
	./turing_machine.bin tasks/task9_word1.txt tasks/task9_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task9_word2.txt tasks/task9_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task9_word3.txt tasks/task9_commands.txt  #&>test1.txt
	./turing_machine.bin tasks/task9_word4.txt tasks/task9_commands.txt  #&>test1.txt


compile_and_run:
	$(CC) $(flags) turing_machine.c funcs.c -o turing_machine.bin -fsanitize=address
	@printf "\nProg1\n"
	./turing_machine.bin word.txt commands.txt
	@printf "\nProg2\n"
	./turing_machine.bin word2.txt commands2.txt
	@printf "\nProg3\n"
	./turing_machine.bin word3.txt commands3.txt

compile_debug:
	$(CC) $(flags) turing_machine.c funcs.c -o turing_machine.valgrind.bin -DDEBUG
	$(CC) $(flags) turing_machine.c funcs.c -o turing_machine.fsanitize.bin -DDEBUG -fsanitize=address

check:
	rm logs/valgrind.stdout.log logs/valgrind.stderr.log logs/sanitize.stdout.log logs/sanitize.stderr.log
	valgrind ./turing_machine.valgrind.bin word.txt commands.txt 1>>logs/valgrind.stdout.log 2>>logs/valgrind.stderr.log || cat logs/valgrind.stdout.log
	./turing_machine.fsanitize.bin word.txt commands.txt 1>>logs/sanitize.stdout.log 2>>logs/sanitize.stderr.log || gvimdiff logs/sanitize.stderr.log logs/check.etl.log
	valgrind ./turing_machine.valgrind.bin word2.txt commands2.txt 1>>logs/valgrind.stdout.log 2>>logs/valgrind.stderr.log || cat logs/valgrind.stdout.log
	./turing_machine.fsanitize.bin word2.txt commands2.txt 1>>logs/sanitize.stdout.log 2>>logs/sanitize.stderr.log || logs/sanitize.stderr.log
	valgrind ./turing_machine.valgrind.bin word3.txt commands3.txt 1>>logs/valgrind.stdout.log 2>>logs/valgrind.stderr.log || cat logs/valgrind.stdout.log
	./turing_machine.fsanitize.bin word3.txt commands3.txt 1>>logs/sanitize.stdout.log 2>>logs/sanitize.stderr.log  || gvimdiff logs/sanitize.stderr.log logs/check.etl.log

lib:
	$(CC) funcs.c -c
	ar rc funcs.a funcs.o
	ranlib funcs.a

clean:
	rm -f *.o *.a *.exe *.bin a.out

test_funcs:
	$(CC) test.c funcs.c -o test.exe
	./test.exe 2>check.log
	cat check.log
	diff check.etl.log check.log && echo success! # || gvimdiff check.etl.log check.log

change_etl:
	cat logs/sanitize.stderr.log >logs/check.etl.log

stack:
	$(CC) $(flags) stack.c -o stack
	./stack
