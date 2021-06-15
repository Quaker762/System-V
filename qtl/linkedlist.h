/**
 * 
 */
#pragma once

#define DEBUG_LL

namespace qtl
{

template<typename T>
class LinkedList
{
private:
    struct Node
    {
        explicit Node(const T& value)
        : m_value(value) {}
        T m_value;
        Node* next;
    };

public:
    LinkedList() {};

    inline bool empty() { return m_head == nullptr; }
    inline bool contains(T&);
    inline void insert(T&);
    inline void remove(T&);
    inline T* take();

private:
    Node* m_head { nullptr };
    Node* m_tail { nullptr };
};

template<typename T>
inline bool LinkedList<T>::contains(T& value)
{
    for(T* node = m_head; node; node = node->next)
    {
        if(node->data == value)
            return true;
    }

    return false;
}

template<typename T>
inline void LinkedList<T>::insert(T& data)
{
    Node* node = new Node(data);

    if(m_head == nullptr)
    {
        m_head = node;
        m_tail = m_head;
        return;
    }

    m_tail->next = node;
    m_tail = node;
    m_tail->next = nullptr;
}

template<typename T>
inline void LinkedList<T>::remove(T& find)
{
    if(find == m_head)
    {
        m_head = m_head->next;
    }
    else if(find == m_tail)
    {
        for(T* node = m_head; node; node = node->next)
        {
            if(node->next == m_tail)
            {
                m_tail = node;
                m_tail->next = nullptr;
            }
        }
    }
    else
    {
        for(T* node = m_head; node; node = node->next)
        {
            if(node->next == find)
                node->next = find->next;
        }
    }
}

template<typename T>
inline T* LinkedList<T>::take()
{
    if(!m_head)
        return nullptr;

    Node* node = m_head;
    m_head = m_head->next;

    return &node->m_value;
}

}
