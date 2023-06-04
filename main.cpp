#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

const int MAX_THREADS = 4; // Maximum number of threads

struct ThreadArgs {
    std::vector<int>* arr;
    int start;
    int end;
};

void quicksort(std::vector<int>& arr, int low, int high) {
    if (low >= high) {
        return;
    }

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);

    int partitionIndex = i + 1;

    quicksort(arr, low, partitionIndex - 1);
    quicksort(arr, partitionIndex + 1, high);
}

void hyperQuickSort(std::vector<int>& arr, int low, int high, int numThreads) {
    std::stack<ThreadArgs> stack;
    stack.push({ &arr, low, high });

    while (!stack.empty()) {
        ThreadArgs current = stack.top();
        stack.pop();

        int start = current.start;
        int end = current.end;

        if (start >= end) {
            continue;
        }

        if (numThreads <= 1 || end - start < 2) {
            quicksort(*current.arr, start, end);
            continue;
        }

        int pivot = (*current.arr)[end];
        int i = start - 1;

        for (int j = start; j <= end - 1; j++) {
            if ((*current.arr)[j] < pivot) {
                i++;
                std::swap((*current.arr)[i], (*current.arr)[j]);
            }
        }

        std::swap((*current.arr)[i + 1], (*current.arr)[end]);

        int partitionIndex = i + 1;

        int threadsPerPartition = std::min(numThreads / 2, MAX_THREADS);
        int threadsLeft = numThreads - threadsPerPartition;

        stack.push({ current.arr, partitionIndex + 1, end });

        if (threadsLeft > 0) {
            stack.push({ current.arr, start, partitionIndex - 1 });
            numThreads = threadsPerPartition;
        } else {
            quicksort(*current.arr, start, partitionIndex - 1);
        }
    }
}

int main() {
    int numThreads, numElements;
    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;

    std::cout << "Enter the number of elements in the array: ";
    std::cin >> numElements;

    std::cout << "Enter the elements of the array:" << std::endl;
    std::vector<int> array(numElements);
    for (int i = 0; i < numElements; i++) {
        std::cin >> array[i];
    }

    hyperQuickSort(array, 0, numElements - 1, numThreads);

    std::cout << "Sorted Array:" << std::endl;
    for (int num : array) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
