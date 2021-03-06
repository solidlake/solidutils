/**
 * @file Alloc.hpp
 * @brief Memory allocation utilities;
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017-2018, Solid Lake LLC
 * @version 1
 * @date 2017-10-10
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */



#ifndef SOLIDUTILS_INCLUDE_ALLOC_HPP
#define SOLIDUTILS_INCLUDE_ALLOC_HPP



#include <cstring>
#include <new>
#include <string>


namespace sl
{


class NotEnoughMemoryException :
    public std::bad_alloc
{
  public:
      NotEnoughMemoryException(
          size_t const numChunks,
          size_t const chunkSize) noexcept :
        std::bad_alloc(),
        m_msg(std::string("Failed to allocate ") + \
              std::to_string(numChunks*chunkSize) + \
              std::string(" bytes in ") + std::to_string(numChunks) + \
              std::string(" chunks of size ") + \
              std::to_string(chunkSize) + std::string("."))
      {
        // do nothing
      }

      const char * what() const noexcept override
      {
        return m_msg.c_str();
      }

  private:
    std::string m_msg;
};


/**
* @brief The Alloc class provides a set of static functions for allocating
* templated memory via malloc() and associated functions.
*/
class Alloc
{
  public:
    /**
    * @brief Allocate a block of uninitialized memory that can be free'd with a
    * call to Alloc::free(). If the amount of memory
    * requested is 0, then nullptr will be returned.
    *
    * @tparam T The type of memory to allocate.
    * @param num The number of elements.
    *
    * @return The memory.
    *
    * @throws std::bad_alloc If the amount of memory fails to get allocated.
    */
    template<typename T>
    static T * uninitialized(
        size_t const num)
    {
      constexpr size_t const chunkSize = sizeof(T);

      size_t const numBytes = chunkSize*num;

      if (numBytes > 0) {
        T * const data = reinterpret_cast<T*>(malloc(numBytes));

        if (data == nullptr) {
          throw NotEnoughMemoryException(num, chunkSize);
        }
        return data;
      } else {
        return nullptr;
      }
    }


    /**
    * @brief Allocate and initialize a block of memory to a constant value.
    *
    * @tparam T The type of memory to allocate.
    * @param num The number of elements.
    * @param val The value to initialize elements to.
    *
    * @return The memory.
    *
    * @throws std::bad_alloc If the amount of memory fails to get allocated.
    */
    template<typename T>
    static T * initialized(
        size_t const num,
        T const val = static_cast<T>(0))
    {
      T * const data = uninitialized<T>(num);
      for (size_t i = 0; i < num; ++i) {
        data[i] = val;
      }

      return data;
    }

    /**
    * @brief Allocate and and fill a block of memory from another block.
    *
    * @tparam T The type of memory to allocate.
    * @param ptr The other bock of memory.
    * @param num The size of the other block (in terms of elements).
    *
    * @return The memory.
    *
    * @throws std::bad_alloc If the amount of memory fails to get allocated.
    */
    template<typename T>
    static T * duplicate(
        T const * const ptr,
        size_t const num)
    {
      T * const data = uninitialized<T>(num);
      for (size_t i = 0; i < num; ++i) {
        data[i] = ptr[i];
      }

      return data;
    }


    /**
    * @brief Resize an allocation.
    *
    * @tparam T The type of element.
    * @param ptr The pointer to resize.
    * @param num The number of elements.
    *
    * @throws std::bad_alloc If the amount of memory fails to get allocated.
    */
    template<typename T>
    static void resize(
        T ** const ptr,
        size_t const num)
    {
      constexpr size_t const chunkSize = sizeof(T);
      T * const newPtr = reinterpret_cast<T*>( \
          std::realloc(*ptr, num*chunkSize));
      if (newPtr == nullptr) {
          throw NotEnoughMemoryException(num, chunkSize);
      }

      *ptr = newPtr;
    }


    /**
    * @brief Free a block of memory allocated with this class.
    *
    * @tparam T The type of memory to free.
    * @param ptr A pointer to the memory to free.
    */
    template<typename T>
    static void free(
        T * const ptr) noexcept
    {
      if (ptr != nullptr) {
        std::free(ptr);
      }
    }
};

}


#endif
