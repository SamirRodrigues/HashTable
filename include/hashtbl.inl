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

namespace MyHashTable
{
    template <class KeyType, class DataType>
    std::ostream& operator<<(std::ostream& os, const HashEntry<KeyType, DataType> entry_type)
    {
        os<<entry_type.m_data;
        return os;
    }
    
    template <class KeyType, class DataType>
    DataType HashEntry<KeyType,DataType>::getData(){ return m_data; }
    
    template <class KeyType, class DataType>
    bool HashEntry<KeyType,DataType>::operator==(HashEntry<KeyType, DataType> other)
    {        
        return this->m_key == other.m_key ? true : false;
    }
 
    
   // ======================== HASH TBL ======================== //
    /*template <class KeyType, class DataType>
    HashTbl< KeyType, DataType >::HashTbl(size_t tbl_size_ = DEFAULT_SIZE){
        //m_data_table = std::make_unique<std::list<entry_type>[]>(find_prime(tbl_size));
                m_data_table = new std::forward_list< entry_type >[tbl_size_];
                //m_size = find_prime(tbl_size_);
                m_size = tbl_size_;
                m_count = 0;
    }*/

    
    template <class KeyType, class DataType>
    HashTbl< KeyType, DataType >::~HashTbl(){
        this->clear();
        //std::unique_ptr does not require deletion
    }

   
    template <class KeyType, class DataType>
    HashTbl<KeyType,DataType>::HashTbl(const HashTbl<KeyType, DataType>& other){
        m_size = other.size();
        //m_data_table = std::make_unique<std::list<entry_type>[]>(find_prime(m_size));
        m_data_table = new std::list< entry_type >[m_size];
        for(size_t i = 0; i < other.m_size; i++){
            auto list = other.listCol(i);
            for(auto entry : list){
                this->insert(entry.m_key, entry.m_data);
            }
        }
    }

   
    template <class KeyType, class DataType>
    HashTbl< KeyType,DataType >::HashTbl( std::initializer_list<entry_type> ilist){
        //m_data_table = std::make_unique<std::list<entry_type>[]>(find_prime(DEFAULT_SIZE));
        m_data_table = new std::list< entry_type >[DEFAULT_SIZE];
        m_size = DEFAULT_SIZE;
        m_count = 0;
        for(auto entry : ilist){
            this->insert(entry.m_key, entry.m_data);
        }
    }

    
    template <class KeyType, class DataType>
    HashTbl<KeyType, DataType>& HashTbl<KeyType,DataType>::operator=(const HashTbl<KeyType, DataType>& other){
        m_size = other.m_size;
        
        //m_data_table = std::make_unique<std::list<entry_type>[]>(find_prime(m_size));
        m_data_table = new std::list< entry_type >[m_size];
        
        for(size_t i = 0; i < other.m_size; i++){
            auto list = other.listCol(i);
            for(auto entry : list){
                this->insert(entry.m_key, entry.m_data);
            }
        }
        return *this;
    }

    
    template <class KeyType, class DataType>
    HashTbl<KeyType, DataType>& HashTbl<KeyType,DataType>::operator= (std::initializer_list<entry_type> ilist){
        //m_data_table = std::make_unique<std::list<entry_type>[]>(find_prime(DEFAULT_SIZE));
        m_data_table = new std::list< entry_type >[m_size];
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
    template <class KeyType, class DataType>
    std::list< HashEntry<KeyType,DataType> >& HashTbl<KeyType,DataType>::listCol(const int& idx)const{
        return m_data_table[idx];
    }

    /*!
    * Square brackets operator works by taking a key and checking if it is already stored.
    * If the key has never been placed in the table, inserts it and puts the data to be equal to the default constructor of datatype.
    * If the key is already in the list, it returns a reference to the data associetated to it.
    * \param ilist the list being copied.
    * @return reference to the datatype associetade to given key.
    */
    template< class KeyType, class DataType >
    DataType& HashTbl<KeyType,DataType>::operator[](const KeyType& k_)
    {   
        KeyHash hashFunc;
        KeyEqual equalFunc;

        int end =  hashFunc(k_);
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
    template <class KeyType, class DataType>
    size_t HashTbl< KeyType, DataType >::size() const{
        return m_count;
    }

    /*!
    * Receives a key and ifnds the table corresponding to it. Then counts the elements in that table and returns it.
    * \param key a key corresponding to the table one wants to count.
    * @return the count of items in the corresponding table.
    */         
    template <class KeyType, class DataType>
    size_t HashTbl< KeyType, DataType >::count(const KeyType& key) const {
        std::hash<KeyType> hasher;
        int end = hasher(key);
        end = end%this->m_size;
        size_t i{0};
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
    template <class KeyType, class DataType>
    bool HashTbl< KeyType, DataType >::insert(const KeyType& key, const DataType& data)
    {   
        KeyHash hashFunc;
        KeyEqual equalFunc;

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
    template <class KeyType, class DataType>
    bool HashTbl< KeyType, DataType >::erase(const KeyType& key)
    {   
        KeyHash hashFunc;
        KeyEqual equalFunc;

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
                m_data_table[end].remove(entry_type(item.m_key, item.m_data));
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
    template <class KeyType, class DataType>
    bool HashTbl< KeyType, DataType >::retrieve(const KeyType& k_, DataType& d_)const
    {
        KeyHash hashFunc;
        KeyEqual equalFunc;

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
    template <class KeyType, class DataType>
    void HashTbl< KeyType, DataType >::clear()
    {
        for(size_t i{0}; i < this->m_size; i++){
            m_data_table[i].clear();
        }
        m_count = 0;
    }

    /*!
    * Checks if no elements are stored.
    * @return true if the count of elements is 0. False in any other situation.
    */
    template <class KeyType, class DataType>
    bool HashTbl< KeyType, DataType >::empty() const{
        return m_count == 0;
    }

    /*!
    * Takes a key and finds the data corresponding to it. Throws an exception if it is not found.
    * \param key the key to the value.
    * @return a reference to the data corresponding to the value.
    */
    template <class KeyType, class DataType>
    DataType& HashTbl< KeyType, DataType >::at(const KeyType& k_)
    {
        KeyHash hashFunc;
        KeyEqual equalFunc;

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
            return(*itr).m_data;
        }
    }

    
    template <class KeyType, class DataType>
    void HashTbl< KeyType, DataType >::rehash()
    {
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
    /*
    template <class KeyType, class DataType>
    std::ostream & HashTbl< KeyType, DataType >::operator<<( std::ostream & os_, const HashTbl & ht_ )
    {
        for(int i{0}; i < ht_.m_size; i++)
        {
            os_<<"TABLE "<<i<<": ";
            
            auto list = ht_.listCol(i);

            for(auto item : list){ os_ << item<<"; "; }

            os_<<"\n";
        }
        return os_;
    }
    */
}


