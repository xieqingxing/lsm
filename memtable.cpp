//
// Created by xieqingxing on 2025/1/20.
//

#include "memtable.h"
#include "index.h"
#include <thread>

void memtable::write(std::string key, std::string value) {
    write_mtx.lock();
    item *td = new item(key,value);
    data.push_back(*td);
    delete td;
//    if(data.size() >= memMaxLength){
//        refresh_mem();
//    }
    write_mtx.unlock();
}

void memtable::show_mem() {
    for(auto & i : data){
        i.print();
    }
}

void memtable::refresh_mem() {
    refresh_mtx.lock();
    if(data.size()==0){
        refresh_mtx.unlock();
        return;
    }
    std::cout << "refresh mem length: " << data.size() << std::endl;
    // 深拷贝
    std::vector<item> copy_data = data;
    data.clear();
    // 排序
    std::sort(copy_data.begin(),copy_data.end(),[](const item& a,const item& b){
        return a.key < b.key;
    });
    // 去除重复的key，只保留最新版本
    std::string pkey = copy_data[0].key;
    std::vector<item> nd;
    nd.push_back(copy_data[0]);
    for(int i =1;i < copy_data.size();i++){
        if(copy_data[i].key == pkey){
            nd[nd.size()-1] = copy_data[i];
        }
        pkey = copy_data[i].key;
    }
    // 添加到已排序的数组中
    sorted_data.push_back(nd);
    refresh_mtx.unlock();
}

void memtable::cron_refresh_mem(std::atomic<bool>& stop_flag) {
    while(true){
        refresh_mem();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


memtable::memtable(int l,int l2) :
   worker_thread(&memtable::cron_refresh_mem, this, std::ref(stop_flag)),stop_flag(false){
    memMaxLength = l;
    writer = new disk_writer("storage");
    flushDiskLength = l2;
}

int memtable::flush_disk() {
    refresh_mtx.lock();
    if(sorted_data.size() == 0){
        refresh_mtx.unlock();
        return -1;
    }
    flush_mtx.lock();
    std::cout << "flush disk" << std::endl;
    // 合并排序，去除同key的数据;
    std::vector<item> new_data;
    int data_index[sorted_data.size()];
    std::memset(data_index,0,sizeof(int));
    while(true){
        int fi = 0;
        item currentItem;
        // 找到第一个没写入的元素
        bool found = false;
        for(int i =0;i < sorted_data.size();i++){
            if(data_index[i] < sorted_data[i].size()){
                fi = i;
                currentItem = sorted_data[i][data_index[i]];
                found = true;
                break;
            }
        }
        if(!found){
            break;
        }
        for(int i =0;i < sorted_data.size();i++){
            if(i == fi){
                continue;
            }
            if(sorted_data[i][data_index[i]].key < currentItem.key){
                currentItem = sorted_data[i][data_index[i]];
                fi = i;
                continue;
            }
            if(sorted_data[i][data_index[i]].key == currentItem.key){
                // 等于的情况下，保留更新的
                currentItem = sorted_data[i][data_index[i]];
                data_index[fi]++;
                fi = i;
            }
        }
        new_data.push_back(currentItem);
        data_index[fi]++;
    }
    sorted_data.clear();
    // 写入数据
    int index = writer->write_in_new_file(new_data);
    refresh_mtx.unlock();
    flush_mtx.unlock();
    return index;
}

search_result memtable::search(std::string key) {
    // 1. 查询sorted_datat
    refresh_mtx.lock();
    search_result search_res;
    search_res.res_code = -1;
    // 从后往前遍历
    for(int i = sorted_data.size() - 1;i >= 0;i--){
        for(int j = sorted_data[i].size()-1;j>=0;j--){
            if(sorted_data[i][j].key == key){
                search_res.i = sorted_data[i][j];
                search_res.res_code = 1;
                return search_res;
            }
        }
    }
    refresh_mtx.unlock();
    return search_res;
}

memtable::~memtable() {
    stop_flag = true; // 设置停止标志
    if (worker_thread.joinable()) {
        worker_thread.join(); // 等待线程结束
    }
    delete writer; // 清理资源
}

void search_result::print() {
    std::cout << "res_code: " << res_code << std::endl;
    i.print();
}
