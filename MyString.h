#include <cstring>
#include <iostream>

class MyString {
private:
    std::size_t m_size{};
    char* m_string{};

    explicit MyString(std::size_t len, const char* str);

public:
    explicit MyString(const char* str);
    
    MyString(const MyString& other);

    ~MyString();
    
    MyString& operator=(const MyString& other);
    
    friend std::ostream& operator<<(std::ostream& out, const MyString& mystring);
    
    std::size_t size() const;
    
};


int main() {
    MyString str1{"hi"};
    std::cout << str1;
    MyString str2{"bye"};
    std::cout << str2;
    MyString copyStr1{ str1 };
    std::cout << copyStr1;
    str2 = str1;
    std::cout << str2;
}

