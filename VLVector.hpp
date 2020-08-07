//
// Created by michael on 05/08/2020.
//

#ifndef CPPEXAM_VLVECTOR_HPP
#define CPPEXAM_VLVECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <cmath>
#include <iterator>

#define DEFAULT_STATIC_CAPACITY     16              // default stack array size capacity
#define CAP_C_FACTOR                (3.0 / 2.0)     // ratio for best memory management
#define DEFAULT_ADDED_ELEMENTS      1               // number of elements added in a signle insert
#define ERR_OUT_OF_RANGE            "Out of range!"

/**
 * @brief Variable lenght vector - using stack alloc'd array if possible ( until max size reached
 * then switches to heap alloced array
 * @tparam T type
 * @tparam StaticCapacity stack alloced array size
 */
template<class T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector
{
public:
    /* iterator traits */
    typedef std::random_access_iterator_tag iterator_category;
    typedef T               value_type;
    typedef T*              pointer;
    typedef T&              reference;
    typedef ptrdiff_t       difference_type;

    typedef pointer         iterator;
    typedef const pointer   const_iterator;

    /**
     * @brief default ctor
     */
    VLVector() : mSize(0), mCapacity(StaticCapacity), mainArr(stackArr), mArrType(STATIC) {};
    /**
     * @brief copy ctor
     * @param vecToCopy
     */
    VLVector(const VLVector<T>& vecToCopy);
    /**
     * @brief ctor
     * @tparam InputIterator
     * @param first
     * @param last
     */
    template<class InputIterator>
    VLVector(InputIterator first, InputIterator last);
    /**
     * @brief dtor
     */
    ~VLVector();

    /* iterator methods */
    /**
     * @brief
     */
    inline  iterator        begin()            {return mainArr;};
    /**
     * @brief
     */
    inline  iterator        end()              {return mainArr + mSize;};
    /**
     * @brief
     */
    inline  const_iterator  begin()    const   {return mainArr;}
    /**
     * @brief
     */
    inline  const_iterator  end()      const   {return mainArr + mSize;};
    /**
     * @brief
     */
    inline  const_iterator  cbegin()   const   {return mainArr;};
    /**
     * @brief
     */
    inline  const_iterator  cend()     const   {return mainArr + mSize;};

    /* class methods */
    /**
     * @brief
     */
    inline size_t  size()      const noexcept   {return mSize;}
    /**
     * @brief
     */
    inline size_t  capacity()  const noexcept   {return mCapacity;}
    /**
     * @brief
     */
    inline bool    empty()     const noexcept   {return (mSize == 0);}
    /**
     * @brief
     */
    inline T*      data()      const noexcept   {return mainArr;}
    /**
     * @brief
     */
    T&      at (size_t i);
    /**
     * @brief
     */
    void    push_back(const T& element);
    /**
     * @brief
     */
    void    pop_back()  noexcept;
    /**
     * @brief
     */
    void    clear()     noexcept;

    /* operators overloads */
    /**
     * @brief
     */
    T&          operator[]  (const size_t& i)        noexcept;
    /**
     * @brief
     */
    const T&    operator[]  (const size_t& i)  const noexcept;
    /**
     * @brief
     */
    bool        operator!=  (const VLVector<T, StaticCapacity>& rhs) const noexcept;
    /**
     * @brief
     */
    bool        operator==  (const VLVector<T, StaticCapacity>& rhs) const noexcept;
    /**
     * @brief
     */
    VLVector<T, StaticCapacity>& operator= (const VLVector<T,StaticCapacity>& rhs);

    /* modifiers */
    /**
     * @brief
     */
    auto    erase (const_iterator position);
    /**
     * @brief
     */
    auto    erase (const_iterator first, const_iterator last);
    /**
     * @brief
     */
    auto    insert(const_iterator position, const T& element);
    /**
     * @brief
     * @tparam InputIter
     * @param position
     * @param first
     * @param last
     * @return
     */
    template<class InputIter>
    auto    insert(const_iterator position, InputIter first, InputIter last);

private:
    /**
     * @brief representing current array type pointed by mainArr
     */
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

    /**
     * @brief allocates dynamic array accroding to capC
     * @param newElements
     */
    void    _enlargeCapacity (const size_t &newElements);
    /**
     * @brief copies all elemnts one-by-one from main arr to destinatopn
     */
    void    _copyAllElements (T* destination)               const;
    /**
     * @brief returns index of given iterator (if not found returns last index )
     */
    size_t  _findIndexOf     (const_iterator position)      const;
    /**
     * @brief calcs best capcity
     */
    size_t  _capC            (const size_t& newElements)    const;
    /**
     * @brief changes all class members to stack array (deletes dynamic array)
     */
    void    _switchToStackArr();
};

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @tparam InputIterator
 * @param first
 * @param last
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param i
 * @return
 */
template<class T, size_t StaticCapacity>
T &VLVector<T, StaticCapacity>::at(size_t i)
{
    if (i >= mSize || i < 0)
    {
        throw std::out_of_range(ERR_OUT_OF_RANGE);
    }
    return mainArr[i];
}

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param element
 */
template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::push_back(const T &element)
{
    if (mSize >= mCapacity)
    {
        _enlargeCapacity(DEFAULT_ADDED_ELEMENTS);
    }
    mainArr[mSize++] = element;
}

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param rhs
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param i
 * @return
 */
template<class T, size_t StaticCapacity>
T &VLVector<T, StaticCapacity>::operator[](const size_t &i) noexcept
{
    return mainArr[i];
}

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param i
 * @return
 */
template<class T, size_t StaticCapacity>
const T &VLVector<T, StaticCapacity>::operator[](const size_t &i) const noexcept
{
    return mainArr[i];
}

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param rhs
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param rhs
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @param element
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @tparam InputIterator
 * @param position
 * @param first
 * @param last
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @return
 */
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

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @return
 */
template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::_switchToStackArr()
{
    delete[] mainArr;
    mainArr = stackArr;
    mCapacity = StaticCapacity;
    mArrType = STATIC;
}

/**
 * @brief
 * @tparam T
 * @tparam StaticCapacity
 * @param position
 * @return
 */
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
