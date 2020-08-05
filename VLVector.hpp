//
// Created by michael on 05/08/2020.
//

#ifndef CPPEXAM_VLVECTOR_HPP
#define CPPEXAM_VLVECTOR_HPP

#include <cstdio>

#define DEFAULT_STATIC_CAPACITY     16

template<class T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector
{
public:
    typedef typename VLVector<T>::VecIter iterator;
    typedef typename VLVector<T>::ConstVecIter const_iterator;

    VLVector() : mSize(0), mCapacity(0) {};
    VLVector(const VLVector<T>& vecToCopy);
    template<class InputIterator>
    VLVector(InputIterator& first, InputIterator& last);
    ~VLVector();

    size_t  size()      const noexcept;
    size_t  capacity()  const noexcept;
    bool    empty()     const noexcept;

    void    push_back(T);

private:
    size_t mSize;
    size_t mCapacity;
    T      mStaticArr[StaticCapacity];

    //region class Iterators
    class VecIter
    {
    public:
        VecIter(T* ptr = nullptr) : currIter(ptr) {};
        inline T& operator*() const     {return *currIter;} // dereference current iter
        inline iterator& operator++()   {currIter++; return *this;}
        iterator operator++(T num)
        {
            iterator temp = *this;
            currIter++;
            return temp;
        }

    private:
        T* currIter;
    };

    class ConstVecIter
    {

    };
    //endregion


};

#endif //CPPEXAM_VLVECTOR_HPP
