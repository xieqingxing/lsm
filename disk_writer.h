//
// Created by xieqingxing on 2025/1/20.
//

#ifndef LSM_DISK_WRITER_H
#define LSM_DISK_WRITER_H

#include "iostream"
#include "fstream"
#include "item.h"


class disk_writer {
public:
    disk_writer(std::string);
    std::string dir_name;
    int write_in_new_file(std::vector<item>); // 写入一个新的文件
    std::mutex mtx; // 互斥锁
    long long currentFileIndex; // 当前的文件序号
private:
    std::ofstream _file_writer;
};


#endif //LSM_DISK_WRITER_H
