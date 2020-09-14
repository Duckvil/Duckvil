#pragma once

#include <cstddef>

namespace Duckvil { namespace Memory {

    template <typename Type>
    struct Iterator
    {
    public:
        using pointer = Type*;
        using reference = Type&;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;

    private:
        pointer m_pointer;

    public:
        Iterator(pointer _pointer) :
            m_pointer(_pointer)
        {

        }

        Iterator<Type>& operator=(const Iterator<Type>& _iterator) = default;
        Iterator<Type>& operator=(Type* _pointer) { m_pointer = _pointer; return (*this); }

        reference operator*() { return *m_pointer; }
        const reference operator*() const { return *m_pointer; }
        pointer operator->() { return m_pointer; }

        operator bool() const
        {
            if(m_pointer != nullptr)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        bool operator==(const Iterator<Type>& _iterator) const { return m_pointer == _iterator.m_pointer; }
        bool operator!=(const Iterator<Type>& _iterator) const { return m_pointer != _iterator.m_pointer; }

        Iterator<Type>& operator++() { m_pointer++; return *this; }
        Iterator<Type>  operator++(int) { auto _tmp(*this); ++m_pointer; return _tmp; }
        Iterator<Type>& operator--() { m_pointer--; return *this; }
        Iterator<Type>  operator--(int) { auto _tmp(*this); --m_pointer; return _tmp; }

        Iterator<Type>& operator+=(const difference_type& offset) { m_pointer += offset; return *this; }
        Iterator<Type>& operator-=(const difference_type& offset) { m_pointer -= offset; return *this; }

        Iterator<Type> operator+(const difference_type& offset) { auto _old_ptr = m_pointer; m_pointer += offset; auto tmp(*this); m_pointer = _old_ptr; return tmp; }
        Iterator<Type> operator-(const difference_type& offset) { auto _old_ptr = m_pointer; m_pointer -= offset; auto tmp(*this); m_pointer = _old_ptr; return tmp; }

        difference_type operator-(const Iterator<Type>& _iterator) { return _iterator.m_pointer - m_pointer; }
    };

}}