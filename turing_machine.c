#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcs.h"

int main(int argc, char const *argv[])
{
#   ifdef DEBUG
    fprintf(stderr, "call main\n");
#   endif

    if (argc != 3) {
        fprintf(stderr, "give a file with an input word and a file with commands\n");
    } else {
        int err, width, height;
        ssize_t line_size;
        char *word = NULL;
        char *command = NULL;
        char *alphabet = NULL;
        size_t len;
        FILE *word_file = NULL;
        FILE *prog_file = NULL;
        char ***machine_prog = NULL;
        char *tape_ptr;
        char *tape_center_ptr;

#       ifdef DEBUG
        debug_int_assign(&err, "err", 0);
        word_file = debug_fopen(argv[1], "r", word_file, "word_file");
        prog_file = debug_fopen(argv[2], "r", prog_file, "prog_file");
#       else
        err = 0;
        word_file = fopen(argv[1], "r");
        prog_file = fopen(argv[2], "r");
#       endif

        if (NULL == word_file || NULL == prog_file) {
            fprintf(stderr, "Error: can't open file\n");
#           ifdef DEBUG
            debug_int_assign(&err, "err", 1);
#           else
            err = 1;
#           endif
        }

        if (err == 0) {
            line_size = getline(&word, &len, word_file);
            if (line_size == -1) { // empty word
                strcpy(word, "^");
            }
            make_tape_for_word(word, strlen(word), &tape_ptr, &tape_center_ptr);

            printf("Input word: %s\n", tape_center_ptr);
#           ifdef DEBUG
            fprintf(stderr, "Input word: %s\n", tape_center_ptr);
#           endif

            if ((getline(&command, &len, prog_file)) != -1) {
                check_prog_size(command, &width, &height, &err);
                make_arr(&machine_prog, width, height, &err);
                if (NULL == machine_prog) {
                    err = 1;
                } else {
                    alphabet = get_alphabet(prog_file);
                    parse_commands(prog_file, &machine_prog, width, height);
                }
            };
            print_prog(machine_prog, width, height);

            execute_prog(machine_prog, width, &tape_center_ptr, alphabet, tape_ptr);

            free_all(&prog_file, &word_file, &word, &command, &machine_prog, width, height, &tape_ptr, &alphabet);
        }

    }

#   ifdef DEBUG
    fprintf(stderr, "return from main\n");
#   endif
    return 0;
}

char *get_alphabet(FILE *prog_file)
{
#   ifdef DEBUG
    fprintf(stderr, "call get_alphabet\n");
#   endif

    char *alphabet = NULL;
    size_t len;

    if ((getline(&alphabet, &len, prog_file)) == -1) {
        fprintf(stderr, "ERROR in get_alphabet, problem in commands");
        exit(EXIT_FAILURE); // обработай
    }
    alphabet[strlen(alphabet)-1] = '\0'; // get rid of \n

#   ifdef DEBUG
    fprintf(stderr, "%s = string:[%s]\n", "alphabet", alphabet);
    fprintf(stderr, "return from get_alphabet\n");
#   endif
    return alphabet;
}

void print_word(char *tape_ptr, char *word, int first_word_index)
{
    char *word_start_ptr = word;
    int word_start_index;

#   ifdef DEBUG
    fprintf(stderr, "call print_word\n");
    fprintf(stderr, "char *word_start_ptr = char *[%s]\n", word);
#   endif


    if (first_word_index < 0) {
#       ifdef DEBUG
        debug_int_assign(&word_start_index, "word_start_index", word - tape_ptr + first_word_index);
        word_start_ptr = tape_ptr + word_start_index;
        fprintf(stderr, "char *word_start_ptr = char *[%s]\n", word_start_ptr);
#       else
        word_start_index = word - tape_ptr + first_word_index;
        word_start_ptr = tape_ptr + word_start_index;
#       endif
    }

    while ((*word_start_ptr) != '\0' && (*word_start_ptr) == '^') { // word left border
        word_start_ptr++;
#       ifdef DEBUG
        fprintf(stderr, "char *word_start_ptr = char *[%s]\n", word_start_ptr);
#       endif
    }
    for (int i = 0; word_start_ptr[i] != '\0'; ++i) { // word right border
        if (word_start_ptr[i] == '^') {
            word_start_ptr[i] = '\0';
        }
    }

#   ifdef DEBUG
    fprintf(stderr, "Word: %s\n", word_start_ptr);
    printf("Word: %s\n", word_start_ptr);
#   else
    printf("Word: %s\n", word_start_ptr);
#   endif


#   ifdef DEBUG
    fprintf(stderr, "return from print_word\n");
#   endif
}

void execute_prog(char ***prog, int width, char **word, char *alphabet, char *tape_ptr)
{
    int status;
    char symbol;
    int prog_end;
    int current_command_index;
    int current_word_index;
    int first_word_index;
    char *current_prog;

#   ifdef DEBUG
    fprintf(stderr, "call execute_prog\n");
    debug_int_assign(&status, "status", 0);
    debug_int_assign(&prog_end, "prog_end", 0);
    debug_int_assign(&current_word_index, "current_word_index", 0);
    debug_int_assign(&first_word_index, "first_word_index", 0);
#   else
    status = 0;
    prog_end = 0;
    current_word_index = 0;
    first_word_index = 0;
#   endif

    while (prog_end == 0) {
        get_current_symbol(word, current_word_index, &symbol);
#       ifdef DEBUG
        debug_int_assign(&current_command_index, "current_command_index", get_current_command_index(symbol, width, alphabet));
        fprintf( stderr, "TEST %d %d\n", status, current_command_index);
        current_prog = prog[status][current_command_index];
        fprintf(stderr, "current_prog = char *[%s]\n", current_prog);
#       else
        current_command_index = get_current_command_index(symbol, width, alphabet);
        current_prog = prog[status][current_command_index];
#       endif

        if (current_word_index < 0) {
#           ifdef DEBUG
            debug_int_assign(&first_word_index, "first_word_index", current_word_index);
#           else
            first_word_index = current_word_index;
#           endif
        }
        if (strlen(current_prog) == 3) {
            write_new_symbol(&word, current_word_index, current_prog);
            move_head(current_prog, &current_word_index);
            change_status(&status, current_prog, &prog_end);

            print_word(tape_ptr, *word, first_word_index);
        } else {
            printf("NOOO (probably prog == x)\n");
            exit(EXIT_FAILURE);
        }

#       ifdef DEBUG
        fprintf(stderr, "symbol = %c , current_prog = %s , current_word_index = %d , status = %d, prog_end = %d \n",
            symbol, current_prog, current_word_index, status, prog_end);
#       endif
    }
#   ifdef DEBUG
    fprintf(stderr, "return from execute_prog\n");
#   endif
}

void change_status(int *status, char *current_prog, int *prog_end)
{
    int command_status, status_value;
#   ifdef DEBUG
    fprintf(stderr, "call change_status\n");
#   endif

    // возможны состояния только с 0 по 9, мб изменить
    status_value = atoi(&(current_prog[2]));

#   ifdef DEBUG
    fprintf(stderr, "TEST status_value = int[%d], current_prog[2] = char[%c], ascii current_prog[2] =[%d]\n",
        status_value, current_prog[2], current_prog[2]);
#   endif

    if (current_prog[2] == 48 && current_prog[2] == '0') {
        *prog_end = 1;
        command_status = -1;
#       ifdef DEBUG
        fprintf(stderr, "prog_end = %d\n", *prog_end);
#       endif
    } else if (current_prog[2] > 47 && current_prog[2] < 58) {
        status_value--;
    } else if (current_prog[2] > 57) {
        status_value = current_prog[2] - 49;
    } else {
        fprintf(stderr, "Error: incorrect status in programm\n");
        exit(1);
    }
#       ifdef DEBUG
        debug_int_assign(&command_status, "command_status", status_value);
#       else
        command_status = status_value;
#       endif
    // }

#   ifdef DEBUG
    debug_int_assign(status, "*status", command_status);
    fprintf(stderr, "return from change_status\n");
#   else
    *status = command_status;
#   endif
}

void move_head(char *current_prog, int *current_word_index)
{
    char move_instruction;
    int current_word_index_change;
#   ifdef DEBUG
    fprintf(stderr, "call move_head\n");
    debug_char_assign(&move_instruction, "move_instruction", current_prog[1]);
#   else
    move_instruction = current_prog[1];
#   endif

    if (move_instruction == 'L') {
        current_word_index_change = -1;
    } else if (move_instruction == 'R') {
         current_word_index_change = 1;
    } else if (move_instruction == 'N') {
         current_word_index_change = 0;
    } else {
         fprintf(stderr, "super error - programm isn't correct\n");
         exit(EXIT_FAILURE);
    }
#   ifndef DEBUG
    *current_word_index = *current_word_index + current_word_index_change;
#   else
    debug_int_assign(&(*current_word_index), "*current_word_index", *current_word_index + current_word_index_change);
    fprintf(stderr, "return from move_head\n");
#   endif
}

void write_new_symbol(char ***word, int current_word_index, char *current_prog)
{
#   ifdef DEBUG
    fprintf(stderr, "call write_new_symbol\n");
#   endif

    char symbol = current_prog[0];

#   ifdef DEBUG
    char var_name[50];
    sprintf(var_name, "(**word)[%d (current_word_index)]", current_word_index);
    debug_char_assign(&((**word)[current_word_index]), var_name, symbol);
#   else
    (**word)[current_word_index] = symbol;
#   endif

#   ifdef DEBUG
    fprintf(stderr, "return from write_new_symbol\n");
#   endif
}

void get_current_symbol(char **word, int current_word_index, char *symbol)
{
    (int)(*word)[current_word_index] == 0 ? (*symbol = '^') : (*symbol = (*word)[current_word_index]);

#   ifdef DEBUG
    fprintf(stderr, "call get_current_symbol\n");
    fprintf(stderr, "%s = char:[%c] - %d ascii\n", "symbol", *symbol, (int)(*symbol));
    fprintf(stderr, "word = %c , symbol = %c\n", (*word)[current_word_index], *symbol);
    fprintf(stderr, "return from get_current_symbol\n");
#   endif
}

int get_current_command_index(char symbol, int width, char *alphabet)
{
#   ifdef DEBUG
    fprintf(stderr, "call get_current_command_index\n");
#   endif

    int command_index = -1;

    for (int j = 0; j < width && command_index == -1; ++j) {

#       ifdef DEBUG
        fprintf(stderr, "j = %d, alphabet[j] = %c, symbol = %c\n", j, alphabet[j], symbol);
#       endif

        if (alphabet[j] == symbol) {
#           ifdef DEBUG
            debug_int_assign(&command_index, "command_index", j);
#           else
            command_index = j;
#           endif
        }
    }

    if (command_index == -1) {
        fprintf(stderr, "Error in get_current_command_index\n");
        exit(EXIT_FAILURE);
    }
#   ifdef DEBUG
    fprintf(stderr, "return from get_current_command_index\n");
#   endif

    return command_index;
}

void print_prog(char ***machine_prog, int width, int height)
{
#   ifdef DEBUG
    fprintf(stderr, "call print_prog\n");
#   endif

    printf("Programm:\n");
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
           printf("%5s", machine_prog[i][j]);
        }
        printf("\n");
    }
#   ifdef DEBUG
    fprintf(stderr, "return from print_prog\n");
#   endif
}

void parse_commands(FILE *prog_file, char ****machine_prog, int width, int height)
{
#   ifdef DEBUG
    fprintf(stderr, "call parse_commands\n");
#   endif

    char *command = NULL;
    size_t len, chars;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            chars = getline(&command, &len, prog_file);

#           ifdef DEBUG
            fprintf(stderr, "%zu chars in %s",  chars - 1, command);
#           endif

            if (chars != (size_t)-1) {
                if (chars - 1 >= 5) {
                    int status = atoi(&(command[4])) + 48;
                    // printf("TEST %d \n ", status);
#                   ifdef DEBUG
                    fprintf(stderr, "i = %d, j = %d\n", i, j);
                    debug_char_assign(&((*machine_prog)[i][j][0]), "(*machine_prog)[i][j][0]", command[0]);
                    debug_char_assign(&((*machine_prog)[i][j][1]), "(*machine_prog)[i][j][1]", command[2]);
                    debug_char_assign(&((*machine_prog)[i][j][2]), "(*machine_prog)[i][j][2]", status);
                    (*machine_prog)[i][j][3] = '\0';
                    if ((*machine_prog)[i][j][3] == '\0') {
                        fprintf(stderr, "(*machine_prog)[i][j][3] = char:[%s]\n",  "\\0");
                    }
#                   else
                    (*machine_prog)[i][j][0] = command[0];
                    (*machine_prog)[i][j][1] = command[2];
                    (*machine_prog)[i][j][2] = status;
                    (*machine_prog)[i][j][3] = '\0';
#                   endif
                } else if (1 == chars - 1) {
#                   ifdef DEBUG
                    fprintf(stderr, "i = %d, j = %d\n", i, j);
                    debug_char_assign(&((*machine_prog)[i][j][0]), "(*machine_prog)[i][j][0]", command[0]);
                    (*machine_prog)[i][j][1] = '\0';
                    if ((*machine_prog)[i][j][1] == '\0') {
                        fprintf(stderr, "(*machine_prog)[i][j][1] = char:[%s]\n",  "\\0");
                    }
#                   else
                    (*machine_prog)[i][j][0] = command[0];
                    (*machine_prog)[i][j][1] = '\0';
#                   endif
                }
            } else {
                fprintf(stderr, "ERRROR in parse_commands: not enough commands\n");
            }
        }
    }
    free(command);

#   ifdef DEBUG
    fprintf(stderr, "return from parse_commands\n");
#   endif
}

void check_prog_size(char *command, int *width, int *height, int *err)
{
#   ifdef DEBUG
    fprintf(stderr, "call check_prog_size\n");
#   endif

    if (sscanf(command, "%d%d", width, height) != 2) {
        *err=1;
        fprintf(stderr, "%s = int:[%d]\n", "err", *err);
    }

#   ifdef DEBUG
    fprintf(stderr, "%s = int:[%d]\n", "width", *width);
    fprintf(stderr, "%s = int:[%d]\n", "height", *height);
    fprintf(stderr, "return from check_prog_size\n");
#   endif
}

void make_arr(char ****machine_prog, int width, int height, int *err)
{
#   ifdef DEBUG
    fprintf(stderr, "call make_arr\n");
#   endif

    *machine_prog = (char ***)malloc(height * sizeof(char ***));
    if (NULL == *machine_prog) {
        fprintf(stderr, "can't allocate mem 1 in make_arr\n");
        *err = 1;
    }

    for (int i = 0; i < height && *err == 0; ++i) {

        (*machine_prog)[i] = (char **)malloc(width * sizeof(char **));
        if (NULL == (*machine_prog)[i]) {
            fprintf(stderr, "can't allocate mem 2 in make_arr\n");
            *err = 1;
        }

        for (int j = 0; j < width && *err == 0; ++j) {

            (*machine_prog)[i][j] = (char *)malloc(sizeof(char *));
            if (NULL == (*machine_prog)[i][j]) {
                fprintf(stderr, "can't allocate mem 3 in make_arr\n");
                *err = 1;
            }
        }
    }

#   ifdef DEBUG
    fprintf(stderr, "return from make_arr\n");
#   endif
}

void free_all(FILE **prog_file, FILE **word_file, char **word, char **command, char ****machine_prog, int width, int height, char **tape_ptr, char **alphabet)
{
#   ifdef DEBUG
    fprintf(stderr, "call free_all\n");
#   endif

    fclose(*prog_file);
    fclose(*word_file);
    free(*word);
    free(*command);
    free(*tape_ptr);
    free(*alphabet);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            free((*machine_prog)[i][j]);
        }
        free((*machine_prog)[i]);
    }
    free((*machine_prog));

#   ifdef DEBUG
    fprintf(stderr, "return from free_all\n");
#   endif
}

int make_tape_for_word(char *word, int word_len, char **tape_ptr, char **tape_center_ptr)
{
#   ifdef DEBUG
    fprintf(stderr, "call make_tape_for_word\n");
#   endif

    int tape_len_multiplier = 10; // random huge
    int tape_final_len = word_len * tape_len_multiplier * sizeof(char);
    word[strlen(word)-1] = '\0'; // get rid of \n

    *tape_ptr = (char *)calloc(word_len * tape_len_multiplier, sizeof(char));
    if (NULL == *tape_ptr) {
        fprintf(stderr, "calloc failed!\n");
        exit(EXIT_FAILURE); // обработай
    } else {
        *tape_center_ptr = *tape_ptr + (tape_final_len / 2);
        strcpy(*tape_center_ptr, word);
#       ifdef DEBUG
        fprintf(stderr, "%d bytes allocated in tape_ptr\n", tape_final_len);
        fprintf(stderr, "%s = string:[%s]\n", "*tape_center_ptr", *tape_center_ptr);
#       endif
    }

#   ifdef DEBUG
    fprintf(stderr, "return from make_tape_for_word\n");
#   endif
    return tape_final_len;
}
