#ifndef VECTOR_H
#define VECTOR_H

#include <initializer_list>
#include <memory>

template <class T> class Vector
{
public:
    class Iterator
    {
        T* it;

    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using iterator_category = std::random_access_iterator_tag;


        Iterator() = default;
        Iterator(const Iterator& other) noexcept : it{other.it} {}
        Iterator(Iterator&& other) noexcept : it{other.it}
        {
            other.it = nullptr;
        }

        Iterator& operator=(const Iterator& other) noexcept
        {
            it = other.it;
            return *this;
        }
        Iterator& operator=(Iterator&& other) noexcept
        {
            it = other.it;
            other.it = nullptr;
            return *this;
        }

        bool operator==(const Iterator& other) const noexcept
        {
            return it == other;
        }
        bool operator!=(const Iterator& other) const noexcept
        {
            return !(*this == other);
        }
        Iterator& operator++()
        {
            it += 1;
            return it;
        }
        Iterator& operator++(int)
        {
            auto temp = it;
            it += 1;
            return temp;
        }

        Iterator& operator--()
        {
            it -= 1;
            return it;
        }

        Iterator& operator--(int)
        {
            auto temp = it;
            it -= 1;
            return temp;
        }

        reference operator*() noexcept { return &(*it); }
        const_reference operator*() const noexcept { return &(*it); }

        pointer operator->() noexcept { return it; }
        const_pointer operator->() const noexcept { return it; }

        void swap(Iterator& other) noexcept(
            noexcept(std::is_move_constructible<T>::value))
        {
            Iterator tmp{std::move(it)};
            it = std::move(other.it);
            other.it = std::move(tmp);
        }

        ~Iterator(){};
    };

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = Iterator;
    using const_iterator = const Iterator;
    using size_type = std::size_t;

    Vector() = default;
    Vector(size_type size);
    Vector(size_type, const_reference value);
    Vector(std::initializer_list<value_type> lst);
    Vector(const Vector& v);
    Vector(Vector&& v);

    // iterators
    iterator begin() { return m_elem; }
    iterator end() { return m_elem[m_space]; }
    const_iterator cbegin() const { return m_elem; }
    const_iterator cend() const { return m_elem[m_space]; }

    // element access
    reference operator[](size_type n) { return m_elem[n]; }
    const_reference operator[](size_type n) const { return m_elem[n]; }
    reference at(size_type n);
    const_reference at(size_type n) const;

    // capacity
    bool empty() { return m_size == 0; }
    size_type size() { return m_size; }
    size_type space() { return m_space; }
    void reserve(size_type new_space);


    // modifiers
    void push_back(const_reference elem);
    void push_back(value_type&& elem);
    void pop_back();
    constexpr void resize(size_type new_size);

private:
    pointer m_elem;
    size_type m_size;
    size_type m_space;
};


template <class T>
Vector<T>::Vector(std::size_t size)
    : m_elem{new T[size]}, m_size{size}, m_space{size}
{
}

template <class T>
Vector<T>::Vector(std::size_t size, const_reference value)
    : m_elem{new T[size]}, m_size{size}, m_space{size}
{
    for (auto it = m_elem->begin(); it != this->end(); ++it)
    {
        *it = value;
    }
}

template <class T>
Vector<T>::Vector(std::initializer_list<T> lst)
    : m_elem{new T[lst.size()]}, m_size{lst.size()}, m_space{lst.size()}
{
    std::size_t i{0};
    for (const auto& elem : lst)
    {
        m_elem[i] = elem;
        ++i;
    }
}

template <class T>
Vector<T>::Vector(const Vector<T>& v)
    : m_elem{new T[v.m_space]}, m_size{v.m_size}, m_space{v.m_space}
{
    for (auto i{0}; i < m_size; ++i)
    {
        m_elem[i] = v.m_elem[i];
    }
}

template <class T>
Vector<T>::Vector(Vector&& v)
    : m_elem{std::move(v.m_elem)}, m_size{v.m_size}, m_space{v.m_space}
{
    v.m_elem = nullptr;
    v.m_size = 0;
    v.m_space = 0;
}

template <class T> T& Vector<T>::at(std::size_t n)
{
    if (n < 0 || n > (m_size - 1))
        throw std::runtime_error("Out of range\n");
    return m_elem[n];
}

template <class T> const T& Vector<T>::at(std::size_t n) const
{
    if (n < 0 || n > (m_size - 1))
        throw std::runtime_error("Out of range\n");
    return m_elem[n];
}

template <class T> void Vector<T>::reserve(size_type new_space)
{
    if (m_space < new_space)
        return;

    T* temp{new T[new_space]};
    for (size_type i{0}; i < m_size; ++i)
    {
        temp[i] = m_elem[i];
    }
    delete[] m_elem;
    m_elem = temp;
    m_space = new_space;
}

template <class T> void Vector<T>::push_back(const_reference elem)
{
    if (!m_space)
    {
        m_size = m_space = 1;
        m_elem = new value_type[1]{elem};
    }
    else if (m_size < m_space)
    {
        m_elem[m_size] = elem;
        ++m_size;
    }
    else
    {
        constexpr double coef_of_growth{1.5};
        reserve(m_space * coef_of_growth);
        m_elem[m_size] = elem;
    }
}

template <class T> void Vector<T>::push_back(value_type&& elem)
{
    if (!m_space)
    {
        m_size = m_space = 1;
        m_elem = new value_type[1]{std::move(elem)};
    }
    else if (m_size < m_space)
    {
        m_elem[m_size] = std::move(elem);
        ++m_size;
    }
    else
    {
        constexpr double coef_of_growth{1.5};
        reserve(m_space * coef_of_growth);
        m_elem[m_size] = std::move(m_elem);
    }
}

template <class T> void Vector<T>::pop_back()
{
    --m_size;
}

template <class T> constexpr void Vector<T>::resize(size_type new_size)
{
    if (new_size <= m_space)
        m_size = new_size;
    else
    {
        this->reserve(new_size);
        m_size = new_size;
    }
}


#endif // VECTOR_H
