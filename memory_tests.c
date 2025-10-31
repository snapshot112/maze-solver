/*
 * Created by snapshot112 on 27/10/2025
 *
 * Naam: Jeroen Boxhoorn
 * UvAnetID: 16333969
 * Studie: BSC Informatica
 *
 * A file to test the stack and the queue interfaces.
 */

#define TEST_COUNT 2
#include <stdio.h>

#include "queue.h"

#include "stack.h"

int stack_pop_test(void) {
    int result = 0;
    struct stack *s = stack_init(15);
    if (s == NULL) {
        printf("Help, no stack created!!!\n");
        return -1;
    }

    for (int i = 0; i < 15; i++) {
        stack_push(s, i);
    }

    for (int i = 14; i >= 0; i--) {
        const int pop_status = stack_pop(s);
        if (pop_status != i) {
            result++;
            printf("Stack pop failed on popping item '%d' code: '%d'\n", i, pop_status);
        }
    }

    stack_stats(s);
    stack_cleanup(s);

    return result;
}

int stack_resize_test(void) {
    int result = 0;
    struct stack *s = stack_init(0);
    if (s == NULL) {
        printf("Help, no stack created!!!\n");
        return -1;
    }

    for (int i = 0; i < 15; i++) {
        stack_push(s, i);
    }

    for (int i = 14; i >= 0; i--) {
        const int pop_status = stack_pop(s);
        if (pop_status != i) {
            result++;
            printf("Stack pop failed on popping item '%d' code: '%d'\n", i, pop_status);
        }
    }

    stack_stats(s);
    stack_cleanup(s);

    return result;
}

int queue_pop_test(void) {
    int result = 0;
    struct queue *q = queue_init(15);
    if (q == NULL) {
        printf("Help, no queue created!!!\n");
    }

    for (int i = 0; i < 15; i++) {
        queue_push(q, i);
    }
    queue_stats(q);

    for (int i = 0; i < 15; i++) {
        const int pop_status = queue_pop(q);
        if (pop_status != i) {
            printf("Queue pop failed on popping item '%d' code: '%d'\n", i, pop_status);
            result++;
        }
    }

    queue_stats(q);
    queue_cleanup(q);

    return result;
}

int queue_resize_test(void) {
    int result = 0;
    struct queue *q = queue_init(5);
    if (q == NULL) {
        printf("Help, no queue created!!!\n");
    }

    for (int i = 0; i < 200; i++) {
        queue_push(q, i);
    }
    for (int i = 0; i < 200; i++) {
        const int pop_status = queue_pop(q);
        if (pop_status != i) {
            printf("Queue pop failed on popping item '%d' code: '%d'\n", i, pop_status);
            result++;
        }
    }

    queue_stats(q);
    queue_cleanup(q);

    return result;
}

int main(void) {
    const int results[] = {
        // stack_pop_test(),
        // stack_resize_test(),
        // queue_pop_test(),
        queue_resize_test()
    };

    int failures = 0;
    int passes = 0;
    int failed_to_run = 0;

    for (size_t i = 0; i < sizeof(results) / sizeof(int); i++) {
        if (results[i] == -1) {
            failed_to_run++;
            continue;
        }

        if (results[i] == 0) {
            passes++;
            continue;
        }

        failures += results[i];
    }

    if (passes > 0) {
        printf("%d tests passed\n", passes);
    }
    if (failures > 0) {
        printf("%d tests failed\n", failures);
    }
    if (failed_to_run > 0) {
        printf("%d tests failed to run\n", failed_to_run);
    }
}
