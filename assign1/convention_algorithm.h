#ifndef CONVENTION_ALGORITHM_H
#define CONVENTION_ALGORITHM_H

#include <vector>
#include <iostream>
#include <vector>
#include <climits> // for INT_MAX

using namespace std;
using Element = pair<int, int>; // (key, id) for stability test

int partition(vector<Element>& arr, int low, int high);
void merge_sort(vector<Element>& arr, int left, int right); 
void heap_sort(vector<Element>& arr);
void bubble_sort(vector<Element>& arr);
void insertion_sort(vector<Element>& arr, int left, int right);
void selection_sort(vector<Element>& arr);
void quick_sort(vector<Element>& arr, int low, int high);

#endif
