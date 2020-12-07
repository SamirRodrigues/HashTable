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
            {/*empty*/}

            DataType getData();

            bool operator==(HashEntry<KeyType, DataType> other);

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


            HashTbl( const HashTbl& other);
            HashTbl( initializer_list< Entry > ilist);
            HashTbl& operator=( const HashTbl& other);
            HashTbl& operator=( initializer_list< Entry > ilist);

            virtual ~HashTbl();

            bool insert( const KeyType & k_, const DataType & d_ );
            bool retrieve( const KeyType & k_, DataType & d_) const;
            bool erase( const KeyType & key);
            void clear();
            bool empty() const;
            inline size_t size() const;
            DataType& at( const KeyType& k_);
            DataType& operator[]( const KeyType& k_);
            size_t count( const KeyType& k_) const;
            
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
            
            void rehash();
        
            unsigned int m_size;  //!< Tamanho da tabela.
            unsigned int m_count; //!< Numero de elementos na tabel. 
            list< Entry > *m_data_table; //!< Tabela de listas para entradas de tabela.
            // std::unique_ptr< std::forward_list< Entry > [] > m_data_table;
            static const short DEFAULT_SIZE = 11;
    };

} // MyHashTable
#include "hashtbl.inl"
#endif