//
// Created by xieqingxing on 2025/1/21.
//

#include "kv_data.h"
#include "vector"

kv_data::kv_data(std::vector<item> is,std::string fileName) {
    keyNum = is.size();
    for(int i = 0;i < is.size();i++){
        pairs.push_back(kv_pair(is[i].key,is[i].value));
    }
    this->fileName = fileName;
}

kv_pair::kv_pair(std::string key, std::string value) {
    keyLength = sizeof(key);
    this->key = key;
    valueLength = sizeof(value);
    this->value = value;
}



void kv_data::serialize() {
    std::ofstream out(fileName,std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << fileName << std::endl;
        return;
    }
    // 写入 keyNum
    out.write(reinterpret_cast<const char*>(&keyNum), sizeof(keyNum));
    for (const auto &p : pairs) {
        p.serialize(out);
    }
    std::cout << "write file success " << fileName << std::endl;

    out.close();
}

void kv_data::deserialize() {
    std::ifstream in(fileName, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;
        return;
    }
    // 读取 keyNum
    in.read(reinterpret_cast<char*>(&keyNum), sizeof(keyNum));

    // 读取 pairs
    pairs.resize(keyNum);
    for (auto &p : pairs) {
        p.deserialize(in);
    }

    in.close();
}

kv_data::kv_data(std::string fileName) {
    this->fileName = fileName;
}
