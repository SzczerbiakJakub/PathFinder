#include "algorithm.h"

Algorithm::Algorithm()
{
    QImage pathImage(512, 512, QImage::Format_ARGB32);
    pathImage.fill(qRgba(0,0,0,0));
}

double Algorithm::distance_between_points(int x1, int y1, int x2, int y2)
{
    /*int x = x2 - x1, y = y2 - y1;
    return abs(x)+abs(y);   */   //  MANHATTAN HEURISTIC

    int dx = abs(x1 - x2), dy = abs(y1 - y2), D = 10, D2 = 14;
    //double D2 = 1.41;                 //    D2 = sqrt(2) - NIEWYMIERNOSC BUGUJE ALGORYTM
                                      //    NIECH D = 2 I D2 = 3 - CALKOWITE LICZBY
    if (dx < dy)
        return D * (dx + dy) + (D2 - 2 * D) * dx;
    else
        return D * (dx + dy) + (D2 - 2 * D) * dy;  // DIAGONAL HEURISTIC

    //return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)); // EUCLIDEAN HEURISTIC
}

QPixmap Algorithm::create_path(QImage image)
{
    return QPixmap::fromImage(image);
}


void Algorithm::nearest_space_clockwise(DoubleList::pathElement * wall,
                                        DoubleList::list * succesors, DoubleList::list * elements/*, DoubleList::list2d * paths*/)
{
    int position = 1;
    DoubleList::pathElement * leftElement = 0, * rightElement = 0,
            * current = succesors->head, * previousElement;


    while (current != wall)     // OKRESLENIE, KTORA W KOLEJNOSCI JEST SCIANA (ELEMENTY STWORZONE ODWROTNIE DO WSKAZOWEK ZEGARA)
    {
        current = current->next;
        position ++;
    }

    qDebug() << "ŚCIANA NA POZYCJI: " << position;
    qDebug() << "KOORDY SCIANY: " << wall->x <<" "<<wall->y;

    for (int i = position; i < succesors->counter; i++)    // OD POZYCJI DO KONCA LISTY
    {
        previousElement = current;
        current = current->next;

        qDebug() << current->x <<" "<<current->y<<" "<<current->type;

        if (leftElement == 0)
        {
            if (current->type == 0 && previousElement->type == 1)
            {
                leftElement = current;
            }
        }
        if (rightElement == 0)
        {
            if (current->type == 1 && previousElement->type == 0)
            {
                rightElement = previousElement;
            }
        }
    }

    previousElement = current;
    current = succesors->head;

    for (int i = 0; i < position; i++)
    {
        qDebug() << current->x <<" "<<current->y<<" "<<current->type;

        if (leftElement == 0)
        {
            if (current->type == 0 && previousElement->type == 1)
            {
                leftElement = current;
            }
        }
        if (rightElement == 0)
        {
            if (current->type == 1 && previousElement->type == 0)
            {
                rightElement = previousElement;
            }
        }
        previousElement = current;
        current = current->next;
    }

    if (leftElement == 0)
    {
        current = wall;

        for (int i = position; i < succesors->counter; i++)    // OD POZYCJI DO KONCA LISTY
        {
            previousElement = current;
            current = current->next;

            qDebug() << current->x <<" "<<current->y<<" "<<current->type;

            if (leftElement == 0)
            {
                if (current->type == 2 && previousElement->type != 2)
                {
                    leftElement = current;
                }
            }
        }

        previousElement = current;
        current = succesors->head;

        for (int i = 0; i < position; i++)
        {
            qDebug() << current->x <<" "<<current->y<<" "<<current->type;

            if (leftElement == 0)
            {
                if (current->type == 2 && previousElement->type != 2)
                {
                    leftElement = current;
                }
            }
            previousElement = current;
            current = current->next;
        }
    }

    if (rightElement == 0)
    {
        current = wall;

        for (int i = position; i < succesors->counter; i++)    // OD POZYCJI DO KONCA LISTY
        {
            previousElement = current;
            current = current->next;

            qDebug() << current->x <<" "<<current->y<<" "<<current->type;

            if (rightElement == 0)
            {
                if (current->type != 2 && previousElement->type == 2)
                {
                    rightElement = previousElement;
                }
            }
        }

        previousElement = current;
        current = succesors->head;

        for (int i = 0; i < position; i++)
        {
            qDebug() << current->x <<" "<<current->y<<" "<<current->type;

            if (rightElement == 0)
            {
                if (current->type != 2 && previousElement->type == 2)
                {
                    rightElement = previousElement;
                }
            }
            previousElement = current;
            current = current->next;
        }
    }

    //      ZNAMY KOORDY LEWEGO I PRAWEGO, MOZNA DZIALAC DALEJ

    if (leftElement != 0 && rightElement != 0)
        elements->add_element(leftElement->type,leftElement->x,leftElement->y);

    if (rightElement != 0)
        elements->add_element(rightElement->type,rightElement->x,rightElement->y);

    elements->show_list(elements->counter);

    if (rightElement == leftElement)
        delete leftElement;
    else
    {
        delete leftElement;
        delete rightElement;
    }
}

bool Algorithm::build_path_element(int x1, int y1, int x2, int y2, int radius, DoubleList::list * possiblePath,
                                   DoubleList::list2d * list2d, DoubleList::list2d * paths, int mode)
{
    int wartoscF = possiblePath->tail->f, formerF = wartoscF, horizontal, vertical;
    bool firstSuccessorIsForbidden = false, choiceMade = false, isWall = false;

    DoubleList::pathElement * chosenElement, * successorElement, * forbiddenElement;
    DoubleList::list * forbiddenList = new DoubleList::list;

        DoubleList::list * succesorsList = new DoubleList::list;

    for (int i = -radius; i < 2*radius; i+= radius)             //  8 NASTĘPCÓW BADANEGO ELEMENTU
        {
            succesorsList->add_element(list2d->check_type(x1-radius, y1+i, list2d), x1-radius, y1+i);
            succesorsList->tail->g = possiblePath->tail->g + Algorithm::distance_between_points(x1,y1,x1-radius,y1+i);
            succesorsList->tail->h = Algorithm::distance_between_points(x1-radius,y1+i,x2,y2);
            succesorsList->tail->f = succesorsList->tail->g + succesorsList->tail->h;

            forbiddenList = paths->listHead;

            while (forbiddenList != 0)
            {
                forbiddenElement = forbiddenList->head;
                while (forbiddenElement != 0)
                {
                    if (succesorsList->tail->x == forbiddenElement->x && succesorsList->tail->y == forbiddenElement->y)
                    {
                        succesorsList->tail->type = 2;
                        if (succesorsList->tail == succesorsList->head)
                        {
                            firstSuccessorIsForbidden = true;
                        }
                        else
                            succesorsList->tail->prev->type = 2;
                        //qDebug() << "WYKRYTO PRZESZKODE: " << succesorsList->tail->x << ", " << succesorsList->tail->y;
                    }
                    forbiddenElement = forbiddenElement->next;
                }
                forbiddenList = forbiddenList->nextList;
            }
        }

        succesorsList->add_element(list2d->check_type(x1, y1+radius, list2d), x1, y1+radius);
        succesorsList->tail->g = possiblePath->tail->g + Algorithm::distance_between_points(x1,y1,x1,y1+radius);
        succesorsList->tail->h = Algorithm::distance_between_points(x1,y1+radius,x2,y2);
        succesorsList->tail->f = succesorsList->tail->g + succesorsList->tail->h;

        forbiddenList = paths->listHead;
        while (forbiddenList != 0)
        {
            forbiddenElement = forbiddenList->head;
            while (forbiddenElement != 0)
            {
                if (succesorsList->tail->x == forbiddenElement->x && succesorsList->tail->y == forbiddenElement->y)
                {
                    succesorsList->tail->type = 2;
                    succesorsList->tail->prev->type = 2;
                    //qDebug() << "WYKRYTO PRZESZKODE: " << succesorsList->tail->x << ", " << succesorsList->tail->y;
                }
                forbiddenElement = forbiddenElement->next;
            }
            forbiddenList = forbiddenList->nextList;
        }

        for (int i = -radius; i < 2*radius; i+= radius)
        {
            succesorsList->add_element(list2d->check_type(x1+radius, y1-i, list2d), x1+radius, y1-i);
            succesorsList->tail->g = possiblePath->tail->g + Algorithm::distance_between_points(x1,y1,x1+radius,y1-i);
            succesorsList->tail->h = Algorithm::distance_between_points(x1+radius,y1-i,x2,y2);
            succesorsList->tail->f = succesorsList->tail->g + succesorsList->tail->h;

            forbiddenList = paths->listHead;
            while (forbiddenList != 0)
            {
                forbiddenElement = forbiddenList->head;
                while (forbiddenElement != 0)
                {
                    if (succesorsList->tail->x == forbiddenElement->x && succesorsList->tail->y == forbiddenElement->y)
                    {
                        succesorsList->tail->type = 2;
                        succesorsList->tail->prev->type = 2;
                        //qDebug() << "WYKRYTO PRZESZKODE: " << succesorsList->tail->x << ", " << succesorsList->tail->y;
                    }
                    forbiddenElement = forbiddenElement->next;
                }
                forbiddenList = forbiddenList->nextList;
            }
        }

        succesorsList->add_element(list2d->check_type(x1, y1-radius, list2d), x1, y1-radius);
        succesorsList->tail->g = possiblePath->tail->g + Algorithm::distance_between_points(x1,y1,x1,y1-radius);
        succesorsList->tail->h = Algorithm::distance_between_points(x1,y1-radius,x2,y2);
        succesorsList->tail->f = succesorsList->tail->g + succesorsList->tail->h;

        forbiddenList = paths->listHead;
        while (forbiddenList != 0)
        {
            forbiddenElement = forbiddenList->head;
            while (forbiddenElement != 0)
            {
                if (succesorsList->tail->x == forbiddenElement->x && succesorsList->tail->y == forbiddenElement->y)
                {
                    succesorsList->tail->type = 2;
                    succesorsList->tail->prev->type = 2;
                    //qDebug() << "WYKRYTO PRZESZKODE: " << succesorsList->tail->x << ", " << succesorsList->tail->y;
                }
                forbiddenElement = forbiddenElement->next;
            }
            forbiddenList = forbiddenList->nextList;
        }

        if (firstSuccessorIsForbidden == true)
            succesorsList->tail->type = 2;


        choiceMade = false;
        isWall = false;

        while (choiceMade == false)     //  DOPOKI NIE WYBRANO NASTEPCY TO NIE RUSZA DALEJ
        {
            DoubleList::list * chosenSuccessors = new DoubleList::list;     // LISTA WYBRANYCH NASTEPCOW

            while (chosenSuccessors->counter == 0 && isWall == false)
            {
                successorElement = succesorsList->head;
                for (int i = 0; i < 8; i++)
                {                      // WYBOR TYCH Z NAJMNIEJSZYM KOSZTEM (najkorzystniejszy wybor, uwzglednia ściany)
                    if (successorElement->f <= wartoscF && successorElement->f >= formerF)
                    {
                        /*
                        if (successorElement->type == 1)  // WARUNEK NA SCIANE
                        {
                            isWall = true;
                            chosenElement = successorElement;
                        }
                        else if (successorElement->type == 0)       //  WARUNEK NA MOŻLIWY KROK
                        {
                            chosenSuccessors->add_element(successorElement->type,successorElement->x,
                                                          successorElement->y);
                            chosenSuccessors->tail->g = successorElement->g;
                            chosenSuccessors->tail->h = successorElement->h;
                            chosenSuccessors->tail->f = successorElement->f;


                        }
                           */

                        chosenSuccessors->add_element(successorElement->type,successorElement->x,
                                                      successorElement->y);
                        chosenSuccessors->tail->g = successorElement->g;
                        chosenSuccessors->tail->h = successorElement->h;
                        chosenSuccessors->tail->f = successorElement->f;
                    }
                    successorElement = successorElement->next;
                }


                if (chosenSuccessors->counter == 0 && isWall == false)
                    //  JEŚLI NIE MA ŚCIANY ANI KORZYSTNYCH KROKÓW - ZWIĘKSZYĆ AKCEPTOWALNE WARUNKI KORZYŚCI
                {
                    wartoscF ++;
                    formerF ++;
                }
            }

            if (isWall)     // PRZY WYBORZE ŚCIANY TRAKTUJEMY JĄ JAKO PRZESZKODE - IDZIEMY PO JEJ OBWODZIE,
                    //  AŻ NASTEPNYM KROKIEM NIE BĘDZIE ŚCIANA, MOŻE BYĆ KILKA ŚCIEŻEK - NALEŻY WYBRAĆ NAJLEPSZĄ
            {
                DoubleList::list * wallPathElements = new DoubleList::list;

                horizontal = chosenElement->x - possiblePath->tail->x;
                vertical = chosenElement->y - possiblePath->tail->y;

                qDebug() << "KIERUNEK: " << horizontal << "   " << vertical;
                successorElement = succesorsList->head;

                qDebug() << successorElement->type << " " << succesorsList->tail->type << " " << succesorsList->tail->prev->type;
                qDebug() << successorElement->next->type << " " << 9 << " " << succesorsList->tail->prev->prev->type;
                qDebug() << successorElement->next->next->type << " " << successorElement->next->next->next->type << " " << successorElement->next->next->next->next->type;


                nearest_space_clockwise(chosenElement, succesorsList, wallPathElements);


                DoubleList::pathElement * leftWallPathElement = wallPathElements->head;
                DoubleList::pathElement * rightWallPathElement = leftWallPathElement->next;


                if (mode == 0)
                {
                    choiceMade = true;
                    possiblePath->add_element(leftWallPathElement->type, leftWallPathElement->x, leftWallPathElement->y);
                    possiblePath->tail->g = possiblePath->tail->prev->g + Algorithm::distance_between_points(x1,y1,possiblePath->tail->x,possiblePath->tail->y);
                    possiblePath->tail->h = Algorithm::distance_between_points(possiblePath->tail->x,possiblePath->tail->y,x2,y2);
                    possiblePath->tail->f = possiblePath->tail->g + possiblePath->tail->h;
                }
                else if (mode == 1)
                {
                    choiceMade = true;
                    possiblePath->add_element(rightWallPathElement->type, rightWallPathElement->x, rightWallPathElement->y);
                    possiblePath->tail->g = possiblePath->tail->prev->g + Algorithm::distance_between_points(x1,y1,possiblePath->tail->x,possiblePath->tail->y);
                    possiblePath->tail->h = Algorithm::distance_between_points(possiblePath->tail->x,possiblePath->tail->y,x2,y2);
                    possiblePath->tail->f = possiblePath->tail->g + possiblePath->tail->h;
                }

                delete wallPathElements;

            }
            else            //      JEŻELI NIE WYBRANO ŚCIANY, TO POSTĘPUJEMY TAK JAK ZWYKLE
            {
                qDebug() << chosenSuccessors->counter;

                chosenSuccessors->show_list(chosenSuccessors->counter);

                successorElement = chosenSuccessors->head;

                if (successorElement->type == 2)
                {
                    //possiblePath->tail->type = 3;
                }
                else
                {
                    choiceMade = true;
                    possiblePath->add_element(successorElement->type, successorElement->x, successorElement->y);
                    possiblePath->tail->g = successorElement->g;
                    possiblePath->tail->h = successorElement->h;
                    possiblePath->tail->f = successorElement->f;


                }
            }

            if (choiceMade == false)
            {
                formerF = wartoscF;
                wartoscF ++;
            }

            delete chosenSuccessors;

        }       //  UPEWNIASZ SIE, ZE WYBRALES JAKIS ELEMENT

        qDebug() << possiblePath->counter << " KROK (Fmin: " << possiblePath->tail->f << " ), x: " << possiblePath->tail->x << ", y: " << possiblePath->tail->y;

        wartoscF = possiblePath->tail->g + possiblePath->tail->h;
        formerF = wartoscF;

        delete succesorsList;

        if (isWall == true)
            return true;

        else if (choiceMade == true)
        {
            return false;
        }

        return false;
}




DoubleList::list2d * Algorithm::find_ways(int x1, int y1, int x2, int y2, int radius, DoubleList::list2d * list2d, DoubleList::list2d * paths)
{

    //  ZASADY ALGORYTMU:
    //  1) JAK TRAFIA NA ŚCIANE - IDZIE JEJ OBWODEM W DWIE STRONY, DOPÓKI NASTĘPNY KROK NIE BĘDZIE ŚCIANĄ,
    //      POTEM WYBIERA KRÓTSZA SCIEŻKĘ;          (Poki co idzie tylko w lewo, dorobic ruch prawostronny)
    //  2) ALGORYTM BAZUJE NA A*;
    //  3) NA KOŃCU MUSI PUNKTY GIĘCIA WYPLASZCZYĆ ZEBY UZYSKAĆ NAJKRÓTSZA ŚCIEŻKE IOK      (dodać takową opcję)

    paths->add_list();
    paths->add_list();

    DoubleList::list * possiblePath = paths->listHead, * second = paths->listHead->nextList, * forbiddenList = new DoubleList::list;

    DoubleList::list2d * checkpoints = new DoubleList::list2d;

    checkpoints->add_list();
    checkpoints->add_list();

    if (possiblePath->head == 0)
    {
        possiblePath->add_element(list2d->check_type(x1,y1,list2d),x1,y1);
    }

    if (second->head == 0)
    {
        second->add_element(list2d->check_type(x1,y1,list2d),x1,y1);
    }

    checkpoints->listHead->add_element(list2d->check_type(x1,y1,list2d),x1,y1);
    checkpoints->listTail->add_element(list2d->check_type(x1,y1,list2d),x1,y1);

    DoubleList::pathElement * element = possiblePath->head, * chosenElement, * successorElement,
            * forbiddenElement;

    possiblePath->tail->g = 0;
    possiblePath->tail->h = Algorithm::distance_between_points(x1,y1,x2,y2);
    possiblePath->tail->f = possiblePath->tail->g + possiblePath->tail->h;

    second->tail->g = 0;
    second->tail->h = Algorithm::distance_between_points(x1,y1,x2,y2);
    second->tail->f = second->tail->g + second->tail->h;

    int /*pathElementsTransfered = 0,*/ licznik = 0, wartoscF = element->f,
            formerF = element->h; // OD NIEGO ZALEZY CZY UZNA TRASE ZA ZŁĄ

    element->g = 0;
        element->h = Algorithm::distance_between_points(x1,y1,x2,y2);
        element->f = element->g + element->h;

        bool stop = false, rightWasWall = false, leftWasWall = false, rightIsWall = false, leftIsWall = false;

        int walls = 0;

        while (licznik < 500 && stop == false)   //BACKUPA ZROB
        {

            rightWasWall = rightIsWall;
            leftWasWall = leftIsWall;

            if (!(abs(x2-possiblePath->tail->x) <= radius - 1 && abs(y2-possiblePath->tail->y) <= radius - 1))
            {
                leftIsWall = build_path_element(possiblePath->tail->x,possiblePath->tail->y,x2,y2,radius,possiblePath,list2d, paths, 0);
                if (leftIsWall)
                    walls ++;
                if (leftWasWall == true && leftIsWall != true)
                {
                    checkpoints->listHead->add_element(possiblePath->tail->type, possiblePath->tail->x, possiblePath->tail->y);
                }
            }
            else
                stop = true;

            if (!(abs(x2-second->tail->x) <= radius - 1 && abs(y2-second->tail->y) <= radius - 1))
            {
                rightIsWall = build_path_element(second->tail->x,second->tail->y,x2,y2,radius,second,list2d, paths, 1);
                if (rightIsWall)
                    walls ++;
                if (rightWasWall == true && rightIsWall != true)
                {
                    checkpoints->listTail->add_element(second->tail->type, second->tail->x, second->tail->y);
                }
            }
            else
                stop = true;

            licznik ++;
        }

        checkpoints->listHead->add_element(list2d->check_type(x2,y2,list2d),x2,y2);
        checkpoints->listTail->add_element(list2d->check_type(x2,y2,list2d),x2,y2);

        qDebug() << "CHECKPOINTY LISTY PIERWSZEJ: ";
        checkpoints->listHead->show_list(checkpoints->listHead->counter);

        qDebug() << "CHECKPOINTY LISTY DRUGIEJ: ";
        checkpoints->listTail->show_list(checkpoints->listTail->counter);

        DoubleList::list2d * paths2 = new DoubleList::list2d;

        DoubleList::pathElement * tracer = new DoubleList::pathElement;


        /// TRZEBA ZROBIC NAJKROTSZE SCIEZKI W SCIEZCE, OD POCZATKU SPRAWDZA CZY BEDZIE SCIANA W NAJKROTSZEJ SCIEZCE DO KAZDEGO
        /// DAJ WARUNEK NA SCIANE< WTEDY BEDZIE SIE ODPALAC TO CO NIZEJ - POSORTOWANIE SCIEZKI

        /* if (walls > 0)
        {
            DoubleList::pathElement * finish = new DoubleList::pathElement, * start = new DoubleList::pathElement,
                                * sortedPathElement = new DoubleList::pathElement;
            DoubleList::list2d * theUltimatePath = new DoubleList::list2d;
            bool wall = false;

            start = possiblePath->tail;
            finish = possiblePath->head;

            while (finish != start)
            {
                DoubleList::list * sortingPath = new DoubleList::list;

                wall = false;

                sortingPath->add_element(start->type, start->x, start->y);

                /// TU MA BYC FUNKCJA PROSTUJACA SCIEZKE
                ///

            }

            delete finish;

            return theUltimatePath;
        }   */


        /*paths2->add_list();
        paths2->add_list();

        paths2->listHead->add_element(list2d->check_type(x1,y1,list2d),x1,y1);
        paths2->listTail->add_element(checkpoints->listTail->head->type, checkpoints->listTail->head->x, checkpoints->listTail->head->y);

        if ((possiblePath->tail->x == x2 && possiblePath->tail->y == y2) && checkpoints->listHead->counter > 2)
        {
            tracer = checkpoints->listHead->head->next;
            while (tracer != 0)
            {
                while (!(paths2->listHead->tail->x == tracer->x && paths2->listHead->tail->y == tracer->y))
                {
                    leftIsWall = build_path_element(paths2->listHead->tail->x, paths2->listHead->tail->y, tracer->x, tracer->y, radius, paths2->listHead, list2d, paths2, 0);
                }
                tracer = tracer->next;
            }

            delete tracer;

            return paths2;
        }

        if ((second->tail->x == x2 && second->tail->y == y2) && checkpoints->listTail->counter > 2)
        {

            tracer = checkpoints->listTail->head->next;
            while (tracer != 0)
            {
                while (!(paths2->listTail->tail->x == tracer->x && paths2->listTail->tail->y == tracer->y))
                {
                    leftIsWall = build_path_element(paths2->listTail->tail->x, paths2->listTail->tail->y, tracer->x, tracer->y, radius, paths2->listTail, list2d, paths2, 1);
                }
                tracer = tracer->next;
            }

            delete tracer;

            return paths2;

        }

        /*if (checkpoints->listHead->counter > 2 || checkpoints->listTail->counter > 2)
        {
            DoubleList::pathElement * tracer = new DoubleList::pathElement;

            tracer = checkpoints->listHead->head;

            possiblePath = paths2->listHead;
            while (tracer != 0)
            {
                while (!(paths2->listHead->tail->x == tracer->x && paths2->listHead->tail->y == tracer->y))
                {
                    leftIsWall = build_path_element(possiblePath->tail->x,possiblePath->tail->y,tracer->x,tracer->y,radius,possiblePath,list2d, paths2, 0);
                }
                tracer = tracer->next;
            }

            tracer = checkpoints->listTail->head;
            possiblePath = paths2->listTail;
            while (tracer != 0)
            {
                while (!(paths2->listTail->tail->x == tracer->x && paths2->listTail->tail->y == tracer->y))
                {
                    leftIsWall = build_path_element(possiblePath->tail->x,possiblePath->tail->y,tracer->x,tracer->y,radius,possiblePath,list2d, paths2, 0);
                }
                tracer = tracer->next;
            }

            delete tracer;

            return paths2;
        }   */

    return paths;

}





DoubleList::list2d * Algorithm::find_ways_2(int x1, int y1, int x2, int y2, int radius, DoubleList::list2d * list2d, DoubleList::list2d * paths)
{

    //  ZASADY ALGORYTMU:
    //  1) JAK TRAFIA NA ŚCIANE - IDZIE JEJ OBWODEM W DWIE STRONY, DOPÓKI NASTĘPNY KROK NIE BĘDZIE ŚCIANĄ,
    //      POTEM WYBIERA KRÓTSZA SCIEŻKĘ;          (Poki co idzie tylko w lewo, dorobic ruch prawostronny)
    //  2) ALGORYTM BAZUJE NA A*;
    //  3) NA KOŃCU MUSI PUNKTY GIĘCIA WYPLASZCZYĆ ZEBY UZYSKAĆ NAJKRÓTSZA ŚCIEŻKE IOK      (dodać takową opcję)

    paths->add_list();

    DoubleList::list * path = paths->listHead;

    DoubleList::list2d * checkpoints = new DoubleList::list2d;

    checkpoints->add_list();

    if (path->head == 0)
    {
        path->add_element(list2d->check_type(x1,y1,list2d),x1,y1);
    }


    checkpoints->listHead->add_element(list2d->check_type(x1,y1,list2d),x1,y1);

    DoubleList::pathElement * element = path->head, * chosenElement, * successorElement,
            * forbiddenElement;

    path->tail->g = 0;
    path->tail->h = Algorithm::distance_between_points(x1,y1,x2,y2);
    path->tail->f = path->tail->g + path->tail->h;


    int /*pathElementsTransfered = 0,*/ licznik = 0, wartoscF = element->f,
            formerF = element->h; // OD NIEGO ZALEZY CZY UZNA TRASE ZA ZŁĄ

    element->g = 0;
        element->h = Algorithm::distance_between_points(x1,y1,x2,y2);
        element->f = element->g + element->h;

        bool stop = false, rightWasWall = false, leftWasWall = false, rightIsWall = false, leftIsWall = false;

        int walls = 0;

        while (licznik < 500 && stop == false)   //BACKUPA ZROB
        {
            build_path_element(path->tail->x,path->tail->y,x2,y2,radius,path,list2d, paths, 0);

            if (!(abs(x2-path->tail->x) <= radius - 1 && abs(y2-path->tail->y) <= radius - 1))
            {
                checkpoints->listHead->add_element(path->tail->type, path->tail->x, path->tail->y);
            }
            else
                stop = true;

            licznik ++;
        }

        checkpoints->listHead->add_element(list2d->check_type(x2,y2,list2d),x2,y2);

        qDebug() << "CHECKPOINTY LISTY PIERWSZEJ: ";
        checkpoints->listHead->show_list(checkpoints->listHead->counter);


    return paths;

}


DoubleList::list * Algorithm::new_elements(DoubleList::pathElement * element, DoubleList::list2d * list2d, DoubleList::list * open_list, DoubleList::list * closed_list, QImage & img)
{
    //qDebug() << "WYBIERAM NOWE ELEMENTY";
    DoubleList::list * new_elements_list = new DoubleList::list;
    DoubleList::pathElement * new_element;

    int base_x = element->x-1, base_y = element->y-1, counter = 1;
    bool in_open_list, in_closed_list;

    //qDebug() << "START";

    for (int i = -1; i < 2; i+= 1)             //  8 NASTĘPCÓW BADANEGO ELEMENTU
    {
        new_element = list2d->get_element(base_x-1, base_y+i);

        //qDebug() << new_element->x << ", " << new_element->y;

        //qDebug() << "OTWARTA: " << open_list->counter << ", ZAMKNIETA: " << closed_list->counter;


        in_open_list = open_list->in_list(new_element);
        //qDebug() << in_open_list;
        in_closed_list = closed_list->in_list(new_element);
        //qDebug() << in_closed_list;

        if (new_element->type == 0 && in_open_list == false && in_closed_list == false)
        {
            //qDebug() << "WYBRANO ELEMENT...";
            new_elements_list->add_element(new_element->type, new_element->x, new_element->y);
            img.setPixel(new_element->x-1, new_element->y-1, qRgba(30,255,0,255));
        }
        else
            //qDebug() << "POMINIETO: " << counter;

        counter += 1;
    }

    new_element = list2d->get_element(base_x, base_y-1);


    //qDebug() << new_element->x << ", " << new_element->y;

    in_open_list = open_list->in_list(new_element);
    //qDebug() << in_open_list;
    in_closed_list = closed_list->in_list(new_element);
    //qDebug() << in_closed_list;
    if (new_element->type == 0 && in_open_list == false && in_closed_list == false)
    {
        //qDebug() << "WYBRANO ELEMENT...";
        new_elements_list->add_element(new_element->type, new_element->x, new_element->y);
        img.setPixel(new_element->x-1, new_element->y-1, qRgba(30,255,0,255));
    }
    else
        //qDebug() << "POMINIETO: " << counter;

    counter += 1;

    new_element = list2d->get_element(base_x, base_y+1);


    //qDebug() << new_element->x << ", " << new_element->y;

    in_open_list = open_list->in_list(new_element);
    //qDebug() << in_open_list;
    in_closed_list = closed_list->in_list(new_element);
    //qDebug() << in_closed_list;
    if (new_element->type == 0 && in_open_list == false && in_closed_list == false)
    {
        //qDebug() << "WYBRANO ELEMENT...";
        new_elements_list->add_element(new_element->type, new_element->x, new_element->y);
        img.setPixel(new_element->x-1, new_element->y-1, qRgba(30,255,0,255));

    }
    else
        //qDebug() << "POMINIETO: " << counter;

    counter += 1;


    for (int i = -1; i < 2; i+= 1)
    {
        new_element = list2d->get_element(base_x+1, base_y+i);


        //qDebug() << new_element->x << ", " << new_element->y;

        in_open_list = open_list->in_list(new_element);
        //qDebug() << in_open_list;
        in_closed_list = closed_list->in_list(new_element);
        //qDebug() << in_closed_list;
        if (new_element->type == 0 && in_open_list == false && in_closed_list == false)
        {
            //qDebug() << "WYBRANO ELEMENT...";
            new_elements_list->add_element(new_element->type, new_element->x, new_element->y);
            //qDebug() << new_element->x << ", " << new_element->y;

            img.setPixel(new_element->x-1, new_element->y-1, qRgba(30,255,0,255));;

            //qDebug() << "OTWARTA: " <<open_list->counter;
            //qDebug() << "ZAMKNIETA: " << closed_list->counter;

        }
        else
            //qDebug() << "POMINIETO: " << counter;

        counter += 1;
    }

    return new_elements_list;
}


void Algorithm::new_elements_values(int start_x, int start_y, int end_x, int end_y, DoubleList::pathElement * last_element, DoubleList::list * new_elements_list, DoubleList::list * closed_list, DoubleList::list * open_list)
{
    //qDebug() << "NADAJE NOWYM ELEMENTOM WARTOSCI";

    if (new_elements_list->counter > 0)
    {
        DoubleList::pathElement * element = new_elements_list->head;

        if (closed_list->counter < 1)
        {
            while (element != new_elements_list->tail->next)
            {
                element->g = Algorithm::distance_between_points(start_x, start_y, element->x ,element->y);
                element->h = Algorithm::distance_between_points(element->x, element->y, end_x, end_y);
                element->f = element->g + element->h;

                //qDebug() << element->x << ", " << element->y << ": h = " << element->h <<": g = " << element->g << ", f = " << element->f;

                element = element->next;
            }
        }

        else
        {
            while (element != new_elements_list->tail->next)
            {
                //element->g = last_element->g + Algorithm::distance_between_points(last_element->x, last_element->y, element->x ,element->y);
                element->g = Algorithm::distance_between_points(start_x, start_y, element->x ,element->y);
                element->h = Algorithm::distance_between_points(element->x, element->y, end_x, end_y);
                element->f = element->g + element->h;

                //qDebug() << element->x << ", " << element->y << ": h = " << element->h <<": g = " << element->g << ", f = " << element->f;

                element = element->next;
            }
        }

        new_elements_list->merge_to_list(open_list);
    }
    else
        delete new_elements_list;
}


void Algorithm::path_elements_values(DoubleList::pathElement * element, int end_x, int end_y, DoubleList::list * new_elements_list, DoubleList::list * open_list)
{
    //qDebug() << "NADAJE NOWYM ELEMENTOM WARTOSCI";

    if (new_elements_list->counter > 0)
    {
        DoubleList::pathElement * _element = new_elements_list->head;

        while (_element != new_elements_list->tail->next)
        {
            _element->g = element->g + Algorithm::distance_between_points(element->x, element->y, _element->x, _element->y);
            _element->h = Algorithm::distance_between_points(_element->x, _element->y, end_x, end_y);
            _element->f = _element->g + _element->h;

            //qDebug() << _element->x << ", " << _element->y << ": h = " << _element->h <<": g = " << _element->g << ", f = " << _element->f;

            _element = _element->next;
        }

        new_elements_list->merge_to_list(open_list);
    }
    else
        delete new_elements_list;
}


void Algorithm::next_element(DoubleList::list * open_list, DoubleList::list * closed_list, QImage & img)
{

    /// SZUKANIE ELEMENTU O NAJMNIEJSZEJ WARTOSCI F Z LISTY OPEN_LIST

    //qDebug() << "WYBIERAM NAJLEPSZY ELEMENT";

    DoubleList::pathElement * element = open_list->head, * best_element = open_list->head;


    int lowest_f = element->f, lowest_h = element->h;

    element = element->next;

    while (element->next != 0)
    {
        if (element->f < lowest_f)  //  SZUKA NAJMNIEJSZEGO F POPRZEZ JEGO INDEKS
        {
            lowest_f = element->f;
            lowest_h = element->h;
            best_element = element;
        }

        else if (element->f == lowest_f)  //  SZUKA NAJMNIEJSZEGO F POPRZEZ JEGO INDEKS
        {
            if (element->h < lowest_h)
            {
                lowest_h = element->h;
                best_element = element;
            }

        }

        element = element->next;
    }

    //qDebug() << "BEST ELEMENT: " << best_element->x << ", " << best_element->y;


    closed_list->add_element(best_element->type, best_element->x, best_element->y);
    //qDebug() << "DODANO DO ZAMKNIETEJ LISTY";
    img.setPixel(best_element->x-1, best_element->y-1, qRgba(255,30,0,255));
    //qDebug() << "USUWANIE Z LISTY OTWARTEJ...";

    // bool best_in_open = open_list->in_list(best_element);

    //qDebug() << best_in_open;

    open_list->remove_element(best_element);

    //qDebug() << "WYBRANO NAJLEPSZY ELEMENT";

}


DoubleList::pathElement * Algorithm::most_promising_element(DoubleList::list * open_list)
{
    DoubleList::pathElement * element = open_list->head, * _element = element;

    DoubleList::pathElement * best = new DoubleList::pathElement;

    int least_f = element->f, least_h = element->h, _counter = 1;

    while (_counter < open_list->counter)
    {
        _element = _element->next;

        if (_element->f == least_f)
        {
            if (_element->h < least_h)
            {
                element = _element;
                least_h = _element->h;
            }
        }
        else if (_element->f < least_f)
        {
            element = _element;
            least_h = _element->h;
            least_f = _element->f;
        }

        _counter += 1;
    }

    best = element;
    open_list->unlink(element);

    return best;
}



DoubleList::list * Algorithm::path_successors(DoubleList::pathElement * element, DoubleList::list * closed_list, DoubleList::list * path, DoubleList::list2d * list2d)
{
    //qDebug() << "WYBIERAM NOWE ELEMENTY";
    DoubleList::list * new_elements_list = new DoubleList::list;
    DoubleList::pathElement * new_element;

    int base_x = element->x-1, base_y = element->y-1, counter = 1;
    bool in_closed_list, in_path;

    //qDebug() << "START";

    for (int i = -1; i < 2; i+= 1)             //  8 NASTĘPCÓW BADANEGO ELEMENTU
    {
        new_element = list2d->get_element(base_x-1, base_y+i);

        //qDebug() << counter << ": " << new_element->x << ", " << new_element->y;

        in_closed_list = closed_list->in_list(new_element);
        in_path = path->in_list(new_element);
        //qDebug() << in_closed_list << ", " << in_path;

        if (in_closed_list && in_path == false)
        {
            //qDebug() << "WYBRANO ELEMENT...";
            new_elements_list->add_element(new_element->type, new_element->x, new_element->y);
        }
        else
            //qDebug() << "POMINIETO: " << counter;

        counter += 1;
    }

    new_element = list2d->get_element(base_x, base_y-1);

    //qDebug() << counter << ": " << new_element->x << ", " << new_element->y;

    in_closed_list = closed_list->in_list(new_element);
    in_path = path->in_list(new_element);
    //qDebug() << in_closed_list << ", " << in_path;

    if (in_closed_list && in_path == false)
    {
        //qDebug() << "WYBRANO ELEMENT...";
        new_elements_list->add_element(new_element->type, new_element->x, new_element->y);
    }
    else
        //qDebug() << "POMINIETO: " << counter;

    counter += 1;

    new_element = list2d->get_element(base_x, base_y+1);


    //qDebug() << counter << ": " << new_element->x << ", " << new_element->y;

    in_closed_list = closed_list->in_list(new_element);
    in_path = path->in_list(new_element);
    //qDebug() << in_closed_list << ", " << in_path;

    if (in_closed_list && in_path == false)
    {
        //qDebug() << "WYBRANO ELEMENT...";
        new_elements_list->add_element(new_element->type, new_element->x, new_element->y);
    }
    else
        //qDebug() << "POMINIETO: " << counter;

    counter += 1;


    for (int i = -1; i < 2; i+= 1)
    {
        new_element = list2d->get_element(base_x+1, base_y+i);


        //qDebug() << counter << ": " << new_element->x << ", " << new_element->y;

        in_closed_list = closed_list->in_list(new_element);
        in_path = path->in_list(new_element);
        //qDebug() << in_closed_list << ", " << in_path;

        if (in_closed_list && in_path == false)
        {
            //qDebug() << "WYBRANO ELEMENT...";
            new_elements_list->add_element(new_element->type, new_element->x, new_element->y);
        }
        else
            //qDebug() << "POMINIETO: " << counter;

        counter += 1;
    }

    return new_elements_list;
}


void Algorithm::next_path_element(DoubleList::list * open_list, DoubleList::list * closed_list, QImage & img)
{

    /// SZUKANIE ELEMENTU O NAJMNIEJSZEJ WARTOSCI F Z LISTY OPEN_LIST

    //qDebug() << "WYBIERAM NAJLEPSZY ELEMENT";

    DoubleList::pathElement * element = open_list->head, * best_element = open_list->head;


    int lowest_f = element->f, lowest_h = element->h;

    element = element->next;

    while (element->next != 0)
    {
        if (element->f < lowest_f)  //  SZUKA NAJMNIEJSZEGO F POPRZEZ JEGO INDEKS
        {
            lowest_f = element->f;
            lowest_h = element->h;
            best_element = element;
        }

        else if (element->f == lowest_f)  //  SZUKA NAJMNIEJSZEGO F POPRZEZ JEGO INDEKS
        {
            if (element->h < lowest_h)
            {
                lowest_h = element->h;
                best_element = element;
            }

        }

        element = element->next;
    }

    //qDebug() << "BEST ELEMENT: " << best_element->x << ", " << best_element->y;


    closed_list->add_element(best_element->type, best_element->x, best_element->y);
    //qDebug() << "DODANO DO ZAMKNIETEJ LISTY";
    img.setPixel(best_element->x-1, best_element->y-1, qRgba(0,30,255,255));
    //qDebug() << "USUWANIE Z LISTY OTWARTEJ...";

    // bool best_in_open = open_list->in_list(best_element);

    //qDebug() << best_in_open;

    open_list->remove_element(best_element);

    //qDebug() << "WYBRANO NAJLEPSZY ELEMENT";

}


void Algorithm::draw_path(DoubleList::list *path, QImage & img)
{
    DoubleList::pathElement * element = path->head;

    if (path->counter == 0)
        qDebug() << "ok";

    else if (path->counter == 1)
    {
        img.setPixel(element->x-1, element->y-1, qRgba(0,30,255,255));
    }

    else
        while (element != path->tail->next)
        {
            img.setPixel(element->x-1, element->y-1, qRgba(0,30,255,255));
            element = element->next;
        }

}


void Algorithm::set_path(int start_x, int start_y, int end_x, int end_y, DoubleList::list *closed_list, DoubleList::list2d * list2d, QImage & img)
{
    DoubleList::list * path = new DoubleList::list;
    DoubleList::list * path_open_list = new DoubleList::list;
    DoubleList::pathElement * most_promising_element = new DoubleList::pathElement;

    int path_counter = 1;

    path->add_element(0, start_x, start_y);

    qDebug() << "STARTUJE Z PATHEM...";

    while(!(path->tail->x == end_x && path->tail->y == end_y))
    {
        if (path_open_list->counter > 1)
        {
            //qDebug() << "WYBIERAM NAJLEPSZY ELEMENT...";

            //qDebug() << path_open_list->counter;
            //path_open_list->show_list(path_open_list->counter);

            most_promising_element = Algorithm::most_promising_element(path_open_list);

            //qDebug() << "BEST: " << most_promising_element->x << ", " << most_promising_element->y;

            path->add_existing_element(most_promising_element);
            path->tail->f = most_promising_element->f;
        }
        else if (path_open_list->counter == 1)
        {
            //qDebug() << "WYBIERAM NAJLEPSZY ELEMENT...";

            //qDebug() << path_open_list->counter;
            //path_open_list->show_list(path_open_list->counter);

            most_promising_element = path_open_list->head;

            //Debug() << "BEST: " << most_promising_element->x << ", " << most_promising_element->y;

            path->add_existing_element(most_promising_element);
            path->tail->f = most_promising_element->f;
        }
        else
        {
            //qDebug() << "WYBIERAM OGON";
            most_promising_element = path->tail;
        }

        qDebug() << "ROBIE PATHA: " << path_counter;
        qDebug() << path->tail->x << ", " << path->tail->y;


        DoubleList::list * successors = Algorithm::path_successors(most_promising_element, closed_list, path, list2d);
        Algorithm::path_elements_values(most_promising_element, end_x, end_y, successors, path_open_list);
        path_counter += 1;
    }



    qDebug() << "PATH ZROBIONY";

    Algorithm::draw_path(path, img);

}
