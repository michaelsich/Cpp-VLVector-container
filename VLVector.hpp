//
// Created by michael on 05/08/2020.
//

#ifndef CPPEXAM_VLVECTOR_HPP
#define CPPEXAM_VLVECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <cmath>
#include <iterator> //TODO: delete~~~~!!!!
#include <vector>

#define DEFAULT_STATIC_CAPACITY     16
#define CAP_C_FACTOR                (3.0 / 2.0)
#define DEFAULT_ADDED_ELEMENTS      1

#define ERR_OUT_OF_RANGE            "Out of range"
#define ERR_INVALID_POP             "No elements to pop"

template<class T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector
{
public:
    typedef T* iterator;
    
    //TODO: delete commented
//    typedef typename VLVector<T>::Iterator      iterator;
//    typedef typename VLVector<T>::ConstIterator const_iterator;
//
//    typedef typename std::vector<T>::iterator       iterator;
//    typedef typename std::vector<T>::const_iterator const_iterator;

    VLVector() : mSize(0), mCapacity(StaticCapacity), mainArr(stackArr), mArrType(STATIC) {};
    VLVector(const VLVector<T>& vecToCopy);
    template<class InputIterator>
    VLVector(InputIterator& first, InputIterator& last);
    ~VLVector();

    iterator begin();

    inline size_t  size()      const noexcept   {return mSize;}
    inline size_t  capacity()  const noexcept   {return mCapacity;}
    inline bool    empty()     const noexcept   {return (mSize == 0);}
    inline T*      data()      const noexcept   {return mainArr;}
    T&      at (size_t i);
    void    push_back(const T& element);
    void    pop_back()  noexcept;
    void    clear()     noexcept;

    T&          operator[]  (const size_t& i)       noexcept;
    const T&    operator[]  (const size_t& i) const noexcept;
    bool        operator==  (const VLVector<T, StaticCapacity>& rhs) const noexcept;
    bool        operator!=  (const VLVector<T, StaticCapacity>& rhs) const noexcept;
    VLVector<T, StaticCapacity>& operator= (const VLVector<T,StaticCapacity>& rhs);



private:
    enum ArrType
    {
        DYNAMIC,
        STATIC
    };

    //region class members
    size_t  mSize;
    size_t  mCapacity;
    T*      mainArr;
    T       stackArr[StaticCapacity];
    ArrType mArrType;
    //endregion

    size_t  _capC            (const size_t& newElements)    const;
    void    _enlargeCapacity (const size_t &newElements);
    void    _copyAllElements (T* destination)               const;
    void    _switchToStackArr();

    template<class InputIterator>
    unsigned int _countElements (InputIterator from, InputIterator to) const;

public://TODO: try to change to private
    //region class Iterators
    class Iterator
    {
    public:
        /* iter traits */
        typedef T           value_type;
        typedef T&          reference;
        typedef T*          pointer;
        typedef ptrdiff_t   difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        /* read / write */
        Iterator(pointer ptr = nullptr) : currIter(ptr) {};
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

    class ConstIterator
    {
    public:
        /* iter traits */
        typedef T           value_type;
        typedef T&          reference;
        typedef T*          pointer;
        typedef ptrdiff_t   difference_type;
        typedef std::random_access_iterator_tag iterator_category;

        /* read / write */
        ConstIterator(pointer ptr = nullptr) : currIter(ptr) {};
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

template<class T, size_t StaticCapacity>
template<class InputIterator>
VLVector<T, StaticCapacity>::VLVector(InputIterator& first, InputIterator& last)
{
    mCapacity = StaticCapacity;
    mSize = 0;
    mainArr(stackArr);
    mArrType(STATIC);

    while (first != last)
    {
        if (mSize > mCapacity)
        {   // need to enlarge
            _enlargeCapacity(DEFAULT_ADDED_ELEMENTS);
        }
        mainArr[mSize++] = *first;
    }
}

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
    mArrType = vecToCopy.mArrType;

    switch (mArrType)
    {
        case STATIC:
            mainArr = stackArr;
            break;

        case DYNAMIC:
            mainArr = new T[mCapacity];
            break;
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

template<class T, size_t StaticCapacity>
T &VLVector<T, StaticCapacity>::at(size_t i)
{
    if (i >= mSize || i < 0)
    {
        throw std::out_of_range(ERR_OUT_OF_RANGE);
    }
    return mainArr[i];
}

template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::push_back(const T &element)
{
    if (mSize >= mCapacity)
    {
        _enlargeCapacity(DEFAULT_ADDED_ELEMENTS);
    }
    mainArr[mSize++] = element;
}

template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::clear() noexcept
{
    if (mArrType == DYNAMIC)
    {
        delete [] mainArr;
    }
    mainArr = stackArr;
    mArrType = STATIC;
    mSize = 0;
    mCapacity = StaticCapacity;

}


template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::pop_back() noexcept
{
    mSize--;
    if (mSize <= StaticCapacity && mArrType == DYNAMIC)
    {
        _switchToStackArr();
    }
}


template<class T, size_t StaticCapacity>
VLVector<T, StaticCapacity> &
VLVector<T, StaticCapacity>::operator=(const VLVector<T, StaticCapacity> &rhs)
{
    if (this == rhs)
    {
        return *this;
    }
    if (this->mArrType == DYNAMIC)
    {
        delete[] mainArr;
        mainArr = stackArr;
    }
    mArrType    = rhs.mArrType;
    mCapacity   = rhs.mCapacity;
    mSize       = rhs.mSize;

    if (mArrType == DYNAMIC)
    {
        mainArr = new T[mCapacity];
    }
    for (size_t i = 0; i < mSize; ++i)
    {
        mainArr[i] = rhs.mainArr[i];
    }
    return *this;
}


template<class T, size_t StaticCapacity>
T &VLVector<T, StaticCapacity>::operator[](const size_t &i) noexcept
{
    return mainArr[i];
}

template<class T, size_t StaticCapacity>
const T &VLVector<T, StaticCapacity>::operator[](const size_t &i) const noexcept
{
    return mainArr[i];
}


template<class T, size_t StaticCapacity>
bool VLVector<T, StaticCapacity>::operator==(const VLVector<T, StaticCapacity> &rhs) const noexcept
{
    if (mSize != rhs.mSize)
    {
        return false;
    }
    for (size_t i = 0; i < mSize; ++i)
    {
        if (mainArr[i] != rhs.mainArr[i])
            return false;
    }
    return true;
}

template<class T, size_t StaticCapacity>
bool VLVector<T, StaticCapacity>::operator!=(const VLVector<T, StaticCapacity> &rhs) const noexcept
{
    if (mSize != rhs.mSize)
    {
        return true;
    }
    for (size_t i = 0; i < mSize; ++i)
    {
        if (mainArr[i] != rhs.mainArr[i])
            return true;
    }
    return false;
}




//region private methods
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

template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::_enlargeCapacity(const size_t &newElements)
{
    size_t  newCap = _capC(newElements);
    T*      newArr = new T[newCap];

    _copyAllElements(newArr);
    if (mArrType == DYNAMIC)
    {
        delete[] mainArr;
    }
    mArrType = DYNAMIC;
    mainArr = newArr;
    mCapacity = newCap;
}

template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::_copyAllElements(T *destination) const
{
    if (destination)
    {
        for (int i = 0; i < mSize; ++i)
        {
            destination[i] = mainArr[i];
        }
    }
}

//TODO: consider deletion of this method
template<class T, size_t StaticCapacity>
template<class InputIterator>
unsigned int VLVector<T, StaticCapacity>::_countElements(InputIterator from, InputIterator to) const
{
    unsigned int countedElements = 0;
    while (from != to)
    {
        countedElements++;
        from++;
    }
    return countedElements;
}

template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::_switchToStackArr()
{
    for (int i = 0; i < mSize; ++i)
    {
        stackArr[i] = mainArr[i];
    }
    delete[] mainArr;
    mainArr = stackArr;
    mCapacity = StaticCapacity;
    mArrType = STATIC;
}

//endregion


#endif //CPPEXAM_VLVECTOR_HPP
