/**
 * Circular queue template class
 */
#pragma once

namespace MJ
{
template<typename T>
class CircularLinkedListNode
{
public:
    void set_next(T*);
    T* next() const;
};

template<typename T>
inline void CircularLinkedListNode<T>::set_next(T* next)
{
    static_cast<T*>(this)->m_next = next;
}

template<typename T>
inline T* CircularLinkedListNode<T>::next() const
{
    return static_cast<const T*>(this)->m_next;
}

template<typename T>
class CircularLinkedList
{
public:
    CircularLinkedList() {}

    T* head() const { return m_head; }
    T* tail() const { return m_tail; }

    void append(T*);

private:
    T* m_head;
    T* m_tail;
};

template<typename T>
inline void CircularLinkedList<T>::append(T* node)
{
    if(m_head == nullptr)
    {
        m_head = node;
        m_tail = m_head;
        return;
    }

    m_tail->set_next(node);
    m_tail = node;
    m_tail->set_next(m_head);
}

} // namespace MJ
