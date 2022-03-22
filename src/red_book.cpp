#include <iostream>
#include <vector>
#include <string>

int merge_sort(int left, int right, std::vector<int>& nums, std::vector<int>& arr) {
    if(left >= right) {
        return 0;
    }
    int mid = (left + right) / 2;
    int sum = merge_sort(left, mid, nums, arr) + merge_sort(mid + 1, right, nums, arr);
    for(int i = left; i <= right; ++i) {
        arr[i] = nums[i];
    }
    int ptr1 = left;
    int ptr2 = mid+1;
    for(int i = left; i <= right; ++i) {
        arr[i] = nums[i];
    }
    for(int i = left; i <= right; ++i) {
        if(ptr1 == mid + 1) {
            nums[i] = arr[ptr2++];
        } else if(ptr2 == right + 1 || arr[ptr1] <= arr[ptr2]) {
            nums[i] = arr[ptr1++];
        } else {
            nums[i] = arr[ptr2++];
            sum += mid - ptr1 + 1;
        }
    }
    return sum;
};


int main() {
    std::string str;
    std::vector<int> nums;
    std::cin>> str;
    for(auto& i : str) {
        if(i >= '0' && i <= '9') {
            nums.emplace_back(i - '0');
        }
    }
    std::vector<int> arr(nums.size(), 0);
    int val = merge_sort(0, nums.size() - 1, nums, arr);
    std::cout <<  val <<std::endl;
}
