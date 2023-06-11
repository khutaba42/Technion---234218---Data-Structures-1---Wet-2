#include "../lib/recordsCompany.h"

/**
 * O(1)
*/
RecordsCompany::RecordsCompany()
{

}

/**
 * O(n + m)
*/
RecordsCompany::~RecordsCompany()
{

}

/**
 * traverse in each costumer and make his debt to 0.
 * O(n + m)
*/
StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records)
{
    if(number_of_records < 0)
        return StatusType::INVALID_INPUT;
    try
    {

    }
    catch(const std::bad_alloc &)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

/**
 * I think the coustumers should be stored in a hash table
 * O(1) on avrage based on the input
*/
StatusType RecordsCompany::addCostumer(int c_id, int phone)
{
    if(c_id < 0 || phone < 0)
        return StatusType::INVALID_INPUT;
    try
    {

    }
    catch(const std::bad_alloc &)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

/**
 * O(1) on avrage based on the input
*/
Output_t<int> RecordsCompany::getPhone(int c_id)
{
    if(c_id < 0)
        return StatusType::INVALID_INPUT;
    try
    {

    }
    catch(...)
    {
       
    }
    return StatusType::SUCCESS;
}

/**
 * O(log(n))
*/
StatusType RecordsCompany::makeMember(int c_id)
{
    if(c_id < 0)
        return StatusType::INVALID_INPUT;
    try
    {

    }
    catch(const std::bad_alloc &)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

/**
 * O(1) on avrage based on the input
*/
Output_t<bool> RecordsCompany::isMember(int c_id)
{
    if(c_id < 0)
        return StatusType::INVALID_INPUT;
    try
    {

    }
    catch(...)
    {

    }
    return StatusType::SUCCESS;
}

/**
 * O(log(n))
*/
StatusType RecordsCompany::buyRecord(int c_id, int r_id)
{
    if(c_id < 0 || r_id < 0)
        return StatusType::INVALID_INPUT;
    try
    {

    }
    catch(...)
    {

    }
    return StatusType::SUCCESS;
}

/**
 * O(log(n))
*/
StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount)
{
    if(c_id1 < 0 || c_id2 < c_id1 || amount <= 0)
        return StatusType::INVALID_INPUT;
    
    return StatusType::SUCCESS;
}

/**
 * O(log(n))
*/
Output_t<double> RecordsCompany::getExpenses(int c_id)
{
    if(c_id < 0)
        return StatusType::INVALID_INPUT;
    
}

/**
 * I think the records should be union find 
 * O(log*(m))
*/
StatusType RecordsCompany::putOnTop(int r_id1, int r_id2)
{
    if(r_id1 < 0 || r_id2 < 0)
        return StatusType::INVALID_INPUT;
    try
    {

    }
    catch(...)
    {

    }
    return StatusType::SUCCESS;
}

/**
 * O(log*(m))
*/
StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight)
{

}
