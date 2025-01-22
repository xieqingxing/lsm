//
// Created by xieqingxing on 2025/1/21.
//

#include <vector>
#include "index.h"
#include "fstream"

lsm::index::index(std::vector<item> is,std::string fileName) {
    smallKey = is[0].key;
    bigKey = is[is.size()-1].key;
    keyNum = is.size();
    int s = 8;
    for(int i =0;i < is.size();i++){
        pairs.push_back(pair(s,is[i].key_length));
        s += is[i].key_length;
        s += 4; // 4个字节的value长度
        s += is[i].value_length;
        s += 4; // 4个字节的key长度（下个key的）
    }
    this->smallKeyLength = smallKey.size();
    this->bigKeyLength = bigKey.size();
    this->fileName = fileName;
}

void lsm::index::serialize() {
    std::ofstream out(fileName,std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << fileName << std::endl;
        return;
    }
    // 写入 smallKeyLength 和 smallKey
    out.write(reinterpret_cast<const char*>(&smallKeyLength), sizeof(smallKeyLength));
    out.write(smallKey.data(), smallKeyLength);

    // 写入 bigKeyLength 和 bigKey
    out.write(reinterpret_cast<const char*>(&bigKeyLength), sizeof(bigKeyLength));
    out.write(bigKey.data(), bigKeyLength);

    // 写入 keyNum
    out.write(reinterpret_cast<const char*>(&keyNum), sizeof(keyNum));
    for (const auto &p : pairs) {
        p.serialize(out);
    }
    std::cout << "write file success " << fileName << std::endl;
    out.close();
}

void lsm::index::deserialize() {
    std::ifstream in(fileName, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;
        return;
    }

    // 读取 smallKeyLength 和 smallKey
    in.read(reinterpret_cast<char*>(&smallKeyLength), sizeof(smallKeyLength));
    smallKey.resize(smallKeyLength);
    in.read(&smallKey[0], smallKeyLength);

    // 读取 bigKeyLength 和 bigKey
    in.read(reinterpret_cast<char*>(&bigKeyLength), sizeof(bigKeyLength));
    bigKey.resize(bigKeyLength);
    in.read(&bigKey[0], bigKeyLength);

    // 读取 keyNum
    in.read(reinterpret_cast<char*>(&keyNum), sizeof(keyNum));

    // 读取 pairs
    pairs.resize(keyNum);
    for (auto &p : pairs) {
        p.deserialize(in);
    }

    in.close();
}

lsm::index::index(std::string fileName) {
    this->fileName = fileName;
}

lsm::pair::pair(uint8_t o,uint8_t l) {
    offset = o;
    length = l;
}
