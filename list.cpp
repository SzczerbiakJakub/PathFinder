#include "list.h"

DoubleList::pathElement::pathElement()
{
    next = 0;
    prev = 0;
    g = 0;
    h = 0;
    f = 0;
}

DoubleList::list::list()
{
    head = 0;
    nextList = 0;
    prevList = 0;
    tail = head;
    counter = 0;
}

DoubleList::list2d::list2d()
{
    listHead = 0;
    listTail = listHead;
    listCounter = 0;
}

void DoubleList::list::add_element(int elementType, int elementX, int elementY)
{
    pathElement * newElement = new pathElement;
    newElement->type = elementType;
    newElement->x = elementX;
    newElement->y = elementY;

    if (head == 0)
    {
        head = newElement;
        head->next = 0;
        tail = head;
        counter ++;
    }
    else
    {
        pathElement * temp = head;

        while (temp->next)
        {
            temp = temp->next;
        }
        newElement->next = 0;
        tail = newElement;
        temp->next = tail;
        tail->prev = temp;
        counter ++;
    }

}


void DoubleList::list::add_existing_element(DoubleList::pathElement *element)
{
    pathElement * newElement = new pathElement;
    newElement->type = element->type;
    newElement->x = element->x;
    newElement->y = element->y;

    if (head == 0)
    {
        head = newElement;
        head->next = 0;
        tail = head;
        counter ++;
    }
    else
    {
        pathElement * temp = head;

        while (temp->next)
        {
            temp = temp->next;
        }
        newElement->next = 0;
        tail = newElement;
        temp->next = tail;
        tail->prev = temp;
        counter ++;
    }
}


void DoubleList::list::remove_last_element()
{
    if (counter == 0)
        return;

    if (counter == 1)
    {
        head = 0;
        tail = head;
        counter --;
    }
    else
    {
        pathElement * newLast = head, * last;

        last = newLast->next;
        while (last->next)
        {
            newLast = newLast->next;
            last = last->next;
        }
        delete last;
        newLast->next = 0;
        tail = newLast;
        counter --;
    }
}
void DoubleList::list::show_list(int howMany)
{
    if (counter == 0)
    {
        qDebug() << "NO ELEMENTS";
        return;
    }
    int howFar;
    QString blad;
    if (howMany > counter)
    {
        howFar = counter;
        blad = "PODANO ZA DUZO WYRAZOW";
    }
    else
        howFar = howMany;

    pathElement * element = head;

    while (howFar > 0)
    {
        //qDebug() << element->x << "  " << element->y;
        qDebug() << element->type << "  x: " << element->x << "  y: " << element->y << "  f: " << element->f << "  type: " << element->type;
        element = element->next;
        howFar --;
    }
    if (blad.size() > 0)
        qDebug() << blad;

}

DoubleList::pathElement * DoubleList::list::get_element(int index)
{
    DoubleList::pathElement * element = this->head;
    int element_index = 0;

    if (element == 0)
        return NULL;

    while (element_index < index)
    {
        element = element->next;
        element_index += 1;
    }

    return element;
}

int DoubleList::list::get_index(DoubleList::pathElement *element)
{
    DoubleList::pathElement * _element = this->head;
    int _element_index = 0;

    if (_element == 0)
        return NULL;


    while (_element != element)
    {
        _element = _element->next;
        _element_index += 1;
    }

    return _element_index;
}



void DoubleList::list::remove_element(pathElement * element)
{
    DoubleList::pathElement * _element = this->head;

    while (_element != element)
    {
        _element = _element->next;
    }

    if (_element == this->head)
    {
        this->head = _element->next;
        _element->next->prev = 0;
    }
    else if (_element == this->tail)
    {
        this->tail = _element->prev;
        _element->prev->next = 0;
    }
    else
    {
        _element->next->prev = _element->prev;
        _element->prev->next = _element->next;
    }

    counter -= 1;

    delete _element;
}


void DoubleList::list::unlink(pathElement * element)
{
    DoubleList::pathElement * _element = this->head;

    while (_element != element)
    {
        _element = _element->next;
    }

    if (_element == this->head)
    {
        this->head = _element->next;
        _element->next->prev = 0;
    }
    else if (_element == this->tail)
    {
        this->tail = _element->prev;
        _element->prev->next = 0;
    }
    else
    {
        _element->next->prev = _element->prev;
        _element->prev->next = _element->next;
    }

    counter -= 1;

}


bool DoubleList::list::in_list(DoubleList::pathElement *element)
{
    DoubleList::pathElement * _element = this->head;

    if (_element == 0)
    {
        //qDebug() << "BRAK ELEMENTOW W LISCIE";
        return false;
    }

    //this->show_list(this->counter);

    //qDebug() << "PROCES POSZEDL DALEJ";

    //qDebug() << "SPRAWDZA...";

    int _counter = 1;

    while (_element != this->tail->next)
    {
        //qDebug() << "PRZED: " << _counter;

        if (_element->x == element->x && _element->y == element->y)
        {
            //qDebug() << element->x << "," << element->y << " są te same co " << _element->x << "," << _element->y;
            return true;
        }

        _element = _element->next;
        _counter += 1;
        //qDebug() << "PO: " << _counter;
    }

    //qDebug() << element->x << "," << element->y << " nie są takie same jak " << this->tail->x << "," << this->tail->y;
    return false;
}


void DoubleList::list::merge_to_list(list * main_list)
{
    DoubleList::pathElement * element = this->head;

    if (element == 0)
        return;

    while (element != this->tail->next)
    {
        main_list->add_element(element->type, element->x, element->y);
        main_list->tail->f = element->f;
        main_list->tail->g = element->g;
        main_list->tail->h = element->h;
        element = element->next;
    }

    delete this;
}




void DoubleList::list2d::add_list()
{
    list * newList = new list;

    if (listHead == 0)
    {
        listHead = newList;
        listHead->nextList = 0;
        listTail = listHead;
        listCounter ++;
    }
    else
    {
        list * tempList = listHead;

        while (tempList->nextList)
        {
            tempList = tempList->nextList;
        }
        newList->nextList = 0;
        tempList->nextList = newList;
        listTail = newList;
        listTail->prevList = tempList;
        listCounter ++;
    }
}

void DoubleList::list2d::create_lists(int quantity, list2d * nazwa)
{
    DoubleList::list2d * lista = nazwa;
    for (int i = 0; i < quantity; i++)
    {
        lista->add_list();
    }
}

void DoubleList::list2d::show_list_2d(int howMany)
{
    if (listCounter == 0)
    {
        qDebug() << "NO ELEMENTS";
        return;
    }
    int howFar;
    QString blad;
    if (howMany > listCounter)
    {
        howFar = listCounter;
        blad = "PODANO ZA DUZO LIST";
    }
    else
        howFar = howMany;

    list * list = listHead;

    while (howFar > 0)
    {
        //qDebug() << element->x << "  " << element->y;
        list->show_list(list->counter);
        list = list->nextList;
        howFar --;
    }
    if (blad.size() > 0)
        qDebug() << blad;

}

void DoubleList::list2d::show_all()
{
    list * list = listHead;
    for(int i = 0; i < listCounter; i++)
    {
        list->show_list(list->counter);
        list = list->nextList;
    }
}

int DoubleList::list2d::check_type(int x, int y, list2d * nazwa)
{
    if (x < 1 || y < 1 || x > 512 || y > 512)
        return 1;
    list * list = nazwa->listHead;
    for (int i = 0; i < y; i++)
    {
        list = list->nextList;
    }
    pathElement * element = list->head;
    for (int i = 0; i < x; i++)
    {
        element = element->next;
    }
    return element->type;
}


DoubleList::pathElement * DoubleList::list2d::get_element(int x, int y)
{
    if (x < 1 || y < 1 || x > 512 || y > 512)
        return NULL;
    DoubleList::list * list = this->listHead;
    for (int i = 0; i < y; i++)
    {
        list = list->nextList;
    }
    pathElement * element = list->head;
    for (int i = 0; i < x; i++)
    {
        element = element->next;
    }
    return element;
}
