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

        /**
         * @brief HashEntry
         * 
         * KeyType m_key; 			                         
         * DataType m_data;								            
         * HashEntry( KeyType kt_, DataType dt_ );					   
         * DataType getData();											    
         * bool operator==(HashEntry<KeyType, DataType> other); 						
         * friend std::ostream& operator<<(std::ostream& os, const HashEntry<KeyType, DataType> entry_type)		    	
         * 
         */
        
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

            /**
			 * @brief Constructor
             * 
             * explicit HashTbl( size_t tbl_size_ = DEFAULT_SIZE ); 			create a HashTbl 
			 * HashTbl( const HashTbl& other);									create a HashTbl from another
			 * HashTbl( initializer_list< Entry > ilist); 					    Create a HashTbl from initializer_list
			 * virtual ~HashTbl();											    Default destructor
			 * HashTbl& operator=( const HashTbl& other); 						Copy content from another HashTbl
			 * HashTbl& operator=( initializer_list< Entry > ilist);		    Copy content from initializer_list	
			 * 
			 */
            
            /**
             * @brief Construct a new HashTbl<KeyType,DataType>::HashTbl object
             * 
             * @tparam KeyType 
             * @tparam DataType
             * @param tbl_size_ 
             */

            explicit HashTbl( size_t tbl_size_ = DEFAULT_SIZE )                 // create a HashTbl 
            {
            
                m_data_table = new list< Entry >[tbl_size_];
                m_size = tbl_size_;
                m_count = 0;
            
            }
            HashTbl( const HashTbl& other);                                     // create a HashTbl from another
            HashTbl( initializer_list< Entry > ilist);                          // Create a HashTbl from initializer_list
            HashTbl& operator=( const HashTbl& other);                          // Default destructor
            HashTbl& operator=( initializer_list< Entry > ilist);               // Copy content from another HashTbl
            virtual ~HashTbl();                                                 // Copy content from initializer_list

            /**
			 * @brief Function
             * 
             * bool insert( const KeyType & k_, const DataType & d_ );              // inserts the information that is in d_ associated to k_ key.
             * bool erase( const KeyType & key);                                    // Removes the item from the Table that is associated to the k_ key.
             * bool retrieve( const KeyType & k_, DataType & d_) const;             // Retrieves the information that is in d_ associated to k_ key.
             * void clear();                                                        // Clean the table, removing all elements.
             * bool empty() const;                                                  // Inform if the table is empty.
             * inline size_t size() const;                                          // Inform the amount of elements from the Table.
             * DataType& at( const KeyType& k_);                                    // Returns a reference to the data associated with the k_ key.
             * DataType& operator[]( const KeyType& k_);                            // Returns a reference to the data associated with the k_ key, by overloading the operator[].
             * size_t count( const KeyType& k_) const;                              // Returns the number of elements in the table that are in the associated collision list the k_ key.
             * friend ostream & operator<<( ostream & o_, const HashTbl & table_)   // Overload of the debugging method used to generate a representation table text and its elements.
			 * 
			 */
            
            bool insert( const KeyType & k_, const DataType & d_ );                 // inserts the information that is in d_ associated to k_ key.
            bool erase( const KeyType & k_);                                        // Removes the item from the Table that is associated to the k_ key.
            bool retrieve( const KeyType & k_, DataType & d_) const;                // Retrieves the information that is in d_ associated to k_ key.
            void clear();                                                           // Clean the table, removing all elements.
            bool empty() const;                                                     // Inform if the table is empty.
            inline size_t size() const;                                             // Inform the amount of elements from the Table.
            DataType& at( const KeyType& k_);                                       // Returns a reference to the data associated with the k_ key.
            DataType& operator[]( const KeyType& k_);                               // Returns a reference to the data associated with the k_ key, by overloading the operator[].
            size_t count( const KeyType& k_) const;                                 // Returns the number of elements in the table that are in the associated collision list the k_ key.
            
            friend ostream & operator<<( ostream & o_, const HashTbl & table_ )     // Overload of the debugging method used to generate a representation table text and its elements.
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

        /**
         * @brief Private:
         * void rehash();                               Function responsable to increase the size of the table
         * list< Entry > *m_data_table;  		        Pointer to receive the dynamic allocation of a vector of lists of the table 
         * unsigned int m_count;		                The amount of elements in the table 
         * unsigned int m_size;	                        The size of the Hash Table
         * static const short DEFAULT_SIZE = 11;        The default size for the Hash Table
         */

        private:
            
            void rehash();                              //Function responsable to increase the size of the table

            unsigned int m_size;                        //The size of the Hash Table
            unsigned int m_count;                       //The amount of elements in the table 
            list< Entry > *m_data_table;                //Pointer to receive the dynamic allocation of a vector of lists of the table 
    
            static const short DEFAULT_SIZE = 11;       //The default size for the Hash Table
    };

}

/**
 * @file hashtbl.h
 * @author Samir Rodrigues & Italo Lima (git @SamirRodrigues & @italo-ce)
 * @brief Hash Table Implementation
 * @version 0.1
 * @date 2020-12-06
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "hashtbl.inl"
#endif