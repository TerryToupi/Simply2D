/**
 * @file span.hpp
 * @brief Lightweight non-owning view over a contiguous sequence of objects.
 *
 * This file defines the @ref utils::Span class template, which provides a safe,
 * read-only view over arrays, initializer lists, or raw pointers for generic
 * code that needs sequence access without ownership or copying.
 *
 * @date 2025-08-13
 * author Terry Toupi
 */

#ifndef __SPAN_HPP__
#define __SPAN_HPP__

#include <initializer_list>
#include <stdexcept>

/**
 * @brief Non-owning view over a contiguous sequence of objects.
 *
 * The Span class provides a convenient, bounds-checked interface for accessing
 * arrays, initializer lists, or raw memory regions as a sequence, without managing
 * their lifetime. It is especially useful for function arguments, simplifying code
 * that works with slices of data.
 *
 * @tparam T Type of elements in the sequence.
 */
template <typename T>
class Span {
public:
    /**
     * @brief Default constructor. Creates an empty span.
     */
    Span()
        : m_Data(nullptr), m_Size(0) {}

    /**
     * @brief Constructs a span from a raw pointer and size.
     * @param data Pointer to the first element.
     * @param size Number of elements.
     */
    Span(const T* data, size_t size)
        : m_Data(data), m_Size(size) {}

    /**
     * @brief Constructs a span from a C-style array.
     * @tparam N Number of elements in the array.
     * @param arr Reference to the array.
     */
    template <size_t N>
    Span(const T(&arr)[N])
        : m_Data(arr), m_Size(N) {}

    /**
     * @brief Constructs a span from an initializer list.
     * @param list Initializer list.
     */
    Span(const std::initializer_list<T>& list)
        : m_Data(list.begin()), m_Size(list.size()) {}

    /**
     * @brief Returns a pointer to the first element.
     * @return Pointer to data.
     */
    const T* data() const { return m_Data; }

    /**
     * @brief Returns the number of elements in the span.
     * @return Number of elements.
     */
    size_t size() const { return m_Size; }

    /**
     * @brief Returns a pointer to the first element (for range-based for loops).
     * @return Pointer to the beginning.
     */
    const T* begin() const { return m_Data; }

    /**
     * @brief Returns a pointer to one past the last element (for range-based for loops).
     * @return Pointer to the end.
     */
    const T* end() const { return m_Data + m_Size; }

    /**
     * @brief Returns a reference to the element at the specified index, with bounds checking.
     * @param idx Index of the element.
     * @return Reference to the element.
     * @throws std::out_of_range if idx >= size().
     */
    const T& at(size_t idx) const {
        if (idx >= m_Size) {
            throw std::out_of_range("Span out of Range!");
        }
        return m_Data[idx];
    }

    /**
     * @brief Returns a reference to the element at the specified index.
     *        Equivalent to at(idx).
     * @param idx Index of the element.
     * @return Reference to the element.
     * @throws std::out_of_range if idx >= size().
     */
    const T& operator[](size_t idx) const {
        return at(idx);
    }

private:
    const T* m_Data = nullptr; /**< Pointer to the first element */
    size_t m_Size = 0;         /**< Number of elements in the span */
};

#endif // __SPAN_HPP__