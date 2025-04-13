// contemporary algorithm
#include <iostream>
#include <cmath>
#include <vector>
#include <climits> // for INT_MAX
#include <unordered_map> // for TournamentSort2
#include "convention_algorithm.h"

using namespace std;
using Element = pair<int, int>; // (key, id) for stability test

/* 1. Library Sort */
// 짝수 = gap
void rebalance(vector<Element>& S, int size) {
    vector<Element> newS(S.size(), {-1,-1}); // S.size = 2*n
    int j = 1; // 홀수 인덱스에만 원소 삽입
    int m = 0;
    for (size_t i = 0; i < S.size(); ++i) {
        if (S[i].first != -1) {
            newS[j] = S[i];
            j += 2; // gap을 한 칸씩 확보
            m++;
        }
        if (m == size) break; // m = S의 원소 개수
    }
    S = newS;
}

//binary search (gap 무시 ver)
int BinarySearch(int key, const vector<Element>& S, int length) {
    int left = 0, right = length - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        // 값만 비교해야 하므로 gap은 skip
        int val = S[mid * 2 + 1].first;  // mid * 2 + 1 = 홀수 인덱스
        if (val == -1 || val > key)
            right = mid - 1;
        else
            left = mid + 1;
    }
    return left;
}

void LibrarySort(vector<Element>& arr, int& re_num) {
    int n = arr.size();
    int size = 4; // 초기 배열 크기 (원소 + gap 포함)
    vector<Element> S(size, {-1,-1});
    S[1] = arr[0];
    int m = 1;

    for (int i = 1; i < n; ++i) {
        // 크기 초과 시 확장
        // gap이 없을 때 확장하는 개념이 아닌, 크기마다 확장하는 개념을 사용함으로써 log n 보장장
        if (m * 2 >= (int)S.size()) { // # of m : 1, 2, 4, 8 ... 마다 rebal
            size *= 2;
            S.resize(size, {-1,-1});
            rebalance(S, m);
            re_num++;
        }

        int pos = BinarySearch(arr[i].first, S, m);
        int idx = pos * 2 + 1; // 홀수 인덱스에 정렬된 값

        // 충돌 시 shift
        if (S[idx].first != -1) {
            // right shift to make gap
            int j = idx;
            while (j < (int)S.size() && S[j].first != -1) j += 2;
            if (j >= (int)S.size()) {
                // 공간 부족 시 rebalance + 확장
                size *= 2;
                S.resize(size, {-1,-1});
                rebalance(S, m);
                re_num++;
                pos = BinarySearch(arr[i].first, S, m);
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
        if (S[i].first != -1) {
            arr.push_back(S[i]);
        }
    }
}



/* 2. Tim Sort */
//  Insertion sort + merge sort (+ run)
// void merge_sort(vector<Element>& arr, int left, int right);
// void insertion_sort(vector<Element>& arr, int left, int right);

const int RUN = 32; // 기본 32로 잡음 (성능 주요 요소)

void TimSort(vector<Element>& arr) {
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
void cocktail_shaker_sort(vector<Element>& arr){
    bool swapped = true;
    int start = 0;
    int end = arr.size() - 1;

    while (swapped){
        swapped = false;
        // 정방향 pass (큰거 뒤로로)
        for (int i = start; i < end; ++i){
            if (arr[i].first > arr[i+1].first){
                swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
        if (!swapped) break; // 이미 정렬된 경우

        swapped = false;
        --end;

        // 역방향 pass (작은거 앞으로)
        for (int i = end - 1; i >= start; --i){
            if (arr[i].first > arr[i+1].first){
                swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
        ++start;
    }
}



/* 4. Comb Sort */

void CombSort(vector<Element>& arr) {
    int gap = arr.size();
    bool swapped = true;
    const double shrink = 1.3;

    while (gap > 1 || swapped) {
        gap = max(1, int(floor(gap / shrink))); // gap = n/(1.3)^k (k = # of pass)
        swapped = false;

        for (size_t i = 0; i + gap < arr.size(); ++i) {
            if (arr[i].first > arr[i + gap].first) {
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
/////////////////////////////
// Use this implementation //
/////////////////////////////

struct TreeNode {
    Element val;
    int leafIndex; // 해당 값이 어느 리프에서 왔는지 추적
};

void buildTree(vector<TreeNode>& tree, const vector<Element>& data, int leafStart) {
    int n = data.size();
    for (int i = 0; i < n; ++i) {
        tree[leafStart + i] = {data[i], leafStart + i};
    } // index도 설정정

    for (int i = leafStart + n; i < (int)tree.size(); ++i) {
        tree[i] = {{INT_MAX, INT_MAX}, i};
    }

    for (int i = leafStart - 1; i >= 0; --i) {
        TreeNode left = tree[2 * i + 1];
        TreeNode right = tree[2 * i + 2];
        tree[i] = (left.val.first < right.val.first) ? left : right;
    }
}

void updateTree(vector<TreeNode>& tree, int index) {
    tree[index] = {{INT_MAX, INT_MAX}, index}; // 삭제

    while (index > 0) {
        int parent = (index - 1) / 2;
        int left = 2 * parent + 1;
        int right = 2 * parent + 2;
        TreeNode leftNode = tree[left];
        TreeNode rightNode = tree[right];
        tree[parent] = (leftNode.val.first < rightNode.val.first) ? leftNode : rightNode;
        index = parent;
    }
}

void TournamentSort(vector<Element>& data) {
    int n = data.size();
    int leafCount = pow(2, ceil(log2(n)));
    int treeSize = 2 * leafCount - 1;
    int leafStart = treeSize / 2;

    vector<TreeNode> tree(treeSize, {{INT_MAX, INT_MAX}, -1});
    vector<Element> result;

    buildTree(tree, data, leafStart);

    for (int i = 0; i < n; ++i) {
        TreeNode minNode = tree[0];
        result.push_back(minNode.val);
        updateTree(tree, minNode.leafIndex); // 바로 갱신
    }

    data = result;
}


/* 6. Introsort*/
//quick sort(평균적 빠른 성능) + heapsort (최악이어도 보장)

const int INSERTION_THRESHOLD = 16;

void introsort_util(vector<Element>& arr, int begin, int end, int depthLimit){
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

void Introsort(vector<Element>& arr){
    int depthLimit = log2(arr.size()) * 2; // 깊이 제한
    introsort_util(arr, 0, arr.size() - 1, depthLimit);
}



/* for test */

// void printData(const vector<Element>& data, const string& label) {
//     cout << label << ":\n";
//     for (const auto& e : data) {
//         cout << "(" << e.first << ", " << e.second << ") ";
//     }
//     cout << "\n\n";
// }

// int main() {
//     //테스트용 입력 데이터 (원하는 대로 바꿔도 됨)
//     vector<Element> data = {
//         {5, 1}, {3, 2}, {8, 3}, {1, 4}, {4, 5}
//     };

//     printData(data, "Before Sort");

//     TournamentSort(data); //너의 정렬 함수 호출

//     printData(data, "After Sort");

//     return 0;
// }