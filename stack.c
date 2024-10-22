#include <stdio.h>
#include <stdlib.h>
#define STACK_SIZE 100

typedef struct int_stack {
    int stack_len;
    int *elements;
} stack;

void push(struct int_stack *stack, int element);
void is_empty(struct int_stack stack);
int peek(struct int_stack stack);
int pop(struct int_stack *stack);


int main() {
    struct int_stack stack = { -1, alloca(STACK_SIZE*4) };
    if (NULL == stack.elements)
    {
        fprintf(stderr, "allocation error\n");
        exit(1);
    }
    is_empty(stack);
    push(&stack, 1);
    is_empty(stack);

    printf("last stack element == %d, stack_len == %d\n", stack.elements[stack.stack_len], stack.stack_len);

    printf("peek == %d\n", peek(stack));

    push(&stack, 2);

    printf("last stack element == %d, stack_len == %d\n", stack.elements[stack.stack_len], stack.stack_len);

    printf("peek == %d\n", peek(stack));

    push(&stack, 3);

    printf("last stack element == %d, stack_len == %d\n", stack.elements[stack.stack_len], stack.stack_len);

    printf("peek == %d\n", peek(stack));

    printf("pop == %d\n", pop(&stack));

    printf("last stack element == %d, stack_len == %d\n", stack.elements[stack.stack_len], stack.stack_len);

    printf("pop == %d\n", pop(&stack));

    printf("last stack element == %d, stack_len == %d\n", stack.elements[stack.stack_len], stack.stack_len);

    return 0;
}

void push(struct int_stack *stack, int element) {
    stack->stack_len++;
    if ((*stack).stack_len >= STACK_SIZE)
    {
        fprintf(stderr, "StackOverflowError\n");
        exit(1);
    }
    stack->elements[(*stack).stack_len] = element;
}

void is_empty(struct int_stack stack) {
    stack.stack_len == -1 ? printf("stack is empty\n") : printf("stack isn't empty\n");
}

int peek(struct int_stack stack) {
    return stack.elements[stack.stack_len];
}

int pop(struct int_stack *stack) {
    int elem = (*stack).elements[(*stack).stack_len];
    stack->stack_len--;
    return elem;
}
