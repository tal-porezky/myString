#include <iostream>
#include <cstring>
#include <memory>

class MyStringSharedPointer {
private:
    class ProxyChar {
    public:
        ProxyChar(MyStringSharedPointer& theString, std::size_t index) : m_theString{ theString }, m_index{ index } {}
        
        ProxyChar(const ProxyChar&) = delete;
        
        ProxyChar(ProxyChar&& other) : m_theString{ other.m_theString }, m_index{ other.m_index } {}
        
        char operator=(char c) {
            m_theString.doCopyOnWrite();
            (*m_theString.m_str)[m_index] = c;
            return c;
        }
        
        operator char() const {
            return (*m_theString.m_str)[m_index];
        }
        
        operator char&() {
            m_theString.doCopyOnWrite();
            return (*m_theString.m_str)[m_index];
        }
    private:
        MyStringSharedPointer& m_theString;
        std::size_t m_index{};
    };

public:
    MyStringSharedPointer(const char* str="") : m_str{ std::make_shared<std::string>(str) } {}
    
    ProxyChar operator[](std::size_t index) {
        return { *this, index };
    }
    
    char operator[](std::size_t index) const {
        return (*m_str)[index];
    }
    
    std::size_t length() const {
        return m_str->length();
    }
    
    friend std::ostream& operator<<(std::ostream& out, const MyStringSharedPointer& theString) {
        return out << *(theString.m_str);
    }
private:
    std::shared_ptr<std::string> m_str{};
    
    void doCopyOnWrite() {
        if (!m_str.unique()) {
            *this = m_str->c_str();  // c_str returns the equivalent C-string
        }
    }
};

void foo(char& c, char change) {
    c = change;
}

// int main() {
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     MyStringSharedPointer str1 = "Hello";
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     MyStringSharedPointer str2 = str1;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     str2[4] = '!'; // here we need Copy-on-Write
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     std::cout << "str1 = " << str1 << ", str2 = " << str2 << std::endl;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     str2 = "Bye";
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     std::cout << "str1 = " << str1 << ", str2 = " << str2 << std::endl;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     MyStringSharedPointer str3 = "Hi";
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     std::cout << str3[0] << std::endl;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     MyStringSharedPointer str4 = MyStringSharedPointer("no crash!");
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     str1 = str2 = str3 = str4;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     std::cout << "str1 = " << str1 << ", str2 = " << str2 << ", str3 = " << str3 << ", str4 = " << str4 << std::endl;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     foo(str1[0], 'J');
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     str2[4] = 'R';
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     std::cout << "str1 = " << str1 << ", str2 = " << str2 << ", str3 = " << str3 << ", str4 = " << str4 << std::endl;
// }
