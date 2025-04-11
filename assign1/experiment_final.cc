#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <queue>
#include <sys/resource.h>

#include "convention_algorithm.h"

using namespace std;
using namespace chrono;

typedef pair<int, int> Element; // {key, id}

long get_memory_usage_kb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // in KB
}


// Sorting algorithm declarations
void merge_sort(vector<int>& arr, int left, int right); 
void heap_sort(vector<int>& arr);
void bubble_sort(vector<int>& arr);
void insertion_sort(vector<int>& arr, int left, int right);
void selection_sort(vector<int>& arr);
void quick_sort(vector<int>& arr, int low, int high);

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
void run_experiment(const string& name, void(*sort_func)(vector<int>&),
                    vector<Element> (*data_gen)(int),
                    const string& input_type, const vector<int>& sizes, ofstream& out) {
    for (int size : sizes) {
        cout << "▶ [" << name << "] " << input_type << ", size=" << size << " ... running..." << endl;

        double total_time = 0.0;
        vector<double> trial_times;
        bool stable = true;

        for (int t = 0; t < 10; ++t) {
            vector<Element> data = data_gen(size);
            vector<Element> original = data;

            vector<int> raw_data(size);
            for (int i = 0; i < size; ++i)
                raw_data[i] = data[i].first;

            auto mem_before = get_memory_usage_kb();
auto start = high_resolution_clock::now();
            sort_func(raw_data);
            auto end = high_resolution_clock::now();
auto mem_after = get_memory_usage_kb();
long mem_used = mem_after - mem_before;
            double elapsed = duration<double, milli>(end - start).count();
            trial_times.push_back(elapsed);
            cout << "  - Trial " << t+1 << ": time = " << elapsed << " ms, memory = " << mem_used << " KB" << endl;
            total_time += elapsed;

            // Reconstruct sorted Element array
            unordered_map<int, queue<int>> key_to_ids;
            for (auto& [key, id] : original)
                key_to_ids[key].push(id);

            vector<Element> after(size);
            for (int i = 0; i < size; ++i) {
                int key = raw_data[i];
                int id = key_to_ids[key].front();
                key_to_ids[key].pop();
                after[i] = {key, id};
            }

            stable &= is_stable(original, after);
        }

        out << name << "," << input_type << "," << size << "," << total_time / 10;
        for (double t : trial_times)
            out << "," << t;
        out << "," << (stable ? "Yes" : "No") << endl;
    }
}


int main() {
    ofstream out("experiment_results.csv");
    out << "Algorithm,InputType,Size,Time(ms)";
    for (int i = 1; i <= 10; ++i)
        out << ",Trial" << i;
    out << ",Stable\n";

    vector<int> sizes = {1000, 10000, 100000};

    // run_experiment("HeapSort", [](vector<int>& a) {
    //     merge_sort(a, 0, a.size() - 1);
    // }, generate_sorted, "Sorted", sizes, out);

    run_experiment("quick_sort", [](vector<int>& a) {
        quick_sort(a, 0, a.size() - 1);
    }, generate_sorted, "Sorted", sizes, out);

    run_experiment("quick_sort", [](vector<int>& a) {
        quick_sort(a, 0, a.size() - 1);
    }, generate_random, "Random", sizes, out);

    run_experiment("quick_sort", [](vector<int>& a) {
        quick_sort(a, 0, a.size() - 1);
    }, generate_reverse, "Reverse", sizes, out);

    run_experiment("quick_sort", [](vector<int>& a) {
        quick_sort(a, 0, a.size() - 1);
    }, generate_partial, "Partial", sizes, out);

    out.close();
    return 0;
}

// void quick_sort(vector<int>& arr, int low, int high);