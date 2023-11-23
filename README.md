# Suduko Solver

* 0_naive.cpp is a basic backtracking solver
* 1_min_branch.cpp is backtracking, with choosing the cell that leads to the
minimum branch width at each step.
* 2_dlx.cpp is a Dancing Links implementation

2_dlx_data.cpp is meant to be run on data/puzzles5_forum_hardest_1905_11+.
Achieves, 25.5s runtime on 48766 puzzles, 1912 puzzles/sec, compare to
24,001.7 puzzles/sec by [tdouku](https://github.com/t-dillon/tdoku).
