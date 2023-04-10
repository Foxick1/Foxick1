
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include "MyString.h"



int main() {
    try
    {
        
        MyString<char> str1("Hello World!");
        MyString<char>::iterator it = str1.begin();
        it = it + 1;
        it += 1;
        it = it - 1;
        it -= 1;
        it++;
        ++it;
        it--;
        --it;





        //str1.erace(&(str1[6]));

        return 0;
    }



    catch (std::bad_alloc) {
        std::cout << "Bud alloc\n";
    }
    catch (std::out_of_range) {
        std::cout << "out_of_range\n";
    }
}