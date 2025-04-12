#!/bin/bash

#ALGORITHMS=("merge_sort" "quick_sort" "heap_sort" "bubble_sort" "insertion_sort" "selection_sort")
# ALGORITHMS=("tim_sort" "cocktail_shaker_sort"
#   "comb_sort" "tournament_sort" "introsort")
# ALGORITHMS=("merge_sort")
# INPUTS=("sorted" "random" "reverse" "partial")

# for algo in "${ALGORITHMS[@]}"; do
#     for input in "${INPUTS[@]}"; do
#         echo "▶ Running $algo on $input input"
#         ./experiment $algo $input
#     done
# done


./library_experiment sorted
./library_experiment random
./library_experiment reverse
./library_experiment partial
