#include "ZillionMapSet.h"
#include "UnitTest.h"
#include <time.h>
#include <iostream>
#include <map>

using namespace std;

bool Test_CharSet_InsertEraseExistsClear()
{
    try
    {

        Zillion::CharSet items;

        for (char c = 1; c < CHAR_MAX; c++)
        {
            items.insert(c);
        }
        for (char c = 1; c < CHAR_MAX; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (char c = 1; c < CHAR_MAX; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x20);
        items.insert(0x30);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_UCharSet_InsertEraseExistsClear()
{
    try
    {

        Zillion::UCharSet items;

        for (unsigned char c = 1; c < UCHAR_MAX; c++)
        {
            items.insert(c);
        }
        for (unsigned char c = 1; c < UCHAR_MAX; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (unsigned char c = 1; c < UCHAR_MAX; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x20);
        items.insert(0xF0);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_WCharSet_InsertEraseExistsClear()
{
    try
    {

        Zillion::WCharSet items;

        for (wchar_t c = 1; c < 100; c++)
        {
            items.insert(c);
        }
        for (wchar_t c = 1; c < 100; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (wchar_t c = 1; c < 100; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x2020);
        items.insert(0x70F0);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Int16Set_InsertEraseExistsClear()
{
    try
    {

        Zillion::Int16Set items;

        for (int16_t c = 1; c < 100; c++)
        {
            items.insert(c);
        }
        for (int16_t c = 1; c < 100; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (int16_t c = 1; c < 100; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x2020);
        items.insert(0x70F0);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_UInt16Set_InsertEraseExistsClear()
{
    try
    {

        Zillion::UInt16Set items;

        for (uint16_t c = 1; c < 100; c++)
        {
            items.insert(c);
        }
        for (uint16_t c = 1; c < 100; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (uint16_t c = 1; c < 100; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x2020);
        items.insert(0x70F0);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Int32Set_InsertEraseExistsClear()
{
    try
    {

        Zillion::Int32Set items;

        for (int32_t c = 1; c < 100; c++)
        {
            items.insert(c);
        }
        for (int32_t c = 1; c < 100; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (int32_t c = 1; c < 100; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x202020);
        items.insert(0x707070);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_UInt32Set_InsertEraseExistsClear()
{
    try
    {

        Zillion::UInt32Set items;

        for (uint32_t c = 1; c < 100; c++)
        {
            items.insert(c);
        }
        for (uint32_t c = 1; c < 100; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (uint32_t c = 1; c < 100; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x202020);
        items.insert(0xF07070);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Int64Set_InsertEraseExistsClear()
{
    try
    {

        Zillion::Int64Set items;

        for (int64_t c = 1; c < 100; c++)
        {
            items.insert(c);
        }
        for (int64_t c = 1; c < 100; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (int64_t c = 1; c < 100; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x202020202020);
        items.insert(0x707070707070);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_UInt64Set_InsertEraseExistsClear()
{
    try
    {

        Zillion::UInt64Set items;

        for (uint64_t c = 1; c < 100; c++)
        {
            items.insert(c);
        }
        for (uint64_t c = 1; c < 100; c++)
        {
            if (!items.exists(c)) return false;
        }
        for (uint64_t c = 1; c < 100; c++)
        {
            items.erase(c);
            if (items.exists(c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(0x202020202020);
        items.insert(0xF07070707070);
        if (items.size() != 2) return false;

        items.clear();
        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_PointerSet_InsertEraseExistsClear()
{
    try
    {
        Zillion::PointerSet items;

        int tempValue = 0;
        int * pointer = &tempValue;
        for (uint64_t c = 1; c < 100; c++)
        {
            items.insert(pointer + c);
        }
        for (uint64_t c = 1; c < 100; c++)
        {
            if (!items.exists(pointer + c)) return false;
        }
        for (uint64_t c = 1; c < 100; c++)
        {
            items.erase(pointer + c);
            if (items.exists(pointer + c)) return false;
        }

        if (items.size() != 0) return false;

        items.insert((void*)(uintptr_t)0x202020202020);
        items.insert((void*)(uintptr_t)0xF0F0F0F0F0F0);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_WCharPtrSet__InsertEraseExistsClear()
{
    try
    {
        Zillion::WCharPtrSet items;

        wchar_t strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            swprintf(strs[i], 20, L"test string %d", i + 100000);
            items.insert(strs[i]);
            if (!items.exists(strs[i])) return false;
        }
        for (int i = 0; i < 20; i++)
        {
            items.erase(strs[i]);
            if (items.exists(strs[i])) return false;
        }

        if (items.size() != 0) return false;

        items.insert(strs[0]);
        items.insert(strs[1]);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_CharPtrSet_InsertEraseExistsClear()
{
    try
    {
        Zillion::CharPtrSet items;

        char strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            sprintf(strs[i], "test string %d", i + 100000);
            items.insert(strs[i]);
            if (!items.exists(strs[i])) return false;
        }
        for (int i = 0; i < 20; i++)
        {
            items.erase(strs[i]);
            if (items.exists(strs[i])) return false;
        }

        if (items.size() != 0) return false;

        items.insert(strs[0]);
        items.insert(strs[1]);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_StringSet_InsertEraseExistsClear()
{
    try
    {

        Zillion::StringSet items;

        char strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            sprintf(strs[i], "test string %d", i + 100000);
            string s = strs[i];
            items.insert(s);
            if (!items.exists(s)) return false;
        }
        for (int i = 0; i < 20; i++)
        {
            string s = strs[i];
            items.erase(s);
            if (items.exists(s)) return false;
        }

        if (items.size() != 0) return false;

        items.insert(strs[0]);
        items.insert(strs[1]);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_WString_SetInsertEraseExistsClear()
{
    try
    {

        Zillion::WStringSet items;

        wchar_t strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            swprintf(strs[i], 20, L"test string %d", i + 100000);
            wstring s = strs[i];
            items.insert(s);
            if (!items.exists(s)) return false;
        }
        for (int i = 0; i < 20; i++)
        {
            wstring s = strs[i];
            items.erase(s);
            if (items.exists(s)) return false;
        }

        items.insert(strs[0]);
        items.insert(strs[1]);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_BlockSet_InsertEraseExistsClearCountFindFastInsert()
{
    try
    {

        Zillion::BlockSet items;

        unsigned char strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            sprintf((char*)strs[i], "test string %d", i + 100000);
            items.insert(strs[i], 20);
            if (!items.exists(strs[i], 20)) return false;
        }
        for (int i = 0; i < 20; i++)
        {
            items.erase(strs[i], 20);
            if (items.exists(strs[i], 20)) return false;
        }

        items.clear();
        items.insert(strs[0], 20);
        items.insert(strs[1], 20);

        if (items.count(strs[0], 20) != 1) return false;
        if (items.count(strs[1], 20) != 1) return false;
        if (items.find(strs[0], 20) == items.end()) return false;
        if (items.find(strs[1], 20) == items.end()) return false;

        items.insert(strs[2], 20);

        if (items.size() != 3) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Map_CharPtr_InsertEraseExistsClear()
{
    try
    {

        Zillion::Map<char *, int> items;

        char strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            sprintf((char*)strs[i], "test string %d", i + 100000);
            items.insert(strs[i], i);
            if (items[strs[i]] != i) return false;
        }
        for (int i = 0; i < 20; i++)
        {
            items.erase(strs[i]);
            if (items.exists(strs[i])) return false;
        }

        items.insert(strs[0], 20);
        items.insert(strs[1], 22);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Map_String_InsertEraseExistsClear()
{
    try
    {
        Zillion::Map<string, int> items;

        char strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            sprintf((char*)strs[i], "test string %d", i + 100000);
            items.insert(strs[i], i);
            if (items[strs[i]] != i) return false;
        }
        for (int i = 0; i < 20; i++)
        {
            items.erase(strs[i]);
            if (items.exists(strs[i])) return false;
        }

        items.insert(strs[0], 20);
        items.insert(strs[1], 22);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Map_WString_InsertEraseExistsClear()
{
    try
    {

        Zillion::Map<wstring, int> items;

        wchar_t strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            swprintf((wchar_t*)strs[i], 20, L"test string %d", i + 100000);
            items.insert(strs[i], i);
            if (items[strs[i]] != i) return false;
        }
        for (int i = 0; i < 20; i++)
        {
            items.erase(strs[i]);
            if (items.exists(strs[i])) return false;
        }

        items.insert(strs[0], 20);
        items.insert(strs[1], 22);
        if (items.size() != 2) return false;

        items.clear();

        return items.size() == 0;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Map_WString_AtFastInsert()
{
    try
    {
        Zillion::Map<wstring, int> items;
        const Zillion::Map<wstring, int>& constItems = items;

        wchar_t strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            swprintf((wchar_t*)strs[i], 20, L"test string %d", i + 100000);
            items.insert(strs[i], i);
            if (items.at(strs[i]) != i) return false;
            if (constItems.at(strs[i]) != i) return false;
        }
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_OperatorEqual1()
{
    try
    {
        Zillion::UInt32Set items1;
        Zillion::UInt32Set items2;

        items2.insert(10);
        items2.insert(20);
        items1 = items2;
        if (items1.size() != 2) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_OperatorEqual2()
{
    try
    {
        Zillion::UInt32Set items1;
        Zillion::UInt32Set items2;
        const Zillion::UInt32Set &items3 = items2;

        items2.insert(10);
        items2.insert(20);
        items1 = items3;
        if (items1.size() != 2) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_OperatorEqualEqual()
{
    try
    {
        Zillion::UInt32Set items1;
        Zillion::UInt32Set items2;

        items1.insert(10);
        items1.insert(20);
        items2.insert(10);
        items2.insert(20);

        return items1 == items2;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_OperatorNotEqual()
{
    try
    {
        Zillion::UInt32Set items1;
        Zillion::UInt32Set items2;

        items1.insert(10);
        items1.insert(20);
        items2.insert(10);

        if (!(items1 != items2)) return false;
        items2.insert(30);
        if (!(items1 != items2)) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Container_Begin()
{
    try
    {
        Zillion::UInt32Set items;

        items.insert(10);
        items.insert(20);

        auto it = items.begin();
        if (*it != 10 && *it != 20) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Container_CBegin()
{
    try
    {
        Zillion::UInt32Set items;

        items.insert(10);
        items.insert(20);

        Zillion::UInt32Set::const_iterator it = items.cbegin();
        if (*it != 10 && *it != 20) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Container_Count()
{
    try
    {
        Zillion::UInt32Set items;

        items.insert(10);
        items.insert(20);

        if (items.count(10) != 1) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Container_Empty()
{
    try
    {
        Zillion::UInt32Set items;

        if (!items.empty()) return false;

        items.insert(10);
        items.insert(20);
        items.clear();

        if (!items.empty()) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Container_End()
{
    try
    {
        Zillion::UInt32Set items;

        if (items.begin() != items.end()) return false;

        items.insert(10);

        if (items.begin() == items.end()) return false;

        auto it = items.end();
        --it;

        if (*it != 10) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_EraseWithConstIterator()
{
    try
    {
        Zillion::UInt32Set items;

        items.insert(10);
        Zillion::UInt32Set::const_iterator cit = items.cbegin();
        items.erase(cit);

        if (items.size() != 0) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_EraseWithIteratorRange()
{
    try
    {
        Zillion::UInt32Set items;

        items.insert(10);
        items.insert(20);
        items.insert(30);
        items.insert(40);
        items.insert(50);
        Zillion::UInt32Set::iterator it1 = items.begin();
        ++it1;

        Zillion::UInt32Set::iterator it2 = it1;
        ++it2;
        ++it2;

        items.erase(it1, it2);

        if (items.size() != 3) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_ConstFind()
{
    try
    {
        Zillion::UInt32Set items;
        const Zillion::UInt32Set & items2 = items;

        items.insert(10);
        items.insert(20);
        items.insert(30);
        items.insert(40);
        items.insert(50);

        Zillion::UInt32Set::const_iterator it = items2.find(30);
        if (*it != 30) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_InsertRange()
{
    try
    {
        Zillion::UInt32Set items1;
        Zillion::UInt32Set items2;

        items1.insert(10);
        items1.insert(20);
        items1.insert(30);
        items1.insert(40);
        items1.insert(50);

        Zillion::UInt32Set::iterator it1 = items1.begin();
        ++it1;
        Zillion::UInt32Set::iterator it2 = it1;
        ++it2;
        ++it2;

        items2.insert(it1, it2);

        if (items2.size() != 2) return false;

        //try erasing them all, the two in items2 can be any two from items1's five items
        items2.erase(10);
        items2.erase(20);
        items2.erase(30);
        items2.erase(40);
        items2.erase(50);

        if (items2.size() != 0) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_DecreasingInterator()
{
    try
    {
        Zillion::UInt32Set items;

        items.insert(10);
        items.insert(20);
        items.insert(30);
        items.insert(40);
        items.insert(50);

        Zillion::UInt32Set::iterator it = items.end();

        --it;
        if (!((*it) <= 50 && (*it) >= 10)) return false; //1
        --it;
        if (!((*it) <= 50 && (*it) >= 10)) return false; //2
        --it;
        if (!((*it) <= 50 && (*it) >= 10)) return false; //3
        --it;
        if (!((*it) <= 50 && (*it) >= 10)) return false; //4
        --it;
        if (!((*it) <= 50 && (*it) >= 10)) return false; //5
        --it;

        if (it != items.end()) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_Map_Iterator()
{
    try
    {
        Zillion::Map<char *, int> items;

        char strs[20][20];
        for (int i = 0; i < 20; i++)
        {
            sprintf((char*)strs[i], "test string %d", i + 100000);
            items.insert(strs[i], 20);
            char * s = strs[i];
            if (items[s] != 20) return false;
        }
        for (auto it : items)
        {
            for (int i = 0; i < 20; i++)
            {
                if (strs[i][0] == 0)
                    continue;
                else if (strcmp(it.first, strs[i]) == 0)
                    strs[i][0] = 0;
            }
        }

        for (int i = 0; i < 20; i++)
        if (strs[i][0] != 0) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_CopyConstructor()
{
    try
    {
        Zillion::UInt32Set items;
        items.insert(10);
        items.insert(20);
        items.insert(30);
        items.insert(40);
        items.insert(50);

        Zillion::UInt32Set items2(items);

        if (items2.size() != 5) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}

bool Test_MapUpdate()
{
    try
    {
        Zillion::Map<string, int> items;
        items[string("key1")] = 0;
        items[string("key1")]++;
        if (items[string("key1")] != 1) return false;
    }
    catch (exception ex)
    {
        cout << ex.what() << endl;
        return false;
    }

    return true;
}


int main()
{
    UnitTest ut;

    ut.AddCase("Test_CharSet_InsertEraseExistsClear", Test_CharSet_InsertEraseExistsClear);
    ut.AddCase("Test_UCharSet_InsertEraseExistsClear", Test_UCharSet_InsertEraseExistsClear);
    ut.AddCase("Test_WCharSet_InsertEraseExistsClear", Test_WCharSet_InsertEraseExistsClear);
    ut.AddCase("Test_Int16Set_InsertEraseExistsClear", Test_Int16Set_InsertEraseExistsClear);
    ut.AddCase("Test_UInt16Set_InsertEraseExistsClear", Test_UInt16Set_InsertEraseExistsClear);
    ut.AddCase("Test_Int32Set_InsertEraseExistsClear", Test_Int32Set_InsertEraseExistsClear);
    ut.AddCase("Test_UInt32Set_InsertEraseExistsClear", Test_UInt32Set_InsertEraseExistsClear);
    ut.AddCase("Test_Int64Set_InsertEraseExistsClear", Test_Int64Set_InsertEraseExistsClear);
    ut.AddCase("Test_UInt64Set_InsertEraseExistsClear", Test_UInt64Set_InsertEraseExistsClear);
    ut.AddCase("Test_PointerSet_InsertEraseExistsClear", Test_PointerSet_InsertEraseExistsClear);
    ut.AddCase("Test_CharPtrSet_InsertEraseExistsClear", Test_CharPtrSet_InsertEraseExistsClear);
    ut.AddCase("Test_WCharPtrSet__InsertEraseExistsClear", Test_WCharPtrSet__InsertEraseExistsClear);
    ut.AddCase("Test_StringSet_InsertEraseExistsClear", Test_StringSet_InsertEraseExistsClear);
    ut.AddCase("Test_WString_SetInsertEraseExistsClear", Test_WString_SetInsertEraseExistsClear);
    ut.AddCase("Test_BlockSet_InsertEraseExistsClearCountFindFastInsert", Test_BlockSet_InsertEraseExistsClearCountFindFastInsert);
    ut.AddCase("Test_Map_CharPtr_InsertEraseExistsClear", Test_Map_CharPtr_InsertEraseExistsClear);
    ut.AddCase("Test_Map_String_InsertEraseExistsClear", Test_Map_String_InsertEraseExistsClear);
    ut.AddCase("Test_Map_WString_InsertEraseExistsClearClear", Test_Map_WString_InsertEraseExistsClear);
    ut.AddCase("Test_Map_WString_AtFastInsert", Test_Map_WString_AtFastInsert);
    ut.AddCase("Test_Container_Begin", Test_Container_Begin);
    ut.AddCase("Test_Container_CBegin", Test_Container_CBegin);
    ut.AddCase("Test_Container_Count", Test_Container_Count);
    ut.AddCase("Test_Container_Empty", Test_Container_Empty);
    ut.AddCase("Test_OperatorEqual1", Test_OperatorEqual1);
    ut.AddCase("Test_OperatorEqual2", Test_OperatorEqual2);
    ut.AddCase("Test_OperatorEqualEqual", Test_OperatorEqualEqual);
    ut.AddCase("Test_OperatorNotEqual", Test_OperatorNotEqual);
    ut.AddCase("Test_EraseWithConstIterator", Test_EraseWithConstIterator);
    ut.AddCase("Test_EraseWithIteratorRange", Test_EraseWithIteratorRange);
    ut.AddCase("Test_ConstFind", Test_ConstFind);
    ut.AddCase("Test_InsertRange", Test_InsertRange);
    ut.AddCase("Test_DecreasingInterator", Test_DecreasingInterator);
    ut.AddCase("Test_Map_Iterator", Test_Map_Iterator);
    ut.AddCase("Test_CopyConstructor", Test_CopyConstructor);
    ut.AddCase("Test_MapUpdate", Test_MapUpdate);

    for (int i = 0; i < 1; i++)
    {
        if (!ut.RunTillFailed())
        {
            getchar();
            return 1;
        }
    }
    return 0;
}
