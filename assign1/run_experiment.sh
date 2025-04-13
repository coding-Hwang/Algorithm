#!/bin/bash

# ALGORITHMS=("merge_sort" "heap_sort" "bubble_sort" "insertion_sort" "quick_sort" "selection_sort" "tim_sort" "cocktail_shaker_sort"
#   "comb_sort" "tournament_sort" "introsort") 

ALGORITHMS=("introsort")
INPUTS=("sorted" "random" "reverse" "partial")

for algo in "${ALGORITHMS[@]}"; do
    for input in "${INPUTS[@]}"; do
        echo "▶ Running $algo on $input input"
        ./experiment $algo $input
    done
done


# #for library sort
# ./library_experiment sorted
# ./library_experiment random
# ./library_experiment reverse
# ./library_experiment partial
