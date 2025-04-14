/*convention algorithm implementation*/
#include <iostream>
#include <vector>
#include <climits> // for INT_MAX

using namespace std;
using Element = pair<int, int>; // (key, id) for stability test

void merge(vector<Element>& arr, int left, int mid, int right) {
    vector<Element> L(arr.begin() + left, arr.begin() + mid + 1);
    vector<Element> R(arr.begin() + mid + 1, arr.begin() + right + 1); // arr 복사해서 R, L 공간 추가 사용
    L.push_back({INT_MAX, INT_MAX}); // Sentinel
    R.push_back({INT_MAX, INT_MAX}); // Sentinel
    int i = 0, j = 0;
    for (int k = left; k <= right; ++k) {
        if (L[i].first <= R[j].first) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
    }
}

void merge_sort(vector<Element>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}