#pragma once

#include <cstdint>
#include <iterator>

template <typename T> class atPool
{
public:
    T *      m_pObjects;
    uint8_t *m_pMask;
    int      m_nMaxElements;
    int      m_nSize;
    int      m_nFirstFree;
    int      m_nCount;
    bool     unk_24;

    // Because why not, right? :P
    class atPoolIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T *;
        using difference_type   = void;
        using pointer           = T **;
        using reference         = T *;

        atPool<T> *pool  = nullptr;
        int32_t    index = 0;

        explicit atPoolIterator (atPool<T> *pool, int32_t index = 0)
        {
            this->pool  = pool;
            this->index = index;
        }

        atPoolIterator &
        operator++ ()
        {
            for (index++; index < pool->m_nMaxElements; index++)
                if (pool->IsValidIndex (index))
                    return *this;

            index = pool->m_nMaxElements;
            return *this;
        }

        atPoolIterator
        operator++ (int)
        {
            atPoolIterator retval = *this;
            ++(*this);
            return retval;
        }

        bool
        operator== (atPoolIterator other) const
        {
            return index == other.index;
        }
        bool
        operator!= (atPoolIterator other) const
        {
            return !(*this == other);
        }

        atPoolIterator &
        operator-- ()
        {
            for (index--; index > -1; index--)
                if (pool->IsValidIndex (index))
                    return *this;

            index = pool->m_nMaxElements;
            return *this;
        }

        atPoolIterator
        operator-- (int)
        {
            atPoolIterator retval = *this;
            --(*this);
            return retval;
        }

        reference
        operator* ()
        {
            return pool->GetAt (index);
        }
    };

    atPoolIterator
    begin ()
    {
        return ++atPoolIterator (this, -1);
    }

    atPoolIterator
    end ()
    {
        return atPoolIterator (this, this->m_nMaxElements);
    }

    /*******************************************************/
    bool
    IsValidIndex (int index) const
    {
        return index < m_nMaxElements && (m_pMask[index] & 128) == 0;
    }

    /*******************************************************/
    T *
    GetAt (int index)
    {
        if (!IsValidIndex (index))
            return nullptr;

        return reinterpret_cast<T *> (reinterpret_cast<uint8_t *> (m_pObjects)
                                      + m_nSize * index);
    }

    /*******************************************************/
    T *
    GetAtHandle (int handle)
    {
        return GetAt (handle >> 8);
    }
};

struct CVehicleStruct;
class CPed;

class CPools
{
public:
    static atPool<CVehicleStruct> *&g_pVehicleStructPool ();
    static atPool<CPed> *&          g_pPedsPool ();

    static void InitialisePatterns ();
    static void InitialisePatternsCE ();
};
