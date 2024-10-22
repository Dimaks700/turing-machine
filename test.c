#include "funcs.h"

#include <stdio.h>

int test_debug_int_assign();
int test_debug_float_assign();
int test_debug_char_assign();
int test_debug_str_assign();
void test_debug_call_func_no_args();


int main(int argc, char const *argv[])
{
    int errors = 0;

    errors += test_debug_int_assign();
    errors += test_debug_float_assign();
    errors += test_debug_char_assign();
    errors += test_debug_str_assign();

    test_debug_call_func_no_args();

    fprintf(stderr, "Errors: %d total\n", errors);
    return 0;
}

int test_debug_int_assign()
{
    int a;
    int errors = 0;

    #ifdef DEBUG
    debug_int_assign(&a, "a", 1);
    #else
    a=1;
    #endif

    if (a != 1) {
        errors++;
        fprintf(stderr, "Error: %d\n", a);
    }

    #ifdef DEBUG
    debug_int_assign(&a, "a", 2);
    #else
    a = 2;
    #endif

    if (a != 2) {
        errors++;
        fprintf(stderr, "Error: %d\n", a);
    }

    #ifdef DEBUG
    debug_int_assign(&a, "a", 3);
    #else
    a = 3;
    #endif

    if (a != 3) {
        errors++;
        fprintf(stderr, "Error: %d\n", a);
    }

    return errors;
}

int test_debug_float_assign()
{
    float a;
    int errors = 0;

    #ifdef DEBUG
    debug_float_assign(&a, "a", 1.1);
    #else
    a = 1.1;
    #endif

    if (a != 1.1f) {
        errors++;
        fprintf(stderr, "Error: %f\n", a);
    }

    #ifdef DEBUG
    debug_float_assign(&a, "a", 2.2);
    #else
    a = 2.2;
    #endif

    if (a != 2.2f) {
        errors++;
        fprintf(stderr, "Error: %f\n", a);
    }


    #ifdef DEBUG
    debug_float_assign(&a, "a", 3.3);
    #else
    a = 3.3;
    #endif

    if (a != 3.3f) {
        errors++;
        fprintf(stderr, "Error: %f\n", a);
    }

    return errors;
}

int test_debug_char_assign()
{
    char a;
    int errors = 0;

    #ifdef DEBUG
    debug_char_assign(&a, "a", 'a');
    #else
    a = 'a';
    #endif

    if (a != 'a') {
        errors++;
        fprintf(stderr, "Error: %c\n", a);
    }

    #ifdef DEBUG
    debug_char_assign(&a, "a", 'b');
    #else
    a = 'b';
    #endif

    if (a != 'b') {
        errors++;
        fprintf(stderr, "Error: %c\n", a);
    }

    #ifdef DEBUG
    debug_char_assign(&a, "a", 'c');
    #else
    a = 'c';
    #endif

    if (a != 'c') {
        errors++;
        fprintf(stderr, "Error: %c\n", a);
    }

    return errors;
}

int test_debug_str_assign()
{
    char *a;
    int errors = 0;

    #ifdef DEBUG
    debug_str_assign(&a, "a", "abc");
    #else
    a = "abc";
    #endif

    if (a != "abc") {
        errors++;
        fprintf(stderr, "Error: %s\n", a);
    }

    #ifdef DEBUG
    debug_str_assign(&a, "a", "qwe");
    #else
    a = "qwe";
    #endif

    if (a != "qwe") {
        errors++;
        fprintf(stderr, "Error: %s\n", a);
    }

    #ifdef DEBUG
    debug_str_assign(&a, "a", "asd");
    #else
    a = "asd";
    #endif

    if (a != "asd") {
        errors++;
        fprintf(stderr, "Error: %s\n", a);
    }

    return errors;
}

void test_debug_call_func_no_args()
{
    debug_call_func_no_args(&test_debug_float_assign, "test_debug_float_assign");

    debug_call_func_no_args(&test_debug_str_assign, "test_debug_str_assign");

    debug_call_func_no_args(&test_debug_char_assign, "test_debug_char_assign");
}

