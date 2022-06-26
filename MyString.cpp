#include <cstring>
#include <iostream>

class MyString {
private:
    std::size_t m_size{};
    char* m_string{};

    explicit MyString(std::size_t len, const char* str) : m_size{ len }, m_string{ new char[m_size + 1]} {
        strcpy(m_string, str);
    }

public:
    explicit MyString(const char* str) : MyString(strlen(str), str) {}
    
    MyString(const MyString& other) : MyString(other.m_size, other.m_string) {}

    ~MyString() {
        delete[] m_string;
        m_string = nullptr;
    }
    
    MyString& operator=(const MyString& other) {
        if (this == &other) {
            return *this;
        }
        delete[] m_string;
        m_size = other.m_size;
        m_string = new char[other.m_size + 1];
        strcpy(m_string, other.m_string);
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const MyString& mystring);
    
    std::size_t size() const {
        return m_size;
    }
    
};

std::ostream& operator<<(std::ostream& out, const MyString& mystring) {
    out << mystring.m_string << " size: " << mystring.m_size << "\n";
    return out;
}

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

