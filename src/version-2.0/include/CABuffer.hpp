#ifndef CABUFFER_HPP
#define CABUFFER_HPP

#include <vector>
#include <optional>

template <typename T, size_t Size>
class CABuffer{
public:
    CABuffer();

    bool push(const T& item);
    std::optional<T> pop();
    void clear();

    bool is_full() const;
    bool is_empty() const;

private:
    T m_buffer[Size];
    size_t m_head;
    size_t m_tail;
    size_t m_count;
};

#endif // CABUFFER_HPP