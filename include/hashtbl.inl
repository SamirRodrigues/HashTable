#pragma once
#include <iostream>
#include <list>
#include <memory>
#include <algorithm>
#include "account.h"

namespace MyHashTable
{
    /*
    template <typename KeyType, typename DataType>
    std::ostream& operator<<(std::ostream& os, const HashEntry<KeyType, DataType> entry_type)
    {
        os<<entry_type.m_data;
        return os;
    }
    */
    
    template <typename KeyType, typename DataType>
    DataType HashEntry<KeyType,DataType>::getData()
    { 

        return m_data; 
    
    }
    

    template <typename KeyType, typename DataType>
    bool HashEntry<KeyType,DataType>::operator==(HashEntry<KeyType, DataType> other)
    {        
        return this->m_key == other.m_key ? true : false;
    }
    
    
/***************************************Hashtbl***************************************/
    
    /*
    template <typename KeyType, typename DataType>
    explicit HashTbl< KeyType, DataType >::HashTbl(size_t tbl_size_ = DEFAULT_SIZE){
        
        m_data_table = new list< Entry >[tbl_size_];
        m_size = tbl_size_;
        m_count = 0;
    
    }
    */
    
    template <typename KeyType, typename DataType>
    HashTbl< KeyType, DataType >::~HashTbl(){
        
        this->clear();

        delete[] m_data_table;
    
    }
    
   
    template <typename KeyType, typename DataType>
    HashTbl<KeyType,DataType>::HashTbl(const HashTbl<KeyType, DataType>& other){
                 
        m_size = other.size();
        m_data_table = new std::list< Entry >[m_size];
        
        for(size_t i = 0; i < other.m_size; i++){

            for(auto entry : other.m_data_table[i]){
        
                this->insert(entry.m_key, entry.m_data);
        
            }
        
        }

    }

   
    template <typename KeyType, typename DataType>
    HashTbl< KeyType,DataType >::HashTbl( std::initializer_list<Entry> ilist){
               
        m_data_table = new std::list< Entry >[DEFAULT_SIZE];
        m_size = DEFAULT_SIZE;
        m_count = 0;
        
        for(auto entry : ilist){

            this->insert(entry.m_key, entry.m_data);
        
        }
            
    }
    
    
    template <typename KeyType, typename DataType>
    HashTbl<KeyType, DataType>& HashTbl<KeyType,DataType>::operator=(const HashTbl<KeyType, DataType>& other){
                    
        m_size = other.m_size;
        
        m_data_table = new std::list< Entry >[m_size];
        
        for(size_t i = 0; i < other.m_size; i++){
            
            for(auto entry : other.m_data_table[i]){
        
                this->insert(entry.m_key, entry.m_data);
        
            }
        
        }
        return *this;

    }

    
    template <typename KeyType, typename DataType>
    HashTbl<KeyType, DataType>& HashTbl<KeyType,DataType>::operator= (std::initializer_list<Entry> ilist){
        
        m_size = DEFAULT_SIZE;
        m_data_table = new std::list< Entry >[m_size];
        m_count = 0;

        for(auto entry : ilist){
        
            this->insert(entry.m_key, entry.m_data);
        
        }
        
        return *this;
    
    }
    
    /*!
    * Square brackets operator works by taking a key and checking if it is already stored.
    * If the key has never been placed in the table, inserts it and puts the data to be equal to the default constructor of datatype.
    * If the key is already in the list, it returns a reference to the data associetated to it.
    * \param ilist the list being copied.
    * @return reference to the datatype associetade to given key.
    */
   
    template< typename KeyType, typename DataType >
    DataType& HashTbl<KeyType,DataType>::operator[](const KeyType& k_)
    {   
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        int end =  HashFunc(k_);
        end = end%this->m_size;
        
        auto itr = m_data_table[end].begin();

        for(auto item : m_data_table[end]){
            
            if(EqualFunc(item.m_key, k_)){
            
                return (*itr).m_data;
            
            }

            itr++;
        
        }

        DataType n = DataType();
        
        this->insert(k_, n);
        
        return this->at(k_);
        
    }

    /*!
    * Returns the ammount of items stored in the table.
    * @return an unsigned int storing the count of items.
    */
    
    template <typename KeyType, typename DataType>
    size_t HashTbl< KeyType, DataType >::size() const{
                
        return m_count;
    
    }
    
    /*!
    * Receives a key and ifnds the table corresponding to it. Then counts the elements in that table and returns it.
    * \param key a key corresponding to the table one wants to count.
    * @return the count of items in the corresponding table.
    */         
    
    template <typename KeyType, typename DataType>
    size_t HashTbl< KeyType, DataType >::count(const KeyType& k_) const {
        
        //size_t count = 0;

        KeyHash HashFunc;
        int end = HashFunc(k_);
        end = end%this->m_size;
        
        /*
        for(auto item : m_data_table[end])
        {

            count++;
        
        }
        return count;
        */

        return m_data_table[end].size();

    }
    
    /*!
    * Takes a key and its corresponding data and stores it in the table.
    * If the key is already stored, it replaces the previously stored value.
    * \param key the key to the value.
    * \param data the data corresponding to the key.
    * @return true if the element wasn't previously in the table, false if it was replaced.
    */
    
    template <typename KeyType, typename DataType>
    bool HashTbl< KeyType, DataType >::insert(const KeyType& k_, const DataType& d_)
    {   
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        bool newInsert = true;

        int end = HashFunc(k_);
        end = end% this->m_size;
        
        for(auto item : m_data_table[end])
        {

            if(EqualFunc(item.m_key, k_))
            {

                newInsert = false;
                replace(m_data_table[end].begin(), m_data_table[end].end(), item, HashEntry<KeyType, DataType>(k_, d_));
                break;
            
            }

        }
        
        if(newInsert){

            m_data_table[end].push_back(HashEntry<KeyType, DataType>(k_, d_));
            m_count++;

        }
        
        return newInsert;
    }
    
    /*!
    * Takes a key and deletes the element associated to it in the table.
    * \param key the key to the value.
    * @return true if the element was deleted of the table, false if it was not there.
    */
    
    template <typename KeyType, typename DataType>
    bool HashTbl< KeyType, DataType >::erase(const KeyType& key)
    {   
         
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        bool removed = false;
        
        int end = HashFunc(key);
        end = end % this->m_size;

        for(auto item : m_data_table[end])
        {

            if(EqualFunc(item.m_key, key))
            {

                m_count--;
                removed = true;
                m_data_table[end].remove(Entry(item.m_key, item.m_data));
                break;
            
            }

        }

        return removed;
            
    }
    
    /*!
    * Takes a key and corresponding data. It searches for it in the table, and if is found, updates the assigned data by the one passed.
    * \param key the key to the value.
    * \param data the data corresponding to the key.
    * @return true if the element was updated, false if it was not found.
    */
    
    template <typename KeyType, typename DataType>
    bool HashTbl< KeyType, DataType >::retrieve(const KeyType& k_, DataType& d_)const
    {
         
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        int end = HashFunc(k_);
        end = end%this->m_size;
        
        bool found = false;
        
        for(auto item : m_data_table[end])
        {

            if(EqualFunc(item.m_key, k_))
            {

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
    
    template <typename KeyType, typename DataType>
    void HashTbl< KeyType, DataType >::clear()
    {

        for(size_t i = 0; i < this->m_size; i++){
        
            m_data_table[i].clear();
        
        }
        
        m_count = 0;
    
    }
    /*!
    * Checks if no elements are stored.
    * @return true if the count of elements is 0. False in any other situation.
    */
    
    template <typename KeyType, typename DataType>
    bool HashTbl< KeyType, DataType >::empty() const{
                        
        return m_count == 0;
            
    }
    
    /*!
    * Takes a key and finds the data corresponding to it. Throws an exception if it is not found.
    * \param key the key to the value.
    * @return a reference to the data corresponding to the value.
    */
    
    template <typename KeyType, typename DataType>
    DataType& HashTbl< KeyType, DataType >::at(const KeyType& k_)
    {
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        int end = HashFunc(k_);
        end = end%this->m_size;
        
        auto itr = m_data_table[end].begin();
        
        for(auto item : m_data_table[end]){

            if(EqualFunc(item.m_key, k_)){

                return (*itr).m_data;
        
            }
        
            itr++;
        
        }

        throw std::out_of_range("KEY NOT FOUND");
    }
    
    template <typename KeyType, typename DataType>
    void HashTbl< KeyType, DataType >::rehash()
    {
        
        HashTbl<KeyType, DataType> newHashTbl(this->m_size*2);
                
        for(size_t i = 0; i < this->m_size; i++){

            for(auto item : m_data_table[i]){
        
                newHashTbl.insert(item.m_key, item.m_data);
        
            }
        
        }
        
        this->m_data_table = newHashTbl.m_data_table;
        
        this->m_size = newHashTbl.m_size;
        
        this->m_count = newHashTbl.m_count;

    }
    /*
    template <typename KeyType, typename DataType>
    std::ostream & HashTbl< KeyType , DataType >::operator<<( std::ostream & o_, const HashTbl<KeyType, DataType> & table_ )
    {
        for(size_t i = 0; i < table_.m_size; i++)
        {

            o_ << "Table " << i << ": ";

            for(auto item : table_.m_data_table[i])
            { 
                
                o_ << item<<";\n \t"; 
                
            }

            o_ << "\n";
        
        }

        return o_;
    }*/

/***************************************Hashtbl***************************************/

}


