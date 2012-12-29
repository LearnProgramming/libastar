/*
 * This program reads in two text files which are expected to begin
 * with a line containing only numerical values, which are the
 * ordered values of the tiles in an n-puzzle,
 * where n = k^2 - 1 and k lies in [2, inf).
 *
 * The program then solves this puzzle using three different
 * methods and records the process to a file.
 */

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include "Puzzle.h"
#include "astar/solver.h"

using std::abs;
using std::cerr;
using std::endl;
using std::ofstream;

/*
 * These are the three examined heuristic functions.
 * displaced(...) counts how many tiles are out of place
 * manhattan(...) determines how far away each tile is from
 * its correct location, as specified in the solution.
 * sumdisman(...) simply sums displaced(...) and manhattan(...).
 */
int displaced(const Puzzle& state, const Puzzle& solution) {
    int result = -1;

    if(state.get_width() == solution.get_width()) {
        result = 0;

        for(TileIt it1 = state.tiles.begin(), it2 = solution.tiles.begin(); it1 != state.tiles.end(); it1++, it2++) {
            if(*it1 != 0 && *it1 != *it2) {
                result++;
            }
        }
    }

    return result;
}

int manhattan(const Puzzle& state, const Puzzle& solution) {
    int result = -1;

    /*
     * These loops compare two vectors piece by piece,
     * finding the row and column indeces for corresponding
     * pieces, and adding the magnitudes of the differences.
     *
     * This is presently O(n^2).
     * TODO: investigate O(n) solution
     */
    if(state.get_width() == solution.get_width()) {
        size_t width = state.get_width();
        int i = 0;
        for(TileIt it1 = state.tiles.begin(); it1 != state.tiles.end(); it1++, i++) {
            if(*it1 != 0) {

                int j = 0;
                for(TileIt it2 = solution.tiles.begin(); *it1 != *it2; it2++, j++);

                int r1 = i / width;
                int r2 = j / width;
                int c1 = i % width;
                int c2 = j % width;

                result += abs(r2 - r1) + abs(c2 - c1);
            }
        }
    }

    return result;
}

int sumdisman(const Puzzle& state, const Puzzle& solution) {
    return displaced(state, solution) + manhattan (state, solution);
}

int main (int argc, char **argv) {
    if(argc < 3) {
        cerr << "Usage: " << argv[0] << " start goal\n";
        exit(1);
    }

    try {
        Puzzle to_solve(argv[1]);
        Puzzle solution(argv[2]);

        auto generator = [](std::vector<Puzzle>& v, const Puzzle& p) { return p.get_neighbors(v); };
        auto distance = [](const Puzzle&, const Puzzle&) { return 1; };

        auto displaced_solver = make_solver(to_solve, solution, generator, distance, displaced);
        auto manhattan_solver = make_solver(to_solve, solution, generator, distance, manhattan);
        auto sumdisman_solver = make_solver(to_solve, solution, generator, distance, sumdisman);

        /*
         * Make it so!
         */
        displaced_solver.solve();
        {
        ofstream displaced_out("displaced.txt");
        displaced_solver.print_solution(displaced_out);
        }

        manhattan_solver.solve();
        {
        ofstream manhattan_out("manhattan.txt");
        manhattan_solver.print_solution(manhattan_out);
        }

        sumdisman_solver.solve();
        {
        ofstream sumdisman_out("displaced-manhattan.txt");
        sumdisman_solver.print_solution(sumdisman_out);
        }
    } catch (const char* str) {
        cerr << str << endl;
    }

    return 0;
}
