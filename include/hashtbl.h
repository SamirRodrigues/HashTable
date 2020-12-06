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

            DataType getData(){ return m_data; }

            bool operator==(HashEntry<KeyType, DataType> other){        
                return this->m_key == other.m_key ? true : false;
            }

            friend std::ostream& operator<<(std::ostream& os, const HashEntry<KeyType, DataType> entry_type)
            {
                os<<entry_type.m_data;
                return os;
            }
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
            HashTbl( const HashTbl& other){
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
            HashTbl( initializer_list< entry_type > ilist){
                //m_data_table = std::make_unique<std::list<entry_type>[]>(find_prime(DEFAULT_SIZE));
                m_data_table = new std::list< entry_type >[DEFAULT_SIZE];
                m_size = DEFAULT_SIZE;
                m_count = 0;
                for(auto entry : ilist){
                    this->insert(entry.m_key, entry.m_data);
                }
            }
            HashTbl& operator=( const HashTbl& other){
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
            HashTbl& operator=( initializer_list< entry_type > ilist){
                //m_data_table = std::make_unique<std::list<entry_type>[]>(find_prime(DEFAULT_SIZE));
                m_data_table = new std::list< entry_type >[m_size];
                m_size = DEFAULT_SIZE;
                m_count = 0;
                for(auto entry : ilist){
                    this->insert(entry.m_key, entry.m_data);
                }
                return *this;
            }

            virtual ~HashTbl(){
                this->clear();
                //std::unique_ptr does not require deletion
            }            

            bool insert( const KeyType & key, const DataType & data ){   
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
            bool retrieve( const KeyType & k_, DataType & d_) const{
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
            bool erase( const KeyType & key){   
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
            void clear(){
                for(size_t i{0}; i < this->m_size; i++){
                    m_data_table[i].clear();
                }
                m_count = 0;
            }
            bool empty() const{
                return m_count == 0;
            }
            inline size_type size() const{
                return m_count;
            }
            DataType& at( const KeyType& k_){
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
            DataType& operator[]( const KeyType& k_)
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
            size_type count( const KeyType& key) const{
                std::hash<KeyType> hasher;
                int end = hasher(key);
                end = end%this->m_size;
                size_t i{0};
                for(auto item : m_data_table[end]){
                    i++;
                }
                return i;
            }
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
            list< entry_type > & listCol(const int & idx) const{
                return m_data_table[idx];
            }
            

        private:
            //=== Private methods
            void rehash(){        //!< Change Hash table size if load factor >1.0
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