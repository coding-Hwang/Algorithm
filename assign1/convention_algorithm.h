#ifndef CONVENTION_ALGORITHM_H
#define CONVENTION_ALGORITHM_H

#include <vector>

void merge_sort(vector<int>& arr, int left, int right); 
void heap_sort(vector<int>& arr);
void bubble_sort(vector<int>& arr);
void insertion_sort(vector<int>& arr);
void selection_sort(vector<int>& arr);
void quick_sort(vector<int>& arr, int low, int high);

#endif
