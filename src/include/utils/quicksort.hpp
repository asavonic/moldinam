#pragma once

#include <vector>

template <typename Container, class SwapFunctor, class LessFunctor>
int partition (Container& arr, int l, int h, LessFunctor& less, SwapFunctor& swap)
{
    int i = (l - 1);
 
    for (int j = l; j <= h- 1; j++)
    {
        if (less(arr[j], arr[h]) || arr[j] == arr[h])
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[h]);
    return (i + 1);
}
 
/* A[] --> Array to be sorted, l  --> Starting index, h  --> Ending index */
template <typename Container, class SwapFunctor, class LessFunctor>
void quicksort (Container& arr, int l, int h, LessFunctor& less, SwapFunctor& swap)
{
    // Create an auxiliary stack
    std::vector<int> stack(h - l + 1);
 
    // initialize top of stack
    int top = -1;
 
    // push initial values of l and h to stack
    stack[++top] = l;
    stack[++top] = h;
 
    // Keep popping from stack while is not empty
    while (top >= 0)
    {
        // Pop h and l
        h = stack[top--];
        l = stack[top--];
 
        // Set pivot element at its correct position in sorted array
        int p = partition(arr, l, h, less, swap);
 
        // If there are elements on left side of pivot, then push left
        // side to stack
        if (p - 1 > l) {
            stack[++top] = l;
            stack[++top] = p - 1;
        }
 
        // If there are elements on right side of pivot, then push right
        // side to stack
        if (p + 1 < h) {
            stack[++top] = p + 1;
            stack[++top] = h;
        }
    }
}

struct std_swap {
    template<typename T>
    void operator()(T& lhs, T&rhs)
    {
        std::swap(lhs, rhs);
    }
};

struct std_less {
    template<typename T>
    bool operator()(T& lhs, T&rhs)
    {
        return lhs < rhs;
    }
};

template <typename Container>
void quicksort(Container& arr)
{
    std_swap swap;
    std_less less;
    quicksort(arr, 0, arr.size() - 1, less, swap);
}

template <typename Container, class LessFunctor>
void quicksort(Container& arr, LessFunctor& less)
{
    std_swap swap;
    quicksort(arr, 0, arr.size() - 1, less, swap);
}

template <typename Container, class LessFunctor, class SwapFunctor>
void quicksort(Container& arr, LessFunctor& less, SwapFunctor& swap)
{
    quicksort(arr, 0, arr.size() - 1, less, swap);
}
