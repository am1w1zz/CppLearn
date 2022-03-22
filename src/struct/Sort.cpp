#include <iostream>
#include <array>
#include <vector>

void selecttion_sort(std::vector<int>& arr) {
    for(int i = 0; i < arr.size() - 1; ++i) {
        int index = i;
        for(int j  = i + 1; j < arr.size(); ++j) {
            if(arr[j] < arr[index]) {
                index = j;
            }
        }
        std::swap(arr[i], arr[index]);
    }
}

void bubble_sort(std::vector<int>& arr) {
    for(int i = 0; i < arr.size() - 1; ++i) {
        for(int j  = i + 1; j < arr.size(); ++j) {
            if(arr[i] > arr[j]) {
                std::swap(arr[i], arr[j]);
            }
        }
    }
}

void insert_sort(std::vector<int>& arr) {
    for(int i = 0; i < arr.size(); i++) {
            int temp = arr[i];
            int j = i-1;
            while(j >= 0 && arr[j] >temp) {
                arr[j+1] = arr[j];
                j--;
            }
            arr[j+1] = temp;
    }
}

void heapfy(std::vector<int>& vec, int beg, int end) {
    int parent = beg;
    int child = parent * 2 + 1;
    while(child <= end) {
        if(child + 1 <= end && vec[child] < vec[child + 1]) {
            child += 1;
        }
        if(vec[parent] > vec[child]) {
            return ;
        } else {
            std::swap(vec[child], vec[parent]);
            parent = child;
            child = parent * 2 + 1;
        }
    }
}

void heap_srot(std::vector<int>& arr) {
    for(int i = (arr.size() - 2) / 2; i >= 0; --i ) {
        heapfy(arr, i , arr.size() - 1);
    }
    for(int i  = arr.size() - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapfy(arr, 0, i - 1);
    }
}

void merge_sort(std::vector<int>& arr, std::vector<int>& sup_arr, int left, int right) {
    if(left >= right) {
        return;
    }
    int mid = left + ((right - left) >> 1) ;
    merge_sort(arr, sup_arr, left, mid);
    merge_sort(arr, sup_arr, mid + 1, right);
    int ptr1 = left;
    int ptr2 = mid + 1;
    for(int  i = left; i <= right; ++i) {
        if(ptr1 == mid + 1 || ptr2 <= right && arr[ptr1] > arr[ptr2]) {
            sup_arr[i] = arr[ptr2++];
        } else {
            sup_arr[i] = arr[ptr1++];
        }
    }
    for(int i = left; i <= right; ++i) {
        arr[i] = sup_arr[i];
    }
}

void counting_sort(std::vector<int>& arr) {
    std::vector<int> count(100010, 0);
    std::vector<int> res(arr.size(), 0);
    for(auto& i : arr) {
        ++count[i];
    }
    for(int i = 1; i< 100010; ++i) {
        count[i] += count[i - 1];
    }
    for(int i = arr.size() - 1; i >= 0; --i) {
        res[--count[arr[i]]] = arr[i];
    }
    for(auto& i : res) {
        std::cout<<i<<" ";
    }
}

int partition(std::vector<int>& nums,int low,int high){
    int compare_base = nums[low];
    while(low < high) {
        while(low < high && nums[high] >= compare_base){
            --high;
        }
        nums[low] = nums[high];
        while(low < high && nums[low] <= compare_base) {
            ++low;
        }
        nums[high] = nums[low];
    }
    nums[low] = compare_base;
    return low;
}

void quick_sort(std::vector<int>& nums,int low,int high){
    if(low<high) {
        int mid_position = partition(nums,low,high);
        quick_sort(nums,low,mid_position-1);
        quick_sort(nums,mid_position+1,high);
    }
}

auto main() -> int {
    std::vector<int> arr{9,8,7,6,5,4,3,2,1,0};
    std::vector<int> sup_arr(arr.size(), 0);
    quick_sort(arr, 0, arr.size() - 1);
    for(auto& i : arr) {
        std::cout<<i<<" ";
    }
    return 0;
}