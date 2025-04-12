#ifndef CONVENTION_ALGORITHM_H
#define CONVENTION_ALGORITHM_H
#include <iostream>
#include <cmath>
#include <vector>
#include <climits> // for INT_MAX
#include "convention_algorithm.h"

using namespace std;
using Element = pair<int, int>; // (key, id) for stability test

void LibrarySort(vector<Element>& arr, int& re_num);
void TimSort(vector<Element>& arr);
void cocktail_shaker_sort(vector<Element>& arr);
void CombSort(vector<Element>& arr);
void TournamentSort(vector<Element>& data);
void Introsort(vector<Element>& arr);

#endif