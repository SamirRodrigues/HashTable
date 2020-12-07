HashTable
# TAD-HashTable

In this project, a abstract type data Hashtable was implemented.

This project was proposed by professor Selan Rodrigues from UFRN, for EDB1 material from the IT course. To see more, access the PDF
`projeto_hashtable.pdf`.

# Developed by students / authors Samir Rodrigues and Italo Lima.

>> To compile this project with [cmake](https://cmake.org) follow these steps:

1. `cd TAD`: enters into the main project directory.
2. `cmake -S . -Bbuild`:  asks cmake to create the build directory and generate the Unix Makefile based on the script found in `CMakeLists.txt`, on the current level.
3. `cd build`: enter the build folder.
4. `cmake --build .` or `make`: triggers the compiling process.


>> To run with our tests, first follow the compilation step above, then run inside the folder  `cd TAD/build` the command  `./play`.

# This project is documented in the doxygen style. Possible errors can be found in the statement. If you find them, please contact us.

# Comments
1. The tests do not show the errors, if there are any, it is necessary to view them manually, one by one. 

2. The use of this code is free, for any purpose, please credit if you use them.

3. Project Authors Samir Rodrigues and Italo Lima. (gits @SamirRodrigues & @italo-ce)

4. Development:
    Samir Rodrigues was responsable for the implemention of:
        >   The constructor from a initializer_list;                           <
        >   The constructor overloading the operator= from another hashtable;  <
        >   The constructor overloading the operator= from a initializer_list; <
        >   The retrieve function;                                             <
        >   The erase function;                                                <
        >   The empty function;                                                <
        >   The size function;                                                 <
        >   The overload of the operator[];                                    <
        >   The HashEntry Class;                                               <

    Italo Lima was responsable for the implementation of: 
        >   The default constructor,                                           <
        >   The constructor from another hashtable,                            <
        >   The destructor,                                                    <
        >   The insert function,                                               <
        >   The clear function,                                                <
        >   The at function,                                                   <
        >   The count function                                                 <
        >   The overload of the operator<< (HashTbl)                           <
        >   The rehash metod;                                                  <

OBS.: Although, both team members constantly participated in the implementation of all build stages) 

5. functions that are not working: 
        size_t count( const KeyType& k_) const;
            for some unknown reason, the count function did not passed the tests.