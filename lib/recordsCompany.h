#ifndef RECORDS_COMPANY_H
#define RECORDS_COMPANY_H

#include "utilesWet2.h"

#include "AVLTree2.h"
#include "hashTable.h"
#include "union-find.h"

#include "Costumer.h"
#include "Record.h"

#include <memory>


class RecordsCompany {
  private:
    // holds the members of the records company
    AVLTree2<std::shared_ptr<Costumer>> __members;

    // holds all the costumers of the records company
    hashTable<std::shared_ptr<Costumer>> __costumers;

    // holds all the records that are in the records company
    //UnionFind __records;

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