//
// Created by xieqingxing on 2025/1/20.
//

#ifndef LSM_MEMTABLE_H
#define LSM_MEMTABLE_H

#include <vector>
#include "iostream"
#include "item.h"
#include "mutex"
#include "disk_writer.h"
#include "thread"


class search_result{
public:
    item i;
    int res_code;
    void print();
    std::string to_string();
};

class memtable {
public:
    memtable(){};
    memtable(int,int);
    std::mutex write_mtx;
    std::mutex refresh_mtx;
    std::mutex flush_mtx;
    void write(std::string key,std::string value);
    search_result search(std::string key);
    void show_mem();
    void refresh_mem(); // 刷新无序的内存数据，生成有序的内存片段，可以用于查询
    void cron_refresh_mem(std::atomic<bool>& stop_flag); // 定时刷新内存数据
    int flush_disk(); // 数据落盘
    disk_writer *writer;
    ~memtable();
private:
    std::vector<item> data;
    std::vector<std::vector<item>> sorted_data; // 排序后的内存数据
    int memMaxLength;
    int flushDiskLength; // 落盘的数量 todo 支持1s落盘一次？
    std::thread worker_thread;
    std::atomic<bool> stop_flag;
};


#endif //LSM_MEMTABLE_H
