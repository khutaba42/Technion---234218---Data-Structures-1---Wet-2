#ifndef _INCLUDES_H_
#define _INCLUDES_H_


#include <memory>
#include "ourUtilityFunctions.h"
#include "CostumerAndRecord.h"

/**
 * Compare Functions for the Trees
*/

inline Comparison Compare_shared_ptr_to_costumers_by_ID(const std::shared_ptr<Costumer> &left, const std::shared_ptr<Costumer> &right)
{
    if (*left < *right)
        return Comparison::less;
    else if (*left > *right)
        return Comparison::greater;
    else
        return Comparison::equal;
}

#endif // _INCLUDES_H_