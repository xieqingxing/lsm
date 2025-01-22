//
// Created by xieqingxing on 2025/1/20.
//

#include <iostream>

#ifndef LSM_DB_H
#define LSM_DB_H

#include "memtable.h"
#include "mutex"


class db {
public:
    db(std::string);
    void write(std::string key,std::string value);
    search_result search(std::string key);
    void cron_flush_disk();
    ~db();
private:
    memtable *mem;
    std::string dir_name;
    std::vector<std::string> index_files; // 索引的文件列表
    std::mutex index_mtx;
    std::thread worker_thread;
};


#endif //LSM_DB_H
