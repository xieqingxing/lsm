//
// Created by xieqingxing on 2025/1/21.
//

#ifndef LSM_KV_SEARCH_H
#define LSM_KV_SEARCH_H
#include "iostream"
#include "index.h"
#include "kv_data.h"
#include "memtable.h"

// 联合index和data数据
// 实现key的查找
class kv_search {
public:
    kv_search(std::string fileName);
    lsm::index index;
    kv_data kvd;
    search_result search(std::string key);
    std::string load_key_by_index(int i);
    item load_item_by_index(int i); // 返回kv的信息
    std::string fileName;
};


#endif //LSM_KV_SEARCH_H
