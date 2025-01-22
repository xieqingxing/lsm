//
// Created by xieqingxing on 2025/1/20.
//

#include "item.h"

item::item(std::string key, std::string value) {
    this->key = key;
    this->value = value;
    this->key_length = sizeof(key);
    this->value_length = sizeof(value);
}

void item::print() {
    std::cout << "key: " << key << " value: "<< value <<  std::endl;
}
