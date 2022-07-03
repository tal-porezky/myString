#include <cstring>
#include <iostream>

class MyString {
private:
    std::size_t m_size{};
    char* m_string{};

    explicit MyString(std::size_t len, const char* str);

public:
    MyString(const char* str);
    
    MyString(const MyString& other);

    MyString(MyString&& other) noexcept;

    ~MyString();
    
    MyString& operator=(const MyString& other);

    MyString& operator=(MyString&& other) noexcept;
    
    friend std::ostream& operator<<(std::ostream& out, const MyString& mystring);
    
    std::size_t size() const;
    
};