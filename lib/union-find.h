#ifndef _UNION_FIND_H_
#define _UNION_FIND_H_

#include <algorithm>
#include <iostream>
#include <stdexcept>

/**
 * simple union-find (the 4th implementation) find+union log* amortized.
 * the first element in the array (element at index 0) is not used for the sake of simplicity
 * meaning the array is of size N+1
 * all function return 0 instead of an index value in case of out of out-of-range or no actual result to return
 */
// {1,...,N}
class UnionFind
{
    // typedef int /*_set*/ int;
    // typedef int /*_element*/ int;
public:
    UnionFind(int N = 0);
    ~UnionFind();

    //
    /**
     * @brief creates a set of the integer i like we learned
     * @return 0 in case of out-of-range integer or element is in a set,
     * otherwise returns the integer (set/group)
     */
    /*_set*/ int make_set(/*_element*/ int i);
    //
    /**
     * @brief finds the root of integer i like we learned
     * @return 0 in case of out-of-range integer or element is NOT in a set,
     * otherwise returns the root/set-representative
     */
    /*_set*/ int find(/*_element*/ int i);
    /**
     * @brief like we learned
     * @return 0 in case of out-of-range integer or one of the parameters is NOT a root/set-representative,
     *  otherwise returns the new root/set-representative of the newly created set (the bigger tree's root)
     */
    /*_set*/ int unionize(/*_set*/ int set_A, /*_set*/ int set_B);

    /**
     * @brief takes in a pointer to an array of heights and update the heights of the boxes
     */
    void add_heights(int *heights_arr, int len);

private:
    int __size_of_array;
    int *__sizes_array;
    /*_element*/ int *__parents_array;
    int *__rel_heights_of_boxes; // the rel height of a root is always it's true height
    int *__real_height_of_towers;
    //  helper

    inline int get_size() const
    {
        return __size_of_array;
    }

    bool element_in_range(/*_element*/ int i) const
    {
        if ((i > 0) && (i <= get_size()))
            return true;
        else
            return false;
    }

    bool element_in_set(/*_element*/ int i) const
    {
        if ((element_in_range(i)) && ((__parents_array[i]) || (__sizes_array[i])))
            return true;
        else
            return false;
    }

    /*_element*/ int get_root(/*_element*/ int i) const
    {
        if (!element_in_set(i))
        {
            return 0;
        }
        while (__parents_array[i] /*exists*/)
        {
            i = __parents_array[i];
        }
        return i;
    }

    /*_element*/ int get_root_and_height_sum(/*_element*/ int i, int &sumR) const
    {
        if (!element_in_set(i))
        {
            return 0;
        }
        sumR = 0;
        while (__parents_array[i] /*exists*/)
        {
            sumR += __rel_heights_of_boxes[i];
            i = __parents_array[i];
        }
        // sum now has the sum of the heights up to the root (not included because of the __parents_array[i] != in the while)
        return i;
    }

public:
    void display() const
    {
        std::cout << "\n";
        std::cout << "elements: ";
        for (size_t i = 1; i <= get_size(); i++)
        {
            std::cout << i << " ";
        }
        std::cout << "\n\n";

        std::cout << "sizes:    ";
        for (size_t i = 1; i <= get_size(); i++)
        {
            std::cout << __sizes_array[i] << " ";
        }
        std::cout << "\n\n";

        std::cout << "parents:  ";
        for (size_t i = 1; i <= get_size(); i++)
        {
            std::cout << __parents_array[i] << " ";
        }
        std::cout << "\n\n";
    }

    bool compare_sizes(/*_element*/ int *arr, int len) const
    {
        if (len > get_size())
        {
            return false;
        }
        for (size_t i = 1; i <= get_size(); i++)
        {
            if (arr[i - 1] != __sizes_array[i])
            {
                return false;
            }
        }
        return true;
    }

    bool compare_parents(/*_element*/ int *arr, int len) const
    {
        if (len > get_size())
        {
            return false;
        }
        for (size_t i = 1; i <= get_size(); i++)
        {
            if (arr[i - 1] != __parents_array[i])
            {
                return false;
            }
        }
        return true;
    }
};

UnionFind::UnionFind(int N) : __size_of_array(N),
                              __sizes_array(nullptr),
                              __parents_array(nullptr),
                              __rel_heights_of_boxes(nullptr),
                              __real_height_of_towers(nullptr)
{
    if (N < 0)
    {
        throw std::invalid_argument("N < 0 in Union-Find.");
    }
    __sizes_array = new int[N + 1]();
    __parents_array = new int[N + 1]();
    __rel_heights_of_boxes = new int[N + 1]();
    __real_height_of_towers = new int[N + 1]();
    for (int i = 1; i <= N; i++)
    {
        make_set(i);
    }
}

UnionFind::~UnionFind()
{
    delete[] __sizes_array;
    delete[] __parents_array;
    delete[] __rel_heights_of_boxes;
    delete[] __real_height_of_towers;
}

/*_set*/ int UnionFind::make_set(/*_element*/ int i)
{
    if (!element_in_range(i) || __sizes_array[i])
    {
        return 0;
    }
    __sizes_array[i]++;
    return i;
}

/*_set*/ int UnionFind::find(/*_element*/ int i)
{
    if (!element_in_set(i))
    {
        return 0;
    }
    int old_parent = i;
    /*_element*/ int root = __parents_array[i];
    while (root != __parents_array[root])
    {
        // store the parent for path compression later
        int next_old_parent = __parents_array[old_parent];
        // path compression
        __parents_array[old_parent] = __parents_array[root];
        if (old_parent != root) // don't update height if you are at the root
        {
            // update the relative height
            __rel_heights_of_boxes[old_parent] += __rel_heights_of_boxes[root];
        }
        // move up the tree
        old_parent = next_old_parent;
        root = __parents_array[old_parent];
    }
    return root;
}

/*_set*/ int UnionFind::unionize(/*_set*/ int set_A, /*_set*/ int set_B)
{
    if (!element_in_set(set_A) || !element_in_set(set_B) || __parents_array[set_A] || __parents_array[set_B])
    {
        return 0;
    }
    // make set_B the root of set_A if set_B has greater or equal height
    if (__real_height_of_towers[set_B] >= __real_height_of_towers[set_A])
    {
        // update parent
        __parents_array[set_A] = set_B;
        __sizes_array[set_B] += __sizes_array[set_A];
        // update the height of set_B (if needed)
        __real_height_of_towers[set_B] = std::max(__real_height_of_towers[set_B], __rel_heights_of_boxes[set_A] + __real_height_of_towers[set_A]);
    }
    else
    {
        // update parent
        __parents_array[set_B] = set_A;
        __sizes_array[set_A] += __sizes_array[set_B];
        // update the height of set_A
        __real_height_of_towers[set_A] = std::max(__real_height_of_towers[set_A], __rel_heights_of_boxes[set_B] + __real_height_of_towers[set_B]);
    }
    return __parents_array[set_A];
}

void UnionFind::add_heights(int *heights_arr, int len)
{
    if (len != get_size())
    {
        return;
    }
    for (int i = 0; i < len; i++)
    {
        __rel_heights_of_boxes[i + 1] = heights_arr[i];
    }
}

#endif // _UNION_FIND_H_