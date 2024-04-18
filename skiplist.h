#include <iostream> 
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>

//node类结构
template <typename K,typename V>
class Node {
public:
    Node() {};
    Node(K k,V v,int) ;
    ~Node();
    K get_key() const ;
    V get_value() const ;
    void set_key(V) ;

    Node<K,V> **forward;//在c++中，二维指针等于指针数组
    int node_level;
private:
    K key;
    V value;
    //key和value，分别存储键和值
}

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
    int _max_level;              // 跳表允许的最大层数
    int _skip_list_level;        // 跳表当前的层数
    Node<K, V> *_header;         // 跳表的头节点
    int _element_count;          // 跳表中组织的所有节点的数量
    std::ofstream _file_writer;  // 文件写入器
    std::ifstream _file_reader;  // 文件读取器

};

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
}