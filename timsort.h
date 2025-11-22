#pragma once
#include <stack>

struct Run {
    int start;
    int length;
};

int GetMinrun(int N);

// Реверс подмассива
void reverseRun(int arr[], int start, int end);

// Сортировка вставками для подмассива
void insertionSort(int arr[], int start, int end);

// Поиск естественного run
int findRun(int arr[], int start, int end, int minrun);

int gallop(int arr[], int start, int length, int value, bool findGreater);

void mergeTwoRuns(int arr[], Run left, Run right);

void mergeRuns(int arr[], std::stack<Run>& runs);

void mergeRunsIfNeeded(int arr[], std::stack<Run>& runs);

// Основная функция Timsort
void timSort(int arr[], int n);
