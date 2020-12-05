#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <algorithm>
#include "account.h"
/*!
 * Receives a integer and finds the next prime (or, in case the number itself already is a prime, finds itself).
 * Special cases may occur in which one prime is skipped, but they are not common as common cases have been treated.
 * \param anchor an integer that sets the starting point for the prime search.
 * @return the first prime found.
 */
int find_prime(int anchor){
    bool isPrime = false;
    if(anchor == 4){
        return 5;
    }
    while(not isPrime){
        for(int i = 2; i < anchor/2; i++){
            if(anchor % i == 0){
                isPrime = false;
                break;
            }
            else{
                isPrime = true;
            }
        }
        anchor++;
    }
    return anchor-1;
}

namespace ac{
    /*!
    * Encompasses an internal class for storing entrys to the HashTable. It is meant to be used as an internal class for the HashTbl class.
    * Methods include constructors, ostream operators and equal operations.
    */
    template<typename KeyType, typename DataType>
    class HashEntry{
        public:
            HashEntry(KeyType k_, DataType d_) : m_key(k_), m_data(d_){};
            HashEntry():m_key(0), m_data(0){} ;
            KeyType m_key;
            DataType m_data;
            friend std::ostream& operator<<(std::ostream& os, const HashEntry<KeyType, DataType> Entry){
                os<<Entry.m_data;
                return os;
            }
            DataType getData(){
                return m_data;
            }
            bool operator==(HashEntry<KeyType, DataType> other){
                if(this->m_key == other.m_key){
                    return true;
                }
                else{
                    return false;
                }
            }
    };

    /*!
    * An implementation of the HashTbl class. It can be used with any kind of datatype or class.
    * It accepts, but doesn't require a functor to generate equality and hashing. Usage of such arguments is reccomended.
    */
    template<class KeyType, class DataType, typename KeyHash = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType>>
    class HashTbl{
        public:
            using Entry = HashEntry<KeyType, DataType>;
            /*!
            * Default constructor, creates a HashTbl of <size>. If <size> is not given, it defaults to 11.
            * \param tbl_size the parameter that determines the size of the HashTbl, bigger values generate overall better results. It is not mandatory, and if none is given, defaults to 11.
            */
            HashTbl(size_t tbl_size = DEFAULT_SIZE){
                m_data_table = std::make_unique<std::list<Entry>[]>(find_prime(tbl_size));
                m_size = find_prime(tbl_size);
                m_count = 0;
            };
            /*!
            * Default destructor. Summons clear method to eliminate all nodes of the tables. No further action is required as unique_ptr takes care of itselft.
            */
            ~HashTbl(){
                this->clear();
                //std::unique_ptr does not require deletion
            }
            /*!
            * Copy constructor, copies all data, size, and count of the other list.
            * \param other the list being copied.
            */
            HashTbl(const HashTbl<KeyType, DataType>& other){
                m_size = other.size();
                m_data_table = std::make_unique<std::list<Entry>[]>(find_prime(m_size));
                for(int i = 0; i < other.m_size; i++){
                    auto list = other.listCol(i);
                    for(auto entry : list){
                        this->insert(entry.m_key, entry.m_data);
                    }
                }
            }
            /*!
            * Initalizer list constructor, creates a default size list and fills it with ilist's contents.
            * \param ilist the list being copied.
            */
            HashTbl(std::initializer_list<Entry> ilist){
                m_data_table = std::make_unique<std::list<Entry>[]>(find_prime(DEFAULT_SIZE));
                m_size = DEFAULT_SIZE;
                m_count = 0;
                for(auto entry : ilist){
                    this->insert(entry.m_key, entry.m_data);
                }
            }
            /*!
            * Copy constructor, copies all data, size, and count of the other list.
            * \param other the list being copied.
            */
            HashTbl<KeyType, DataType>& operator=(const HashTbl<KeyType, DataType>& other){
                m_size = other.m_size;
                
                m_data_table = std::make_unique<std::list<Entry>[]>(find_prime(m_size));
                for(int i = 0; i < other.m_size; i++){
                    auto list = other.listCol(i);
                    for(auto entry : list){
                        this->insert(entry.m_key, entry.m_data);
                    }
                }
                return *this;
            }
            /*!
            * Initalizer list constructor, creates a default size list and fills it with ilist's contents.
            * \param ilist the list being copied.
            */
            HashTbl<KeyType, DataType>& operator= (std::initializer_list<Entry> ilist){
                m_data_table = std::make_unique<std::list<Entry>[]>(find_prime(DEFAULT_SIZE));
                m_size = DEFAULT_SIZE;
                m_count = 0;
                for(auto entry : ilist){
                    this->insert(entry.m_key, entry.m_data);
                }
                return *this;
            }
            /*!
            * A method meant to allow easier access to the tables of m_data_table. Is used internally for several operatios.
            * \param idx the index of the table needed.
            * @return a reference to the requested line of the table.
            */
            std::list<Entry>& listCol(const int& idx)const{
                return m_data_table[idx];
            }
            /*!
            * Square brackets operator works by taking a key and checking if it is already stored.
            * If the key has never been placed in the table, inserts it and puts the data to be equal to the default constructor of datatype.
            * If the key is already in the list, it returns a reference to the data associetated to it.
            * \param ilist the list being copied.
            * @return reference to the datatype associetade to given key.
            */
            DataType& operator[](const KeyType& k_){
                int end = hashFunc(k_);
                end = end%this->m_size;
                int i{0};
                bool found = false;
                for(auto item : listCol(end)){
                    if(equalFunc(item.m_key, k_)){
                        found = true;
                        break;
                    }
                    i++;
                }
                auto list = listCol(end);
                if(not found){
                    DataType n = DataType();
                    this->insert(k_, n);
                    return this->at(k_);
                }
                else{
                    auto itr = listCol(end).begin();
                    while(i > 0 ){
                        itr++;
                        i--;
                    }
                    return (*itr).m_data;
                }
                
            }
            /*!
            * Returns the ammount of items stored in the table.
            * @return an unsigned int storing the count of items.
            */
            size_t size() const{
                return m_count;
            }
            /*!
            * Receives a key and ifnds the table corresponding to it. Then counts the elements in that table and returns it.
            * \param key a key corresponding to the table one wants to count.
            * @return the count of items in the corresponding table.
            */            
            size_t count(const KeyType& key){
                std::hash<KeyType> hasher;
                int end = hasher(key);
                end = end%this->m_size;
                int i{0};
                for(auto item : m_data_table[end]){
                    i++;
                }
                return i;
            }
            /*!
            * Takes a key and its corresponding data and stores it in the table.
            * If the key is already stored, it replaces the previously stored value.
            * \param key the key to the value.
            * \param data the data corresponding to the key.
            * @return true if the element wasn't previously in the table, false if it was replaced.
            */
            bool insert(const KeyType& key, const DataType& data){
                bool newInsertion = true;
                int end = hashFunc(key);
                end = end% this->m_size;
                for(auto item : m_data_table[end]){
                    //finds repeated key
                    if(equalFunc(item.m_key, key)){
                        //replaces it
                        newInsertion = false;
                        replace(m_data_table[end].begin(), m_data_table[end].end(), item, HashEntry<KeyType, DataType>(key, data));
                        break;
                    }
                }
                if(newInsertion){
                    //not repeated? create new entry
                    m_data_table[end].push_back(HashEntry<KeyType, DataType>(key, data));
                    m_count++;
                }
                
                return newInsertion;
            }
            /*!
            * Takes a key and deletes the element associated to it in the table.
            * \param key the key to the value.
            * @return true if the element was deleted of the table, false if it was not there.
            */
            bool erase(const KeyType& key){
                bool removed = false;
                int end = hashFunc(key);
                end = end%this->m_size;
                int i{0};
                for(auto item : m_data_table[end]){
                    //finds repeated key
                    if(equalFunc(item.m_key, key)){
                        //removes it
                        m_count--;
                        removed = true;
                        m_data_table[end].remove(Entry(item.m_key, item.m_data));
                        break;
                    }
                    i++;
                }
                return removed;
            }
            /*!
            * Takes a key and corresponding data. It searches for it in the table, and if is found, updates the assigned data by the one passed.
            * \param key the key to the value.
            * \param data the data corresponding to the key.
            * @return true if the element was updated, false if it was not found.
            */
            bool retrieve(const KeyType& k_, DataType& d_)const{
                //finds a key and stores the associated value in d_
                int end = hashFunc(k_);
                end = end%this->m_size;
                bool found = false;
                for(auto item : m_data_table[end]){
                    //finds repeated key
                    if(equalFunc(item.m_key, k_)){
                        //updates it
                        found = true;
                        d_ = item.m_data;
                        break;
                    }
                }
                return found;
            }
            /*!
            * Deletes all the data stored in the table and resets the counter.
            */
            void clear(){
                for(int i{0}; i < this->m_size; i++){
                    m_data_table[i].clear();
                }
                m_count = 0;
            }
            /*!
            * Checks if no elements are stored.
            * @return true if the count of elements is 0. False in any other situation.
            */
            bool empty() const{
                return m_count == 0;
            }
            /*!
            * Takes a key and finds the data corresponding to it. Throws an exception if it is not found.
            * \param key the key to the value.
            * @return a reference to the data corresponding to the value.
            */
            DataType& at(const KeyType& k_){
                //finds a key and returns a reference to it
                int end = hashFunc(k_);
                end = end%this->m_size;
                bool found = false;
                auto itr = listCol(end).begin();
                for(auto item : m_data_table[end]){
                    //finds repeated key
                    if(equalFunc(item.m_key, k_)){
                        //returns the data
                        found = true;
                        return (*itr).m_data;
                    }
                    itr++;
                }
                if(not found){
                    throw std::out_of_range("KEY UNFOUND");
                }
                else{
                    std::cout<<"\nWell, this really should not happen, call the author\n";
                }
            }
            /*!
            * Prints the table.
            */
            friend std::ostream& operator<<(std::ostream& os, const HashTbl<KeyType, DataType, KeyHash, KeyEqual>& tbl){
                for(int i{0}; i < tbl.m_size; i++){
                    os<<"TABLE "<<i<<": ";
                    auto list = tbl.listCol(i);
                    for(auto item : list){
                        os << item<<"; ";
                    }
                    os<<"\n";
                }
                return os;
            }
            
        private:
            /*!
            * Regenerates the table with a bigger number of positions, reorganizing every single value already stored.
            * May be deprecated.
            */
            void rehash(){
                HashTbl<KeyType, DataType> biggerHashTbl(this->m_size*2);
                for(int i{0}; i < this->m_size; i++){
                    auto list = this->listCol(i);
                    for(auto item : list){
                        biggerHashTbl.insert(item.m_key, item.m_data);
                    }
                }
                this->m_data_table = biggerHashTbl.m_data_table;
                this->m_size = biggerHashTbl.m_size;
                this->m_count = biggerHashTbl.m_count;
            }
            KeyHash hashFunc;
            KeyEqual equalFunc;
            size_t m_size = 0;
            size_t m_count = 0;
            static const short DEFAULT_SIZE = 11;
            std::unique_ptr<std::list<Entry>[]> m_data_table;

    };

}


