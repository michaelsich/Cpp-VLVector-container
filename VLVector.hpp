//
// Created by michael on 05/08/2020.
//

#ifndef CPPEXAM_VLVECTOR_HPP
#define CPPEXAM_VLVECTOR_HPP

#include <cstddef>
#include <iterator>
#include <cmath>

#define DEFAULT_STATIC_CAPACITY     16
#define CAP_C_FACTOR                (3 / 2)

template<class T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector
{
public:
    typedef typename VLVector<T>::VecIter       iterator;
    typedef typename VLVector<T>::ConstVecIter  const_iterator;

    VLVector() : mSize(0), mCapacity(0) {*mainArr = {};};
    VLVector(const VLVector<T>& vecToCopy);
    template<class InputIterator>
    VLVector(InputIterator& first, InputIterator& last);
    ~VLVector();



    inline size_t  size()      const noexcept {return mSize;}
    inline size_t  capacity()  const noexcept {return mCapacity;}
    inline bool    empty()     const noexcept;

    void    push_back(T);

private:
    size_t  mSize;
    size_t  mCapacity;
    T*      mainArr;

    size_t  _capC(const size_t& newElements) const;

    //region class Iterators
    class VecIter
    {
    public:
        /* iter traits */
        typedef T           value_type;
        typedef T&          reference;
        typedef T*          pointer;
        typedef ptrdiff_t   difference_type;    //TODO: verify allowed to use this
        typedef std::random_access_iterator_tag iterator_category;

        /* read / write */
        VecIter(pointer ptr = nullptr) : currIter(ptr) {};
        inline  reference operator*() const     {return *currIter;} // de-reference iter

        /* iteration */
        inline  iterator&   operator++()   {currIter = currIter + 1; return *this;}
        inline  iterator&   operator--()   {currIter = currIter - 1; return *this;}
        inline  iterator&   operator-=(difference_type rhs) {currIter -= rhs; return *this;}
        inline  iterator&   operator+=(difference_type rhs) {currIter += rhs; return *this;}
        inline  iterator    operator-(difference_type rhs) const {return iterator(currIter - rhs);}
        inline  iterator    operator+(difference_type rhs) const {return iterator(currIter + rhs);}

        friend iterator operator-(difference_type distance,
                                  const iterator& rhs) {return Iterator(distance - rhs.currIter);}
        friend iterator operator+(difference_type distance,
                                  const iterator& rhs) {return Iterator(distance + rhs.currIter);}

        iterator operator++(int)
        {
            iterator temp(*this);
            currIter++;
            return temp;
        }
        iterator operator--(int)
        {
            iterator temp(*this);
            currIter++;
            return temp;
        }

        // comparison section
        inline bool operator==  (const iterator& rhs) const {return currIter == rhs.currIter;}
        inline bool operator!=  (const iterator& rhs) const {return currIter != rhs.currIter;}
        inline bool operator>=  (const iterator& rhs) const {return currIter >= rhs.currIter;}
        inline bool operator<=  (const iterator& rhs) const {return currIter <= rhs.currIter;}
        inline bool operator<   (const iterator& rhs) const {return currIter <  rhs.currIter;}
        inline bool operator>   (const iterator& rhs) const {return currIter >  rhs.currIter;}

    private:
        T* currIter;
    };

    class ConstVecIter
    {
    public:
        /* iter traits */
        typedef T           value_type;
        typedef T&          reference;
        typedef T*          pointer;
        typedef ptrdiff_t   difference_type;    //TODO: verify allowed to use this
        typedef std::random_access_iterator_tag iterator_category;
        
        /* read / write */
        ConstVecIter(pointer ptr = nullptr) : currIter(ptr) {};
        inline  value_type operator*() const     {return *currIter;} // de-reference iter

        /* iteration */
        inline  const_iterator&   operator++()   {currIter = currIter + 1; return *this;}
        inline  const_iterator&   operator--()   {currIter = currIter - 1; return *this;}
        inline  const_iterator&   operator-=(difference_type rhs) {currIter -= rhs; return *this;}
        inline  const_iterator&   operator+=(difference_type rhs) {currIter += rhs; return *this;}
        inline  const_iterator    operator-(difference_type rhs) const {return const_iterator(currIter - rhs);}
        inline  const_iterator    operator+(difference_type rhs) const {return const_iterator(currIter + rhs);}

        friend const_iterator operator-(difference_type distance,
                                  const const_iterator& rhs) {return Iterator(distance - rhs.currIter);}
        friend const_iterator operator+(difference_type distance,
                                  const const_iterator& rhs) {return Iterator(distance + rhs.currIter);}

        const_iterator operator++(int)
        {
            iterator temp(*this);
            currIter++;
            return temp;
        }
        const_iterator operator--(int)
        {
            iterator temp(*this);
            currIter++;
            return temp;
        }

        // comparison section
        inline bool operator==  (const const_iterator& rhs) const {return currIter == rhs.currIter;}
        inline bool operator!=  (const const_iterator& rhs) const {return currIter != rhs.currIter;}
        inline bool operator>=  (const const_iterator& rhs) const {return currIter >= rhs.currIter;}
        inline bool operator<=  (const const_iterator& rhs) const {return currIter <= rhs.currIter;}
        inline bool operator<   (const const_iterator& rhs) const {return currIter <  rhs.currIter;}
        inline bool operator>   (const const_iterator& rhs) const {return currIter >  rhs.currIter;}

    private:
        T* currIter;
    };
    //endregion


};

/**
 * @brief Copy Constructor
 * @tparam T
 * @tparam StaticCapacity
 * @param vecToCopy
 */
template<class T, size_t StaticCapacity>
VLVector<T, StaticCapacity>::VLVector(const VLVector<T> &vecToCopy)
{
    mSize = vecToCopy.mSize;
    mCapacity = vecToCopy.mCapacity;

    if (mCapacity > StaticCapacity)
    {   //static array
        *mainArr = {};
    }
    else
    {   // dynamic arr
        mainArr = new T[mCapacity];  // TODO: should I add nothrow?
    }
    for (size_t i = 0; i < mSize; ++i)
    {
        mainArr[i] = vecToCopy.mainArr[i];
    }
}

/**
 * @brief class destructor
 * @tparam T
 * @tparam StaticCapacity
 */
template<class T, size_t StaticCapacity>
VLVector<T, StaticCapacity>::~VLVector()
{
    if (mCapacity > StaticCapacity)
    {
        delete[] mainArr;
    }
}

/**
 * @brief calculates the best capacity for VLVector
 * @param newElements num of elements of T planned to be added
 * @return best calculated capacity
 */
template<class T, size_t StaticCapacity>
size_t VLVector<T, StaticCapacity>::_capC(const size_t &newElements) const
{
    if (mSize + newElements <= StaticCapacity)
    {
        return StaticCapacity;
    }
    else
    {
        return std::floor((CAP_C_FACTOR) * (mSize + newElements) );
    }
}


#endif //CPPEXAM_VLVECTOR_HPP
