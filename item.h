//
// Created by xieqingxing on 2025/1/20.
//

#ifndef LSM_ITEM_H
#define LSM_ITEM_H

#include "iostream"

class item {
public:
    item(){};
    item(std::string key,std::string value);
    std::string key;
    std::string value;
    int key_length;
    int value_length;
    void print();
};


#endif //LSM_ITEM_H
