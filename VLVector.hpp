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

#define ERR_OUT_OF_RANGE            "Out of range!"

template<class T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector
{
public:
    typedef T*          iterator;
    typedef const T*    const_iterator;
    typedef ptrdiff_t   difference_type;    // I think this the default type for pointer diff

    VLVector() : mSize(0), mCapacity(StaticCapacity), mainArr(stackArr), mArrType(STATIC) {};
    VLVector(const VLVector<T>& vecToCopy);
    template<class InputIterator>
    VLVector(InputIterator first, InputIterator last);
    ~VLVector();

    /* iterator methods */
    inline  iterator        begin()            {return mainArr;};
    inline  iterator        end()              {return mainArr + mSize;};
    inline  const_iterator  begin()    const   {return mainArr;}
    inline  const_iterator  end()      const   {return mainArr + mSize;};
    inline  const_iterator  cbegin()   const   {return mainArr;};
    inline  const_iterator  cend()     const   {return mainArr + mSize;};

    /* class methods */
    inline size_t  size()      const noexcept   {return mSize;}
    inline size_t  capacity()  const noexcept   {return mCapacity;}
    inline bool    empty()     const noexcept   {return (mSize == 0);}
    inline T*      data()      const noexcept   {return mainArr;}
    T&      at (size_t i);
    void    push_back(const T& element);
    void    pop_back()  noexcept;
    void    clear()     noexcept;

    /* operators overloads */
    T&          operator[]  (const size_t& i)        noexcept;
    const T&    operator[]  (const size_t& i)  const noexcept;
    bool        operator!=  (const VLVector<T, StaticCapacity>& rhs) const noexcept;
    bool        operator==  (const VLVector<T, StaticCapacity>& rhs) const noexcept;
    VLVector<T, StaticCapacity>& operator= (const VLVector<T,StaticCapacity>& rhs);

    /* modifiers */
    auto    erase (const_iterator position);
    auto    erase (const_iterator first, const_iterator last);
    auto    insert(const_iterator position, const T& element);
    template<class InputIter>
    auto    insert(const_iterator position, InputIter first, InputIter last);

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

    void    _enlargeCapacity (const size_t &newElements);
    void    _copyAllElements (T* destination)               const;
    size_t  _findIndexOf     (const_iterator position)      const;
    size_t  _capC            (const size_t& newElements)    const;
    void    _switchToStackArr();

    template<class InputIterator> //TODO: consider deletion of this method
    unsigned int _countElements (InputIterator from, InputIterator to) const;

};

template<class T, size_t StaticCapacity>
template<class InputIterator>
VLVector<T, StaticCapacity>::VLVector(InputIterator first, InputIterator last)
{
    mCapacity = StaticCapacity;
    mSize = 0;
    mainArr = stackArr;
    mArrType = STATIC;

    while (first != last)
    {
        if (mSize > mCapacity)
        {   // need to enlarge
            _enlargeCapacity(DEFAULT_ADDED_ELEMENTS);
        }
        mainArr[mSize++] = *first;
        first++;
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
        _copyAllElements(stackArr);
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


template<class T, size_t StaticCapacity>
auto VLVector<T, StaticCapacity>::insert(VLVector::const_iterator position, const T &element)
{
    size_t posIndex = _findIndexOf(position);

    if (mSize >= mCapacity)
    {
        _enlargeCapacity(DEFAULT_ADDED_ELEMENTS);
    }
    for (int i = mSize; i > posIndex; --i)
    {
            mainArr[i] = mainArr[i - 1];
    }
    mainArr[posIndex] = element;
    mSize++;
    return mainArr + posIndex;
}


template<class T, size_t StaticCapacity>
template <class InputIterator>
auto VLVector<T, StaticCapacity>::insert(VLVector::const_iterator position, InputIterator first,
                                         InputIterator last)
{
    size_t posIndex = _findIndexOf(position);
    auto currIter = mainArr + posIndex;
    while (first != last)
    {
        currIter = insert(currIter, *first) + 1;
        first++;
    }
    return mainArr + posIndex;
}


template<class T, size_t StaticCapacity>
auto VLVector<T, StaticCapacity>::erase(VLVector::const_iterator position)
{
    size_t indexOfDel = _findIndexOf(position);
    if ((mSize - 1) == StaticCapacity)
    {
        for (size_t i = 0; i < (mSize - 1); ++i)
        {
            if (i < indexOfDel)
            {
                stackArr[i] = mainArr[i];
            }
            else
            {
                stackArr[i] = mainArr[i + 1];
            }
        }
        _switchToStackArr();
    }
    else
    {
        for (int i = indexOfDel; i < mSize; ++i)
        {
            mainArr[i] = mainArr[i + 1];
        }
    }
    --mSize;
    return mainArr + indexOfDel;
}


template<class T, size_t StaticCapacity>
auto
VLVector<T, StaticCapacity>::erase(VLVector::const_iterator first, VLVector::const_iterator last)
{
    size_t  firstIndex   = _findIndexOf(first);
    size_t  lastIndex    = _findIndexOf(last);
    if (firstIndex >= lastIndex)
    {   // empty range
        return mainArr + lastIndex;
    }

    if (mSize - (lastIndex - firstIndex) <= StaticCapacity)
    {   // need to switch back to stack array
        for (size_t i = 0; i < mSize - (lastIndex - firstIndex); ++i)
        {
            if (i < firstIndex)
            {   //copy normally
                stackArr[i] = mainArr[i];
            }
            else
            {
                stackArr[i] = mainArr[lastIndex - firstIndex + i];
            }
        }
        _switchToStackArr();
    }
    else
    {
        for (size_t i = 0; i < mSize - lastIndex; ++i)
        {
            mainArr[firstIndex + i] = mainArr[lastIndex + i];
        }
    }
    mSize -= (lastIndex - firstIndex);
    return mainArr + firstIndex;
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
    delete[] mainArr;
    mainArr = stackArr;
    mCapacity = StaticCapacity;
    mArrType = STATIC;
}

template<class T, size_t StaticCapacity>
size_t VLVector<T, StaticCapacity>::_findIndexOf(VLVector::const_iterator position) const
{
    size_t currIndex = 0;
    auto start = this->begin();
    for (int i = 0; i < mSize; ++i)
    {
        if (start == position)
        {
            return currIndex;
        }
        currIndex++;
        start++;
    }
    return mSize;
}


//endregion


#endif //CPPEXAM_VLVECTOR_HPP
