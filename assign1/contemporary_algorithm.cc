// contemporary algorithm
#include <iostream>
#include <cmath>
#include <vector>
#include <climits> // for INT_MAX
#include "convention_algorithm.h"

using namespace std;

/* 1. Library Sort */
// 짝수 = gap
void rebalance(vector<int>& S, int size) {
    vector<int> newS(S.size(), -1); // S.size = 2*n
    int j = 1; // 홀수 인덱스에만 원소 삽입
    int m = 0;
    for (size_t i = 0; i < S.size(); ++i) {
        if (S[i] != -1) {
            newS[j] = S[i];
            j += 2; // gap을 한 칸씩 확보
            m++;
        }
        if (m == size) break; // m = S의 원소 개수
    }
    S = newS;
}

//binary search (gap 무시 ver)
int BinarySearch(int key, const vector<int>& S, int length) {
    int left = 0, right = length - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        // 값만 비교해야 하므로 gap은 skip
        int val = S[mid * 2 + 1];  // mid * 2 + 1 = 홀수 인덱스
        if (val == -1 || val > key)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return left;
}

void LibrarySort(vector<int>& arr, int& re_num) {
    int n = arr.size();
    int size = 4; // 초기 배열 크기 (원소 + gap 포함)
    vector<int> S(size, -1);
    S[1] = arr[0];
    int m = 1;

    for (int i = 1; i < n; ++i) {
        // 크기 초과 시 확장
        // gap이 없을 때 확장하는 개념이 아닌, 크기마다 확장하는 개념을 사용함으로써 log n 보장장
        if (m * 2 >= (int)S.size()) { // # of m : 1, 2, 4, 8 ... 마다 rebal
            size *= 2;
            S.resize(size, -1);
            rebalance(S, m);
            re_num++;
        }

        int pos = BinarySearch(arr[i], S, m);
        int idx = pos * 2 + 1; // 홀수 인덱스에 정렬된 값

        // 충돌 시 shift
        if (S[idx] != -1) {
            // right shift to make gap
            int j = idx;
            while (j < (int)S.size() && S[j] != -1) j += 2;
            if (j >= (int)S.size()) {
                // 공간 부족 시 rebalance + 확장
                size *= 2;
                S.resize(size, -1);
                rebalance(S, m);
                re_num++;
                pos = BinarySearch(arr[i], S, m);
                idx = pos * 2 + 1;
            } else {
                for (int k = j; k > idx; k -= 2) {
                    S[k] = S[k - 2];
                }
            }
        }

        S[idx] = arr[i];
        m++;
    }

    arr.clear();
    for (size_t i = 0; i < S.size(); ++i) {
        if (S[i] != -1) {
            arr.push_back(S[i]);
        }
    }
}



/* 2. Tim Sort */
//  Insertion sort + merge sort (+ run)
// void merge_sort(vector<int>& arr, int left, int right);
// void insertion_sort(vector<int>& arr, int left, int right);

const int RUN = 32; // 기본 32로 잡음 (성능 주요 요소)

void TimSort(vector<int>& arr) {
    int n = arr.size();

    // 1. 각 run -> insertion sort
    for (int i = 0; i<n; i+=RUN){
        insertion_sort(arr, i, min(i + RUN - 1, n - 1));
    }

    // 2. merge sort
    for (int size = RUN; size < n; size *= 2) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = min(left + size - 1, n - 1);
            int right = min((left + 2 * size - 1), (n - 1));
            if (mid < right) {
                merge_sort(arr, left, right);
            }
        }
    }
}



/* 3. Cocktail Shaker Sort */
void cocktail_shaker_sort(vector<int>& arr){
    bool swapped = true;
    int start = 0;
    int end = arr.size() - 1;

    while (swapped){
        swapped = false;
        // 정방향 pass (큰거 뒤로로)
        for (int i = start; i < end; ++i){
            if (arr[i] > arr[i+1]){
                swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
        if (!swapped) break; // 이미 정렬된 경우

        swapped = false;
        --end;

        // 역방향 pass (작은거 앞으로)
        for (int i = end - 1; i >= start; --i){
            if (arr[i] > arr[i+1]){
                swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
        ++start;
    }
}



/* 4. Comb Sort */

void CombSort(vector<int>& arr) {
    int gap = arr.size();
    bool swapped = true;
    const double shrink = 1.3;

    while (gap > 1 || swapped) {
        gap = max(1, int(floor(gap / shrink))); // gap = n/(1.3)^k (k = # of pass)
        swapped = false;

        for (size_t i = 0; i + gap < arr.size(); ++i) {
            if (arr[i] > arr[i + gap]) {
                swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}



/* 5. Tournament Sort */
// input -> complete binary tree 변환
// min heap
// 최솟값을 차례로 추출하며 정렬
// O(n log n)

// heap sort와의 차이 : heap은 전체 재배열 / tournament는 부분 재배열

//일단 tree구현
void buildTree(vector<int>& tree, const vector<int>& data, int leafStart){
    int n = data.size();
    for (int i = 0; i < n; ++i){
        tree[leafStart + i] = data[i];
    }

    for (size_t i = leafStart + n; i < tree.size(); ++i){
        tree[i] = INT_MAX; // leaf가 아닌 노드는 INT_MAX로 초기화
    }

    for (int i = leafStart - 1; i >= 0; --i){
        tree[i] = min(tree[2*i + 1], tree[2*i + 2]); // 부모 노드에 자식 노드의 최소값 저장
    }
}

void updateTree(vector<int>& tree, int index){
    tree[index] = INT_MAX; // 해당 노드 삭제

    while (index > 0){
        int parent = (index - 1) / 2;
        int left = 2 * parent + 1;
        int right = 2 * parent + 2;
        tree[parent] = min(tree[left], tree[right]); // 부모 노드에 자식 노드의 최소값 저장
        index = parent;
    }
}

void TournamentSort(vector<int>& data){
    int n = data.size();
    int leafCount = pow(2, ceil(log2(n))); // leaf 노드 개수 (n = 리프 개수)
    int treeSize = 2 * leafCount - 1; // 전체 노드 개수 -> 공간을 좀 많이 차지
    int leafStart = treeSize/2;

    vector<int> tree(treeSize, INT_MAX); // 트리 초기화
    vector<int> result;

    buildTree(tree, data, leafStart); // 트리 생성

    for (int i = 0; i < n; ++i){
        int minVal = tree[0];
        result.push_back(minVal); // 최솟값 계속 넣기 -> n번번

        for (int j = 0; j < leafCount; ++j){ 
            if (tree[leafStart + j] == minVal){
                updateTree(tree, leafStart + j); // O(log n), 해당 리프 노드 삭제
                break;
            } // leaf에서 해당 값 찾아 INF로 교체
        }
    }
    data = result; // 정렬된 결과를 원래 배열에 저장    
}

//updateTree에서의 for문 -> 삭제 요소의 인덱스를 바로 알 수 없어서 시간 늘어남 (map 이용해서 공간 복잡도와 trade off 하는 방식 존재) 



/* 6. Introsort*/
//quick sort(평균적 빠른 성능) + heapsort (최악이어도 보장)

const int INSERTION_THRESHOLD = 16;

void introsort_util(vector<int>& arr, int begin, int end, int depthLimit){
    int size = end - begin +1;
    
    if(size < INSERTION_THRESHOLD){
        insertion_sort(arr, begin, end);
        return;
    }

    if (depthLimit == 0){
        heap_sort(arr);
        return;
    }

    int pivot = partition(arr, begin, end);
    introsort_util(arr, begin, pivot - 1, depthLimit - 1);
    introsort_util(arr, pivot + 1, end, depthLimit - 1);
}

void Introsort(vector<int>& arr){
    int depthLimit = log2(arr.size()) * 2; // 깊이 제한
    introsort_util(arr, 0, arr.size() - 1, depthLimit);
}



/* for test */

// int main() {
//     vector<int> arr = {1, 6, 74, 3, 46, 252, 416, 7, 198, 329};

//     cout << "Original: ";
//     for (int n : arr) cout << n << " ";
//     cout << "\n";

//     int renum = 0;
//     LibrarySort(arr, renum); // 문제 있음
//     // TimSort(arr);
//     // cocktail_shaker_sort(arr);
//     // CombSort(arr);
//     // TournamentSort(arr);
//     // Introsort(arr);


//     cout << "Sorted:   ";
//     for (int n : arr) cout << n << " ";
//     cout << "\n";

//     cout << "Rebalance count: " << renum << endl;
//     cout << "Array size: " << arr.size() << endl;

//     return 0;
// }