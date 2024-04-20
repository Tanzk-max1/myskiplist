#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>

#define STORE_FILE "store/dumpFile"//定义数据保存和加载时的文件路径
std::mutex mtx;     // 定义互斥锁
std::string delimiter = ":";

//node类结构
template <typename K,typename V>
class Node {
public:
    Node() {};
    Node(K k,V v,int) ;
    ~Node();
    K get_key() const ;
    V get_value() const ;
    void set_value(V);
    

    Node<K,V> **forward;//在c++中，二维指针等于指针数组
    int node_level;
private:
    K key;
    V value;
    //key和value，分别存储键和值
};

template <typename K,typename V>
Node<K,V>::Node(const K k,const V v,int level) {
    this->key = k;
    this->value = v;
    this->node_level = level;
    this->forward = new Node<K,V> *[level + 1];
    memset(this->forward,0,sizeof(Node<K,V> *) * (level + 1));
};

// 类拥有的析构函数
template <typename K,typename V>
Node<K,V>::~Node() {
    delete[] this->forward;
};
// 类拥有的获取 key 成员函数
template <typename K,typename V>
K Node<K,V>::get_key() const {
    return key;

}
// 类拥有的获取 value 成员函数
template <typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};
// 类拥有的设置 value 成员函数
template <typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value = value;
};

template <typename K, typename V>
class SkipList{
public:
    SkipList(int);                      // 构造函数
    ~SkipList();                        // 析构函数
    int get_random_level();             // 获取节点的随机层级
    Node<K, V> *create_node(K, V, int); // 节点创建
    int insert_element(K, V);           // 插入节点
    void display_list();                // 展示节点
    bool search_element(K);             // 搜索节点
    void delete_element(K);             // 删除节点
    void dump_file();                   // 持久化数据到文件
    void load_file();                   // 从文件加载数据
    void clear(Node<K, V> *);           // 递归删除节点
    int size();                         // 跳表中的节点个数

private:
    void get_key_value_from_string(const std::string &str, std::string *key, std::string *value);
    bool is_valid_string(const std::string &str);

private:
    int _max_level;              // 跳表允许的最大层数
    int _skip_list_level;        // 跳表当前的层数
    Node<K, V> *_header;         // 跳表的头节点
    int _element_count;          // 跳表中组织的所有节点的数量
    std::ofstream _file_writer;  // 文件写入器
    std::ifstream _file_reader;  // 文件读取器

};

template<typename K, typename V> 
int SkipList<K, V>::size() { 
    return _element_count;
}

template<typename K,typename V>
int SkipList<K,V>::get_random_level(){
    int k = 1;
    while (rand()%2)
    {
        k++;
    }
    k = (k < _max_level)? k :_max_level;
    // 判断变量 k 是否小于 _max_level。如果条件成立（即 k 小于 _max_level），则取 k 的值作为结果；
    //如果条件不成立（即 k 大于或等于 _max_level），则取 _max_level 的值作为结果。
    return k;
};

template<typename K,typename V>
SkipList<K,V>::SkipList(int max_level) {
    this->_max_level = max_level;// 设置最大层级
    this->_skip_list_level = 0;// 初始化当前层级为 0
    this->_element_count = 0; // 初始化节点计数为 0
    K k; // 默认键
    V v; // 默认键
    this->_header = new Node<K,V>(k,v,_max_level);
    // 创建头节点，并初始化键值为默认值
};

template<typename K,typename V>
Node<K,V> *SkipList<K,V>::create_node(const K k,const V v,int level){
    // 实例化新节点，并为其分配指定的键、值和层级
    Node<K, V> *n = new Node<K, V>(k, v, level);
    // 定义一个指针 current，, V> *n = new Node<K, V>(k, v, level);
    return n; // 返回新创建的节点
}

template<typename K ,typename V>
bool SkipList<K, V>::search_element(K key) {

    Node<K,V> *current = _header;
    // 从跳表的最高层开始搜索
    for (int i = _skip_list_level; i >= 0; i--)
    {// 遍历当前层级，直到下一个节点的键值大于或等于待查找的键值
       while(current->forward[i] && current->forward[i]->get_key() <key)
            current = current -> forward[i]; 
    }
    // 当前节点的下一个节点的键值大于待查找的键值时，进行下沉到下一层
        // 下沉操作通过循环的 i-- 实现
    current = current->forward[0];
    if (current && current->get_key() == key) {
        // 如果找到匹配的键值，返回 true
        return true;
    }
    // 如果没有找到匹配的键值，返回 false
    return false;
}

template <typename K,typename V>
int SkipList<K,V>::insert_element(const K key,const V value) {
    mtx.lock();
    Node<K,V> *current = this->_header;
    //用于在各层更新指针的数组

    Node<K,V> *update[_max_level + 1];//用于记录每层中待更新的指针的节点
    memset(update, 0, sizeof(Node<K, V> *) * (_max_level + 1));
    //记录大小，_max_level + 1 个 Node<K, V>* 类型指针所占用的总字节数

    //从最高层向下搜索插入的位置
    for (int i = _skip_list_level; i >= 0; i++)
    {
        //寻找当前层中最接近且小于key的节点
        while(current->forward[i] != NULL && current->forward[i] -> get_key() < key){
            current = current -> forward[i];//移动到下一个节点
        }
        //保存每层中该节点，以便后续插入时更新指针
        update[i] = current;
    }

    //移动到最底层的下一节点，准备插入操作
    current = current -> forward[0];
    //检查待的插入的节点的键是否已经存在
    if (current != NULL && current->get_key() == key) {
        // 键已存在，取消插入
        mtx.unlock();
        return 1;
    }

    // 检查待插入的节点是否已存在于跳表中
    if(current == NULL || current->get_key() != key){
        // 通过随机函数决定新节点的层级高度
        int random_level = get_random_level();
        //如果新节点的层级超出了跳表的当前最高层级
        if (random_level > _skip_list_level) {
            // 对所有新的更高层级，将头节点设置为它们的前驱节点
            for (int i = _skip_list_level + 1; i <= random_level; i++) {
                update[i] = _header;
            }
            // 更新跳表的当前最高层级为新节点的层级
            _skip_list_level = random_level;
        }

        Node<K, V> *inserted_node = create_node(key, value, random_level);
        //在各层插入新节点，同时更新前去节点的forward指针
        for(int i = 0;i <= random_level;i++){
            // 新节点指向当前节点的下一个节点
            inserted_node->forward[i] = update[i]->forward[i];
            // 当前节点的下一个节点更新为新节点
            update[i]->forward[i] = inserted_node;
        }
        _element_count++;
    }
    mtx.unlock();
    return 0;
}
template <typename K,typename V>
void SkipList<K,V>::delete_element(K key){
    mtx.lock();  // 加锁
    Node<K, V> *current = this->_header;
    Node<K, V> *update[_max_level + 1];
    // update 数组记录每层待删除节点的前驱节点，以便更新指针关系。
    memset(update,0,sizeof(Node<K,V> *) * (_max_level + 1));

    // 从最高层开始向下搜索待删除节点
    for(int i = _skip_list_level; i>=0;i--){
        while(current->forward[i] != NULL && current->forward[i] -> get_key() < key) {
            current = current -> forward[i];
        }
        update[i] = current; //记录每层待删除节点的前驱
    }
    current = current->forward[0];

    //确认找到了待删除的节点
    if (current != NULL && current->get_key() == key) {
        // 逐层更新指针，移除节点
        for (int i = 0; i <= _skip_list_level; i++) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        // 调整跳表的层级
        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == NULL) {
            _skip_list_level--;
        }
        delete current; // 释放节点占用的内存
        _element_count--; // 节点计数减一
    }
    mtx.unlock();  // 解锁
    return;
}


template <typename K, typename V>
void SkipList<K, V>::display_list() {
    // 从最上层开始向下遍历所有层
    for (int i = _skip_list_level; i >= 0; i--) {
        Node<K, V>* node = this->_header->forward[i]; // 获取当前层的头节点
        std::cout << "Level " << i << ": ";
        // 遍历当前层的所有节点
        while (node != nullptr) {
            // 打印当前节点的键和值，键值对之间用":"分隔
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            // 移动到当前层的下一个节点
            node = node->forward[i];
        }
        std::cout << std::endl; // 当前层遍历结束，换行
    }
}

template <typename K,typename V>
void SkipList<K,V>::dump_file(){
    _file_writer.open(STORE_FILE); //打开文件
    Node<K,V>* node = this-> _header->forward[i];
    //从头结点开始遍历

    while (node!=nullptr) {
        _file_writer << node->get_key() << ":"  << node->get_value() << ";\n";
        //写入键值对
        node = node -> forward[0];
    }
    _file_writer.flush();//刷新缓冲区，确保完全写入
    _file_writer.close(); // 关闭文件
}

template <typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {
    return !str.empty() && str.find(delimiter) != std::string::npos;
    //用于验证字符串的合法性。检查字符串是否为空，以及是否包含分隔符:。
}

template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string* key, std::string* value) {
    if (!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1);
}

template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string &str, std::string *key, std::string *value) {
    if (!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1, str.length());
}

template <typename K, typename V>
void SkipList<K, V>::load_file() {
    _file_reader.open(STORE_FILE);
    std::string line;
    std::string *key = new std::string();
    std::string *value = new std::string();

    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        // Define key as int type
        insert_element(stoi(*key), *value);
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }

    delete key;
    delete value;
    _file_reader.close();
}