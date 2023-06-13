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
    //typedef int /*_group*/ int;
    //typedef int /*_element*/ int;
public:
    UnionFind(int N = 0);
    ~UnionFind();

    // 
    /**
     * @brief creates a set of the integer i like we learned
     * @return 0 in case of out-of-range integer or element is in a set,
     * otherwise returns the integer (set/group)
    */
    /*_group*/ int make_set(/*_element*/ int i);
    // 
    /**
     * @brief finds the root of integer i like we learned
     * @return 0 in case of out-of-range integer or element is NOT in a set,
     * otherwise returns the root/set-representative
    */
    /*_group*/ int find(/*_element*/ int i);
    /**
     * @brief like we learned
     * @return 0 in case of out-of-range integer or one of the parameters is NOT a root/set-representative,
     *  otherwise returns the new root/set-representative of the newly created set (the bigger tree's root)
    */
    /*_group*/ int unionize(/*_group*/ int smaller, /*_group*/ int bigger);

private:
    int __size_of_array;
    int *__sizes_array;
    /*_element*/ int *__parents_array;

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
                              __parents_array(nullptr)
{
    if (N < 0)
    {
        throw std::invalid_argument("N < 0 in Union-Find.");
    }
    __sizes_array = new int[N + 1]();
    __parents_array = new int[N + 1]();
    for (int i = 1; i <= N; i++)
    {
        make_set(i);
    }
}

UnionFind::~UnionFind()
{
    delete[] __sizes_array;
    delete[] __parents_array;
}

/*_group*/ int UnionFind::make_set(/*_element*/ int i)
{
    if (!element_in_range(i) || __sizes_array[i])
    {
        return 0;
    }
    __sizes_array[i]++;
    return i;
}

/*_group*/ int UnionFind::find(/*_element*/ int i)
{
    /*_element*/ int root = get_root(i);

    // update parents
    while (__parents_array[i] /*exists*/)
    {
        __parents_array[i] = root;
        i = __parents_array[i];
    }

    return root;
}

/*_group*/ int UnionFind::unionize(/*_group*/ int smaller, /*_group*/ int bigger)
{
    if (!element_in_set(smaller) || !element_in_set(bigger) || __parents_array[smaller] || __parents_array[bigger])
    {
        return 0;
    }
    // here we are sure that smaller and bigger are groups
    if (__sizes_array[bigger] < __sizes_array[smaller]) 
        std::swap(bigger, smaller); // swap the smaller and the bigger so they fit their names (note that this DOESN'T affect the arrays)
    __parents_array[smaller] = bigger;
    __sizes_array[bigger] += __sizes_array[smaller];
    __sizes_array[smaller] = 0;
    return bigger;
}

#endif // _UNION_FIND_H_