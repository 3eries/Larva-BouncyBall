//
//  SBCollection.hpp
//  libcocos2d iOS
//
//  Created by hwangseongmin on 2017. 10. 22..
//

#ifndef SBCollection_hpp
#define SBCollection_hpp

#include <stdio.h>

#include "cocos2d.h"

#include <vector>
#include <map>
#include <functional>

#define CONTAINS_KEY(__data__, __key__)    SBCollection::containsKey(__data__, __key__)

class SBCollection {
public:
    /**
     * map
     */
    template <class K, class V>
    static std::vector<V> mapToVector(std::map<K, V> map) {
        
        std::vector<V> vector;
        
        for( auto it = map.begin(); it != map.end(); ++it ) {
            vector.push_back(it->second);
        }
        
        return vector;
    }
    
    /**
     * contains
     */
    template <class K, class V>
    static bool containsKey(std::map<K, V> data, K key) {
        return data.find(key) != data.end();
    }

    template <class K>
    static bool containsKey(cocos2d::ValueMap data, K key) {
        return data.find(key) != data.end();
    }
    
    template <class T>
    static bool contains(std::vector<T> data, T obj) {
        return(std::find(data.begin(), data.end(), obj) != data.end());
    }

    /**
     * find
     */
    template <class T, class F>
    static std::vector<T> find(std::vector<T> data, F func) {
        
        std::vector<T> findData;
        copy_if(data.begin(), data.end(), back_inserter(findData), func);
        
        return findData;
    }
    
    template <class T, class F>
    static cocos2d::Vector<T> find(cocos2d::Vector<T> datas, F func) {
        
        cocos2d::Vector<T> findDatas;
        
        for( auto data : datas ) {
            if( func(data) ) {
                findDatas.pushBack(data);
            }
        }
        
        return findDatas;
    }
    
    template <class T>
    static ssize_t getIndex(std::vector<T> datas, T obj) {
        
        auto it = std::find(datas.begin(), datas.end(), obj);
        
        if( it != datas.end() ) {
            return it - datas.begin();
        }
        
        return -1;
    }
    
    /**
     * add
     */
    template <class T>
    static void addAll(std::vector<T> &target, std::vector<T> list) {
        
        for( auto data : list ) {
            target.push_back(data);
        }
    }
    
    /**
     * insert
     */
    template <class T>
    static void insert(std::vector<T> &datas, size_t i, T obj) {
        
        datas.insert((std::begin(datas) + i), obj);
    }
    
    /**
     * remove
     */
    template <class T, class F>
    static void remove(std::vector<T> &datas, F func) {
        
        for( auto it = datas.begin(); it != datas.end(); ) {
            if( func(*it) ) {
                datas.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    template <class T, class F>
    static void remove(cocos2d::Vector<T> &datas, F func) {
        
        auto removeDatas = find(datas, func);

        for( auto data : removeDatas ) {
            datas.eraseObject(data);
        }

        removeDatas.clear();
    }
    
    template <class T>
    static void removeAtIndex(std::vector<T> &datas, size_t index) {
        datas.erase(datas.begin() + index);
    }
    
    template <class T>
    static void remove(std::vector<T> &datas, T obj) {
        
        auto it = std::find(datas.begin(), datas.end(), obj);
        
        if( it != datas.end() ) {
            datas.erase(it);
        }
    }
    
    /**
     * isEqual
     */
    template <class T, class F>
    static bool isEqual(std::vector<T> list1, std::vector<T> list2, F func) {
        
        if( list1.size() != list2.size() ) {
            return false;
        }
        
        for( int i = 0; i < list1.size(); ++i ) {
            auto o1 = list1[i];
            auto o2 = list2[i];
            
            if( !func(o1,o2) ) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * random
     */
    template <class T>
    static std::vector<T> createRandomList(std::vector<T> src, size_t size) {
        
        auto getRandomIndexList = [=](int t) -> std::vector<int> {
            
            std::vector<int> indexList;
            
            for( int i = 0; i < src.size(); ++i ) {
                indexList.push_back(i);
            }
            
            // 중복 방지
            removeAtIndex(indexList, t);
            std::random_shuffle(indexList.begin(), indexList.end());
            
            int insertIdx = (arc4random() % (indexList.size()-1)) + 1;
            insert(indexList, insertIdx, t);
            
            return indexList;
        };
        
        std::vector<T> randomList;
        int lastIdx = 0;
        
        while( true ) {
            bool isBreak = false;
            
            auto indexList = getRandomIndexList(lastIdx);
            lastIdx = indexList[indexList.size()-1];
            
            for( int i = 0; i < indexList.size(); ++i ) {
                randomList.push_back(src[indexList[i]]);
                
                if( randomList.size() == size ) {
                    isBreak = true;
                    break;
                }
            }
            
            if( isBreak ) {
                break;
            }
        }
        
        return randomList;
    }
    
    /**
     * convert
     */
    template <class T, class C>
    static std::vector<C> convert(std::vector<T> datas) {
        
        std::vector<C> convertDatas;
        
        for( auto data : datas ) {
            convertDatas.push_back(static_cast<C>(data));
        }
        
        return convertDatas;
    }
};

#endif /* SBCollection_hpp */
