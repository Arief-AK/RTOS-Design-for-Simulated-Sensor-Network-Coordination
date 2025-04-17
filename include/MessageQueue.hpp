#ifndef MESSAGE_QUEUE_HPP
#define MESSAGE_QUEUE_HPP

#include <iostream>
#include <queue>

struct Message{
    int id;
    int timestamp;
    std::string payload;
};

class MessageQueue{
public:
    void enqueue(const Message &message);
    bool dequeue(Message &message);
    bool isEmpty() const;
    int size() const;

private:
    std::queue<Message> m_queue;
};

#endif // MESSAGE_QUEUE_HPP