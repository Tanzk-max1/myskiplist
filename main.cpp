// #include <iostream>
// #include <cstdlib>
// #include <cmath>
// #include <cstring>
// #include "skiplist.h"
// #define FILE_PATH "./store/dumpFile"

// int main() {

//     // 键值中的key用int型，如果用其他类型，需要自定义比较函数
//     // 而且如果修改key的类型，同时需要修改skipList.load_file函数
//     SkipList<int, std::string> skipList(6);
// 	skipList.insert_element(1, "学"); 
// 	skipList.insert_element(3, "算法"); 
// 	skipList.insert_element(7, "认准"); 
// 	skipList.insert_element(8, "微信公众号：代码随想录"); 
// 	skipList.insert_element(9, "学习"); 
// 	skipList.insert_element(19, "算法不迷路"); 
// 	skipList.insert_element(19, "赶快关注吧你会发现详见很晚！"); 

//     std::cout << "skipList size:" << skipList.size() << std::endl;

//     //skipList.dump_file();

//     // skipList.load_file();

//     //skipList.search_element(9);
//     //skipList.search_element(18);


//     //skipList.display_list();

//     //skipList.delete_element(3);
//     //skipList.delete_element(7);

//     std::cout << "skipList size:" << skipList.size() << std::endl;

//   //  skipList.display_list();
// }

#include <iostream>
#include "skiplist.h"
#define FILE_PATH "./store/dumpFile"

int main() {

    // 键值中的key用int型，如果用其他类型，需要自定义比较函数
    // 而且如果修改key的类型，同时需要修改skipList.load_file函数
    SkipList<int, std::string> skipList(6);
	skipList.insert_element(1, "学"); 
	skipList.insert_element(3, "算法"); 
	skipList.insert_element(7, "认准"); 
	skipList.insert_element(8, "微信公众号：代码随想录"); 
	skipList.insert_element(9, "学习"); 
	skipList.insert_element(19, "算法不迷路"); 
	skipList.insert_element(19, "赶快关注吧你会发现详见很晚！"); 

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();

    // skipList.load_file();

    skipList.search_element(9);
    skipList.search_element(18);


    skipList.display_list();

    skipList.delete_element(3);
    skipList.delete_element(7);

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_list();
}

// int main() {
//     int N, K, M;

//     std::cin >> N >> K >> M;

//     SkipList<int, int> *skiplist = new SkipList<int, int>(16);

//     // 插入数据
//     for (int i = 0; i < N; i++) {
//         int k, v;
//         std::cin >> k >> v;
//         if (skiplist->insert_element(k, v) == 0) {
//             std::cout << "Insert Success" << std::endl;
//         } else {
//             std::cout << "Insert Failed" << std::endl;
//         }
//     }

//     // 删除数据
//     for (int i = 0; i < K; i++) {
//         int k;
//         std::cin >> k;
//         skiplist->delete_element(k);
//     }

//     // 查找数据
//     for (int i = 0; i < M; i++) {
//         int k;
//         std::cin >> k;
//         if (skiplist->search_element(k)) {
//             std::cout << "Search Success" << std::endl;
//         } else {
//             std::cout << "Search Failed" << std::endl;
//         }
//     }
//     return 0;
// }