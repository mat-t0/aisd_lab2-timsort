#pragma once
#include "timSort.h"
#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

int GetMinrun(int N)
{
	int r = 0;           
	while (N >= 64) {
		r |= N & 1;
		N >>= 1;
	}
	return N + r;
}


void reverseRun(int arr[], int start, int end) {
	while (start < end) {
		std::swap(arr[start], arr[end]);
		start++;
		end--;
	}
};


void insertionSort(int arr[], int start, int end) {
	for (int i = start+1; i <= end; i++)
		for (int j = i; j > start && arr[j - 1] > arr[j]; j--) 
			std::swap(arr[j - 1], arr[j]);        
};


int findRun(int arr[], int start, int end, int minrun) {
    
    if (start >= end) {
        return start + 1;
    }

    int runend = start;


    if (arr[start] > arr[start + 1]) {
        while (runend < end && arr[runend] > arr[runend + 1]) {
            runend++;
        }
        reverseRun(arr, start, runend);

    }
    else {
        while (runend < end && arr[runend] <= arr[runend + 1]) {
            runend++;
        }

    }

    int natsize = runend - start + 1;

    if (natsize < minrun) {

        int need = minrun - natsize;
        int avail = end - runend;

        int elementsToTake = (need < avail) ? need : avail;

        runend += elementsToTake;
    }
    return runend + 1;
};


int gallop(int arr[], int start, int length, int value, bool findGreater) {
    if (length <= 0) return 0;

    int left = 0;
    int right = length - 1;
    int result = length; 

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (findGreater) {
            
            if (arr[start + mid] >= value) {
                result = mid;
                right = mid - 1;
            }
            else {
                left = mid + 1;
            }
        }
        else {
            
            if (arr[start + mid] > value) {
                result = mid;
                right = mid - 1;
            }
            else {
                left = mid + 1;
            }
        }
    }

    return result;
}



void mergeTwoRuns(int arr[], Run left, Run right) {
   
    if (left.length <= 0 || right.length <= 0) return;

    std::vector<int> temp(right.length + left.length);
    int i = 0, j = 0, k = 0;

    const int GALLOP_THRESHOLD = 7;
    int leftGallopCount = 0;
    int rightGallopCount = 0;

    while (i < left.length && j < right.length) {
        
        if (arr[left.start + i] <= arr[right.start + j]) {
            temp[k++] = arr[left.start + i++];
            leftGallopCount++;
            rightGallopCount = 0;
        }
        else {
            temp[k++] = arr[right.start + j++];
            rightGallopCount++;
            leftGallopCount = 0;
        }

       
        if (leftGallopCount >= GALLOP_THRESHOLD && i < left.length && j < right.length) {
            
            int gallopCount = gallop(arr, left.start + i, left.length - i,
                arr[right.start + j], true);

            
            gallopCount = std::min(gallopCount, left.length - i);

           
            for (int g = 0; g < gallopCount; g++) {
                temp[k++] = arr[left.start + i++];
            }
            leftGallopCount = 0;
        }

       
        if (rightGallopCount >= GALLOP_THRESHOLD && i < left.length && j < right.length) {
          
            int gallopCount = gallop(arr, right.start + j, right.length - j,
                arr[left.start + i], false);

            
            gallopCount = std::min(gallopCount, right.length - j);

           
            for (int g = 0; g < gallopCount; g++) {
                temp[k++] = arr[right.start + j++];
            }
            rightGallopCount = 0;
        }
    }

   
    while (i < left.length) {
        temp[k++] = arr[left.start + i++];
    }

   
    while (j < right.length) {
        temp[k++] = arr[right.start + j++];
    }

   
    if (k != temp.size()) {
        std::cout << "ERROR: temp size mismatch! k=" << k << ", temp.size()=" << temp.size() << std::endl;
        return;
    }

    
    for (int idx = 0; idx < temp.size(); idx++) {
        arr[left.start + idx] = temp[idx];
    }
}



void mergeRuns(int arr[], std::stack<Run>& runs) {
    if (runs.size() < 2) return;

    Run right = runs.top();
    runs.pop();
    Run left = runs.top();
    runs.pop();

    mergeTwoRuns(arr, left, right);
    runs.push({ left.start, left.length + right.length });
}


void mergeRunsIfNeeded(int arr[], std::stack<Run>& runs) {
    while (runs.size() >= 3) {
        Run Z = runs.top(); runs.pop();
        Run Y = runs.top(); runs.pop();
        Run X = runs.top(); runs.pop();

        
        bool rule1_ok = (X.length > Y.length + Z.length);
        bool rule2_ok = (Y.length > Z.length);

        if (rule1_ok && rule2_ok) {
            runs.push(X);
            runs.push(Y);
            runs.push(Z);
            break;
        }

       
        if (X.length < Z.length) {
            mergeTwoRuns(arr, X, Y);
            runs.push({ X.start, X.length + Y.length });
            runs.push(Z);
        }
        else {
            mergeTwoRuns(arr, Y, Z);
            runs.push(X);
            runs.push({ Y.start, Y.length + Z.length });
        }
    }
}


void timSort(int arr[], int n) {
    if (n <= 1) return;
    if (!arr) return;

    std::stack<Run> runs;
    int minrun = GetMinrun(n);
    int start = 0;

    

    while (start < n) {
        int endrun = findRun(arr, start, n - 1, minrun);
        int runLen = endrun - start;
        insertionSort(arr, start, endrun - 1);
        runs.push({ start, runLen });
        mergeRunsIfNeeded(arr, runs);
        start = endrun;
    }

    while (runs.size() > 1) {
        mergeRuns(arr, runs);
    }
}
