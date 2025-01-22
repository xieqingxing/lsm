//
// Created by xieqingxing on 2025/1/20.
//

#include "disk_writer.h"
#include <chrono>
#include "item.h"
#include "index.h"
#include "kv_data.h"
#include "vector"


// 索引文件内容，方便快速查找数据
// key的offset和length,offset是文件的偏移量,长度是key的长度
// 最小的key和最大的key

// 写入数据文件和索引文件
int disk_writer::write_in_new_file(std::vector<item> is) {
    // 获取当前时间戳，写入一个新的文件
    mtx.lock();
    currentFileIndex++;
    int fileIndex = currentFileIndex;
    // 生成和写入索引
    lsm::index ni = lsm::index(is,dir_name + "/" + std::to_string(currentFileIndex) + ".index");
    ni.serialize();
    kv_data kvd = kv_data(is,dir_name + "/" + std::to_string(currentFileIndex) + ".bin");
    kvd.serialize();
    mtx.unlock();
    return fileIndex;
}

disk_writer::disk_writer(std::string dir_name) {
    this->dir_name = dir_name;
    auto now = std::chrono::system_clock::now();
    // 转换为秒级时间戳
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch()).count();
    this->currentFileIndex = timestamp;
    std::cout << "Current timestamp in seconds: " << timestamp << '\n';
}
