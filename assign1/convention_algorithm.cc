/*convention algorithm implementation*/
#include <iostream>
#include <vector>
#include <climits> // for INT_MAX

using namespace std;

/*1. Merge sort (based on Pseudo code)*/
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> L(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> R(arr.begin() + mid + 1, arr.begin() + right + 1); // arr 복사해서 R, L 공간 추가 사용
    L.push_back(INT_MAX); // Sentinel
    R.push_back(INT_MAX); // Sentinel
    int i = 0, j = 0;
    for (int k = left; k <= right; ++k) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
    }
}

void merge_sort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}


/* 2. Heap Sort */
void max_heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        max_heapify(arr, n, largest);
    }
}

void build_max_heap(vector<int>& arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--) { // 앞 절반만을 대상으로 정렬돌림
        max_heapify(arr, n, i);
    }
}

void heap_sort(vector<int>& arr) {
    int n = arr.size();
    build_max_heap(arr, n);
    for (int i = n - 1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        // A.heapSize = A.heapSize - 1 -> i = n-1
        max_heapify(arr, i, 0);
    }
}

/* 3. Bubble sort */
void bubble_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
        n--;
    }
}

/* 4. Insertion Sort */
void insertion_sort(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/* 5. Selection Sort */
void selection_sort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        swap(arr[i], arr[min_idx]);
    }
}

/* 6. Quick Sort */
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quick_sort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quick_sort(arr, low, pi - 1);
        quick_sort(arr, pi + 1, high);
    }
}


// 전체 코드 훑고, 수도 코드 기반 알고리즘 정확히 이해
// testing 진행

// // for test
// int main(){
//     vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
//     int n = arr.size();
//     merge_sort(arr, 0, n - 1);
//     for (int i = 0; i < n; i++) {
//         cout << arr[i] << " ";
//     }
//     return 0;
// }