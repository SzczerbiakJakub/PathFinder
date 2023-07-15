#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <QDebug>


///     Klasa odpowiedzialna za przechowywanie informacji.
///
///     Klasa zawiera w sobie struktury danych, które określają listy list, listy oraz elementy tworzące listę.

class DoubleList
{
public:

    ///     Struktura danych opisująca poszczególny element zawarty w liście.
    ///
    ///     Struktura zawiera konstruktor oraz takie informacje, jak: koordynaty elementu mapy, typ elementu,
    ///     wartość funkcji: f, g i h dla poszczególnego elementu oraz wskaźnik na następny/poprzedni element.
    struct pathElement
    {
        int
        ///     Zmienna przechowująca wartość określającą typ elementu.
        type,

        ///     Zmienna przechowująca odległość elementu względem osi X.
        x,

        ///     Zmienna przechowująca odległość elementu względem osi Y.
        y;

        double
        ///     Zmienna przechowująca wartość funkcji f dla danego elementu.
        f,

        ///     Zmienna przechowująca wartość funkcji g dla danego elementu.
        g,

        ///     Zmienna przechowująca wartość funkcji h dla danego elementu.
        h;

        ///     Konstruktor structury pathElement
        pathElement();

        pathElement
        ///     Wskaźnik na następny element.
        * next,

        ///     Wskaźnik na poprzedni element.
        * prev;
    };

    ///     Struktura danych przedstawiająca listę dwukierunkową.
    ///
    ///     Struktura zawiera konstruktor oraz takie informacje, jak: wielkość listy,
    ///     wskażnik na pierwszy/ostatni element listy, wskaźnik na poprzednią/następną w kolejności listę
    ///     oraz funkcje służące do manipulacji zawartością listy.
    struct list
    {
        pathElement
        ///     Wskazuje na pierwszy element listy dwukierunkowej.
        * head,

        ///     Wskazuje na ostatni element listy dwukierunkowej.
        * tail;

        ///     Zmienna przechowująca aktualną wielkość listy dwukierunkowej.
        int counter;

        ///     Funkcja odpowiedzialna za przydzielenie do listy elementu o określonych parametrach.
        ///
        ///     @param elementType      Określa typ elementu (1 - przeszkoda, 0 - nie jest przeszkodą).
        ///     @param elementX     Określa pozycję elementu na osi X.
        ///     @param elementY     Określa pozycję elementu na osi Y.
        void add_element(int elementType, int elementX, int elementY);

        void add_existing_element(DoubleList::pathElement * element);

        ///     Funkcja służąca do usuwania ostatniego elementu listy.
        void remove_last_element();

        void remove_element(pathElement * element);

        void remove_element_at(pathElement * element, int index);

        void unlink(pathElement * element);

        DoubleList::pathElement * get_element(int index);

        int get_index(DoubleList::pathElement * element);

        ///     Funkcja służąca do wywołania określonej liczby elementów listy, rozpoczyna od pierwszego elementu.
        ///     @param howMany  Określa ilość elementów, które chcemy wyświetlić.
        void show_list(int howMany);

        bool in_list(DoubleList::pathElement * element);

        void merge_to_list(list * main_list);

        list
        ///     Wskazuje na następną w kolejności listę.
        * nextList,

        ///     Wskazuje na poprzednią w kolejności listę.
        * prevList;

        /// Konstruktor struktury list
        list();
    };

    ///     Struktura danych przedstawiająca dwukierunkową listę dwukierunkowych list.
    ///
    ///     Struktura zawiera konstruktor oraz takie informacje, jak: wielkość listy list,
    ///     wskażnik na pierwszą/ostatnią listę oraz funkcje służące do manipulacji zawartością listy list.
    struct list2d
    {
        list
        ///     Wskaźnik na pierwszą w kolejności listę zawartą w liście list.
        * listHead,

        ///     Wskaźnik na ostatnią w kolejności listę zawartą w liście list.
        * listTail,

        ///     Wskaźnik na obecną listę.
        *currentList;

        int
        ///     Zmienna określająca aktualną wielkość listy list (dotyczy ilości list, nie elementów).
        listCounter;

        ///     Funkcja przypisująca nową listę do końca listy list.
        void add_list();

        ///     Funkcja tworząca określoną ilość list przynależnych do danej listy list.
        ///
        ///     @param quantity     Określa ilość list, które należy utworzyć.
        ///     @param nazwa        Określa listę list, której ma dotyczyć powyższa operacja.
        void create_lists(int quantity, list2d * nazwa);

        ///     Funkcja usuwająca ostatnią listę w danej liście list.
        void remove_list();

        ///     Funckja wyświetlająca zawartość określonej ilości list zawartej w liście, począwszy od początkowej listy.
        ///
        ///     @param howMany  Określa ilość list do wyświetlenia.
        void show_list_2d(int howMany);

        ///     Funkcja wyświetlająca zawartość całej listy list.
        void show_all();

        ///     Funkcja zwracająca typ elementu zawartego w liście list.
        ///
        ///     @param x    Określa pozycję danego elementu względem osi X (rownoznaczne z pozycją elementu na danej liście).
        ///     @param y    Określa pozycję danego elementu względem osi Y (rownoznaczne z pozycją listy na danej liście list).
        ///     @param nazwa    Określa listę list, dla której wywoływana jest funkcja.
        ///
        ///     @return     Zwraca typ elementu określonego parametrami w funkcji.
        int check_type(int x, int y, list2d * nazwa);

        DoubleList::pathElement * get_element(int x, int y);

        ///     Konstruktor struktury list2d.
        list2d();
    };

};

#endif // LIST_H
