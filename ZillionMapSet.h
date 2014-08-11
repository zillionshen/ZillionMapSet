/*
Copyright(C) <2014>
<Zillion Shen zillionshen@yahoo.com>
http://zillionsblog.wordpress.com/

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once
#include <string>
#include <cstddef>
#include <cstdint>
#include <climits>
#include <cinttypes>
#include <stdlib.h>
#include <math.h>

using namespace std;

namespace Zillion
{
#ifdef _MSC_VER
#define INLINE __forceinline
#else
#define INLINE inline
#endif

#if INTPTR_MAX == INT32_MAX
#define DELETED_SLOT (SlotType)UINT32_MAX
#define END_POSITION UINT32_MAX
    typedef uint32_t UInt;
#elif INTPTR_MAX == INT64_MAX
#define DELETED_SLOT (SlotType)UINT64_MAX
#define END_POSITION UINT64_MAX
    typedef uint64_t UInt;
#else
#error this only works with 32bit or 64bit compilation
#endif

#define EMPTY_SLOT (SlotType)0
#define REBUILB_THRESHOLD 0.75f

    typedef void * Pointer;
    typedef unsigned char Byte;

    class Block
    {
        Byte * m_buffer;
        UInt m_size;
        bool m_copied;
    public:
        Block(const Byte * buffer, UInt size) { m_buffer = const_cast<Byte *>(buffer); m_size = size; m_copied = false; }
        Block(const Block & right) { m_buffer = right.m_buffer; m_size = right.m_size; m_copied = false; }
        ~Block() { clear(); }
        INLINE void clear() { if (m_copied) delete m_buffer; m_buffer = NULL; m_size = 0; m_copied = false; }
        INLINE void transfer(Block & right) { clear(); m_buffer = right.m_buffer; m_size = right.m_size; m_copied = right.m_copied; right.m_copied = false; }
        INLINE Byte * buffer() const { return m_buffer; }
        INLINE UInt size() const { return m_size; }
        INLINE bool operator==(const Block & right) const { return m_size == right.m_size && memcmp(m_buffer, right.m_buffer, m_size) == 0; }
        INLINE Block * copy() const
        {
            Block * block = new Block(NULL, 0);
            block->m_buffer = new Byte[m_size];
            memcpy(block->m_buffer, m_buffer, m_size);
            block->m_size = m_size;
            block->m_copied = true;
            return block;
        }
    };

    template<class ContainerType>
    class IteratorBase
    {
    protected:
        UInt m_index;
        ContainerType * m_container;
        IteratorBase & assign(const IteratorBase & it) { return *this; }
        IteratorBase() : m_index(0), m_container(NULL) {}
        IteratorBase(const IteratorBase & it) : m_index(it.m_index), m_container(it.m_container) {}
        IteratorBase(UInt index, const ContainerType * container) : m_index(index), m_container(const_cast<ContainerType *>(container)) {}
    public: //STL methods
        bool operator==(const IteratorBase & it) { return m_index == it.m_index && m_container == it.m_container; }
        bool operator!=(const IteratorBase & it) { return m_index != it.m_index || m_container != it.m_container; }
    public: //Non-STL methods
        UInt getPosition() { return m_index; }
        const ContainerType* getContainer() { return m_container; }
    };

    template<class ContainerType, class ValueType>
    class Iterator : public IteratorBase<ContainerType>
    {
        typedef IteratorBase<ContainerType> Base;
    public:
        Iterator() : Base(0, NULL) {}
        Iterator(UInt index, const ContainerType * container) : Base(index, container)  {}
        Iterator(const Iterator & it) : Base(it) {}
        ValueType & operator* () { return Base::m_container->getReference(Base::m_index); }
        Iterator operator++() { Base::m_index = Base::m_container->nextPosition(Base::m_index); return *this; }
        Iterator operator--() { Base::m_index = Base::m_container->prevPosition(Base::m_index); return *this; }
        ValueType * operator->() { return &(Base::m_container->getReference(Base::m_index)); }
        Iterator & operator = (const Iterator & it) { Base::assign(it); return *this; }
    };

    template<class ContainerType, class ValueType>
    class ConstIterator : public IteratorBase<ContainerType>
    {
        typedef IteratorBase<ContainerType> Base;
    public:
        ConstIterator() : Base(0, NULL) {}
        ConstIterator(UInt index, const ContainerType * container) : Base(index, container)  {}
        ConstIterator(const Base & it) : Base(it) {}
        const ValueType & operator* () { return Base::m_container->getReference(Base::m_index); }
        ConstIterator operator++() { Base::m_index = Base::m_container->nextPosition(Base::m_index); return *this; }
        ConstIterator operator--() { Base::m_index = Base::m_container->prevPosition(Base::m_index); return *this; }
        const ValueType * operator->() { return &(Base::m_container->getReference(Base::m_index)); }
        ConstIterator & operator=(const Iterator<ContainerType, ValueType> & it) { Base::assign(it); return *this; }
        ConstIterator & operator=(const ConstIterator & it) { Base::assign(it); return *this; }
    };

    template<class KeyType, class SlotType = KeyType, class ReturnType = KeyType, class ValueType = KeyType>
    class Container
    {
    private:
        SlotType * m_slots;
        UInt m_count;
        UInt m_capacity;
        UInt m_deletedCount;

        typedef pair<KeyType, ValueType> KVPair;

        struct Hasher
        {
            INLINE uint32_t HashBlock(const Byte *data, uint32_t len)
            {	//FNV-1Aa:: http://www.isthe.com/chongo/tech/comp/fnv/
                uint32_t result = 2166136261U;
                for (uint32_t index = 0; index < len; ++index)
                {
                    result ^= (uint32_t)data[index];
                    result *= 16777619U;
                }
                return result;
            }
            INLINE uint64_t HashBlock(const Byte *data, uint64_t len)
            {	//FNV-1Aa:: http://www.isthe.com/chongo/tech/comp/fnv/
                uint64_t result = 14695981039346656037ULL;
                for (uint64_t index = 0; index < len; ++index)
                {
                    result ^= (uint64_t)data[index];
                    result *= 1099511628211ULL;
                }
                result ^= result >> 32;
                return result;
            }
            INLINE uint32_t HashInteger(uint32_t key)
            {	//https://gist.github.com/badboy/6267743
                uint32_t originalKey = key;

                //Thomas Wang's 32 bit hash function
                key = (key ^ 61) ^ (key >> 16);
                key = key + (key << 3);
                key = key ^ (key >> 4);
                key = key * 0x27d4eb2d;
                key = key ^ (key >> 15);

                //added by Zillion Shen - begin
                //reserve EMPTY_SLOT and DELETED_SLOT values
                static uint32_t shift1[4] = { 0x1FFFFFFF, 0x5FFFFFFD, 0x9FFFFFFB, 0xDFFFFFF9 };
                static uint32_t shift2[4] = { 0x3FFFFFFE, 0x7FFFFFFC, 0xBFFFFFFA, 0xFFFFFFF8 };
                switch (key)
                {
                case EMPTY_SLOT:
                    return shift1[originalKey % 4];
                case DELETED_SLOT:
                    return shift2[originalKey % 4];
                default:
                    return key;
                }
                //added by Zillion Shen - end

                return key;
            }
            INLINE uint64_t HashInteger(uint64_t key)
            {	//https://gist.github.com/badboy/6267743
                uint64_t originalKey = key;

                //Thomas Wang's 64 bit hash function
                key = (~key) + (key << 21); // key = (key << 21) - key - 1;
                key = key ^ (key >> 24);
                key = (key + (key << 3)) + (key << 8); // key * 265
                key = key ^ (key >> 14);
                key = (key + (key << 2)) + (key << 4); // key * 21
                key = key ^ (key >> 28);
                key = key + (key << 31);

                //added by Zillion Shen - begin
                //reserve EMPTY_SLOT and DELETED_SLOT values
                static uint64_t shift1[4] = { 0x1FFFFFFFFFFFFFFF, 0x5FFFFFFFFFFFFFFD, 0x9FFFFFFFFFFFFFFB, 0xDFFFFFFFFFFFFFF9 };
                static uint64_t shift2[4] = { 0x3FFFFFFFFFFFFFFE, 0x7FFFFFFFFFFFFFFC, 0xBFFFFFFFFFFFFFFA, 0xFFFFFFFFFFFFFFF8 };
                switch (key)
                {
                case 0:
                    return shift1[originalKey % 4];
                case 18446744073709551615ULL:
                    return shift2[originalKey % 4];
                default:
                    return key;
                }
                //added by Zillion Shen - end

                return key;
            }
            INLINE UInt operator() (Byte key) { return HashInteger((UInt)(key)); }
            INLINE UInt operator() (char key) { return HashInteger((UInt)key); }
            INLINE UInt operator() (wchar_t key) { return HashInteger((UInt)key); }
            INLINE UInt operator() (int16_t key) { return HashInteger((UInt)key); }
            INLINE UInt operator() (uint16_t key) { return HashInteger((UInt)key); }
            INLINE UInt operator() (int32_t key) { return HashInteger((UInt)key); }
            INLINE UInt operator() (uint32_t key) { return HashInteger((UInt)key); }

            //data loss when compiled as 32bit but use int64_t and uint64_t keys - compile it as 64bit or use Block
            INLINE UInt operator() (int64_t key) { return HashInteger((UInt)key); }
            INLINE UInt operator() (uint64_t key) { return HashInteger((UInt)key); }

            INLINE UInt operator() (Pointer key) { return HashInteger((UInt)key); }
            INLINE UInt operator() (const char * key) { return HashBlock((const Byte *)key, (UInt)strlen(key)); }
            INLINE UInt operator() (const wchar_t * key) { return HashBlock((const Byte *)key, (UInt)(wcslen(key) * sizeof(wchar_t))); }
            INLINE UInt operator() (const string & key) { return HashBlock((const Byte *)key.c_str(), (UInt)key.length()); }
            INLINE UInt operator() (const wstring & key) { return HashBlock((const Byte *)key.c_str(), (UInt)(key.length() * sizeof(wchar_t))); }
            INLINE UInt operator() (const Block & key) { return HashBlock((const Byte *)key.buffer(), (UInt)key.size()); }
        };
        struct Comparater
        {
            INLINE bool operator() (Byte left, Byte right) const { return left == right; }
            INLINE bool operator() (char left, char right) const { return left == right; }
            INLINE bool operator() (wchar_t left, wchar_t right) const { return left == right; }
            INLINE bool operator() (int16_t left, int16_t right) const { return left == right; }
            INLINE bool operator() (uint16_t left, uint16_t right) const { return left == right; }
            INLINE bool operator() (int32_t left, int32_t right) const { return left == right; }
            INLINE bool operator() (uint32_t left, uint32_t right) const { return left == right; }
            INLINE bool operator() (int64_t left, int64_t right) const { return left == right; }
            INLINE bool operator() (uint64_t left, uint64_t right) const { return left == right; }
            INLINE bool operator() (Pointer left, Pointer right) const { return left == right; }
            INLINE bool operator() (const char * left, const char * right) const { return strcmp(left, right) == 0; }
            INLINE bool operator() (const wchar_t * left, const wchar_t * right) const { return wcscmp(left, right) == 0; }
            INLINE bool operator() (const string * left, const string & right) const { return *left == right; }
            INLINE bool operator() (const wstring * left, const wstring & right) const { return *left == right; }
            INLINE bool operator() (const Block * left, const Block & right) const { return *left == right; }
            INLINE bool operator() (const KVPair * left, const KeyType & right) const { return KVKeyComparater()(left->first, right); }
        };
        struct KVKeyComparater
        {
            INLINE bool operator() (Byte left, Byte right) const { return left == right; }
            INLINE bool operator() (char left, char right) const { return left == right; }
            INLINE bool operator() (wchar_t left, wchar_t right) const { return left == right; }
            INLINE bool operator() (int16_t left, int16_t right) const { return left == right; }
            INLINE bool operator() (uint16_t left, uint16_t right) const { return left == right; }
            INLINE bool operator() (int32_t left, int32_t right) const { return left == right; }
            INLINE bool operator() (uint32_t left, uint32_t right) const { return left == right; }
            INLINE bool operator() (int64_t left, int64_t right) const { return left == right; }
            INLINE bool operator() (uint64_t left, uint64_t right) const { return left == right; }
            INLINE bool operator() (const char * left, const char * right) const { return strcmp(left, right) == 0; }
            INLINE bool operator() (const wchar_t * left, const wchar_t * right) const { return wcscmp(left, right) == 0; }
            INLINE bool operator() (const string & left, const string & right) const { return left == right; }
            INLINE bool operator() (const wstring & left, const wstring & right) const { return left == right; }
            INLINE bool operator() (const Block & left, const Block & right) const { return left == right; }
        };
        struct SlotsReleaser
        {
            INLINE void operator() (Byte * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (char * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (wchar_t * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (int16_t * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (uint16_t * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (int32_t * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (uint32_t * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (int64_t * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (uint64_t * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (Pointer * & slots, UInt cap) { free(slots); slots = NULL; }
            INLINE void operator() (char ** & slots, UInt cap)
            {
                for (UInt i = 0; i < cap; ++i)
                {
                    if (slots[i] != EMPTY_SLOT && slots[i] != DELETED_SLOT)
                        free(slots[i]);
                }
                free(slots);
                slots = NULL;
            }
            INLINE void operator() (wchar_t ** & slots, UInt cap)
            {
                for (UInt i = 0; i < cap; ++i)
                {
                    if (slots[i] != EMPTY_SLOT && slots[i] != DELETED_SLOT)
                        free(slots[i]);
                }
                free(slots);
                slots = NULL;
            }
            INLINE void operator() (string ** & slots, UInt cap)
            {
                for (UInt i = 0; i < cap; ++i)
                {
                    if (slots[i] != EMPTY_SLOT && slots[i] != DELETED_SLOT)
                        delete slots[i];
                }
                free(slots);
                slots = NULL;
            }
            INLINE void operator() (wstring ** & slots, UInt cap)
            {
                for (UInt i = 0; i < cap; ++i)
                {
                    if (slots[i] != EMPTY_SLOT && slots[i] != DELETED_SLOT)
                        delete slots[i];
                }
                free(slots);
                slots = NULL;
            }
            INLINE void operator() (Block ** & slots, UInt cap)
            {
                for (UInt i = 0; i < cap; ++i)
                {
                    if (slots[i] != EMPTY_SLOT && slots[i] != DELETED_SLOT)
                        delete slots[i];
                }
                free(slots);
                slots = NULL;
            }
            INLINE void operator() (KVPair ** & slots, UInt cap)
            {
                for (UInt i = 0; i < cap; ++i)
                {
                    if (slots[i] != EMPTY_SLOT && slots[i] != DELETED_SLOT)
                    {
                        KVKeyKeyReleaser()(slots[i]->first);
                        delete slots[i];
                    }
                }
                free(slots);
                slots = NULL;
            }
        };
        struct SlotCreater
        {
            INLINE SlotType operator() (const Byte & key) { return key; }
            INLINE SlotType operator() (const char & key) { return key; }
            INLINE SlotType operator() (const wchar_t & key) { return key; }
            INLINE SlotType operator() (const int16_t & key) { return key; }
            INLINE SlotType operator() (const uint16_t & key) { return key; }
            INLINE SlotType operator() (const int32_t & key) { return key; }
            INLINE SlotType operator() (const uint32_t & key) { return key; }
            INLINE SlotType operator() (const int64_t & key) { return key; }
            INLINE SlotType operator() (const uint64_t & key) { return key; }
            INLINE SlotType operator() (const Pointer & key) { return key; }
            INLINE SlotType operator() (const char * key) { char * s = (char *)malloc(strlen(key) + 1); strcpy(s, key); return s; }
            INLINE SlotType operator() (const wchar_t * key) { wchar_t * s = (wchar_t *)malloc((wcslen(key) + 1) * sizeof(wchar_t)); wcscpy(s, key); return s; }
            INLINE SlotType operator() (const string & key) { return new string(key); }
            INLINE SlotType operator() (const wstring & key) { return new wstring(key); }
            INLINE SlotType operator() (const Block & key) { return key.copy(); }
            INLINE SlotType operator() (KVPair * keyValue) { return keyValue; }
        };
        struct SlotReleaser
        {
            INLINE void operator() (Byte * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (char * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (wchar_t * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (int16_t * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (uint16_t * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (int32_t * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (uint32_t * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (int64_t * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (uint64_t * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (Pointer * slot) { *slot = DELETED_SLOT; }
            INLINE void operator() (char ** slot) { free(*slot); *slot = DELETED_SLOT; }
            INLINE void operator() (wchar_t ** slot) { free(*slot); *slot = DELETED_SLOT; }
            INLINE void operator() (string ** slot) { delete * slot; *slot = DELETED_SLOT; }
            INLINE void operator() (wstring ** slot) { delete * slot; *slot = DELETED_SLOT; }
            INLINE void operator() (Block ** slot) { delete * slot; *slot = DELETED_SLOT; }
            INLINE void operator() (KVPair ** slot) { KVKeyKeyReleaser()((*slot)->first); delete * slot; *slot = DELETED_SLOT; }
        };
        struct KVKeyCreater
        {
            INLINE KeyType operator() (const Byte & key) { return key; }
            INLINE KeyType operator() (const char & key) { return key; }
            INLINE KeyType operator() (const wchar_t & key) { return key; }
            INLINE KeyType operator() (const int16_t & key) { return key; }
            INLINE KeyType operator() (const uint16_t & key) { return key; }
            INLINE KeyType operator() (const int32_t & key) { return key; }
            INLINE KeyType operator() (const uint32_t & key) { return key; }
            INLINE KeyType operator() (const int64_t & key) { return key; }
            INLINE KeyType operator() (const uint64_t & key) { return key; }
            INLINE KeyType operator() (const Pointer & key) { return key; }
            INLINE KeyType operator() (const char * key) { char * s = (char *)malloc(strlen(key) + 1); strcpy(s, key); return s; }
            INLINE KeyType operator() (const wchar_t * key) { wchar_t * s = (wchar_t *)malloc((wcslen(key) + 1) * sizeof(wchar_t)); wcscpy(s, key); return s; }
            INLINE KeyType & operator() (const string & key) { return const_cast<string &>(key); }
            INLINE KeyType & operator() (const wstring & key) { return const_cast<wstring &>(key); }
            INLINE KeyType & operator() (const Block & key) { return *(key.copy()); }
        };
        struct KVKeyKeyReleaser
        {
            INLINE void operator() (Byte & key) { }
            INLINE void operator() (char & key) { }
            INLINE void operator() (wchar_t & key) { }
            INLINE void operator() (int16_t & key) { }
            INLINE void operator() (uint16_t & key) { }
            INLINE void operator() (int32_t & key) { }
            INLINE void operator() (uint32_t & key) { }
            INLINE void operator() (int64_t & key) { }
            INLINE void operator() (uint64_t & key) { }
            INLINE void operator() (Pointer & key) { }
            INLINE void operator() (char * & key) { free(key); }
            INLINE void operator() (wchar_t * & key) { free(key); }
            INLINE void operator() (string & key) { }
            INLINE void operator() (wstring & key) { }
            INLINE void operator() (Block & key) { delete & key; }
        };
        struct Referencer
        {
            INLINE ReturnType & operator() (Byte * slot) const { return *slot; }
            INLINE ReturnType & operator() (char * slot) const { return *slot; }
            INLINE ReturnType & operator() (wchar_t * slot) const { return *slot; }
            INLINE ReturnType & operator() (int16_t * slot) const { return *slot; }
            INLINE ReturnType & operator() (uint16_t * slot) const { return *slot; }
            INLINE ReturnType & operator() (int32_t * slot) const { return *slot; }
            INLINE ReturnType & operator() (uint32_t * slot) const { return *slot; }
            INLINE ReturnType & operator() (int64_t * slot) const { return *slot; }
            INLINE ReturnType & operator() (uint64_t * slot) const { return *slot; }
            INLINE ReturnType & operator() (Pointer * slot) const { return *slot; }
            INLINE ReturnType & operator() (char ** slot) const { return *slot; }
            INLINE ReturnType & operator() (wchar_t ** slot) const { return *slot; }
            INLINE ReturnType & operator() (string ** slot) const { return ** slot; }
            INLINE ReturnType & operator() (wstring ** slot) const { return ** slot; }
            INLINE ReturnType & operator() (Block ** slot) const { return ** slot; }
            INLINE ReturnType & operator() (KVPair ** slot) const { return ** slot; }
        };
        struct SlotToKeyMapper
        {
            INLINE const KeyType & operator() (Byte * slot) const { return *slot; }
            INLINE const KeyType & operator() (char * slot) const { return *slot; }
            INLINE const KeyType & operator() (wchar_t * slot) const { return *slot; }
            INLINE const KeyType & operator() (int16_t * slot) const { return *slot; }
            INLINE const KeyType & operator() (uint16_t * slot) const { return *slot; }
            INLINE const KeyType & operator() (int32_t * slot) const { return *slot; }
            INLINE const KeyType & operator() (uint32_t * slot) const { return *slot; }
            INLINE const KeyType & operator() (int64_t * slot) const { return *slot; }
            INLINE const KeyType & operator() (uint64_t * slot) const { return *slot; }
            INLINE const KeyType & operator() (Pointer * slot) const { return *slot; }
            INLINE const KeyType & operator() (char ** slot) const { return *slot; }
            INLINE const KeyType & operator() (wchar_t ** slot) const { return *slot; }
            INLINE const KeyType & operator() (string ** slot) const { return ** slot; }
            INLINE const KeyType & operator() (wstring ** slot) const { return ** slot; }
            INLINE const KeyType & operator() (Block ** slot) const { return ** slot; }
            INLINE const KeyType & operator() (KVPair ** slot) const { return (*slot)->first; }
        };

        static UInt nextPrime(UInt currentPrime)
        {
            static UInt primes[] =
            {
                0ul, 3ul, 7ul, 11ul,
                23ul, 47ul, 97ul, 197ul,
                397ul, 797ul, 1597ul, 3191ul,
                6379ul, 12757ul, 25471ul, 50929ul,
                101839ul, 203669ul, 407321ul, 814643ul,
                1629281ul, 3258551ul, 6517097ul, 13034191ul,
                26068369ul, 52136729ul, 104273459ul, 208546913ul,
                312820367ul, 469230529ul, 703845773ul, 1055768627ul,
                1583652929ul, 2375479373ul, 3563219059ul, 4294967295ul
            };

            UInt count = sizeof(primes) / sizeof(UInt);

            if (currentPrime >= primes[count - 1]) return primes[count - 1];

            UInt l = 0, h = count - 1, mid;
            while (l <= h)
            {
                mid = (l + h) / 2;
                if (currentPrime == primes[mid]) return primes[mid + 1];
                else if (currentPrime > primes[mid]) l = mid + 1;
                else h = mid - 1;
            }

            return primes[l > h ? l : h];
        }
        static UInt closestPrime(UInt number)
        {
            if (number < 1) return 1;
            else if (number < 4) return number;
            else if (number < 6) return 5;

            if (number % 2 == 0) number++;

            bool isPrime = false;
            while (!isPrime)
            {
                uint32_t divisor = 3;
                uint32_t upperLimit = static_cast<uint32_t>(sqrt(static_cast<double>(number)) + 1);

                isPrime = true;
                while (divisor <= upperLimit)
                {
                    if (number % divisor == 0)
                    {
                        isPrime = false;
                        break;
                    }
                    divisor += 2;
                }

                if (isPrime) break;
                number += 2;
                if (number % 5 == 0) number += 2;
            }

            return number;
        }

        INLINE UInt findKeyOrEmptySlot(SlotType * slots, UInt capacity, const KeyType & key) const
        {
            if (capacity == 0) return END_POSITION;
            UInt index = Hasher()(key) % capacity;
            while (slots[index] == DELETED_SLOT || (slots[index] != EMPTY_SLOT && Comparater()(slots[index], key) == false))
                index = ++index % capacity;
            return index;
        }
        INLINE UInt findKeyOrEmptyOrDeletedSlot(const KeyType & key) const
        {
            if (m_capacity == 0) return END_POSITION;
            UInt index = Hasher()(key) % m_capacity;
            while (m_slots[index] != EMPTY_SLOT && m_slots[index] != DELETED_SLOT && Comparater()(m_slots[index], key) == false)
                index = ++index % m_capacity;
            return index;
        }
        INLINE UInt findKeyOrEmptySlot(const KeyType & key) const
        {
            if (m_capacity == 0) return END_POSITION;
            UInt index = Hasher()(key) % m_capacity;
            while (m_slots[index] == DELETED_SLOT || (m_slots[index] != EMPTY_SLOT && Comparater()(m_slots[index], key) == false))
                index = ++index % m_capacity;
            return index;
        }

        void rebuild(UInt newCapacity)
        {
            SlotType * newSlots = (SlotType *)calloc(newCapacity, sizeof(SlotType));

            for (UInt index = 0; index < m_capacity; ++index)
            {
                if (m_slots[index] == EMPTY_SLOT || m_slots[index] == DELETED_SLOT) continue;
                newSlots[findKeyOrEmptySlot(newSlots, newCapacity, SlotToKeyMapper()(m_slots + index))] = m_slots[index];
            }

            if (m_slots != NULL) delete m_slots;
            m_slots = newSlots;
            m_capacity = newCapacity;
            m_deletedCount = 0;
        }
        void expand() { rebuild(nextPrime(m_capacity)); }

    public:
        Container(UInt bucketSize = 0)
        {
            m_slots = NULL;
            m_count = 0;
            m_capacity = 0;
            m_deletedCount = 0;
            if (bucketSize != 0) rebuild(closestPrime(bucketSize));
        }
        Container(const Container& right)
        {
            m_slots = NULL;
            m_count = 0;
            m_capacity = 0;
            m_deletedCount = 0;
            rebuild(closestPrime(right.m_count));
            for (auto it : right) insert(it);
        }
    public:
        //STL methods and iterator types - begin

        typedef Iterator<Container, ReturnType> iterator;
        typedef ConstIterator<Container, ReturnType> const_iterator;

        Container & operator=(const Container & right)
        {
            this->clear();
            for (auto it : right) this->insert(it);
            return *this;
        }
        Container & operator=(initializer_list<ReturnType> & right)
        {
            this->clear();
            for (auto it : right) this->insert(it);
            return *this;
        }
        bool operator==(const Container & right)
        {
            int count = 0;
            for (const_iterator it = right.begin(); it != right.end(); ++it)
            {
                if (!this->exists(*it)) return false;
                else ++count;
            }

            return count == m_count;
        }
        bool operator!=(const Container & right)
        {
            int count = 0;
            for (const_iterator it = right.begin(); it != right.end(); ++it)
            {
                if (!this->exists(*it)) return true;
                else ++count;
            }

            return count != m_count;
        }
        iterator begin()
        {
            for (uint32_t i = 0; i < m_capacity; ++i)
            {
                if (m_slots[i] != EMPTY_SLOT && m_slots[i] != DELETED_SLOT)
                    return iterator(i, this);
            }
            return iterator(END_POSITION, this);
        }
        const_iterator begin() const
        {
            for (uint32_t i = 0; i < m_capacity; ++i)
            {
                if (m_slots[i] != EMPTY_SLOT && m_slots[i] != DELETED_SLOT)
                    return const_iterator(i, this);
            }
            return const_iterator(END_POSITION, this);
        }
        const_iterator cbegin()
        {
            for (uint32_t i = 0; i < m_capacity; ++i)
            {
                if (m_slots[i] != EMPTY_SLOT && m_slots[i] != DELETED_SLOT)
                    return const_iterator(i, this);
            }
            return const_iterator(END_POSITION, this);
        }
        const_iterator cend() { return const_iterator(END_POSITION, this); }
        void clear()
        {
            SlotsReleaser()(m_slots, m_capacity);
            m_slots = NULL;
            m_capacity = 0;
            m_count = 0;
            m_deletedCount = 0;
        }
        size_t count(const KeyType & key) { return exists(key) ? 1 : 0; }
        bool empty() { return m_count == 0; }
        iterator end() { return iterator(END_POSITION, this); }
        const_iterator end() const { return const_iterator(END_POSITION, this); }
        iterator erase(const_iterator pos)
        {
            if (pos.getContainer() != this || pos.getPosition() >= m_capacity) return iterator(END_POSITION, this);
            if (m_slots[pos.getPosition()] == EMPTY_SLOT || m_slots[pos.getPosition()] == DELETED_SLOT) return iterator(END_POSITION, this);
            SlotReleaser()(m_slots + pos.getPosition());
            --m_count;
            ++m_deletedCount;
            return iterator(pos.getPosition(), this);
        }
        size_t erase(const KeyType & key)
        {
            UInt index = findKeyOrEmptySlot(key);
            if (index == END_POSITION || m_slots[index] == EMPTY_SLOT) return 0;
            SlotReleaser()(m_slots + index);
            --m_count;
            ++m_deletedCount;
            return 1;
        }
        iterator erase(iterator first, iterator last)
        {
            if (first.getContainer() != this) return end();

            UInt lastDeletedIndex = END_POSITION;
            UInt firstIndex = first.getPosition();
            UInt lastIndex = last.getPosition();

            while (firstIndex != lastIndex)
            {
                if (m_slots[firstIndex] != EMPTY_SLOT && m_slots[firstIndex] != DELETED_SLOT)
                {
                    SlotReleaser()(m_slots + firstIndex);
                    lastDeletedIndex = firstIndex;
                    --m_count;
                    ++m_deletedCount;
                }
                ++first;
                firstIndex = first.getPosition();
            }
            /* from http ://www.cplusplus.com/:
            Iterators specifying a range within the unordered_set container to be removed : [first, last).i.e., the
            range includes all the elements between first and last, including the element pointed by first but not
            the one pointed by last.Notice that unordered_set containers do not follow any particular order to
            organize its elements, therefore the effect of range deletions may not be easily predictable.
            */
            return iterator(nextPosition(lastDeletedIndex), this);
        }
        iterator find(const KeyType & key)
        {
            UInt index = findKeyOrEmptySlot(key);
            if (index == END_POSITION || m_slots[index] == EMPTY_SLOT) return iterator(END_POSITION, this);
            return iterator(index, this);
        }
        const_iterator find(const KeyType & key) const
        {
            UInt index = findKeyOrEmptySlot(key);
            if (index == END_POSITION || m_slots[index] == EMPTY_SLOT) return const_iterator(END_POSITION, this);
            return const_iterator(index, this);
        }
        iterator insert(iterator first, iterator last) { while (first != last) { fast_insert(*first); ++first; } return --last; }
        pair<iterator, bool> insert(const KeyType & key) { return pair<iterator, bool>(iterator(fast_insert(key), this), true); }
        size_t size() { return m_count; }
        //STL methods and iterator types - end

        //Non-STL methods - begin
        INLINE UInt nextPosition(UInt currentIndex)
        {
            for (++currentIndex; currentIndex < m_capacity; ++currentIndex)
            {
                if (m_slots[currentIndex] != EMPTY_SLOT && m_slots[currentIndex] != DELETED_SLOT)
                    return currentIndex;
            }
            return END_POSITION;
        }
        INLINE UInt prevPosition(UInt currentIndex)
        {
            if (currentIndex == 0) return END_POSITION;

            if (currentIndex == END_POSITION) currentIndex = m_capacity;

            for (--currentIndex; currentIndex > 0; --currentIndex)
            {
                if (m_slots[currentIndex] != EMPTY_SLOT && m_slots[currentIndex] != DELETED_SLOT)
                    return currentIndex;
            }

            return (m_slots[currentIndex] == EMPTY_SLOT || m_slots[currentIndex] == DELETED_SLOT) ? END_POSITION : 0;
        }
        ReturnType & getReference(UInt index) { return Referencer()(m_slots + index); }
        bool exists(const KeyType & key) 
        {
            UInt index = findKeyOrEmptySlot(key);
            if (index == END_POSITION || m_slots[index] == EMPTY_SLOT) return false;
            return true;
        }
        UInt fast_insert(const KeyType & key)
        {
            if (m_count + m_deletedCount >= (uint32_t)(m_capacity * REBUILB_THRESHOLD))
            {
                if (m_deletedCount != 0 && m_deletedCount >= (uint32_t)(m_count * REBUILB_THRESHOLD))
                    rebuild(nextPrime(m_count));
                else
                    expand();
            }
            UInt index = findKeyOrEmptyOrDeletedSlot(key);
            if (m_slots[index] != DELETED_SLOT && m_slots[index] != EMPTY_SLOT) return index;
            if (m_slots[index] == DELETED_SLOT) m_deletedCount--;
            ++m_count;
            m_slots[index] = SlotCreater()(key);
            return index;
        }
        UInt fast_insert(const KeyType & key, const ValueType & value)
        {
            if (m_count + m_deletedCount >= (uint32_t)(m_capacity * REBUILB_THRESHOLD))
            {
                if (m_deletedCount != 0 && m_deletedCount >= (uint32_t)(m_count * REBUILB_THRESHOLD))
                    rebuild(nextPrime(m_count));
                else
                    expand();
            }
            UInt index = findKeyOrEmptyOrDeletedSlot(key);
            if (m_slots[index] != DELETED_SLOT && m_slots[index] != EMPTY_SLOT) return index;
            if (m_slots[index] == DELETED_SLOT) m_deletedCount--;
            ++m_count;
            m_slots[index] = new pair<KeyType, ValueType>(KVKeyCreater()(key), value);
            return index;
        }
        //Non-STL methods - end
    };

    class UCharSet : public Container<Byte> { public: UCharSet(UInt bucketSize = 0) : Container(bucketSize){} };

    class CharSet : public Container<char> { public: CharSet(UInt bucketSize = 0) : Container(bucketSize){} };

    class WCharSet : public Container<wchar_t> { public: WCharSet(UInt bucketSize = 0) : Container(bucketSize){} };

    class Int16Set : public Container<int16_t> { public: Int16Set(UInt bucketSize = 0) : Container(bucketSize){} };

    class UInt16Set : public Container<uint16_t> { public: UInt16Set(UInt bucketSize = 0) : Container(bucketSize){} };

    class Int32Set : public Container<int32_t> { public: Int32Set(UInt bucketSize = 0) : Container(bucketSize){} };

    class UInt32Set : public Container<uint32_t> { public: UInt32Set(UInt bucketSize = 0) : Container(bucketSize){} };

    class Int64Set : public Container<int64_t> { public: Int64Set(UInt bucketSize = 0) : Container(bucketSize){} };

    class UInt64Set : public Container<uint64_t> { public: UInt64Set(UInt bucketSize = 0) : Container(bucketSize){} };

    class PointerSet : public Container<Pointer> { public: PointerSet(UInt bucketSize = 0) : Container(bucketSize){} };

    class CharPtrSet : public Container<char * > { public: CharPtrSet(UInt bucketSize = 0) : Container(bucketSize){} };

    class WCharPtrSet : public Container<wchar_t * > { public: WCharPtrSet(UInt bucketSize = 0) : Container(bucketSize){} };

    class StringSet : public Container<string, string * > { public: StringSet(UInt bucketSize = 0) : Container(bucketSize){} };

    class WStringSet : public Container<wstring, wstring * > { public: WStringSet(UInt bucketSize = 0) : Container(bucketSize){} };

    class BlockSet : public Container<Block, Block * >
    {
        typedef Container<Block, Block * > Base;
    public:
        BlockSet(UInt bucketSize = 0) : Base(bucketSize){}
        size_t count(Byte * buffer, UInt size) { Block block(buffer, size); return Base::count(block); }
        size_t erase(Byte * buffer, UInt size) { Block block(buffer, size); return Base::erase(block); }
        iterator find(Byte * buffer, UInt size) { Block block(buffer, size); return Base::find(block); }
        const_iterator find(Byte * buffer, UInt size) const { Block block(buffer, size); return Base::find(block); }
        void insert(Byte * buffer, UInt size) { Block block(buffer, size); Base::insert(block); }
        UInt fast_insert(Byte * buffer, UInt size) { Block block(buffer, size); return Base::fast_insert(block); }
        bool exists(Byte * buffer, UInt size) { Block block(buffer, size); return Base::exists(block); }
    };

    template<class KeyType, class ValueType>
    class Map : public Container<KeyType, pair<KeyType, ValueType> *, pair<KeyType, ValueType>, ValueType>
    {
        typedef Container<KeyType, pair<KeyType, ValueType> *, pair<KeyType, ValueType>, ValueType> Base;
    public:
        Map(UInt bucketSize = 0) : Base(bucketSize){}
        ValueType & operator[](const KeyType & key) 
        { 
            typename Base::iterator it = Base::find(key);
            if (it != Base::end()) return (*it).second;
            
            ValueType v;
            typename Base::iterator nit(fast_insert(key, v), this);
            return (*nit).second;
        }
        ValueType & at(const KeyType & key) 
        { 
            typename Base::iterator it = Base::find(key);
            if (it != Base::end()) return (*it).second;

            ValueType v;
            typename Base::iterator nit(fast_insert(key, v), this);
            return (*nit).second;
        }
        const ValueType & at(const KeyType & key) const { return (*Base::find(key)).second; }
        UInt insert(const KeyType & key, const ValueType & value) { return Base::fast_insert(key, value); }
        UInt fast_insert(const KeyType & key, const ValueType & value) { return Base::fast_insert(key, value); }
    };
}
