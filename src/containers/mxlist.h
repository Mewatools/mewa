/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
/****************************************************************************
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
** SPDX-License-Identifier: Qt commercial license
****************************************************************************/
#ifndef MXLIST_H
#define MXLIST_H

#include "mxdebug.h"
#include "mxrefcounter.h"
#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include <new>



template <typename T>
struct MxClassInitializer
{
    static inline void construct(T *n, const T &t)
    {
        new (n) T(t);
    }

    static inline void empty_construct(T *n)
    {
        new (n) T();
    }

    static inline void destroy(T *n)
    {
        reinterpret_cast<T*>(n)->~T();
    }

    static inline void destroyArray(T *from, T *to)
    {
               while (from != to) --to, to->~T();
    }

    static inline void copy(T *from, T *to, T *src)
    {
        T *current = from;
        while(current != to) {
            new (current) T(*src);
            ++current;
            ++src;
        }
    }
};


template <typename T>
struct MxPodInitializer
{
    static inline void construct(T *n, const T &t)
    {
     *n = t;
    }

    static inline void empty_construct(T *)
    {
    }

    static inline void destroy(T */*n*/)
    {}

    static inline void destroyArray(T */*from*/, T */*to*/)
    {}

    static inline void copy(T *from, T *to, T *src)
    {
        Q_ASSERT(src != from && to - from > 0);
        memcpy(from, src, (to - from) * sizeof(T));
    }
};



/*!
 MxList is an array designed for fast removing from the beggining or end of the array.

 MxList is implicit shared but not copy-on-write. A copy of
 the list need to be explicitly called with detach().
  */
template <typename T, typename Initializer = MxClassInitializer<T>, int Prealloc = 4 >
class MxList
{
public:
    //! these members are public to allow unions
    struct Data : MxRefCounter {

        int alloc, begin, end;
        T array[1];
    };
    enum { DataHeaderSize = sizeof(Data) - sizeof(T) };

    Data *d;

public:
    inline MxList()
        : d(0)
    {}

    MxList( Data *listData )
    {
        d = listData;
    }


    ~MxList();

    MxList<T,Initializer,Prealloc> &operator=(const MxList<T,Initializer,Prealloc> &l);


    inline int size() const
    {
        if(d)
        return d->end - d->begin;

        return 0;
    }

    inline bool isEmpty() const { return (d == NULL || d->end == d->begin); }
    inline bool isNull() const { return d == NULL; }

    void clear();

    inline const T &at(int i) const;
    inline T& allocEnd();
    void reserve(int size);
    void append(const T &t);
    void prepend(const T &t);
    // increment size and return pointer to added entry
    T* appendAndGet();
    void insert(int i, const T &t);
    T* insertAndGet(int i);
    void append(const T *vector, int count);
    void removeAt(int i);
    void removeFirst();
    void removeLast();

    //!Removes the first item in the list and returns it. This is the same as takeAt(0). This function assumes the list is not empty.
    T takeFirst();
    T takeLast();

    int indexOf(const T &t, int from = 0) const;
    bool contains(const T &t) const;
    inline T& last();
    inline T& first();

    bool removeOne( const T &value )
    {
        if(d)
        {
            T *b = p_begin();
            T *i = p_end();

            while (i != b) {
                if (*b == value) {
                    node_destruct(b);
                    p_remove( b - p_begin() );
                    return true;
                }
                 ++b;
            }
        }
        return false;

    }

    void resize( int size )
    {
        Q_ASSERT( d != NULL );
        d->end = d->begin + size;
    }

    T* data()
    {
        Q_ASSERT( d->refCounter == 1 );
        return d->array + d->begin;
    }

    const T* constData() const
    {
        return d->array + d->begin;
    }

    const T &operator[](int i) const;
    T &operator[](int i);


private:
    void p_remove(int i);

    T* p_append(int n);
    T* p_prepend();
    T* p_insert(int i);

    inline void detach() { if (d->refCounter != 1) detach(d->alloc); }

    inline T* p_at(int i) const { return d->array + d->begin + i; }
    inline T* p_begin() const { return d->array + d->begin; }
    inline T* p_end() const { return d->array + d->end; }
    inline T* end() const { return d->array + d->end; }
    inline T* begin() const { return d->array + d->begin; }

    void node_construct(T *n, const T &t);
    void node_destruct(T *n);
    void node_copy(T *from, T *to, T *src);
    void node_destruct(T *from, T *to);

    void init(int alloc);
    Data* detach(int alloc);

    void grow(int alloc);
    void detachAndCopy(int alloc);

    //T* detach_helper_grow(int i, int c);
    Data* p_detach_grow(int *idx, int num);
    void dealloc(Data *d);
    T* p_reserveSpaceForAppend();

};



template <typename T, typename Initializer, int Prealloc>
inline MxList<T,Initializer,Prealloc>::~MxList()
{

    if ( d && !d->decRefCounter()) {
        dealloc(d);
    }

}


template <typename T, typename Initializer, int Prealloc>
inline MxList<T,Initializer,Prealloc> &MxList<T,Initializer,Prealloc>::operator=(const MxList<T,Initializer,Prealloc> &l)
{
    if (d != l.d) {
        if(l.d != NULL) {
            l.d->incRefCounter();
        }

        if ( d && !d->decRefCounter())
            dealloc(d);
        d = l.d;
    }
    return *this;
}


template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::clear()
{
    if(d)
    {
        d->begin = 0;
        d->end = 0;
    }
}

template <typename T, typename Initializer, int Prealloc>
const T& MxList<T,Initializer,Prealloc>::at(int i) const
{
    Q_ASSERT_X(i >= 0 && i < size(), "MxList<T>::at", "index out of range");
    return *(d->array + d->begin + i);
}

template <typename T, typename Initializer, int Prealloc>
T& MxList<T,Initializer,Prealloc>::allocEnd()
{
    T *n = p_reserveSpaceForAppend();
    d->end += 1;
    return *n;
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::reserve(int alloc)
{
    if(d)
    {
        if (d->alloc < alloc) {
                grow(alloc); // extends current array ( ref == 1)
        }
    }
    else {
        // Null list
        init(alloc);
    }
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::append(const T &t)
{
    T *n = p_reserveSpaceForAppend();
    node_construct(n, t);
    d->end += 1;
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::prepend(const T &t)
{
    p_reserveSpaceForAppend();
memmove(d->array + 1, d->array, d->end * sizeof(T));
T *n = &(d->array[0]);
        node_construct(n, t);
    d->end += 1;
}

template <typename T, typename Initializer, int Prealloc>
inline T* MxList<T,Initializer,Prealloc>::appendAndGet()
{
    T *n = p_reserveSpaceForAppend();
    Initializer::empty_construct(n);
    d->end += 1;
    return n;
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::insert(int i, const T &t)
{

        T *n = p_insert(i);
        node_construct(n, t);

}

template <typename T, typename Initializer, int Prealloc>
inline T* MxList<T,Initializer,Prealloc>::insertAndGet(int i)
{

        T *n = p_insert(i);
        return n;
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::append(const T *vector, int count)
{
    Q_ASSERT(d != NULL);
    Q_ASSERT( (d->end - d->begin) + count <= d->alloc );
    memcpy(d->array + d->end, vector, count*sizeof(T));
    d->end += count;
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::removeAt(int i)
{
    Q_ASSERT(i >= 0 && i < size());
    //detach();
    node_destruct(d->array + d->begin + i);
    p_remove(i);
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::removeFirst()
{
    Q_ASSERT(d);
    d->begin++;
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::removeLast()
{
    Q_ASSERT(d);
    d->end--;
}

template <typename T, typename Initializer, int Prealloc>
inline T MxList<T,Initializer,Prealloc>::takeFirst()
{
    Q_ASSERT(d);
    Q_ASSERT(size() > 0);
    T first = *(d->array + d->begin);
    node_destruct(d->array + d->begin);
    d->begin++;
    return first;
}

template <typename T, typename Initializer, int Prealloc>
inline T MxList<T,Initializer,Prealloc>::takeLast()
{
    Q_ASSERT(d);
    Q_ASSERT(size() > 0);
    T *lastPtr = d->array + d->end - 1;
    T last = *lastPtr;
    node_destruct(lastPtr);
    d->end--;
    return last;
}

template <typename T, typename Initializer, int Prealloc>
inline int MxList<T,Initializer,Prealloc>::indexOf( const T &t, int from ) const
{
    Q_ASSERT(from >= 0);
    T *n = p_at(from -1);
    T *e = p_end();
    while (++n != e)
        if (*n == t)
            return int(n - d->array);

    return -1;
}


template <typename T, typename Initializer, int Prealloc>
inline bool MxList<T,Initializer,Prealloc>::contains(const T &t) const
{
    if(d)
    {
        T *b = p_begin();
        T *i = p_end();

        while (i != b) {
            if (*b == t)
                return true;
             ++b;
        }
    }
    return false;
}

template <typename T, typename Initializer, int Prealloc>
 T& MxList<T,Initializer,Prealloc>::last()
 {
     return *(d->array + d->end - 1);
 }

 template <typename T, typename Initializer, int Prealloc>
  T& MxList<T,Initializer,Prealloc>::first()
  {
      return *(d->array + d->begin);
  }


template <typename T, typename Initializer, int Prealloc>
inline const T & MxList<T,Initializer,Prealloc>::operator[](int i) const
{
    Q_ASSERT_X(i >= 0 && i < size(), "MxList<T>::operator[]", "index out of range");
    return *p_at(i);
}

template <typename T, typename Initializer, int Prealloc>
inline T & MxList<T,Initializer,Prealloc>::operator[](int i)
{
    Q_ASSERT_X(i >= 0 && i < size(), "MxList<T>::operator[]", "index out of range");
    return *p_at(i);
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::p_remove(int i)
{
    //Q_ASSERT(d->ref == 1);
    i += d->begin;
    if (i - d->begin < d->end - i) {
        if (int offset = i - d->begin)
            memmove(d->array + d->begin + 1, d->array + d->begin, offset * sizeof(T));
        d->begin++;
    } else {
        if (int offset = d->end - i - 1)
            memmove(d->array + i, d->array + i + 1, offset * sizeof(T));
        d->end--;
    }
}

// ensures that enough space is available to append n elements
template <typename T, typename Initializer, int Prealloc>
T* MxList<T,Initializer,Prealloc>::p_append(int n)
{
    Q_ASSERT(d->refCounter == 1);
    int e = d->end;
    if (e + n > d->alloc) {
        int b = d->begin;
        if (b - n >= 2 * d->alloc / 3) {
            // we have enough space. Just not at the end -> move it.
            e -= b;
            memcpy(d->array, d->array + b, e * sizeof(T));
            d->begin = 0;
        } else {
            grow(d->alloc + n);
        }
    }
    d->end = e + n;
    return d->array + e;
}


template <typename T, typename Initializer, int Prealloc>
T* MxList<T,Initializer,Prealloc>::p_prepend()
{
    Q_ASSERT(d->refCounter == 1);
    if (d->begin == 0) {
        if (d->end >= d->alloc / 3)
            grow(d->alloc + 1);

        if (d->end < d->alloc / 3)
            d->begin = d->alloc - 2 * d->end;
        else
            d->begin = d->alloc - d->end;

        memmove(d->array + d->begin, d->array, d->end * sizeof(T));
        d->end += d->begin;
    }
    return d->array + --d->begin;
}

template <typename T, typename Initializer, int Prealloc>
inline T* MxList<T,Initializer,Prealloc>::p_insert(int i)
{
    //Q_ASSERT(d->ref == 1);
    if (i <= 0)
        return p_prepend();
    int size = d->end - d->begin;
    if (i >= size)
        return p_append(1);

    bool leftward = false;

    if (d->begin == 0) {
        if (d->end == d->alloc) {
            // If the array is full, we expand it and move some items rightward
            grow(d->alloc + 1);
        } else {
            // If there is free space at the end of the array, we move some items rightward
        }
    } else {
        if (d->end == d->alloc) {
            // If there is free space at the beginning of the array, we move some items leftward
            leftward = true;
        } else {
            // If there is free space at both ends, we move as few items as possible
            leftward = (i < size - i);
        }
    }

    if (leftward) {
        --d->begin;
        memmove(d->array + d->begin, d->array + d->begin + 1, i * sizeof(T));
    } else {
        memmove(d->array + d->begin + i + 1, d->array + d->begin + i,
                  (size - i) * sizeof(T));
        ++d->end;
    }
    return d->array + d->begin + i;
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::node_construct(T *n, const T &t)
{
    Initializer::construct(n,t);

}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::node_destruct(T *n)
{
    //if (QxTypeInfo<T>::isClass) reinterpret_cast<T*>(n)->~T();
    Initializer::destroy(n);
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::node_copy(T *from, T *to, T *src)
{
    Initializer::copy(from, to, src);
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::node_destruct(T *from, T *to)
{
    Initializer::destroyArray(from,to);
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::init(int alloc)
{
    Q_ASSERT( 0 == d );
    d = static_cast<Data *>(malloc(DataHeaderSize + alloc * sizeof(T)));
    Q_ASSERT(d);

    d->refCounter = 1;
    d->alloc = alloc;

        d->begin = 0;
        d->end = 0;

}


template <typename T, typename Initializer, int Prealloc>
inline typename MxList<T,Initializer,Prealloc>::Data* MxList<T,Initializer,Prealloc>::detach(int alloc)
{
    Data *x = d;
    Data* t = static_cast<Data *>(malloc(DataHeaderSize + alloc * sizeof(T)));
    Q_ASSERT(t);

    t->refCounter = 1;
    t->alloc = alloc;
    if (!alloc) {
        t->begin = 0;
        t->end = 0;
    } else {
        t->begin = x->begin;
        t->end   = x->end;
    }
    d = t;

    return x;
}


template <typename T, typename Initializer, int Prealloc>
void MxList<T,Initializer,Prealloc>::grow(int alloc)
{
    Q_ASSERT( d != NULL && d->refCounter > 0 );
    Data *x = static_cast<Data *>(realloc(d, DataHeaderSize + alloc * sizeof(T)));
    Q_ASSERT(x);

    d = x;
    d->alloc = alloc;
}


template <typename T, typename Initializer, int Prealloc>
inline typename MxList<T,Initializer,Prealloc>::Data* MxList<T,Initializer,Prealloc>::p_detach_grow(int *idx, int num)
{
    Data *x = d;
    int l = x->end - x->begin;
    int nl = l + num;
    int alloc = grow(nl);
    Data* t = static_cast<Data *>(malloc(DataHeaderSize + alloc * sizeof(T)));

    t->refCounter = 1;
    t->alloc = alloc;
    // The space reservation algorithm's optimization is biased towards appending:
    // Something which looks like an append will put the data at the beginning,
    // while something which looks like a prepend will put it in the middle
    // instead of at the end. That's based on the assumption that prepending
    // is uncommon and even an initial prepend will eventually be followed by
    // at least some appends.
    int bg;
    if (*idx < 0) {
        *idx = 0;
        bg = (alloc - nl) >> 1;
    } else if (*idx > l) {
        *idx = l;
        bg = 0;
    } else if (*idx < (l >> 1)) {
        bg = (alloc - nl) >> 1;
    } else {
        bg = 0;
    }
    t->begin = bg;
    t->end = bg + nl;
    d = t;

    return x;
}

template <typename T, typename Initializer, int Prealloc>
inline void MxList<T,Initializer,Prealloc>::dealloc(Data *data)
{
    node_destruct(data->array + data->begin,
                  data->array + data->end);
    free(data);
}

// ensures that enough space is available to append 1 element
template <typename T, typename Initializer, int Prealloc>
T* MxList<T,Initializer,Prealloc>::p_reserveSpaceForAppend()
{
    if(d)
    {
        int e = d->end;
        if (e + 1 > d->alloc) {
            int b = d->begin;
            if (b - 1 >= 2 * d->alloc / 3) {
                // we have enough space. Just not at the end -> move it.
                e -= b;
                memcpy((void*)d->array, (void*)(d->array + b), e * sizeof(T));
                d->begin = 0;
                d->end = e;
            } else {
                grow(d->alloc + Prealloc);
            }
        }
        return d->array + e;
    }
    else
    {
        init(Prealloc);
        return d->array;
    }
}




#endif

