/*convention algorithm implementation*/
#include <iostream>
#include <vector>
#include <climits> // for INT_MAX

using namespace std;
using Element = pair<int, int>; // (key, id) for stability test


/* 3. Bubble sort */
void bubble_sort(vector<Element>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].first > arr[j + 1].first) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) {
            break; // 이미 정렬된 경우
        }
    }
}