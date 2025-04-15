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
#include <climits> // for INT_MAX



using namespace std;
using namespace chrono;

typedef pair<int, int> Element; // {key, id}



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

long get_memory_usage_kb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // in KB
}


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
void run_experiment(const string& name, vector<Element> (*data_gen)(int),
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

            auto start = high_resolution_clock::now();
            heap_sort(raw_data);
            auto end = high_resolution_clock::now();
            if (!is_sorted(raw_data)) {
                cerr << "Error: Data is not sorted correctly!" << endl;
                return;
            }
            long mem_after = get_memory_usage_kb();
            memory_usages.push_back(mem_after);

            double elapsed = duration<double, milli>(end - start).count();
            trial_times.push_back(elapsed);
            cout << "  - Trial " << t + 1 << ": time = " << elapsed << " ms, memory = " << mem_after << " KB" << endl;
            total_time += elapsed;

            stable &= is_stable(original, raw_data);
        }

        // 평균 메모리 사용량 계산
        long avg_mem = memory_usages[9] ; // 마지막 trial의 메모리 사용량을 평균으로 사용



        // 결과 출력
        out << name << "," << input_type << "," << size << "," << avg_mem << "," << total_time / 10;
        for (double t : trial_times)
            out << "," << t;
        out << "," << (stable ? "Yes" : "No") << endl;
    }
}




int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: ./experiment [input_type]" << endl;
        return 1;
    }

    string algo = "heap"; // "merge_sort", "quick_sort", etc.
    string input_type = argv[1]; // "sorted", "random", etc.

    vector<int> sizes = {1000, 10000, 100000};
    string filename = algo + ".csv";

    // 헤더가 없는 경우에만 쓰기
    ifstream check(filename);
    bool file_exists = check.good();
    check.close();

    ofstream out(filename, ios::app);
    if (!file_exists) {
        out << "Algorithm,InputType,Size,Memory(KB),Time(ms)";
        for (int i = 1; i <= 10; ++i)
            out << ",Trial" << i;
        out << ",Stable\n";
    }

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

    run_experiment(algo, data_gen, input_type, sizes, out);
    out.close();
    return 0;
}
