#ifndef _HASHTBL_H_
#define _HASHTBL_H_

#include <iostream>
#include <list> // forward_list

using namespace std;

namespace MyHashTable {

    template<class KeyType, class DataType>
    class HashEntry
    {
        public:

            KeyType m_key;  // Armazena a chave associada a informcao
            DataType m_data;// A informacao.

            HashEntry( KeyType kt_, DataType dt_ ) : m_key(kt_), m_data(dt_)
            {/*Empty*/}

            DataType getData();

            bool operator==(HashEntry<KeyType, DataType> other);


    };

    template< class KeyType, class DataType, typename KeyHash = std::hash<KeyType>, typename KeyEqual = std::equal_to<KeyType> >
    class HashTbl
    {
        public:
            // Aliases
            using entry_type = HashEntry<KeyType,DataType>;
            using list_type = list< entry_type >;
            using size_type = size_t;

            explicit HashTbl( size_t tbl_size_ = DEFAULT_SIZE ){
                //m_data_table = std::make_unique<std::list<entry_type>[]>(find_prime(tbl_size));
                m_data_table = new list< entry_type >[tbl_size_];
                //m_size = find_prime(tbl_size_);
                m_size = tbl_size_;
                m_count = 0;
            }
            HashTbl( const HashTbl& );
            HashTbl( initializer_list< entry_type > );
            HashTbl& operator=( const HashTbl& );
            HashTbl& operator=( initializer_list< entry_type > );

            virtual ~HashTbl();

            bool insert( const KeyType &, const DataType &  );
            bool retrieve( const KeyType &, DataType & ) const;
            bool erase( const KeyType & );
            void clear();
            bool empty() const;
            inline size_type size() const;
            DataType& at( const KeyType& );
            DataType& operator[]( const KeyType& );
            size_type count( const KeyType& ) const;
            void print();
            friend ostream & operator<<( ostream & os_, const HashTbl & ht_ )
            {
                for(size_t i{0}; i < ht_.m_size; i++)
                {
                    os_<<"TABLE "<<i<<": ";
                    
                    auto list = ht_.listCol(i);

                    for(auto item : list){ os_ << item<<"; "; }

                    os_<<"\n";
                }
                return os_;
            }
            list< entry_type > & listCol(const int &) const;
            

        private:
            //=== Private methods
            void rehash();        //!< Change Hash table size if load factor >1.0
           
            //=== Private memnbers
        
            unsigned int m_size;  //!< Tamanho da tabela.
            unsigned int m_count; //!< Numero de elementos na tabel. 
            list< entry_type > *m_data_table; //!< Tabela de listas para entradas de tabela.
            // std::unique_ptr< std::forward_list< entry_type > [] > m_data_table;
            static const short DEFAULT_SIZE = 11;
    };

} // MyHashTable
#include "hashtbl.inl"
#endif