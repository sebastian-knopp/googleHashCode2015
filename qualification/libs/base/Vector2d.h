#pragma once

#include "Exception.h"
#include <vector>


namespace base {


/**
 * @brief A dynamic two dimensional array.
 *
 * @tparam T The type of the elements.
 * @tparam Container The type of the underlying (random access) container.
 *                   E.g., this can be a vector or a deque.
 */
template <typename T, typename Container = std::vector<T> >
class Vector2d
{

public:

    /**
     * @brief Thrown when a interval of time operation causes an overflow.
     */
    struct OutOfRangeException : public base::Exception
    {
        OutOfRangeException()
        : base::Exception("Vector2d was accessed beyond its current width and height")
        {
        }
    };

    /** @brief Constructs an empty two dimensional vector. */
    Vector2d();

    /**
     * @brief Constructs a two dimensional vector with given width and height.
     *
     * @param width The width of the Vector2d object to be constructed.
     * @param height The height of the Vector2d object to be constructed.
     * @param d The default value the elements are initialized with.
     */
    Vector2d(size_t width, size_t height, const T& d = T());

    /**
     * @brief Returns a constant reference to an element
     *        at a given position inside the array.
     *
     * @param x The horizontal index inside the array.
     * @param y The vertical index inside the array.
     * @return Reference to the element at the given index.
     */
    const T& operator()(size_t x, size_t y) const;

    /**
     * @brief Returns a reference to an element
     *        at a given position inside the array.
     *
     * @param x The horizontal index inside the array.
     * @param y The vertical index inside the array.
     * @return Reference to the element at the given index.
     */
    T& operator()(size_t x, size_t y);

    bool operator== (const Vector2d<T>& a_other) const
    {
        return m_height == a_other.m_height &&
               m_array == a_other.m_array;
    }

    bool operator!= (const Vector2d<T>& a_other) const
    {
        return !operator==(a_other);
    }

    /**
     * @brief Returns a constant reference to an element
     *        at a given position inside the array.
     *
     * @param x The horizontal index inside the array.
     * @param y The vertical index inside the array.
     * @return Reference to the element at the given index.
     */
    const T& at(size_t x, size_t y) const;

    /**
     * @brief Returns a reference to an element
     *        at a given position inside the array.
     *
     * @param x The horizontal index inside the array.
     * @param y The vertical index inside the array.
     * @return Reference to the element at the given index.
     */
    T& at(size_t x, size_t y);

    /**
     * @brief Returns if the array is empty.
     *
     * @return \c true if the array is empty, \c false otherwise.
     */
    bool empty() const;

    /**
     * @brief Deletes all elements of the two dimensional array
     *        and sets width and height to zero.
     */
    void clear();

    /**
     * @brief Resizes the array to given sizes.
     *
     * @param width The horizontal size of the array.
     * @param height The vertical size of the array.
     * @param d The default value the elements are initialized with.
     */
    void resize(size_t width, size_t height, const T& d = T());

    /**
     * @brief Exchanges contents of the current array
     *        with those of an other array.
     *
     * This function operates in constant time.
     *
     * @param other Reference to the Array2d object to exchange contents with.
     */
    void swap(Vector2d<T, Container>& other);

    /**
     * @brief Returns the height of this two dimensional array.
     *
     * @return The height of this two dimensional array.
     */
    inline size_t height() const;

    /**
     * @brief Returns the width of this two dimensional array.
     *
     * @return The width of this two dimensional array.
     */
    inline size_t width() const;


private:

    /**
     * @brief Computes the index in m_array of an two dimensional index.
     *
     * @param x The horizontal index inside the array.
     * @param y The vertical index inside the array.
     * @return The index inside m_array.
     */
    inline size_t index(size_t x, size_t y) const;


private:

    /** @brief The height. */
    size_t m_height;

    /** @brief The two dimensional array stored as vector. */
    Container m_array;


};


#include "detail/Vector2d.hpp"


} // namespace base

