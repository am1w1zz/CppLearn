#include <iostream>
#include <vector>
#include <array>
#include <algorithm>


struct Range {
    int start, end;
    Range(int s = 0, int e = 0): start(s), end(e) {}
};

void quick_sort(std::vector<int>& arr, int len) {
    if (len <= 0) {
        return;
    }
    Range r[len];
    int p = 0;
    r[p++] = Range(0, len - 1);
    while (p) {
        Range range = r[--p];
        if (range.start >= range.end) {
            continue;
        }
        int mid = arr[range.end];
        int left = range.start, right = range.end - 1;
        while (left < right) {
            while (arr[left] < mid && left < right) left++;
            while (arr[right] >= mid && left < right) right--;
            std::swap(arr[left], arr[right]);
        }
        if (arr[left] >= arr[range.end]) {
            std::swap(arr[left], arr[range.end]);
        } else {
            left++;
        }
        r[p++] = Range(range.start, left - 1);
        r[p++] = Range(left + 1, range.end);
    }
}

int main() {
    
}