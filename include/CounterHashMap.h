//
// Created by Olcay Taner YILDIZ on 24/10/2018.
//

#ifndef DATASTRUCTURE_COUNTERHASHMAP_H
#define DATASTRUCTURE_COUNTERHASHMAP_H

#include <map>
#include <string>

using namespace std;

template <class K> class CounterHashMap : public map<K, int> {
public:
    CounterHashMap();
    void put(K key);
    void putNTimes(K key, int N);
    bool containsKey(K key);
    int count(K key);
    int sumOfCounts();
    K max();
    K max(double threshold);
    void add(CounterHashMap<K> toBeAdded);
    vector<pair<K, int>> topN(int N);
    string to_string();
};

/**
 * A constructor which calls its super.
 */
template<class K> CounterHashMap<K>::CounterHashMap() {
}

template<class K> bool CounterHashMap<K>::containsKey(K key) {
    return this->find(key) != this->end();
}

/**
 * The put method takes a K type input. If this map contains a mapping for the key, it puts this key after
 * incrementing its value by one. If his map does not contain a mapping, then it directly puts key with the value of 1.
 *
 * @param key to put.
 */
template<class K> void CounterHashMap<K>::put(K key) {
    if (containsKey(key)) {
        this->insert_or_assign(key, this->find(key)->second + 1);
    } else {
        this->emplace(key, 1);
    }
}

/**
 * The putNTimes method takes a K and an integer N as inputs. If this map contains a mapping for the key, it puts this key after
 * incrementing its value by N. If his map does not contain a mapping, then it directly puts key with the value of N.
 *
 * @param key to put.
 * @param N   to increment value.
 */
template<class K> void CounterHashMap<K>::putNTimes(K key, int N) {
    if (containsKey(key)) {
        this->insert_or_assign(key, this->find(key)->second + N);
    } else {
        this->emplace(key, N);
    }
}

/**
 * The count method takes a K as input, if this map contains a mapping for the key, it returns the value corresponding
 * this key, 0 otherwise.
 *
 * @param key to get value.
 * @return the value corresponding given key, 0 if it is not mapped.
 */
template<class K> int CounterHashMap<K>::count(K key) {
    if (containsKey(key)) {
        return this->find(key)->second;
    } else {
        return 0;
    }
}

/**
 * The sumOfCounts method loops through the values contained in this map and accumulates the counts of this values.
 *
 * @return accumulated counts.
 */
template<class K> int CounterHashMap<K>::sumOfCounts() {
    int sum = 0;
    for (auto item = this->begin(); item != this->end(); item++) {
        sum += item->second;
    }
    return sum;
}

/**
 * The max method loops through the mappings contained in this map and if the current entry's count value is greater
 * than maxCount, which is initialized as 0, it updates the maxCount as current count and maxKey as the current count's
 * key.
 *
 * @return K type maxKey which is the maximum valued key.
 */
template<class K> K CounterHashMap<K>::max() {
    int maxCount = 0;
    K maxKey;
    for (auto item = this->begin(); item != this->end(); item++) {
        int count = item->second;
        if (count > maxCount) {
            maxCount = count;
            maxKey = item->first;
        }
    }
    return maxKey;
}

/**
 * The max method takes a threshold as input and loops through the mappings contained in this map. It accumulates the
 * count values and if the current entry's count value is greater than maxCount, which is initialized as 0,
 * it updates the maxCount as current count and maxKey as the current count's key.
 * <p>
 * At the end of the loop, if the ratio of maxCount/total is greater than the given threshold it returns maxKey, else null.
 *
 * @param threshold double value.
 * @return K type maxKey if greater than the given threshold, null otherwise.
 */
template<class K> K CounterHashMap<K>::max(double threshold) {
    int maxCount = 0, total = 0;
    K maxKey;
    for (auto item = this->begin(); item != this->end(); item++) {
        int count = item->second;
        total += count;
        if (count > maxCount) {
            maxCount = count;
            maxKey = item->first;
        }
    }
    if (maxCount / (total + 0.0) > threshold) {
        return maxKey;
    } else {
        return nullptr;
    }
}

/**
 * The add method adds value of each key of toBeAdded to the current counterHashMap.
 *
 * @param toBeAdded CounterHashMap to be added to this counterHashMap.
 */
template<class K> void CounterHashMap<K>::add(CounterHashMap<K> toBeAdded) {
    for (auto item = this->begin(); item != this->end(); item++){
        putNTimes(item->first, toBeAdded.find(item->first)->second);
    }
}

/**
 * The topN method takes an integer N as inout. It creates an {@link vector} result and loops through the the
 * mappings contained in this map and adds each entry to the result {@link vector}. Then sort this {@link vector}
 * according to their values and returns an {@link vector} which is a sublist of result with N elements.
 *
 * @param N Integer value for defining size of the sublist.
 * @return a sublist of N element.
 */
template<class K> vector<pair<K, int>> CounterHashMap<K>::topN(int N) {
    vector<pair<K, int>> result;
    for (auto item = this->begin(); item != this->end(); item++) {
        result.push_back(item);
    }
    sort(result.begin(), result.end(),
         [](pair<K, int> o1, pair<K, int> o2) -> bool
         {
             return o1.second > o2.second;
         });
    return vector<pair<K, int>>(result.begin(), result.begin() + N);
}

/**
 * The to_string method loops through the mappings contained in this map and returns the string of each entry's key and value.
 *
 * @return String of the each entry's key and value.
 */
template<class K> string CounterHashMap<K>::to_string() {
    string result = "";
    for (auto item = this->begin(); item != this->end(); item++) {
        result = result + item->first.to_string() + ":" + item->second.to_string() + "-";
    }
    return result;
}

#endif //DATASTRUCTURE_COUNTERHASHMAP_H
