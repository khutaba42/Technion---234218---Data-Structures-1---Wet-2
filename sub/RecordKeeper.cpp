#include "RecordKeeper.h"


RecordKeeper::RecordKeeper(int *records_stock, int number_of_records)
    : __num_of_records(number_of_records),
      __records_arr(nullptr),
      __records_stocks_arr(nullptr),
      __union_find__size(nullptr),
      __union_find__parent(nullptr),
      __union_find__column(nullptr),
      __union_find__column_height(nullptr),
      __union_find__relative_heights(nullptr)
{
    if (number_of_records < 0)
    {
        throw std::invalid_argument("Can't have a negative number of records in RecordKeeper.");
    }
    if (number_of_records > 0)
    {
        __records_arr = static_cast<Record *>(::operator new(sizeof(Record) * number_of_records));
        for (int r_id = 0; r_id < number_of_records; ++r_id)
        {
            new ((void *)(__records_arr + r_id)) Record(r_id);
        }

        // allocate the rest of the arrays and initialize them to 0
        __records_stocks_arr = new int[number_of_records]();
        __union_find__size = new int[number_of_records + 1]();
        __union_find__parent = new int[number_of_records + 1]();
        __union_find__column = new int[number_of_records + 1]();
        __union_find__column_height = new int[number_of_records + 1]();
        __union_find__relative_heights = new int[number_of_records + 1]();
        // update the default values of the arrays
        // sizes are 1
        for (int i = 1; i <= number_of_records; ++i)
        {
            __union_find__size[i] = 1;
        }
        // parents are 0
        // column are the index (NOT r_id) at the beginning
        for (int i = 1; i <= number_of_records; ++i)
        {
            __union_find__column[i] = i;
        }
        // "heights" and the copies
        if (records_stock != nullptr)
        {
            for (int i = 1; i <= number_of_records; ++i)
            {
                __union_find__column_height[i] = records_stock[i - 1];
                __records_stocks_arr[i - 1] = records_stock[i - 1];
            }
        }
        // relative heights are 0
    }
}

RecordKeeper::~RecordKeeper()
{
    for (int r_id = 0; r_id < get_size(); ++r_id)
    {
        (__records_arr + r_id)->~Record();
    }
    // Deallocate the memory
    ::operator delete(__records_arr);

    delete[] __union_find__size;
    delete[] __union_find__parent;
    delete[] __union_find__column;
    delete[] __union_find__column_height;
    delete[] __union_find__relative_heights;
    delete[] __records_stocks_arr;
}

Record &RecordKeeper::operator[](int identifier)
{
    if (identifier < 0 || identifier >= get_size())
    {
        throw std::invalid_argument("r_id doesn't exist (in RecordKeeper).");
    }
    return __records_arr[identifier];
}

const Record &RecordKeeper::operator[](int identifier) const
{
    if (identifier < 0 || identifier >= get_size())
    {
        throw std::invalid_argument("r_id doesn't exist (in RecordKeeper).");
    }
    return __records_arr[identifier];
}

StatusType RecordKeeper::putOnTop(int r_id1, int r_id2)
{
    if (r_id1 < 0 || r_id2 < 0)
    {
        return StatusType::INVALID_INPUT;
    }
    if (r_id1 >= get_size() || r_id2 >= get_size())
    {
        return StatusType::DOESNT_EXISTS;
    }
    if (in_the_same_column(r_id1, r_id2))
    {
        return StatusType::FAILURE;
    }
    // here we must do the union and find
    int record1_index = get_index(r_id1);
    int record2_index = get_index(r_id2);

    // after this line we forget about the records we only use the indicies in union-find
    int record1_root = find(record1_index);
    int record2_root = find(record2_index);
    // we now choose which group to connect to the root of the other ("union")

    // its very crucial not to update the heights before relative height because they depend on it
    update_relative_heights_union(record2_root, record1_root);

    update_column_union(record2_root, record1_root);
    update_column_heights_union(record2_root, record1_root);

    /*
        update_parents_union(record2_root, record1_root);
        // its very crucial not to update the size before anything else because they depend on it
        update_sizes_union(record2_root, record1_root);
    */
    if (__union_find__size[record2_root] >= __union_find__size[record1_root]) // connect B to A
    {
        __union_find__size[record2_root] += __union_find__size[record1_root];
        __union_find__size[record1_root] = 0;

        __union_find__parent[record1_root] = record2_root;
    }
    else //(__union_find__size[set_A] < __union_find__size[set_B]) // connect A to B
    {
        __union_find__size[record1_root] += __union_find__size[record2_root];
        __union_find__size[record2_root] = 0;

        __union_find__parent[record2_root] = record1_root;
    }

    return StatusType::SUCCESS;
}

StatusType RecordKeeper::getPlace(int r_id, int *column, int *height)
{
    if (r_id < 0 || column == nullptr || height == nullptr)
    {
        return StatusType::INVALID_INPUT;
    }
    if (r_id >= get_size())
    {
        return StatusType::DOESNT_EXISTS;
    }
    // here we must do the find
    int record_index = get_index(r_id);

    // after this line we forget about the records we only use the indicies in union-find

    int record_root = find(record_index);

    *column = __union_find__column[record_root] - 1; // -1 because the implementation holds 1 to #records
    *height = height_of_element(record_index);

    return StatusType::SUCCESS;
}

void RecordKeeper::update_sizes_union(int set_A, int set_B)
{

    if (__union_find__size[set_A] >= __union_find__size[set_B]) // connect B to A
    {
        __union_find__size[set_A] += __union_find__size[set_B];
        __union_find__size[set_B] = 0;
    }
    else //(__union_find__size[set_A] < __union_find__size[set_B]) // connect A to B
    {
        __union_find__size[set_B] += __union_find__size[set_A];
        __union_find__size[set_A] = 0;
    }
}

void RecordKeeper::update_parents_union(int set_A, int set_B)
{
    if (__union_find__size[set_A] >= __union_find__size[set_B]) // connect B to A
    {
        __union_find__parent[set_B] = set_A;
        //__union_find__parent[set_A] = 0;
    }
    else //(__union_find__size[set_A] < __union_find__size[set_B]) // connect A to B
    {
        __union_find__parent[set_A] = set_B;
        //__union_find__parent[set_B] = 0;
    }
}

void RecordKeeper::update_column_union(int set_A, int set_B)
{
    if (__union_find__size[set_A] >= __union_find__size[set_B]) // connect B to A
    {
        // __union_find__column[set_A]; // stays the same
        __union_find__column[set_B] = 0;
    }
    else //(__union_find__size[set_A] < __union_find__size[set_B]) // connect A to B
    {
        __union_find__column[set_B] = __union_find__column[set_A];
        __union_find__column[set_A] = 0;
    }
}

void RecordKeeper::update_column_heights_union(int set_A, int set_B)
{
    if (__union_find__size[set_A] >= __union_find__size[set_B]) // connect B to A
    {
        __union_find__column_height[set_A] += __union_find__column_height[set_B];
        __union_find__column_height[set_B] = 0;
    }
    else //(__union_find__size[set_A] < __union_find__size[set_B]) // connect A to B
    {
        __union_find__column_height[set_B] += __union_find__column_height[set_A];
        __union_find__column_height[set_A] = 0;
    }
}

void RecordKeeper::update_relative_heights_union(int set_A, int set_B)
{
    if (__union_find__size[set_A] >= __union_find__size[set_B]) // connect B to A
    {
        //__union_find__relative_heights[set_A];//stays the same
        __union_find__relative_heights[set_B] += __union_find__column_height[set_A] - __union_find__relative_heights[set_A];
    }
    else //(__union_find__size[set_A] < __union_find__size[set_B]) // connect A to B
    {
        __union_find__relative_heights[set_B] += __union_find__column_height[set_A];
        __union_find__relative_heights[set_A] -= __union_find__relative_heights[set_B]; // we remove the NEW B
    }
}

int RecordKeeper::height_of_element(int index) const
{
    if (!in_range(index))
    {
        return 0;
    }
    int sum = 0;
    while (__union_find__parent[index]) // the parent exists
    {
        sum += __union_find__relative_heights[index];
        index = __union_find__parent[index];
    }
    sum += __union_find__relative_heights[index];
    return sum;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////  private  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int RecordKeeper::get_size() const
{
    return __num_of_records;
}

bool RecordKeeper::in_range(int index) const
{
    if (index > 0 || index <= get_size())
    {
        return true;
    }
    return false;
}

int RecordKeeper::get_root(int index) const
{
    if (!element_in_set(index))
    {
        return 0;
    }
    while (__union_find__parent[index] /*exists*/)
    {
        index = __union_find__parent[index];
    }
    return index;
}

bool RecordKeeper::element_in_set(int index) const
{
    if ((in_range(index)) &&
        ((__union_find__parent[index]) || (__union_find__size[index])))
        return true;
    else
        return false;
}

bool RecordKeeper::in_the_same_column(int r_id1, int r_id2) const
{
    if (__union_find__column[get_root(get_index(r_id1))] == __union_find__column[get_root(get_index(r_id2))])
    {
        return true;
    }
    return false;
}

int RecordKeeper::sum_of_relative_heights(int index) const
{
    if (!in_range(index))
    {
        return 0;
    }
    int sum = 0;
    while (__union_find__parent[index]) // the parent exists
    {
        sum += __union_find__relative_heights[index];
        index = __union_find__parent[index];
    }
    return sum;
}

int RecordKeeper::height_of_column_element_is_in(int index)
{
    return __union_find__column_height[get_root(index)];
}

int RecordKeeper::find(int index)
{
    if (!in_range(index))
    {
        return 0;
    }
    int record1_root = get_root(index);
    int sum_of_heights_of_record_1 = sum_of_relative_heights(index);
    while (__union_find__parent[index]) // the parent exists
    {
        int old_parent = __union_find__parent[index];
        __union_find__parent[index] = record1_root;
        int old_rel_height = __union_find__relative_heights[index];
        __union_find__relative_heights[index] = sum_of_heights_of_record_1;
        sum_of_heights_of_record_1 -= old_rel_height;
        ///////
        index = old_parent;
    }
    return index;
}