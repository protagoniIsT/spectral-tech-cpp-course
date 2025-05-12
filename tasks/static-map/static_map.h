#pragma once

#include <vector>

template<class K, class V>
class StaticMap {
public:
    explicit StaticMap(std::vector<std::pair<K, V>> items) : items_(items) {
        sort_by_key(items_);
    }

    template<class Iterator>
    StaticMap(Iterator first, Iterator last) : items_(first, last) {
        sort_by_key(items_);
    }

    const V* Find(const K& key) const {
        auto bound = std::lower_bound(items_.begin(), items_.end(), key, [](const std::pair<K, V>& p, const K& key) {
            return p.first < key;
        });
        if (bound != items_.end() && !(bound->first < key) && !(key < bound->first)) {
            return &bound->second;
        }
        return nullptr;
    }

private:
    std::vector<std::pair<K, V>> items_;

    static void sort_by_key(std::vector<std::pair<K, V>>& items) {
        std::sort(items.begin(), items.end(), [](const std::pair<K, V>& p1, const std::pair<K, V>& p2) {
            return p1.first < p2.first;
        });
    }
};
