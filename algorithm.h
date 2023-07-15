#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QDebug>
#include <QString>
#include <cmath>
#include <iostream>
#include <iomanip>
#include "mainwindow.h"
#include "list.h"

namespace std { class Algorithm; };


///     Klasa stanowiąca serce całego programu.
///
///     Klasa zawiera funkcje odpowiadające za wyznaczanie ścieżki z założonego punktu początkowego do założonego końca,
///     obliczanie potencjalnie najkrótszej odległości między dwoma punktami, omijanie napotkanej przeszkody po jej obwodzie.

class Algorithm
{
public:


    ///     Funkcja zwracająca dystans pomiędzy dwoma punktami mapy.
    ///
    ///     Funkcja ta wykorzystuje heurystykę euklidesową, tzn. uwzględnia ruch w ośmiu kierunkach (góra, dół, lewo, prawo i na ukos).
    ///     Każdy ruch w pionie lub poziomie oznacza koszt = 2, natomiast ruch ukośny oznacza koszt = 3. Dzięki takiemu rozwiązaniu algorytm
    ///     wybiera ruch ukośny za każdym razem, kiedy jest to możliwe (ze względu na mniejszy dystans, który w ten sposób pokonuje. Koszt dotarcia
    ///     do tego miejsca z wykorzystaniem tylko pionowego i poziomego ruchu wynosiłby 4 jednostki, natomiast ruch ukośny kosztuje 3 jednostki,
    ///     co jest wartością mniejszą - algorytm preferuje krótszą ścieżkę).
    ///
    ///     @param x1   Określa poziomy koordynat początkowego punktu.
    ///     @param y1   Określa pionowy koordynat początkowego punktu.
    ///     @param x2   Określa poziomy koordynat punktu końcowego.
    ///     @param y2   Określa pionowy koordynat punktu końcowego
    ///
    ///     @return     Zwraca odległość między określonymi przez parametry punktami.
    static double distance_between_points(int x1, int y1, int x2, int y2);

    ///     Funkcja ilustrująca wyznaczoną scieżkę.
    ///
    ///     Funkcja wykorzystuje elementy gotowej ścieżki (która jest listą kolejnych możliwie najkorzystniejszych ruchów z punktu A do punktu B)
    ///     do zilustrowania jej na wczytanej mapie. W tym celu tworzy przezroczystą pixmapę o wielkości wczytanej w programie mapy, a następnie
    ///     nadaje odpowiednim pikselom zielony kolor oraz nieprzezroczystość - w ten sposób tworzy się ilustracja ścieżki.
    ///
    ///     @param image    Wskazuje na obraz, na którym funkcja ma wykonać operację.
    ///
    ///     @return     Zwraca pixmapę ścieżki na podstawie jej elementów i określonych przez nie parametrów.
    static QPixmap create_path(QImage image);


    ///     Funkcja zwracająca listę list możliwych najkorzystniejszych ścieżek z punktu A do punktu B.
    ///
    ///     Funkcja obliczająca kolejny mozliwie najkorzystniejszy krok w ścieżce. Bazuje na algorytmie A*. Na potrzeby programu określone zostały dodatkowe warunki, takie jak poruszanie się po obwodzie
    ///     napotkanej przeszkody w celu jej ominięcia czy też uwzględnienie poprzednich elementów ścieżki i wykluczenie ich z możliwych
    ///     kroków (dla zapewnienia niepowtarzalności elementów w ścieżce). W ten sposób określany jest następny możliwie najkorzystniejszy krok.
    ///     Utworzona ścieżka zwracana jest w postaci listy list (może być ich kilka, stąd lista list).
    ///
    ///     @param x1       Określa poziomy koordynat punktu A.
    ///     @param y1       Określa pionowy koordynat punktu A.
    ///     @param x2       Określa poziomy koordynat punktu B.
    ///     @param y2       Określa pionowy koordynat punktu B.
    ///     @param radius   Określa różnicę elementów pomiędzy każdym nastepnym elementem listy (standartowa wartość = 1).
    ///     @param list2d   Określa mapę, na której funkcja ma wyznaczyć ścieżkę.
    ///     @param paths    Określa listę list, którą funkcja zaktualizuję o gotowe kroki.
    ///
    ///     @return     Zwraca wyznaczone przez funkcje ścieżki w postaci listy list.
    static DoubleList::list2d * find_ways(int x1, int y1, int x2, int y2, int radius, DoubleList::list2d * list2d, DoubleList::list2d * paths);


    static DoubleList::list2d * find_ways_2(int x1, int y1, int x2, int y2, int radius, DoubleList::list2d * list2d, DoubleList::list2d * paths);


    static bool build_path_element(int x1, int y1, int x2, int y2, int radius, DoubleList::list * path, DoubleList::list2d * list2d,
                                   DoubleList::list2d * paths, int mode);

    ///     Funkcja wyznaczająca element nienależący do ścieżki ani niebędący ścianą.
    ///
    ///     Ze względu na 8 możliwych kroków w każdym miejscu mapy, funkcja analizuje je w sposób zgodny ze wskazówkami zegara (stąd nazwa clockwise).
    ///     Kolejność ta jest niezwykle ważna, bo pozwala kontrolować położenie każdego kolejnego elementu oraz wyznaczenie położenia tego jednego,
    ///     który będzie następnym elementem ścieżki. Funkcja wywoływana jest w momencie napotkania przeszkody, służy do ominięcia jej po obwodzie.
    ///
    ///     @param wall     Określa element, który został rozpoznany jako ściana.
    ///     @param succesors    Określa listę wszystkich ośmiu kroków, włącznie ze ścianami i powtarzającymi się elementami w ścieżce.
    ///     @param elements     Określa dwa elementy (lewy i prawy względem ostatniego elementu ścieżki), które następnie dodaje do końca ścieżki zgodnie z założonymi warunkami.
    static void nearest_space_clockwise(DoubleList::pathElement * wall,
                                        DoubleList::list * succesors, DoubleList::list * elements/*, DoubleList::list2d * paths*/);

    ///     Zmienna ilustrująca ścieżkę.
    ///
    ///


    DoubleList::list * new_elements(DoubleList::pathElement * element, DoubleList::list2d * list2d, DoubleList::list * open_list, DoubleList::list * closed_list, QImage & img);

    void new_elements_values(int start_x, int start_y, int end_x, int end_y, DoubleList::pathElement * last_element, DoubleList::list * elements_list, DoubleList::list * closed_list, DoubleList::list * open_list);

    void next_element(DoubleList::list * open_list, DoubleList::list * closed_list, QImage & img);

    static DoubleList::pathElement * most_promising_element(DoubleList::list * open_list);

    DoubleList::list * path_successors(DoubleList::pathElement * element, DoubleList::list * closed_list, DoubleList::list * path, DoubleList::list2d * list2d);

    void next_path_element(DoubleList::list * open_list, DoubleList::list * closed_list, QImage & img);

    void path_elements_values(DoubleList::pathElement * element, int end_x, int end_y, DoubleList::list * elements_list, DoubleList::list * open_list);

    void set_path(int start_x, int start_y, int end_x, int end_y, DoubleList::list * closed_list, DoubleList::list2d * list2d, QImage &img);

    void draw_path(DoubleList::list * path, QImage & img);


    QImage pathImage;

    ///     Konstruktor klasy Algorithm.
    Algorithm();

};

#endif // ALGORITHM_H
