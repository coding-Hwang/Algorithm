#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <queue>
#include <functional>
#include <sys/resource.h>
#include <malloc.h>  // mallinfo2를 위해 추가


#include "convention_algorithm.h"
#include "contemporary_algorithm.h"

using namespace std;
using namespace chrono;

typedef pair<int, int> Element; // {key, id}

long get_memory_usage_kb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // in KB
}


long get_heap_usage_bytes() {
    struct mallinfo2 info = mallinfo2();
    return info.uordblks;  // 현재 사용 중인 힙 메모리 (Bytes)
}


// Sorting algorithm declarations

int partition(vector<Element>& arr, int low, int high);
void merge_sort(vector<Element>& arr, int left, int right); 
void heap_sort(vector<Element>& arr);
void bubble_sort(vector<Element>& arr);
void insertion_sort(vector<Element>& arr, int left, int right);
void selection_sort(vector<Element>& arr);
void quick_sort(vector<Element>& arr, int low, int high);

void LibrarySort(vector<Element>& arr, int& re_num);
void TimSort(vector<Element>& arr);
void cocktail_shaker_sort(vector<Element>& arr);
void CombSort(vector<Element>& arr);
void TournamentSort(vector<Element>& data);
void TournamentSort2(vector<Element>& data);
void Introsort(vector<Element>& arr);

// Data generation functions
vector<Element> generate_sorted(int size) {
    vector<Element> data(size);
    for (int i = 0; i < size; ++i)
        data[i] = {i, i};
    return data;
}

vector<Element> generate_reverse(int size) {
    vector<Element> data(size);
    for (int i = 0; i < size; ++i)
        data[i] = {size - i, i};
    return data;
}

vector<Element> generate_random(int size) {
    vector<Element> data(size);
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, size);
    for (int i = 0; i < size; ++i)
        data[i] = {dist(rng), i};
    return data;
}

vector<Element> generate_partial(int size) {
    vector<Element> data(size);
    for (int i = 0; i < size / 2; ++i)
        data[i] = {i, i};
    mt19937 rng(random_device{}());
    for (int i = size / 2; i < size; ++i)
        data[i] = {rng() % size, i};
    return data;
}

// Stability checker
bool is_stable(const vector<Element>& before, const vector<Element>& after) {
    unordered_map<int, vector<int>> bmap, amap;
    for (auto& [key, id] : before) bmap[key].push_back(id);
    for (auto& [key, id] : after)  amap[key].push_back(id);
    return bmap == amap;
}

// Sorting correctness checker
bool is_sorted(const vector<Element>& data) {
    for (size_t i = 1; i < data.size(); ++i)
        if (data[i - 1].first > data[i].first)
            return false;
    return true;
}

// Experiment runner
void run_experiment(const string& name, function<void(vector<Element>&)> sort_func, //void(*sort_func)(vector<int>&),
                    vector<Element> (*data_gen)(int),
                    const string& input_type, const vector<int>& sizes, ofstream& out) {
    for (int size : sizes) {
        cout << "▶ [" << name << "] " << input_type << ", size=" << size << " ... running..." << endl;

        double total_time = 0.0;
        vector<double> trial_times;
        vector<long> memory_usages;
        bool stable = true;

        for (int t = 0; t < 10; ++t) {
            vector<Element> data = data_gen(size);
            vector<Element> original = data;

            vector<Element> raw_data(size);
            raw_data=data;
            
            // auto start = high_resolution_clock::now();
            // sort_func(raw_data);
            // auto end = high_resolution_clock::now();

            long before = get_heap_usage_bytes();
            auto start = high_resolution_clock::now();
            sort_func(raw_data);
            auto end = high_resolution_clock::now();
            long after = get_heap_usage_bytes();
            long delta = after - before;

            if (!is_sorted(raw_data)) {
                cerr << "Error: Data is not sorted correctly!" << endl;
                return;
            }
            // long mem_after = get_heap_usage_bytes();
            memory_usages.push_back(delta);

            double elapsed = duration<double, milli>(end - start).count();
            trial_times.push_back(elapsed);
            cout << "  - Trial " << t + 1 << ": time = " << elapsed << " ms, memory = " << delta << " B" << endl;
            total_time += elapsed;

            stable &= is_stable(original, raw_data);
        }

        // 평균 메모리 사용량 계산
        long avg_mem = memory_usages[0] ; // 마지막 trial의 메모리 사용량을 평균으로 사용



        // 결과 출력
        out << name << "," << input_type << "," << size << "," << avg_mem << "," << total_time / 10;
        for (double t : trial_times)
            out << "," << t;
        out << "," << (stable ? "Yes" : "No") << endl;
    }
}




int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: ./experiment [algorithm_name] [input_type]" << endl;
        return 1;
    }

    string algo = argv[1]; // "merge_sort", "quick_sort", etc.
    string input_type = argv[2]; // "sorted", "random", etc.

    vector<int> sizes = {1000, 10000, 100000};
    string filename = algo + ".csv";

    // 헤더가 없는 경우에만 쓰기
    ifstream check(filename);
    bool file_exists = check.good();
    check.close();

    ofstream out(filename, ios::app);
    if (!file_exists) {
        out << "Algorithm,InputType,Size,Memory(B),Time(ms)";
        for (int i = 1; i <= 10; ++i)
            out << ",Trial" << i;
        out << ",Stable\n";
    }

    // sort function 선택
    auto sort_func = [&](vector<Element>& arr) {
        if (algo == "merge_sort") {
            merge_sort(arr, 0, arr.size() - 1);
        } else if (algo == "quick_sort") {
            quick_sort(arr, 0, arr.size() - 1);
        } else if (algo == "heap_sort") {
            heap_sort(arr);
        } else if (algo == "bubble_sort") {
            bubble_sort(arr);
        } else if (algo == "insertion_sort") {
            insertion_sort(arr, 0, arr.size() - 1);
        } else if (algo == "selection_sort") {
            selection_sort(arr);
        } 

        //library는 따로
        else if (algo == "library_sort") {
            int re_num = 0;
            LibrarySort(arr, re_num);
        }
        else if (algo == "tim_sort") {
            TimSort(arr);
        } else if (algo == "cocktail_shaker_sort") {
            cocktail_shaker_sort(arr);
        } else if (algo == "comb_sort") {
            CombSort(arr);
        } else if (algo == "tournament_sort") {
            TournamentSort(arr);
        } else if (algo == "introsort") {
            Introsort(arr);
        }     
        else {
            cerr << "Unknown algorithm: " << algo << endl;
            return;
        }
    };

    // input generator 선택
    vector<Element> (*data_gen)(int) = nullptr;
    if (input_type == "sorted") data_gen = generate_sorted;
    else if (input_type == "random") data_gen = generate_random;
    else if (input_type == "reverse") data_gen = generate_reverse;
    else if (input_type == "partial") data_gen = generate_partial;
    else {
        cerr << "Unknown input type: " << input_type << endl;
        return 1;
    }

    run_experiment(algo, sort_func, data_gen, input_type, sizes, out);
    out.close();
    return 0;
}
