/*convention algorithm implementation*/
#include <iostream>
#include <vector>
#include <climits> // for INT_MAX

using namespace std;
using Element = pair<int, int>; // (key, id) for stability test


/* 4. Insertion Sort */
void insertion_sort(vector<Element>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        Element key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j].first > key.first) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}
