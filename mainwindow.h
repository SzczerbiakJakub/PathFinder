#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QGraphicsScene>
#include <QTextStream>
#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include "algorithm.h"
#include "list.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

///     Klasa odpowidająca za odczyt i komunikację z programem.
///
///     Ta klasa umożliwia korzystanie z aplikacji, wczytywanie mapy, zwizualizowanie jej na ekranie oraz operowanie na elementach wczytanej mapy.
///     Wykorzystuje klasy Algorithm() oraz List().

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ///     Określa okno aplikacji.
    MainWindow(QWidget *parent = nullptr);
    ///     Konstruktor okna aplikacji.
    ~MainWindow();

    ///     Funkcja wczytująca mapę do programu.
    ///
    ///     Funkcja przetwarza tylko pliki o rozszerzeniu .map, przy czym zawartość tych plików musi zawierać informacje o wysokości oraz szerokości mapy
    ///     w pierwszych 7 linijkach tekstu. Od 7-mej linijki funkcja tworzy listę list (których ilość odpowiada wysokości mapy), a każda kolejna lista począwszy od początkowej
    ///     aż do końcowej zawiera w sumie tyle elementów, ile określa wartość szerokości mapy. Wartości dla domyślnych map wynoszą wysokość = 512 oraz szerokość = 512, i dla takich
    ///     rozmiarów mapy funkcja będzie działać prawidłowo. Wczytana mapa jest nastepnie wizualizowana przy pomocy pixmapy. Elementy, po których można się poruszać, zaznaczone są
    ///     na szaro, natomiast przeszkody przedstawione są na czarno.
    void loadMap();

    ///     Zmienna określająca nazwę pliku zawierającego mapę.
    QString fileName;

    ///     Zmienna określająca pixmapę.
    QPixmap pm;

    QImage
    ///     Zmienna zawierająca zbiór pikseli przedstawiających punkt końcowy (punkt B).
    redDot(),

    ///     Zmienna zawierająca zbiór pikseli przedstawiających punkt początkowy (punkt A).
    blueDot(),

    ///     Zmienna zawierająca zbiór pikseli przedstawiających ścieżkę.
    pathImage();


    DoubleList::list2d
    ///     Zmienna określająca liste list zawierającą elementy wczytanej mapy.
    * lista2d;

    bool
    ///     Zmienna określająca prawdziwość zdarzenia, jakim jest: czy mapa została wczytana?
    ///
    ///     Jej domyślna wartość wynosi false.
    mapLoaded = false,

    ///     Zmienna określająca prawdziwość zdarzenia, jakim jest: czy koordynaty punktu A zostały określone w sposób poprawny?
    ///
    ///     Jej domyślna wartość wynosi true.
    wrongPointA = true,

    ///     Zmienna określająca prawdziwość zdarzenia, jakim jest: czy koordynaty punktu A zostały określone w sposób poprawny?
    ///
    ///     Jej domyślna wartość wynosi true.
    wrongPointB = true;

    ///     Funkcja zwracająca koordynat poziomy punktu A.
    ///
    ///     @return     Zwraca koordynat poziomy punktu A.
    int get_point_a_X();

    ///     Funkcja zwracająca koordynat pionowy punktu A.
    ///
    ///     @return     Zwraca koordynat pionowy punktu A.
    int get_point_a_Y();

    ///     Funkcja zwracająca koordynat poziomy punktu B.
    ///
    ///     @return     Zwraca koordynat poziomy punktu B.
    int get_point_b_X();

    ///     Funkcja zwracająca koordynat pionowy punktu B.
    ///
    ///     @return     Zwraca koordynat pionowy punktu B.
    int get_point_b_Y();

    ///     Funkcja zwracająca obraz ścieżki.
    QImage search_for_path();

    ///     Funkcja zwracająca element mapy określony przez jego koordynaty.
    ///
    ///     @param x    Określa pozycje pionową szukanego elementu.
    ///     @param y    Określa pozycje poziomą szukanego elementu.
    ///     @return     Zwraca element mapy zależnie od jego koordynatów.
    DoubleList::pathElement get_path_element(int x, int y);

private slots:
    ///     Funkcja określająca co ma się wydarzyć po naciśnięciu przycisku "Load file".
    ///
    ///     Funkcja umożliwia wczytanie mapy z pliku do programu. Wywołuje funkcje loadMap().
    void on_loadFile_clicked();

    ///     Funkcja określająca co ma się wydarzyć po naciśnięciu na przycisk "Close" i zwolnieniu LPM.
    ///
    ///     Funkcja zamyka program.
    void on_close_released();

    ///     Funkcja określająca co ma się wydarzyć po naciśnięciu przycisku "pushButton".
    ///
    ///     Funkcja umożliwia wyświetlenie w konsoli przykładowego tekstu, ma zastosowanie testowe.
    void on_pushButton_clicked();

    //void on_point_a_x_textEdited(const QString &arg1);

    ///     Funkcja określająca co ma się wydarzyć po skończeniu edycji pola zawierającego koordynaty punktu B.
    ///
    ///     Zawartość pola zostanie usunięta w przypadku określenia koordynatów w zły sposób. Koordynaty należy oddzielić przecinkiem,
    ///     a ich wartość powinna mieścić się w granicach wysokości i szerokości mapy (domyślnie: [1, 512], [1, 512]);
    void on_point_b_coords_editingFinished();

    ///     Funkcja określająca co ma się wydarzyć po skończeniu edycji pola zawierającego koordynaty punktu A.
    ///
    ///     Zawartość pola zostanie usunięta w przypadku określenia koordynatów w zły sposób. Koordynaty należy oddzielić przecinkiem,
    ///     a ich wartość powinna mieścić się w granicach wysokości i szerokości mapy (domyślnie: [1, 512], [1, 512]);
    void on_point_a_coords_editingFinished();

    ///     Funkcja określająca co ma się wydarzyć po naciśnięciu przycisku "Distance".
    ///
    ///     Wykorzystuje funckcje distance_between_points(), zwraca w konsoli dystans między określonymi poprzez koordynaty
    ///     punktem początkowym oraz punktem końcowym.
    void on_distance_clicked();

    ///     Funkcja określająca co ma się wydarzyć po naciśnięciu przycisku "Show path".
    ///
    ///     Po naciśnięciu program zacznie obliczać ścieżkę, by nastepnie przedstawić ją w postacji zielonego koloru na wczytanej mapie.
    void on_show_path_clicked();

    ///     Funkcja określająca co ma się wydarzyć po naciśnięciu przycisku "Search for path".
    ///
    ///     Po naciśnięciu program zacznie obliczać ścieżkę.
    void on_search_for_path_clicked();

    void on_new_algorithm_clicked();

private:
    Ui::MainWindow *ui;

    ///     Zmienna określająca przestrzeń zajętą przez mapę przedstawioną w oknie programu.
    QGraphicsScene * map;

    ///     Funkcja ilustrująca punkt początkowy (punkt A) na mapie.
    ///
    ///     @param x    Określa pozycje poziomą punktu A.
    ///     @param y    Określa pozycje pionową punktu A.
    bool set_point_a(int x, int y, DoubleList::list2d * list2d);

    ///     Funkcja ilustrująca punkt końcowy (punkt B) na mapie.
    ///
    ///     @param x    Określa pozycje poziomą punktu B.
    ///     @param y    Określa pozycje pionową punktu B.
    bool set_point_b(int x, int y, DoubleList::list2d * list2d);
};
#endif // MAINWINDOW_H
