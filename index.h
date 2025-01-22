//
// Created by xieqingxing on 2025/1/21.
//

#ifndef LSM_INDEX_H
#define LSM_INDEX_H
#include "iostream"
#include "item.h"
#include "fstream"
#include "vector"

namespace lsm{
    class pair{
    public:
        pair(){};
        pair(uint8_t,uint8_t);
        uint8_t offset; // 偏移量
        uint8_t length; // key的长度
        // 序列化方法
        void serialize(std::ofstream &out) const {
            out.write(reinterpret_cast<const char*>(&offset), sizeof(offset));
            out.write(reinterpret_cast<const char*>(&length), sizeof(length));
        }

        // 反序列化方法
        void deserialize(std::ifstream &in) {
            in.read(reinterpret_cast<char*>(&offset), sizeof(offset));
            in.read(reinterpret_cast<char*>(&length), sizeof(length));
        }
    };

    class index {
    public:
        index(){};
        index(std::vector<item>,std::string);
        index(std::string);
        uint32_t smallKeyLength;
        uint32_t bigKeyLength;
        std::string smallKey;
        std::string bigKey;
        uint32_t keyNum;
        std::vector<pair> pairs;
        std::string fileName;

        void serialize();
        void deserialize();
    };

}

#endif //LSM_INDEX_H
