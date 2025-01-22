#include <iostream>



// 实现一个lsm树
// 支持增删改查
// 根据时间做索引
// 考虑实现wal
#include "memtable.h"
#include "index.h"
#include "kv_data.h"
#include "db.h"
#include "kv_search.h"

void index_test(){
    std::vector<item> a;
    a.push_back(item("a","b"));
    lsm::index i = lsm::index(a,"/Users/xieqingxing/CLionProjects/lsm/storage/test.index");
    std::cout << i.bigKey << std::endl;
    i.serialize();
}

void index_test2(){
    lsm::index i = lsm::index("./storage/test.index");
    i.deserialize();
    std::cout << i.bigKey << std::endl;
    std::cout << i.keyNum << std::endl;
}

void kv_data_test(){
    std::vector<item> a;
    a.push_back(item("a","b"));
    kv_data i = kv_data(a,"/Users/xieqingxing/CLionProjects/lsm/storage/test.bin");
    std::cout << i.keyNum << std::endl;
    i.serialize();
}

void kv_data_test2(){
    kv_data i = kv_data("/Users/xieqingxing/CLionProjects/lsm/storage/test.bin");
    i.deserialize();
    std::cout << i.keyNum << std::endl;
    std::cout << i.pairs[0].key << std::endl;
}

void kv_search_test(){
    kv_search s = kv_search("./storage/1737512197");
//    s.load_key_by_index(0);
//    s.load_item_by_index(0);
    search_result res = s.search("a");
    res.i.print();
    search_result res2 = s.search("C");
    res2.i.print();
}

void db_test1(){
    db* DB = new db("./storage");
    DB->write("a","b");
    DB->write("C","d");
    DB->write("d","d");
}

void db_test2(){
    db* DB = new db("./storage/");
//    search_result res = DB->search("a");
//    res.i.print();
//    DB->write("123","456");
//    std::this_thread::sleep_for(std::chrono::seconds(5));
    search_result res2 = DB->search("123");
    res2.print();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}


int main() {
//    std::cout << "Hello, World!" << std::endl;
//    memtable* m = new memtable(2,1);
//    m->write("a","b");
//    m->write("a","b");
//    m->show_mem();
//    memtable_search_result res =  m->search("b");
//    res.print();
//    return 0;
//    db_test1();

//    index_test();
//    index_test2();
//    kv_data_test();
//    kv_data_test2();


// kv_search test
//    kv_search_test();
    db_test2();
}
