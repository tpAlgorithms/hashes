#include <vector>
#include <iostream>


typedef std::string data_t;
typedef std::string key_t;

size_t hash_func(const key_t &key) {
  size_t c = 12343;
  size_t a = 31247;
  size_t b = 42589;

  size_t value = c;
  size_t multiplier = 1;
  for (size_t i = 0; i < key.size(); ++i) {
    value += key[i] * multiplier;
    value = value % b;

    multiplier = (multiplier * a) % b;
  }

  return value % b;
}



struct chain_hash_t {
  typedef std::pair<key_t, data_t> pair_t;

  typedef std::vector< std::vector<pair_t> > storage_t;


  storage_t data;
};

typedef chain_hash_t::pair_t pair_t;
typedef chain_hash_t::storage_t storage_t;

void hash_init(chain_hash_t &hash, size_t size) {
  hash.data.resize(size);
  std::cout << "resize: "<< hash.data.size() << std::endl;
}

void hash_clear(chain_hash_t &hash) {
  hash.data.clear();
}

void hash_insert(chain_hash_t &hash, const pair_t& pair) {
  size_t hash_value = hash_func(pair.first);
  std::cout << "hash(" << pair.first << ") = " << hash_value << std::endl;
  hash.data[hash_value % hash.data.size()].push_back(pair);
  std::cout << "do insert: " << hash.data[hash_value % hash.data.size()].size() << std::endl;
}
const pair_t *hash_find(chain_hash_t &hash, const key_t &key) {
  size_t hash_value = hash_func(key);
  std::cout << "hash(" << key << ") = " << hash_value << std::endl;

  const std::vector<pair_t> &chain = hash.data[hash_value % hash.data.size()];
  if (chain.empty()) {
    std::cout << "case 1" << std::endl; 
    return NULL;
  } else {
    for (size_t i = 0; i < chain.size(); ++i) {
      if (chain[i].second == key) {
        return &chain[i];
    std::cout << "case 2" << std::endl; 
      }
    }
    return NULL;
    std::cout << "case 3" << std::endl; 
  }
}

size_t hash_remove(chain_hash_t &hash, const key_t &key) {
  size_t hash_value = hash_func(key);
  size_t cnt = 0;

  std::vector<pair_t> &chain = hash.data[hash_value % hash.data.size()];
  if (chain.empty()) { 
    return 0;
  } else {
    for (size_t i = 0; i < chain.size();) {
      if (chain[i].second == key) {
        chain.erase(chain.begin() + i );
        ++cnt;
      } else {
        ++i;
      }
    }
    return cnt;
  }
}

int main() {

  size_t count;
  std::cin >> count;

  chain_hash_t data;
  hash_init(data, 100);

  for (size_t i = 0;i < count; ++i) {
    std::string key;
    std::string value;
    std::cin >> key >> value;
    
    const pair_t *pair_ptr = hash_find(data, key);
    if (pair_ptr == NULL) {
      std::cout << "INSERT: " << std::endl;
      hash_insert(data, std::make_pair(key, value));
    } else {
      std::cout << "REMOVE: " << pair_ptr->second;
      size_t cnt = hash_remove(data, key );
      std::cout << "\t" << cnt << std::endl;
    }
  }

  return 0;
}
