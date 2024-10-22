#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void debug_int_assign(int *var, char *var_name, int value)
{
    *var=value;
    fprintf(stderr, "%s = int:[%d]\n", var_name, value);
}

void debug_float_assign(float *var, char *var_name, float value)
{
    *var=value;
    fprintf(stderr, "%s = float:[%f]\n", var_name, value);
}

void debug_char_assign(char *var, char *var_name, char value)
{
    *var=value;
    fprintf(stderr, "%s = char:[%c]\n", var_name, value);
}

void debug_str_assign(char **var, char *var_name, char *value)
{
    *var=value;
    fprintf(stderr, "%s = char:[%s]\n", var_name, value);
}

void debug_call_func_no_args(int (*func)(), char *func_name)
{
    fprintf(stderr, "call %s\n", func_name);
    func();
    fprintf(stderr, "return from %s\n", func_name);
}

FILE *debug_fopen(const char *name, char *mode, FILE *var, char *var_name)
{
    fprintf(stderr, "call fopen( %s, %s )\n", name, mode);

    var = fopen(name, mode);

    perror("fopen");
    fprintf(stderr, "%s = FILE:[file]\n", var_name);
    fprintf(stderr, "return from fopen\n");

    return var;
}
