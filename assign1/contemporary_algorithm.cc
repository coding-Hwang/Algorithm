// contemporary algorithm
#include <iostream>
#include <vector>

using namespace std;

/* 1. Library Sort */
void rebalance(vector<int>& arr, int begin, int end){
    int r = end;
    int w = end*2;
    while (r >= begin){
        arr[w+1] = -1; //gap
        arr[w] = arr[r];
        r--;
        w -= 2;
    }
}

void BinarySearch(){

}

void librarysort(vector<int>& arr){
    int n = arr.size();
    int maxSize = 2*n;
    vector<int> gap_arry(maxSize, -1);

    int index = 0;


}

// 이해 후 작성



/* 2. Tim Sort */
// 전체 배열 작은 덩어리들로 잘라 각각의 덩어리를 Insertion sort로 정렬





/* 3. Cocktail Shaker Sort */
void cocktail_shaker_sort(vector<int>& arr){
    bool swapped = true;
    int start = 0;
    int end = arr.size() - 1;

    while (swapped){
        swapped = false;
        for (int i = start; i < end; ++i){
            if (arr[i] > arr[i+1]){
                swap(arr[i], arr[i+1]);
                swapped = true;
            }
        }
        if (!swapped) break;

        swapped = false;
        --end;

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
        gap = max(1, int(gap / shrink));
        swapped = false;

        for (int i = 0; i + gap < arr.size(); ++i) {
            if (arr[i] > arr[i + gap]) {
                swap(arr[i], arr[i + gap]);
                swapped = true;
            }
        }
    }
}



/* 5. Tournament Sort */



/* 6. Introsort*/
// void IntroSort(){
//     if (n > 16){
//         heapsort(arr, n);
//     } else if (n > 1){
//         insertion_sort(arr, n);
//     }
// }



/* for test */
int main(){
    return 0;
}