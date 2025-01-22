//
// Created by xieqingxing on 2025/1/21.
//

#ifndef LSM_KV_DATA_H
#define LSM_KV_DATA_H
#include "iostream"
#include "item.h"
#include "fstream"
#include "vector"

// kv_data，负责具体kv数据的落盘和解析，和index.h类似
class kv_pair{
public:
    kv_pair(){};
    kv_pair(std::string,std::string);
    std::string key;
    std::string value;
    uint32_t keyLength;
    uint32_t  valueLength;

    // 序列化方法
    void serialize(std::ofstream &out) const {
        out.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
        out.write(reinterpret_cast<const char*>(&key), sizeof(key));
        out.write(reinterpret_cast<const char*>(&valueLength), sizeof(valueLength));
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }

    // 反序列化方法
    void deserialize(std::ifstream &in) {
        in.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
        in.read(reinterpret_cast<char*>(&key), sizeof(key));
        in.read(reinterpret_cast<char*>(&valueLength), sizeof(valueLength));
        in.read(reinterpret_cast<char*>(&value), sizeof(value));
    }
};

class kv_data {
public:
    kv_data(){};
    kv_data(std::vector<item>,std::string fileName);
    kv_data(std::string fileName);
    uint32_t  keyNum;
    std::vector<kv_pair> pairs;
    std::string fileName;

    void serialize();
    void deserialize();
};


#endif //LSM_KV_DATA_H
