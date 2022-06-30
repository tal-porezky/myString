#include "MyString.h"
#include <iostream>
#include <vector>


int main() {
    MyString str("hello");
    MyString str2("bye");
    MyString str3("hey");
    str = str2; // Should use the copy assignment operator (str as an lvalue) 
    str = MyString("new temporary string"); // Should use move assignment operator (it is given an rvalue, a temporary)
    str3 = std::move(str2); // Should use move assignment operator (it is given an lvalue (str2) that was cast into an rvalue - given permission to steal from it - act as if it was a temporary)
    
    std::vector<MyString> vec;
    vec.push_back(MyString("another temporary"));
}