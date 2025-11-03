/*
 * Created by snapshot112 on 03/11/2025
 *
 * Naam: Jeroen Boxhoorn
 * UvAnetID: 16333969
 * Studie: BSC Informatica
 *
 * A depth first search algorithm for mazes.
 *
 * Crossroads are stored in a stack and used to store all the paths that need solving.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "maze.h"
#include "stack.h"

#define NOT_FOUND (-1)
#define ERROR (-2)

/*
 * Recursively steps through the maze and handles all the heavy lifting needed to solve the maze.
 *
 * Input:
 * m: pointer to the maze.
 * memory: The memory used to store possible paths while solving.
 *
 * Returns:
 * In case of an error, ERROR
 * In case no exit is found: NOT_FOUND
 * In case an exit is found: 1
 *
 * Side Effects:
 * Writes characters (0, 1, 2, 3, x) to explored parts of the maze.
 */
static int dfs_move(struct maze *m, struct stack *memory) {
    // If stack is empty return not found.
    const int stack_status = stack_empty(memory);
    if (stack_status == -1) {
        return ERROR;       // stack somehow returned an error
    }
    if (stack_status == 1) {
        return NOT_FOUND;   // Stack is empty.
    }
    // stack is not empty, continue

    // Determine current location.
    const int index = stack_pop(memory);

    const int row = maze_row(m, index);
    const int col = maze_col(m, index);

    // If on finish, return victory
    if (maze_at_destination(m, row, col)) {
        return 1;
    }

    // Define the spots we can move to.
    // I don't know the actual maze coordinate system but this covers all of them.
    const int locations[4][2] = {
        {row + 1, col}, // top??
        {row, col + 1}, // right??
        {row - 1, col}, // bottom??
        {row, col - 1}  // left??
    };

    // Store new possible paths to memory.
    for (int i = 0; i < 4; i++) {
        const char c = maze_get(m, locations[i][0], locations[i][1]);

        if (c == FLOOR) {
            // Valid path option, store the coordinates.
            const int new_location = maze_index(m, locations[i][0], locations[i][1]);
            stack_push(memory, new_location);

            // Place a character indicating in which direction we moved to get here.
            maze_set(m, locations[i][0], locations[i][1], (char)('0' + i));
        }
    }
}

/* Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occurred.
 */
int dfs_solve(struct maze *m) {
    // Create memory as either stack or queue
    const int memory_estimate = maze_size(m) * 2;
    struct stack *memory = stack_init(memory_estimate);

    // Find the start of the maze and add it to memory.
    int starting_row = 0;
    int starting_col = 0;

    maze_start(m, &starting_row, &starting_col);

    const int start_location = maze_index(m, starting_row, starting_col);

    stack_push(memory, start_location);

    // Recursively step through the maze and solve.
    int solving_status = dfs_move(m, memory);

    stack_cleanup(memory);

    if (stack_status == -1) {
        return ERROR;       // stack somehow returned an error
    }
    if (stack_status == 1) {
        return NOT_FOUND;   // Stack is empty.
    }

}

int main(void) {
    /* read maze */
    struct maze *m = maze_read();
    if (!m) {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    int path_length = dfs_solve(m);
    if (path_length == ERROR) {
        printf("dfs failed\n");
        maze_cleanup(m);
        return 1;
    } else if (path_length == NOT_FOUND) {
        printf("no path found from start to destination\n");
        maze_cleanup(m);
        return 1;
    }
    printf("dfs found a path of length: %d\n", path_length);

    /* print maze */
    maze_print(m, false);
    maze_output_ppm(m, "out.ppm");

    maze_cleanup(m);
    return 0;
}
