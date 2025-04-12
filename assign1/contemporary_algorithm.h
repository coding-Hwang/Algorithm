#ifndef CONVENTION_ALGORITHM_H
#define CONVENTION_ALGORITHM_H
#include <iostream>
#include <cmath>
#include <vector>
#include <climits> // for INT_MAX
#include "convention_algorithm.h"

using namespace std;

void LibrarySort(vector<int>& arr, int& re_num);
void TimSort(vector<int>& arr);
void cocktail_shaker_sort(vector<int>& arr);
void CombSort(vector<int>& arr);
void TournamentSort(vector<int>& data);
void Introsort(vector<int>& arr);

#endif