#ifndef RECORDS_COMPANY_H
#define RECORDS_COMPANY_H

#include "utilesWet2.h"

#include "RankTree.h"
#include "hashTable.h"
#include "union-find.h"

#include "Costumer.h"
#include "RecordKeeper.h"
#include "CompareFunctions.h"

#include <memory>


class RecordsCompany {
  private:
    // holds the members of the records company
    RankTree<std::shared_ptr<Costumer>, Compare_shared_ptr_to_costumers_by_ID> __members;

    // holds all the costumers of the records company
    hashTable<std::shared_ptr<Costumer>, Compare_shared_ptr_to_costumers_by_ID > __costumers;

    // holds all the records that are in the records company
    UnionFind __Union_records;
    RecordKeeper __records;

    // more like, we use unique_ptr so that the default c'tor is nullptr
    std::unique_ptr<RecordKeeper> __records_ptr;

    // we need a deafult c'tor for union find, or something idk.

  public:
    RecordsCompany();
    ~RecordsCompany();
    StatusType newMonth(int *records_stocks, int number_of_records);
    StatusType addCostumer(int c_id, int phone);
    Output_t<int> getPhone(int c_id);
    StatusType makeMember(int c_id);
    Output_t<bool> isMember(int c_id);
    StatusType buyRecord(int c_id, int r_id);
    StatusType addPrize(int c_id1, int c_id2, double  amount);
    Output_t<double> getExpenses(int c_id);
    StatusType putOnTop(int r_id1, int r_id2);
    StatusType getPlace(int r_id, int *column, int *hight);
};

#endif