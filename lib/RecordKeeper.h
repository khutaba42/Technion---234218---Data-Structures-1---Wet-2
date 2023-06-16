#ifndef _UNION_FIND_RECORDS_HEIGHT_H_
#define _UNION_FIND_RECORDS_HEIGHT_H_

#include <new>

#include <stdexcept>
#include "utilesWet2.h"
#include "CostumerAndRecord.h"

/**
 * @brief holds in the Records and "book keeps" them
 */
class RecordKeeper
{
public:
    RecordKeeper(int *records_stock = nullptr, int number_of_records = 0);
    ~RecordKeeper();

    Record &operator[](int identifier);
    const Record &operator[](int identifier) const;

    StatusType putOnTop(int r_id1, int r_id2);
    StatusType getPlace(int r_id, int *column, int *height);

private:
    const int __num_of_records; //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * inside this array there is __num_of_records slots
     */
    Record *__records_arr;
    /**
     * inside these arrays there is __num_of_records+1 slots
     * each r_id is mapped to r_id+1
     * never access element 0
     */
    // real num of copies
    int *__records_stocks_arr;
    // 0 means the element isn't a set
    int *__union_find__size;
    // 0 means the element doesn't have a parent
    int *__union_find__parent;
    //
    int *__union_find__column;
    // in heights i mean the number of copies the record have (this return there sum)
    int *__union_find__column_height;
    //
    int *__union_find__relative_heights;

    //  helper
    int get_size() const;
    // returns what each records is mapped to , in this case r_id+1
    int get_index(int r_id) const { return r_id + 1; }
    // returns the r_id form index, in this case index-1
    int get_r_id(int index) const { return index - 1; }
    // these functions deal with the union-find (meaning r_id+1)

    // false if index not in range {1,..., #records}
    bool in_range(int index) const;
    bool element_in_set(int index) const;
    bool element_is_a_set(int index) const;
    // takes O(search path)
    int get_root(int index) const;
    // doesn't check for ranges
    bool in_the_same_column(int r_id1, int r_id2) const;
    // get the sum of the heights until the root (not included)
    int sum_of_relative_heights(int index) const;

    int height_of_column_element_is_in(int index);
    int height_of_element(int index) const;

    int find(int index);
    int unionize(int set_A, int set_B);

    // we put set B on top of A in reality but in code its different
    // we assume we really have roots of the sets and we dont check
    void update_sizes_union(int set_A, int set_B);
    // we put set B on top of A in reality but in code its different
    // we assume we really have roots of the sets and we dont check
    void update_parents_union(int set_A, int set_B);
    // we put set B on top of A in reality but in code its different
    // we assume we really have roots of the sets and we dont check
    // assumes that set_A is the column of the new union
    void update_column_union(int set_A, int set_B);
    // we put set B on top of A in reality but in code its different
    // we assume we really have roots of the sets and we dont check
    void update_column_heights_union(int set_A, int set_B);
    // we put set B on top of A in reality but in code its different
    // we assume we really have roots of the sets and we dont check
    void update_relative_heights_union(int set_A, int set_B);
};

#endif // _UNION_FIND_RECORDS_HEIGHT_H_