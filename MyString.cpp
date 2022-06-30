#include "MyString.h"

MyString::MyString(std::size_t len, const char* str) : m_size{ len }, m_string{ new char[m_size + 1]} {
    strcpy(m_string, str);
}

MyString::MyString(const char* str) : MyString(strlen(str), str) {}

MyString::MyString(const MyString& other) : MyString(other.m_size, other.m_string) {}

MyString::MyString(MyString&& other) noexcept {
    std::swap(m_size, other.m_size);
    std::swap(m_string, other.m_string);
}

MyString::~MyString() {
    delete[] m_string;
    m_string = nullptr;
}

MyString& MyString::operator=(const MyString& other) {
    if (this == &other) {
        return *this;
    }
    delete[] m_string;
    m_size = other.m_size;
    m_string = new char[other.m_size + 1];
    strcpy(m_string, other.m_string);
    return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    std::swap(m_size, other.m_size);
    std::swap(m_string, other.m_string);
    return *this;
}

std::ostream& operator<<(std::ostream& out, const MyString& mystring) {
    out << mystring.m_string << " size: " << mystring.m_size << "\n";
    return out;
}

std::size_t MyString::size() const {
    return m_size;
}