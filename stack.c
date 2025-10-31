/*
 * Created by snapshot112 on 27/10/2025
 *
 * Naam: Jeroen Boxhoorn
 * UvAnetID: 16333969
 * Studie: BSC Informatica
 *
 * An implementation of the stack interface provided in stack.h
 *
 * The position of the stack always marks the first empty element in the stack.
 * This stack will try to resize automatically if it's storage capacity is full.
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

struct stack {
    int *data;
    size_t size;
    size_t capacity;
    size_t pops;
    size_t pushes;
    size_t max_elements;
};

struct stack *stack_init(const size_t capacity) {
    struct stack *sp = malloc(sizeof(struct stack));
    if (sp == NULL) {
        return NULL;
    }

    sp->data = malloc(sizeof(int) * capacity);
    if (sp->data == NULL) {
        free(sp);
        return NULL;
    }

    sp->capacity = capacity;
    sp->size = 0;
    sp->pops = 0;
    sp->pushes = 0;
    sp->max_elements = 0;

    return sp;
}

void stack_cleanup(struct stack *s) {
    if (s != NULL) {
        if (s->data != NULL) {
            free(s->data);
        }
        free(s);
    }
}

void stack_stats(const struct stack *s) {
    if (s == NULL) {
        return;
    }
    fprintf(stderr, "stats %lu %lu %lu\n", s->pushes, s->pops, s->max_elements);
}

/*
 * Double the stack capacity, capacity is set to 1 if it was 0 previously.
 *
 * Input:
 * s: The stack whose capacity should be doubled.
 *
 * Output:
 * 0 if the doubling succeeded.
 * -1 on failure.
 */
static int stack_grow(struct stack *s) {
    const size_t new_capacity = s->capacity < 10 ? 20 : s->capacity * 2;
    int *new_memory = realloc(s->data, new_capacity * sizeof(int));
    if (new_memory == NULL) {
        return -1;
    }

    s->data = new_memory;
    s->capacity = new_capacity;
    return 0;
}

int stack_push(struct stack *s, const int c) {
    if (s == NULL || s->data == NULL) {
        return 1;
    }

    // Bounds check and try to resize if needed.
    if (s->size >= s->capacity && stack_grow(s) == -1) {
            return 1;
    }

    s->data[s->size] = c;
    s->size++;

    // stats
    s->pushes++;
    if (s->size > s->max_elements) {
        s->max_elements = s->size;
    }

    return 0;
}

int stack_pop(struct stack *s) {
    if (s == NULL || s->data == NULL) {
        return -1;
    }

    // Bounds check
    if (s->size <= 0) {
        return -1;
    }

    s->size--;
    s->pops++;
    return s->data[s->size];
}

int stack_peek(const struct stack *s) {
    if (s == NULL || s->data == NULL) {
        return -1;
    }

    // Bounds check
    if (s->size <= 0) {
        return -1;
    }

    return s->data[s->size - 1];
}

int stack_empty(const struct stack *s) {
    if (s == NULL || s->data == NULL) {
        return -1;
    }

    if (s->size == 0) {
        return 1;
    }

    return 0;
}

size_t stack_size(const struct stack *s) {
    if (s == NULL || s->data == NULL) {
        return 0;
    }

    return s->size;
}
