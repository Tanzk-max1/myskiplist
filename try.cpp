#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>

template <typename K, typename V>
class Node {
public:
    Node() {}
    Node(K k, V v, int);
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V);
    Node<K, V> **forward;
    int node_level;
private:
    K key;
    V value;
};

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->node_level = level;
    this->forward = new Node<K, V> *[level + 1];
    memset(this->forward, 0, sizeof(Node<K, V> *) * (level + 1));
};

template <typename K, typename V>
Node<K, V>::~Node() {
    delete[] forward;
};

template <typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};

template <typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};

template <typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value = value;
};

// 跳表结构
template <typename K, typename V>
class SkipList {
public:
    SkipList(int);
    int get_random_level();
    Node<K, V> *create_node(K, V, int);
    int insert_element(K, V);
    bool search_element(K);
    void delete_element(K);
private:
    int _max_level;
    int _skip_list_level;
    Node<K, V> *_header;
    int _element_count;
};

template <typename K, typename V>
Node<K, V> *SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}

template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {
    Node<K, V> *current = this->_header;
    Node<K, V> *update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V> *) * (_max_level + 1));
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (current != NULL && current->get_key() == key) {
        return 1;
    }
    if (current == NULL || current->get_key() != key) {
        int random_level = get_random_level();
        if (random_level > _skip_list_level) {
            for (int i = _skip_list_level + 1; i < random_level + 1; i++) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }
        Node<K, V> *inserted_node = create_node(key, value, random_level);
        for (int i = 0; i <= random_level; i++) {
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        _element_count++;
    }
    return 0;
}

template <typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    Node<K, V> *current = _header;
    for (int i = _skip_list_level; i >= 0; i--)
    {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current and current->get_key() == key) {
        return true;
    }
    return false;
}

template <typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
    Node<K, V> *current = this->_header;
    Node<K, V> *update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V> *) * (_max_level + 1));

    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != NULL && current->get_key() == key) {
        for (int i = 0; i <= _skip_list_level; i++) {
            if (update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }
        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) {
            _skip_list_level--;
        }
        delete current;
        _element_count--;
    }
    return;
}

template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;
    K k;
    V v;
    this->_header = new Node<K, V>(k, v, _max_level);
};

template<typename K, typename V>
int SkipList<K, V>::get_random_level(){
    int k = 1;
    while (rand() % 2) {
        k++;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
};

int main() {
    int N, K, M;

    std::cin >> N >> K >> M;

    SkipList<int, int> *skiplist = new SkipList<int, int>(16);

    // 插入数据
    for (int i = 0; i < N; i++) {
        int k, v;
        std::cin >> k >> v;
        if (skiplist->insert_element(k, v) == 0) {
            std::cout << "Insert Success" << std::endl;
        } else {
            std::cout << "Insert Failed" << std::endl;
        }
    }

    // 删除数据
    for (int i = 0; i < K; i++) {
        int k;
        std::cin >> k;
        skiplist->delete_element(k);
    }

    // 查找数据
    for (int i = 0; i < M; i++) {
        int k;
        std::cin >> k;
        if (skiplist->search_element(k)) {
            std::cout << "Search Success" << std::endl;
        } else {
            std::cout << "Search Failed" << std::endl;
        }
    }
    return 0;
}