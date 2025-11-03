/*
 * Created by snapshot112 on 27/10/2025
 *
 * Naam: Jeroen Boxhoorn
 * UvAnetID: 16333969
 * Studie: BSC Informatica
 *
 * An implementation of the queue interface provided in stack.h
 *
 * Features of this queue:
 * > The position of the head marks either the first non-empty element in the queue or the tail.
 * > The position of the tail always marks the first empty element in the queue.
 * > This queue always needs 1 empty space in order to function properly
 *   Not having this empty space would mean that, in case the head and tail index are the same,
 *   The state of the queue would be ambiguous between completely full and completely empty.
 * > This queue will try to resize automatically if it's storage capacity is full.
 */

#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#include <string.h>

struct queue {
    int *data;
    size_t head_index;
    size_t tail_index;
    size_t size;
    size_t capacity;
    size_t pops;
    size_t pushes;
    size_t max_elements;
};

struct queue *queue_init(size_t capacity) {
    capacity++;
    struct queue *q = malloc(sizeof(struct queue));
    if (q == NULL) {
        return NULL;
    }

    q->data = malloc(sizeof(int) * capacity);
    if (q->data == NULL) {
        free(q);
        return NULL;
    }

    q->capacity = capacity;
    q->head_index = 0;
    q->tail_index = 0;
    q->size = 0;
    q->pops = 0;
    q->pushes = 0;
    q->max_elements = 0;

    return q;
}

void queue_cleanup(struct queue *q) {
    if (q != NULL) {
        if (q->data != NULL) {
            free(q->data);
        }
        free(q);
    }
}

void queue_stats(const struct queue *q) {
    if (q == NULL) {
        return;
    }
    fprintf(stderr, "stats %lu %lu %lu\n", q->pushes, q->pops, q->max_elements);
}

/*
 * Determine the next index of an existing data index,
 * this will wrap around to the start if the given index the last one in the data chunk.
 *
 * Input:
 * q: Pointer to the queue.
 * current_index: The index you want to increment.
 *
 * Returns:
 * The next index.
 */
static size_t next_index(const struct queue *q, const size_t current_index) {
    if (q->capacity == 0) {
        return 0;
    }

    const size_t next_index = (current_index + 1) % q->capacity;
    return next_index;
}

/*
 * Double the size of the queue, if no space is allocated, allocates 1 spot.
 *
 * Returns:
 * 0 if the resizing succeeded.
 * -1 otherwise.
 *
 * Side-Effect:
 * The memory of the queue gets bigger and re-allocated
 *
 * Note: malloc is used instead of realloc to prevent accidentally overwriting parts of the
 *       existing queue while reordering the resized one.
 *       We want the only empty spaces to be between the head and the tail to make
 *       The most efficient use of our memory.
 */
static int queue_resize(struct queue *q) {
    const size_t new_size = q->capacity < 10 ? 20 : q->capacity * 2;

    int *new_data = malloc(sizeof(int) * new_size);
    if (new_data == NULL) {
        return -1;
    }

    // If the head and the tail point to the same spot, the queue is empty.

    if (q->tail_index > q->head_index) {
        // Memory is in the normal order in the old queue, just copy everything in between.
        memcpy(new_data, &q->data[q->head_index], q->size * sizeof(int));
    }

    if (q->head_index > q->tail_index) {
        // Memory is in wraparound state and thus divided into 2 separate chunks.
        // We need to copy them 1 by 1.

        // The first part (right after the head)
        const size_t memory_part_1_size = q->capacity - q->head_index;
        memcpy(new_data, &q->data[q->head_index], memory_part_1_size * sizeof(int));

        // The second part (from the start to the tail)
        memcpy(&new_data[memory_part_1_size], q->data, q->tail_index * sizeof(int));
    }

    free(q->data);
    q->data = new_data;
    q->head_index = 0;       // Point to first item in the queue.
    q->tail_index = q->size; // Point to first empty spot after the queue.
    q->capacity = new_size;
    return 0;
}

int queue_push(struct queue *q, const int e) {
    if (q == NULL || q->data == NULL) {
        return 1;
    }

    size_t new_tail_index = next_index(q, q->tail_index);
    if (new_tail_index == q->head_index) {
        if (queue_resize(q) == -1) {
            return 1;
        }

        new_tail_index = next_index(q, q->tail_index);
    }

    q->data[q->tail_index] = e;

    q->tail_index = new_tail_index;

    q->pushes++;
    q->size++;
    if (q->size > q->max_elements) {
        q->max_elements++;
    }

    return 0;
}

int queue_pop(struct queue *q) {
    if (q == NULL || q->data == NULL) {
        return -1;
    }

    // If the queue is empty.
    if (q->head_index == q->tail_index) {
        return -1;
    }

    // Store first because of potential wraparound later.
    const int popped_value = q->data[q->head_index];

    q->head_index = next_index(q, q->head_index);
    q->pops++;
    q->size--;

    return popped_value;
}

int queue_peek(const struct queue *q) {
    if (q == NULL || q->data == NULL || q->head_index == q->tail_index) {
        return -1;
    }

    return q->data[q->head_index];
}

int queue_empty(const struct queue *q) {
    if (q == NULL || q->data == NULL) {
        return -1;
    }

    return q->head_index == q->tail_index;
}

size_t queue_size(const struct queue *q) {
    if (q == NULL || q->data == NULL) {
        return 0;
    }

    return q->size;
}
