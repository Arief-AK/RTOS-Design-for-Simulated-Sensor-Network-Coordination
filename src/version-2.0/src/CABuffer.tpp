#ifndef CABUFFER_TPP
#define CABUFFER_TPP
#include "CABuffer.hpp"

template <typename T, size_t Size>
CABuffer<T, Size>::CABuffer() :m_head(0), m_tail(0), m_count(0) {}

template<typename T, size_t Size>
bool CABuffer<T, Size>::push(const T & item){
    auto flag = false;
    if(!is_full()){
        m_buffer[m_tail] = item;
        m_tail = (m_tail + 1) % Size;
        ++m_count;
        flag = true;
    }
    return flag;
}

template<typename T, size_t Size>
std::optional<T> CABuffer<T, Size>::pop(){
    if(!is_empty()){
        T item = m_buffer[m_head];
        m_head = (m_head + 1) % Size;
        --m_count;
        return item;
    } else{
        return std::nullopt;
    }
}

template<typename T, size_t Size>
void CABuffer<T, Size>::clear(){
    m_head = 0;
    m_tail = 0;
    m_count = 0;
}

template<typename T, size_t Size>
bool CABuffer<T, Size>::is_full() const{
    return m_count == Size;
}

template<typename T, size_t Size>
bool CABuffer<T, Size>::is_empty() const{
return m_count == 0;
}

#endif // CABUFFER_TPP

