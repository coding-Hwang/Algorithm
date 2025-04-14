/*convention algorithm implementation*/
#include <iostream>
#include <vector>
#include <climits> // for INT_MAX

using namespace std;
using Element = pair<int, int>; // (key, id) for stability test


/* 2. Heap Sort */
void max_heapify(vector<Element>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].first > arr[largest].first)
        largest = left;

    if (right < n && arr[right].first > arr[largest].first)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        max_heapify(arr, n, largest);
    }
}

void build_max_heap(vector<Element>& arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) { // 앞 절반만을 대상으로 정렬돌림
        max_heapify(arr, n, i);
    }
}

void heap_sort(vector<Element>& arr) {
    int n = arr.size();
    build_max_heap(arr, n);
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        // A.heapSize = A.heapSize - 1 -> i = n-1
        max_heapify(arr, i, 0);
    }
}