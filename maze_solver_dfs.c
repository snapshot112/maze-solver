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
 * In case an exit is found: Maze index of last position before exit.
 *
 * Side Effects:
 * Writes characters (0, 1, 2, 3) to explored parts of the maze.
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

    // Define the spots we can move to.
    // I don't know the actual maze coordinate system but this covers all of them.
    // Note that these directions are the inverse of what is used in dfs_determine_final_path
    const int locations[4][2] = {
        {row + 1, col}, // up??
        {row, col + 1}, // right??
        {row - 1, col}, // down??
        {row, col - 1}  // left??
    };

    // Store new possible paths to memory.
    for (int i = 0; i < 4; i++) {
        // If on finish, return victory
        if (maze_at_destination(m, locations[i][0], locations[i][1])) {
            return maze_index(m, row, col);
        }

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

/*
 * Takes a marked maze and prints the found path to the goal.
 *
 * Input:
 * m: A maze with a path already determined.
 *
 * Output:
 * The length of the path to the goal.
 *
 * Side effects:
 * Writes the characters 'x' on every cell from the start to the destination of the maze.
 */
int dfs_determine_final_path(struct maze *m, int last_move_position) {
    int current_row = maze_row(m, last_move_position);
    int current_col = maze_col(m, last_move_position);

    // Edge cases (start next to exit).
    if (maze_at_start(m, current_row, current_col)) {
        return 1;
    }

    // We calculate the path length from the finish to the start.
    // Path length is the amount of steps taken to stand on the start position.

    // Since the last_move_position starts at 1 from the exit and the loop goes to 1 before start.
    int path_length = 2;

    while (!maze_at_destination(m, current_row, current_col)) {
        const char direction = maze_get(m, current_row, current_col);
        maze_set(m, current_row, current_col, PATH);
        // Note that these directions are the inverse of what is used in dfs_move
        switch (direction) {
            case '0': // down
                current_row--;
                break;
            case '1': // left
                current_col--;
                break;
            case '2': // up
                current_row++;
                break;
            case '3': // right
                current_col++;
            default:
                return ERROR;
        }
        path_length++;
    }

    return path_length;
}

/*
 * Solves the maze m.
 * Returns the length of the path if a path is found.
 * Returns NOT_FOUND if no path is found and ERROR if an error occurred.
 */
int dfs_solve(struct maze *m) {
    // Create memory as either stack or queue
    const int memory_estimate = maze_size(m);
    struct stack *memory = stack_init(memory_estimate);

    // Find the start of the maze and add it to memory.
    int starting_row = 0;
    int starting_col = 0;

    maze_start(m, &starting_row, &starting_col);

    const int start_location = maze_index(m, starting_row, starting_col);

    stack_push(memory, start_location);

    // Recursively step through the maze and solve.
    const int solving_status = dfs_move(m, memory);

    // Clean up the stack again.
    stack_cleanup(memory);

    if (solving_status == ERROR || solving_status == NOT_FOUND) {
        return solving_status;
    }

    return dfs_determine_final_path(m, solving_status);
}

int main(void) {
    /* read maze */
    struct maze *m = maze_read();
    if (!m) {
        printf("Error reading maze\n");
        return 1;
    }

    /* solve maze */
    const int path_length = dfs_solve(m);
    if (path_length == ERROR) {
        printf("dfs failed\n");
        maze_cleanup(m);
        return 1;
    }
    if (path_length == NOT_FOUND) {
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
