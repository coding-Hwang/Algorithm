#!/bin/bash

# ALGORITHMS=("merge_sort" "heap_sort" "bubble_sort" "insertion_sort" "quick_sort" "selection_sort" "tim_sort" "cocktail_shaker_sort" "library_sort"
#   "comb_sort" "tournament_sort" "introsort") 

# ALGORITHMS=("tim_sort" "comb_sort" "tournament_sort" "introsort") 
ALGORITHMS=("library_sort" "cocktail_shaker_sort")

# INPUTS=("sorted" "random" "reverse" "partial")
INPUTS=("partial")


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
