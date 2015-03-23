
template <typename T, typename Container>
Vector2d<T, Container>::Vector2d()
: m_height(0)
, m_array()
{
}


template <typename T, typename Container>
Vector2d<T, Container>::Vector2d(const size_t width,
                                 const size_t height,
                                 const T& d)
: m_height(height)
, m_array(width * height, d)
{
}


template <typename T, typename Container>
const T& Vector2d<T, Container>::operator()(const size_t x, const size_t y) const
{
    return m_array[index(x, y)];
}


template <typename T, typename Container>
T& Vector2d<T, Container>::operator()(const size_t x, const size_t y)
{
    return m_array[index(x, y)];
}


template <typename T, typename Container>
const T& Vector2d<T, Container>::at(const size_t x, const size_t y) const
{
    return const_cast<T&>(const_cast<Vector2d<T, Container>*>(this)->at(x, y));
}


template <typename T, typename Container>
T& Vector2d<T, Container>::at(const size_t x, const size_t y)
{
    if (x >= width())
        Exception::raise<OutOfRangeException>();
    if (y >= height())
        Exception::raise<OutOfRangeException>();

    return m_array[index(x, y)];
}


template <typename T, typename Container>
bool Vector2d<T, Container>::empty() const
{
    return m_array.empty();
}


template <typename T, typename Container>
void Vector2d<T, Container>::clear()
{
    m_height = 0;
    m_array.clear();
}


template <typename T, typename Container>
void Vector2d<T, Container>::resize(const size_t width,
                                    const size_t height,
                                    const T& d)
{
    m_array.resize(width * height, d);
    m_height = height;
}


template <typename T, typename Container>
void Vector2d<T, Container>::swap(Vector2d<T, Container>& other)
{
    m_array.swap(other.m_array);
    swap(m_height, other.m_height);
}


template <typename T, typename Container>
inline size_t Vector2d<T, Container>::height() const
{
    return m_height;
}


template <typename T, typename Container>
inline size_t Vector2d<T, Container>::width() const
{
    if (m_height == 0)
    {
        return 0;
    }
    return m_array.size() / m_height;
}


template <typename T, typename Container>
inline size_t Vector2d<T, Container>::index(const size_t x, const size_t y) const
{
    return x * m_height + y;
}

