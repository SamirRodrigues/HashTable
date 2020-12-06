#ifndef _HASHTBL_H_
#define _HASHTBL_H_

#include <iostream>
#include <list> // forward_list
#include "account.h"

using namespace std;

namespace MyHashTable {

    template<class KeyType, class DataType>
    class HashEntry
    {
        public:

            KeyType m_key;  // Armazena a chave associada a informcao
            DataType m_data;// A informacao.

            HashEntry( KeyType kt_, DataType dt_ ) : m_key(kt_), m_data(dt_)
            {}

            DataType getData();/*
            { 
                return m_data; 
                }*/

            bool operator==(HashEntry<KeyType, DataType> other);/*{        
                return this->m_key == other.m_key ? true : false;
            }*/

            friend std::ostream& operator<<(std::ostream& os, const HashEntry<KeyType, DataType> entry_type)
            {
                os<<entry_type.m_data;
                return os;
            }
    };

    template< typename KeyType, typename DataType>
    class HashTbl
    {
        public:
            
            using Entry = HashEntry<KeyType,DataType>;

            explicit HashTbl( size_t tbl_size_ = DEFAULT_SIZE ){
            
                m_data_table = new list< Entry >[tbl_size_];
                m_size = tbl_size_;
                m_count = 0;
            
            }


            HashTbl( const HashTbl& other);/*{
                
                m_size = other.size();
                m_data_table = new std::list< Entry >[m_size];
                
                for(size_t i = 0; i < other.m_size; i++){

                    for(auto entry : other.m_data_table[i]){
                
                        this->insert(entry.m_key, entry.m_data);
                
                    }
                
                }

            }*/
            HashTbl( initializer_list< Entry > ilist);/*{
                
                m_data_table = new std::list< Entry >[DEFAULT_SIZE];
                m_size = DEFAULT_SIZE;
                m_count = 0;
                
                for(auto entry : ilist){

                    this->insert(entry.m_key, entry.m_data);
                
                }
            
            }*/
            HashTbl& operator=( const HashTbl& other);/*{
                
                m_size = other.m_size;
                
                m_data_table = new std::list< Entry >[m_size];
                
                for(size_t i = 0; i < other.m_size; i++){
                    
                    for(auto entry : other.m_data_table[i]){
                
                        this->insert(entry.m_key, entry.m_data);
                
                    }
                
                }
                return *this;

            }*/
            HashTbl& operator=( initializer_list< Entry > ilist);/*{
            
                m_size = DEFAULT_SIZE;
                m_data_table = new std::list< Entry >[m_size];
                m_count = 0;

                for(auto entry : ilist){
                
                    this->insert(entry.m_key, entry.m_data);
                
                }
                
                return *this;
            
            }*/

            virtual ~HashTbl();/*{

                this->clear();
                delete[] m_data_table;
            
            }            */

            bool insert( const KeyType & k_, const DataType & d_ );/*{   
                
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

            }*/
            bool retrieve( const KeyType & k_, DataType & d_) const;/*{
                
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
            
            }*/
            bool erase( const KeyType & key);/*{   
                
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
            
            }*/
            void clear();/*{
                
                for(size_t i = 0; i < this->m_size; i++){
                
                    m_data_table[i].clear();
                
                }
                
                m_count = 0;

            }*/
            bool empty() const;/*{
                
                return m_count == 0;
            
            }*/
            inline size_t size() const;/*{
                
                return m_count;
            
            }*/
            DataType& at( const KeyType& k_);/*{
                
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

            }*/
            DataType& operator[]( const KeyType& k_);/*
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
                
            }*/
            size_t count( const KeyType& k_) const;/*
            {   
                
                //size_t count = 0;

                KeyHash HashFunc;
                int end = HashFunc(k_);
                end = end%this->m_size;
                
                
                for(auto item : m_data_table[end])
                {

                    count++;
                
                }
                return count;
                

                return m_data_table[end].size();

            }*/
            
            
            friend ostream & operator<<( ostream & o_, const HashTbl & table_ )
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

            }

        private:
            //=== Private methods
            void rehash();/*{        //!< Change Hash table size if load factor >1.0
                
                HashTbl<KeyType, DataType> newHashTbl(this->m_size*2);
                
                for(size_t i = 0; i < this->m_size; i++){

                    for(auto item : m_data_table[i]){
                
                        newHashTbl.insert(item.m_key, item.m_data);
                
                    }
                
                }
                
                this->m_data_table = newHashTbl.m_data_table;
                
                this->m_size = newHashTbl.m_size;
                
                this->m_count = newHashTbl.m_count;
            
            }*/
            //=== Private memnbers
        
            unsigned int m_size;  //!< Tamanho da tabela.
            unsigned int m_count; //!< Numero de elementos na tabel. 
            list< Entry > *m_data_table; //!< Tabela de listas para entradas de tabela.
            // std::unique_ptr< std::forward_list< Entry > [] > m_data_table;
            static const short DEFAULT_SIZE = 11;
    };

} // MyHashTable
#include "hashtbl.inl"
#endif