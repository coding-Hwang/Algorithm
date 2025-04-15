/*convention algorithm implementation*/
#include <iostream>
#include <vector>
#include <climits> // for INT_MAX

using namespace std;
using Element = pair<int, int>; // (key, id) for stability test


/* 5. Selection Sort */
void selection_sort(vector<Element>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j].first < arr[min_idx].first) {
                min_idx = j;
            }
        }
        swap(arr[i], arr[min_idx]);
    }
}
