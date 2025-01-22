//
// Created by xieqingxing on 2025/1/20.
//

#include "db.h"
#include <filesystem>
#include "index.h"
#include "kv_search.h"


namespace fs = std::__fs::filesystem;

std::vector<std::string> getIndexFiles(const std::string& directory) {
    std::vector<std::string> indexFiles;

    try {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".index") {
                std::string tp = entry.path().string();
                tp = tp.substr(0,tp.size()-6);
                indexFiles.push_back(tp);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return indexFiles;
}


db::db(std::string dir_name) :
        worker_thread(&db::cron_flush_disk, this){
    this->dir_name = dir_name;
    mem = new memtable(2,1);
    // 获取目录下的.index文件
    index_files = getIndexFiles(dir_name);
    // 按照时间升序排序
    std::sort(index_files.begin(),index_files.end(),[](const std::string& a,const std::string& b){
        return a < b;
    });
}

void db::write(std::string key, std::string value) {
    mem->write(key,value);
}

search_result db::search(std::string key) {
    search_result res = mem->search(key);
    if(res.res_code != -1){
        return res;
    }
    // 查找文件
    if(index_files.size() == 0){
        return res;
    }
    // 从后往前查，优先查到更新的数据
    index_mtx.lock();
    for(int i = index_files.size()-1;i >= 0;i--){
        kv_search kvs = kv_search( index_files[i]);
        search_result tres = kvs.search(key);
        if(tres.res_code != -1){
            res = tres;
            break;
        }
    }
    index_mtx.unlock();
    return res;
}

void db::cron_flush_disk() {
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        int index = mem->flush_disk();
        if(index != -1){
            std::cout << "add index " << index << std::endl;
            index_mtx.lock();
            index_files.push_back(dir_name + std::to_string(index));
            index_mtx.unlock();
        }
    }
}

db::~db() {
    worker_thread.join();
    delete mem;
}


