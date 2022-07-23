#include <iostream>
#include <cstring>

class MyStringRefCounter {
private: 
    class ProxyChar {
    public:
        ProxyChar(MyStringRefCounter& theString, std::size_t index) : m_theString{ theString }, m_index{ index } {};
        
        ProxyChar(const ProxyChar&) = delete;
        
        ProxyChar(ProxyChar&& other) noexcept : m_theString{ other.m_theString }, m_index{ other.m_index } {}
        
        char operator=(char c) {
            m_theString.doCopyOnWrite();
            m_theString.m_str[m_index] = c;
            return c;
        }
            
        explicit operator char() const {
            return m_theString.m_str[m_index];
        }
        
        explicit operator char&() {
            m_theString.doCopyOnWrite();
            return m_theString.m_str[m_index];
        }
        
    private:
        MyStringRefCounter& m_theString;
        std::size_t m_index{};
    };
public:
    MyStringRefCounter(const char* str="") : m_size{ strlen(str) }, m_refCounter{ new int(1) } {
        std::cout << "calling MyStringRefCounter(const char* str) constrctor\n";
        m_str = new char[m_size + 1];
        strcpy(m_str, str);
    }
    
    MyStringRefCounter(const MyStringRefCounter& other) {
        std::cout << "calling MyStringRefCounter& other) constrctor\n";
        attach(other);
    }
    
    MyStringRefCounter& operator=(const MyStringRefCounter& other) {
        std::cout << "calling operator=(const MyStringRefCounter& other)\n";
        if (m_str != other.m_str) {
            detach();
            attach(other);
        }
        return *this;
    }
    
    MyStringRefCounter(MyStringRefCounter&& other) noexcept : m_size{ other.m_size } {
        std::cout << "calling MyStringRefCounter(MyStringRefCounter&& other)\n";
        std::swap(m_str, other.m_str);
        std::swap(m_refCounter, other.m_refCounter);
    }
    
    MyStringRefCounter& operator=(MyStringRefCounter&& other) noexcept {
        std::cout << "calling operator=(MyStringRefCounter&& other)\n";
        std::swap(m_size, other.m_size);
        std::swap(m_str, other.m_str);
        std::swap(m_refCounter, other.m_refCounter);
        return *this;
    }
    
    ~MyStringRefCounter() {
        std::cout << "calling ~MyStringRefCounter() on " << m_str << " with " << *m_refCounter << " refCount\n";
        detach();
    }
    
    friend std::ostream& operator<<(std::ostream& out, const MyStringRefCounter& myString) {
        return out << myString.m_str << ", refCounter: " << *myString.m_refCounter;
    }
    
    ProxyChar operator[](std::size_t index) {
        return { *this, index };
    }
    
    char operator[](std::size_t index) const {
        return m_str[index];
    }
    
    std::size_t length() const {
        return m_size;
    }
    
private:
    std::size_t m_size{};
    int* m_refCounter{};
    char* m_str{};
    
    void attach(const MyStringRefCounter& other) {
        m_size = other.m_size;
        m_str = other.m_str;
        m_refCounter = other.m_refCounter;
        ++(*m_refCounter);  // updates all m_refCoutner since they are pointers to the same address
    }
    
    void detach() {
        if (m_refCounter) {
            if (--(*m_refCounter) == 0) {
                delete m_refCounter;
                delete[] m_str;
            }
        }
    }
    
    void doCopyOnWrite() {
        if ((m_refCounter) && (*m_refCounter > 1)) {
            std::cout << "doing copy on write\n";
            *this = m_str;
        }
    }
    
    
};

void foo(char& c, char change) {
    c = change;
}

// int main() {
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     MyStringRefCounter str1 = "Hello";
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     MyStringRefCounter str2 = str1;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     str2[4] = '!'; // here we need Copy-on-Write
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     std::cout << "str1 = " << str1 << ", str2 = " << str2 << std::endl;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     str2 = "Bye";
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     std::cout << "str1 = " << str1 << ", str2 = " << str2 << std::endl;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     MyStringRefCounter str3 = "Hi";
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     std::cout << str3[0] << std::endl;
//     std::cout << "*** " << (__LINE__ + 1) << " ***" << std::endl; 
//     MyStringRefCounter str4 = MyStringRefCounter("no crash!");
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