//
// Created by xieqingxing on 2025/1/21.
//

#include "kv_search.h"
#include "index.h"
#include "kv_data.h"

kv_search::kv_search(std::string fileName) {
    index = lsm::index(fileName + ".index");
    index.deserialize();
    kvd = kv_data(fileName + ".bin");
     kvd.deserialize(); // todo 这里把所有数据都加载到内存了，不应该加载，应该通过偏移量去
    this->fileName = fileName;
}

search_result kv_search::search(std::string key) {
    search_result res;
    res.res_code = -1;
    if(index.smallKey > key || index.bigKey < key){
        return res;
    }
    // 二分法查找数据
    int s = 0;
    int e = index.keyNum - 1;
    while(s <= e){
        int p = ((e - s) / 2) + s;
        std::string tk = load_key_by_index(p);
        if(tk == key){
            res.i =  load_item_by_index(p);
            res.res_code = 1;
            return res;
        }
        if(key < tk){
            e = p - 1;
            continue;
        }
        if(key > tk){
            s = p+1;
            continue;
        }
    }
    return res;
}

// 加载key
std::string kv_search::load_key_by_index(int i) {
    lsm::pair p = index.pairs[i];
    std::ifstream file(fileName + ".bin", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return "";
    }
    // 把key读出去
    file.seekg(p.offset); // 加上4个字节的key长度,4个字节的keynum
    std::string key;
    file.read(reinterpret_cast<char*>(&key), p.length);
    return key;
}

item kv_search::load_item_by_index(int i) {
    lsm::pair p = index.pairs[i];
    std::ifstream file(fileName + ".bin", std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return item();
    }
    // 把key读出去
    file.seekg(p.offset); // 加上4个字节的key长度,4个字节的keynum
    std::string key;
    file.read(reinterpret_cast<char*>(&key), p.length);
    std::string value;
    // 读取4个字节的vaule长度
    uint32_t valueLength;
    file.read(reinterpret_cast<char*>(&valueLength), sizeof(valueLength));
    file.read(reinterpret_cast<char*>(&value),valueLength);
    return item(key,value);
}
