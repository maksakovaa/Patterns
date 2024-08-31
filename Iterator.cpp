//Iterator pattern
#include <stdexcept>
#include <iostream>

#define DEFAULT_LIST_CAPACITY 1000

template <class Item> class List
{
public:
    List(long size = DEFAULT_LIST_CAPACITY);
    List(List&);
    ~List();
    List& operator=(const List&);

    long Count() const;
    Item& Get(long index) const;
    Item& First() const;
    Item& Last() const;
    bool Includes(const Item&);
    void Append(const Item&);
    void Prepend(const Item&);

    void Remove(const Item&);
    void RemoveLast();
    void RemoveFirst();
    void RemoveAll();

    Item& Top() const;
    void Push(const Item&);
    Item& Pop();
private:
    Item* array;
    long counter;
};

template <class Item> List<Item>::List(long size)
{
    array = new Item(size);
    counter = 0;
}

template <class Item> List<Item>::~List()
{
    delete[] array;
}

template <class Item> long List<Item>::Count() const
{
    return counter;
}

template <class Item> Item& List<Item>::Get(long index) const
{
    if (index > counter - 1 || index < 0)
    {
        throw std::runtime_error("ERROR: out of bound in list!");
    }
    
    return array[index];
}

template <class Item> Item& List<Item>::First() const
{
    return array[0];
}

template <class Item> Item& List<Item>::Last() const
{
    return array[counter - 1];
}

template <class Item> bool List<Item>::Includes(const Item& it)
{
    for (long i = 0; i < counter; i++)
    {
        if (array[i] == it)
        {
            return true;
        }
    }
    return false;
}

template <class Item> void List<Item>::Append(const Item& it)
{
    array[counter] = it;
    counter++;
}

template <class Item> void List<Item>::Prepend(const Item& it)
{
    for (long i = counter; i > 0; i--)
    {
        array[i] = array[i - 1];
    }
    array[0] = it;
    counter++;
}

template <class Item> void List<Item>::RemoveFirst()
{
    for (long i = 0; i < counter; i++)
    {
        array[i] = array[i + 1];
    }
    array[counter - 1] = 0;
    counter--;
}

template <class Item> void List<Item>::RemoveLast()
{
    array[counter - 1] = 0;
    counter--;
}

template <class Item> void List<Item>::Remove(const Item& toRemove)
{
    for (long i = 0; i < counter; i++)
    {
        if (array[i] == toRemove && i == 0)
        {
            RemoveFirst();
            break;
        }
        else if (array[i] == toRemove && i == counter - 1)
        {
            RemoveLast();
            break;
        }
        else if (array[i] == toRemove)
        {
            for (long j = i; j < counter; j++)
            {
                array[j] = array[j + 1];
            }
            counter--;
            break;
        }
    }
    
}

template <class Item> void List<Item>::RemoveAll()
{
    delete[] array;
    array = new Item(DEFAULT_LIST_CAPACITY);
    counter = 0;
}

template <class Item> class Iterator
{
public:
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() const = 0;
    virtual Item CurrentItem() const = 0;
protected:
    Iterator() = default;
};

template <class Item> class ListIterator: public Iterator<Item>
{
public:
    ListIterator(const List<Item>* aList): _list(aList), _current(0) {}
    virtual void First()
    {
        _current = 0;
    }
    virtual void Next()
    {
        _current++;
    }
    virtual bool IsDone() const
    {
        return _current >= _list->Count() - 1;
    }
    virtual Item CurrentItem() const
    {
        if (_current > _list->Count())
        {
            throw std::runtime_error("ERROR: Iterator out of bounds!");
        }
        return _list->Get(_current);
    }
private:
    const List<Item>* _list;
    long _current;
};

template <class Item> class ReverseListIterator: public Iterator<Item>
{
public:
    ReverseListIterator(const List<Item>* aList)
    {
        _list = aList;
        First();
    }
    virtual void First()
    {
        _current = _list->Count() - 1;
    }
    virtual void Next()
    {
        _current--;
    }
    virtual bool IsDone() const
    {
        return _current < 0;
    }
    virtual Item CurrentItem() const
    {
        if (_current < 0)
        {
            throw std::runtime_error("ERROR: Iterator out of bounds!");
        }
        return _list->Get(_current);
    }
private:
    const List<Item>* _list;
    long _current;
};

template <class Item> void printList(const List<Item>& aList)
{
    if (aList.Count() == 0)
    {
        std::cout << "List is empty!" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < aList.Count(); i++)
    {
        std::cout << aList.Get(i) << " ";
    }
    std::cout << std::endl;
}

void testList()
{
    List<int> indexes;
    for (long i = 9; i > 4; i--)
    {
        indexes.Append(i);
    }
    printList(indexes);
    std::cout << "Insert in begin of the List:" << std::endl;
    indexes.Prepend(0);
    printList(indexes);
    std::cout << "Remove from begin of the List:" << std::endl;
    indexes.RemoveFirst();
    printList(indexes);
    std::cout << "Remove from the end of List:" << std::endl;
    indexes.RemoveLast();
    printList(indexes);
    std::cout << "Remove item (7) from List:" << std::endl;
    indexes.Remove(7);
    printList(indexes);
    std::cout << "Remove all from List:" << std::endl;
    indexes.RemoveAll();
    printList(indexes);
    std::cout << "Again fill List:" << std::endl;
    for (long i = 9; i > 4; i--)
    {
        indexes.Append(i);
    }
    printList(indexes);
}

void testIterator()
{
    std::cout << "Test iterator:" << std::endl;
    List<int>* indexes = new List<int>;
    for (long i = 9; i > 4; i--)
    {
        indexes->Append(i);
    }
    printList(*indexes);

    ListIterator<int> it(indexes);
    std::cout << it.CurrentItem() << "\n";
    std::cout << "Status is done: " << it.IsDone() << "\n";
    while (true)
    {
        if (it.IsDone())
        {
            break;
        }
        it.Next();
    }
    std::cout << "Status is done: " << it.IsDone() << ", current Item is: " << it.CurrentItem() << "\n";

    delete indexes;
}

void testReverseIterator()
{
    std::cout << "Test reverse iterator:" << std::endl;
    List<int>* indexes = new List<int>;
    for (long i = 9; i > 4; i--)
    {
        indexes->Append(i);
    }
    ReverseListIterator<int> it(indexes);
    std::cout << "Status is done: " << it.IsDone() << "\n";

    for(; !it.IsDone(); it.Next())
    {
        std::cout << it.CurrentItem() << " ";
    }

    std::cout << std::endl << "Status is done: " << it.IsDone() << "\n";

    delete indexes;
}


int main()
{
    testList();
    testIterator();
    testReverseIterator();
}