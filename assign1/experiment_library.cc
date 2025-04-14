#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <random>
#include <queue>
#include <unordered_map>
#include <string>
#include <sys/resource.h>
#include <numeric>

#include "contemporary_algorithm.h" // LibrarySort 함수 선언

using namespace std;
using namespace chrono;

typedef pair<int, int> Element; // {key, id}

long get_memory_usage_kb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // in KB
}

// 데이터 생성기들
auto generate_sorted = [](int size) {
    vector<Element> data(size);
    for (int i = 0; i < size; ++i)
        data[i] = {i, i};
    return data;
};

auto generate_reverse = [](int size) {
    vector<Element> data(size);
    for (int i = 0; i < size; ++i)
        data[i] = {size - i, i};
    return data;
};

auto generate_random = [](int size) {
    vector<Element> data(size);
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, size);
    for (int i = 0; i < size; ++i)
        data[i] = {dist(rng), i};
    return data;
};

auto generate_partial = [](int size) {
    vector<Element> data(size);
    for (int i = 0; i < size / 2; ++i)
        data[i] = {i, i};
    mt19937 rng(random_device{}());
    for (int i = size / 2; i < size; ++i)
        data[i] = {rng() % size, i};
    return data;
};

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

void run_library_experiment(vector<Element> (*data_gen)(int),
                            const string& input_type,
                            const vector<int>& sizes,
                            ofstream& out) {
    for (int size : sizes) {
        vector<double> trial_times;
        int total_rebalance = 0;
        bool stable = true;
        long memory = 0;

        cout << "\u25B6 [LibrarySort] " << input_type << ", size = " << size << " ... running..." << endl;

        for (int t = 0; t < 10; ++t) {
            vector<Element> data = data_gen(size);
            vector<Element> original = data;
            vector<Element> raw_data(size);
            raw_data=data;

            int re_num = 0;
            auto start = high_resolution_clock::now();
            LibrarySort(raw_data, re_num);
            auto end = high_resolution_clock::now();

            if (!is_sorted(raw_data)) {
                cerr << "Error: Data is not sorted correctly!" << endl;
                return;
            }
            if (t == 9)
                memory = get_memory_usage_kb();

            total_rebalance += re_num;
            double elapsed = duration<double, milli>(end - start).count();
            trial_times.push_back(elapsed);

            stable &= is_stable(original, raw_data);

            cout << "  - Trial " << t + 1 << ": time = " << elapsed << " ms, rebalance = " << re_num << endl;
        }

        double avg_time = accumulate(trial_times.begin(), trial_times.end(), 0.0) / trial_times.size();
        int avg_rebalance = total_rebalance / 10;

        out << "LibrarySort," << input_type << "," << size << "," << memory << "," << avg_time;
        for (double t : trial_times)
            out << "," << t;
        out << "," << avg_rebalance << "," << (stable ? "Yes" : "No") << "\n";

        cout << "\u2705 Finished: " << input_type << ", size=" << size
             << " | time(avg)=" << avg_time << " ms"
             << ", rebalance(avg)=" << avg_rebalance
             << ", stable=" << (stable ? "Yes" : "No")
             << ", memory=" << memory << " B" << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./library_experiment [sorted|random|reverse|partial]" << endl;
        return 1;
    }

    string input_type = argv[1];
    vector<int> sizes = {1000, 10000, 100000};

    ofstream out("library_sort.csv", ios::app);
    ifstream check("library_sort.csv");
    bool file_exists = check.good();
    check.close();

    if (!file_exists) {
        out << "Algorithm,InputType,Size,Memory(B),Time(ms)";
        for (int i = 1; i <= 10; ++i)
            out << ",Trial" << i;
        out << ",rebalance,Stable\n";
    }

    if (input_type == "sorted")
        run_library_experiment(generate_sorted, "Sorted", sizes, out);
    else if (input_type == "random")
        run_library_experiment(generate_random, "Random", sizes, out);
    else if (input_type == "reverse")
        run_library_experiment(generate_reverse, "Reverse", sizes, out);
    else if (input_type == "partial")
        run_library_experiment(generate_partial, "Partial", sizes, out);
    else {
        cerr << "Unknown input type: " << input_type << endl;
        return 1;
    }

    out.close();
    return 0;
}
