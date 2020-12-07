#include <iostream>
#include <list>
#include <memory>
#include <algorithm>
#include "account.h"

namespace MyHashTable
{

/**************************************HashEntry**************************************/ 

    /**
	 * @brief getData()
	 * 
     * @tparam KeyType 
     * @tparam DataType
	 * @return DataType
	 */
    template <typename KeyType, typename DataType>
    DataType HashEntry<KeyType,DataType>::getData()
    { 

        return m_data; 
    
    }
    
    /**
	 * @brief operator==()
	 * 
     * @tparam KeyType 
     * @tparam DataType
     * @param other
	 * @return bool 
	 */
    template <typename KeyType, typename DataType>
    bool HashEntry<KeyType,DataType>::operator==(HashEntry<KeyType, DataType> other)
    {        
        return m_key == other.m_key ? true : false;
    }

/**************************************HashEntry**************************************/   
    
/***************************************Hashtbl***************************************/
    
    template <typename KeyType, typename DataType>
    HashTbl< KeyType, DataType >::~HashTbl(){
        
        clear();

        delete[] m_data_table;
    
    }
    
    /**
     * @brief Construct a new HashTbl<KeyType,DataType>::HashTbl object
     * 
     * @tparam KeyType 
     * @tparam DataType
     * @param other 
     */
    template <typename KeyType, typename DataType>
    HashTbl<KeyType,DataType>::HashTbl(const HashTbl<KeyType, DataType>& other){
                 
        m_size = other.size();
        m_data_table = new std::list< Entry >[m_size];
        
        for(size_t i = 0; i < other.m_size; i++){

            for(auto entry : other.m_data_table[i]){
        
                insert(entry.m_key, entry.m_data);
        
            }
        
        }

    }

    /**
     * @brief Construct a new HashTbl<KeyType,DataType>::HashTbl object
     * 
     * @tparam KeyType 
     * @tparam DataType
     * @param ilist 
     */
    template <typename KeyType, typename DataType>
    HashTbl< KeyType,DataType >::HashTbl( std::initializer_list<Entry> ilist){
               
        m_data_table = new std::list< Entry >[DEFAULT_SIZE];
        m_size = DEFAULT_SIZE;
        m_count = 0;
        
        for(auto entry : ilist){

            insert(entry.m_key, entry.m_data);
        
        }
            
    }
    
    /**
	 * @brief operator=()
	 * 
     * @tparam KeyType 
     * @tparam DataType 
	 * @param other 
	 * @return HashTbl<KeyType,DataType>& 
	 */
    template <typename KeyType, typename DataType>
    HashTbl<KeyType, DataType>& HashTbl<KeyType,DataType>::operator=(const HashTbl<KeyType, DataType>& other){
                    
        m_size = other.m_size;
        
        m_data_table = new std::list< Entry >[m_size];
        
        for(size_t i = 0; i < other.m_size; i++){
            
            for(auto entry : other.m_data_table[i]){
        
                insert(entry.m_key, entry.m_data);
        
            }
        
        }
        return *this;

    }

    /**
	 * @brief operator=()
	 * 
     * @tparam KeyType 
     * @tparam DataType 
	 * @param ilist 
	 * @return HashTbl<KeyType,DataType>& 
	 */
    template <typename KeyType, typename DataType>
    HashTbl<KeyType, DataType>& HashTbl<KeyType,DataType>::operator= (std::initializer_list<Entry> ilist){
        
        m_size = DEFAULT_SIZE;
        m_data_table = new std::list< Entry >[m_size];
        m_count = 0;

        for(auto entry : ilist){
        
            insert(entry.m_key, entry.m_data);
        
        }
        
        return *this;
    
    }
    
    /**
	 * @brief operator[]()
	 * 
     * @tparam KeyType 
     * @tparam DataType 
	 * @param k_ 
	 * @return HashTbl<KeyType,DataType>& 
	 */
    template< typename KeyType, typename DataType >
    DataType& HashTbl<KeyType,DataType>::operator[](const KeyType& k_)
    {   
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        int end =  HashFunc(k_);
        end = end % m_size;
        
        auto itr = m_data_table[end].begin();

        for(auto item : m_data_table[end]){
            
            if(EqualFunc(item.m_key, k_)){
            
                return (*itr).m_data;
            
            }

            itr++;
        
        }

        DataType n = DataType();
        
        insert(k_, n);
        
        return at(k_);
        
    }

    /**
	 * @brief size()
	 * 
     * @tparam KeyType 
     * @tparam DataType
     * @param k_
	 * @return size_t 
	 */
    template <typename KeyType, typename DataType>
    size_t HashTbl< KeyType, DataType >::size() const{
                
        return m_count;
    
    }
    
    /**
	 * @brief count()
	 * 
     * @tparam KeyType 
     * @tparam DataType
	 * @return size_t 
	 */
    template <typename KeyType, typename DataType>
    size_t HashTbl< KeyType, DataType >::count(const KeyType& k_) const {
        
        KeyHash HashFunc;
        
        int end = HashFunc(k_);
        end = end % m_size;
    
        return m_data_table[end].size();

    }
    
    /**
	 * @brief insert()
	 * 
     * @tparam KeyType 
     * @tparam DataType
     * @param k_
	 * @return bool 
	 */
    template <typename KeyType, typename DataType>
    bool HashTbl< KeyType, DataType >::insert(const KeyType& k_, const DataType& d_)
    {   
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        bool newInsert = true;

        int end = HashFunc(k_);
        end = end % m_size;
        
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
    
    /**
	 * @brief erase()
	 * 
     * @tparam KeyType 
     * @tparam DataType
     * @param k_
	 * @return bool 
	 */
    template <typename KeyType, typename DataType>
    bool HashTbl< KeyType, DataType >::erase(const KeyType& k_)
    {   
         
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        bool removed = false;
        
        int end = HashFunc(k_);
        end = end % m_size;

        for(auto item : m_data_table[end])
        {

            if(EqualFunc(item.m_key, k_))
            {

                m_count--;
                removed = true;
                m_data_table[end].remove(Entry(item.m_key, item.m_data));
                break;
            
            }

        }

        return removed;
            
    }
    
    /**
	 * @brief retrieve()
	 * 
     * @tparam KeyType 
     * @tparam DataType
     * @param k_
     * @param d_
	 * @return bool 
	 */
    template <typename KeyType, typename DataType>
    bool HashTbl< KeyType, DataType >::retrieve(const KeyType& k_, DataType& d_)const
    {
         
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        int end = HashFunc(k_);
        end = end % m_size;
        
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
    
    /**
	 * @brief clear()
	 * 
     * @tparam KeyType 
     * @tparam DataType 
	 */
    template <typename KeyType, typename DataType>
    void HashTbl< KeyType, DataType >::clear()
    {

        for(size_t i = 0; i < m_size; i++){
        
            m_data_table[i].clear();
        
        }
        
        m_count = 0;
    
    }
     
    /**
     * @brief empty()
     * 
     * @tparam KeyType 
     * @tparam DataType
     * @return bool 
     */
    template <typename KeyType, typename DataType>
    bool HashTbl< KeyType, DataType >::empty() const{
                        
        return m_count == 0;
            
    }
    
    /**
	 * @brief at()
	 * 
     * @tparam KeyType 
     * @tparam DataType
     * @param k_
	 * @return DataType& 
	 */
    template <typename KeyType, typename DataType>
    DataType& HashTbl< KeyType, DataType >::at(const KeyType& k_)
    {
        KeyHash HashFunc;
        KeyEqual EqualFunc;

        int end = HashFunc(k_);
        end = end % m_size;
        
        auto itr = m_data_table[end].begin();
        
        for(auto item : m_data_table[end]){

            if(EqualFunc(item.m_key, k_)){

                return (*itr).m_data;
        
            }
        
            itr++;
        
        }

        throw std::out_of_range("KEY NOT FOUND");
    }
    
    /**
	 * @brief rehash()
	 * 
     * @tparam KeyType 
     * @tparam DataType 
	 */
    template <typename KeyType, typename DataType>
    void HashTbl< KeyType, DataType >::rehash()
    {
        
        HashTbl<KeyType, DataType> newHashtbl( m_size * 2 );
                
        for(size_t i = 0; i < m_size; i++){

            for(auto item : m_data_table[i]){
        
                newHashtbl.insert(item.m_key, item.m_data);
        
            }
        
        }
        
        m_data_table = newHashtbl.m_data_table;
        
        m_size = newHashtbl.m_size;
        
        m_count = newHashtbl.m_count;

    }

/***************************************Hashtbl***************************************/

}


