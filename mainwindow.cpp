#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->map->setStyleSheet("border: 1px solid black");
    ui->map_name->setStyleSheet("border: 1px solid black");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadFile_clicked()
{
    loadMap();
}

void MainWindow::loadMap()
{
    MainWindow::fileName = QFileDialog::getOpenFileName(this,
            tr("Wczytaj mapę"), "",
            tr("Map Format (*.map)"));

    if(fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, tr("Nie można otworzyć pliku."),
                         file.errorString());
            return;
        }

        QTextStream fileStream(&file);
        QString a;
        while (a != "height")
        {
            fileStream >> a;
            qDebug() << a;
        }
        int height, width;
        fileStream >> height;
        fileStream >> a;
        fileStream >> width;

        QImage obraz(512, 512, QImage::Format_RGB32), redDot(5,5,QImage::Format_ARGB32),
                blueDot(5,5,QImage::Format_ARGB32);
        QRgb lightGray, black;
        //int mapa[512][512];
        lightGray = qRgb(200,200,200);
        black = qRgb(0,0,0);
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if(i == 0 || i == 4 || j == 0 || j == 4 || (i == 2 && j == 2))
                {
                    redDot.setPixel(j, i, qRgb(255,0,0));
                    blueDot.setPixel(j, i, qRgb(0,0,255));
                }
                else
                {
                    redDot.setPixel(j, i, qRgb(255,255,255));
                    blueDot.setPixel(j, i, qRgb(255,255,255));
                }
            }
        }
        lista2d = new DoubleList::list2d;

        lista2d->create_lists(height, lista2d);
        DoubleList::list * biezacaLista = lista2d->listHead;
        qDebug() << lista2d->listCounter;
        int rowNumber = 1;

        while (!(fileStream.atEnd()))
        {
            fileStream >> a;
            if (a.size() == width)
            {
                for (int i = 0; i < width; i++)
                {
                    if (a[i] == '@')
                    {
                        biezacaLista->add_element(1, i+1, rowNumber);
                        obraz.setPixel(i, rowNumber - 1, black);
                    }
                    else if (a[i] == '.')
                    {
                        biezacaLista->add_element(0, i+1, rowNumber);
                        obraz.setPixel(i, rowNumber - 1, lightGray);
                    }
                }
                rowNumber ++;
                biezacaLista = biezacaLista->nextList;
            }
        }

        //lista2d->show_all();  // <- po usunięciu pierwszych dwóch slashy można zobaczyć wczytaną mapę jako wypełnioną listę 2D

        qDebug() << rowNumber;
        qDebug() << width << height;

        pm = QPixmap::fromImage(obraz);
        ui->map->setPixmap(pm);
        pm = QPixmap::fromImage(blueDot);
        ui->point_a->setPixmap(pm);
        pm = QPixmap::fromImage(redDot);
        ui->point_b->setPixmap(pm);
        ui->map_name->setText(fileName);
        mapLoaded = true;

        biezacaLista = lista2d->listHead;
        qDebug() << biezacaLista->head->x << "\t" << biezacaLista->head->y;
        biezacaLista = lista2d->listTail;
        qDebug() << biezacaLista->tail->x << "\t" << biezacaLista->tail->y;

        /*DoubleList::list2d *map2 = new DoubleList::list2d;
        for (int i = 0; i < height; i++)
        {
            map2->add_list();
        }
        DoubleList::list * row = new DoubleList::list;
        map2->firstList = row;
        int rowNumber = 0;*/

        /*int ** mapa = new int * [height];
        for (int i = 0; i < height; i++)
        {
            mapa[i] = new int [width];
        }*/

        /*while (!fileStream.atEnd())         //  TU SIE WCZYTUJE MAPA JAK COS
        {
            rowNumber ++;
            fileStream >> a;
            for (int i = 0; i < a.size(); i++)
            {
                if (a[i] == '@')
                {
                    mapa[i][rowNumber] = 1;
                    obraz.setPixel(i, rowNumber - 1, black);
                }
                else if (a[i] == '.')
                {
                    mapa[i][rowNumber] = 0;
                    obraz.setPixel(i, rowNumber - 1, lightGray);
                }
            }*/

            /*for (int i = 0; i < a.size(); i++)
            {
                if (a[i] == '@')
                {
                    row->add_element(1);
                    obraz.setPixel(i, rowNumber, black);
                }
                else if (a[i] == '.')
                {
                    row->add_element(0);
                    obraz.setPixel(i, rowNumber, lightGray);
                }

            }
            row = row->nextList;
            rowNumber ++;*/

            /*for (int i = 0; i < height; i++)
            {
                fileStream >> a;
                for (int j = 0; j < a.size(); j++)
                {
                    if (a[j] == '@')
                    {
                        mapa[j][i] = 1;
                    }
                    else if (a[j] == '.')
                    {
                        mapa[j][i] = 0;
                    }
                    else
                    {
                        mapa[j][i] = -1;
                    }
                }
            }*/
        }

            /*for (int i = 0; i < 512; i++)
            {
                for (int j = 0; j < 512; j++)
                {
                    if (mapa[j][i] == 1)
                        obraz.setPixel(j, i, black);
                    else if (mapa[j][i] == 0)
                        obraz.setPixel(j, i, lightGray);
                }
            } */

}

void MainWindow::on_close_released()
{
    MainWindow::close();
}


void MainWindow::on_pushButton_clicked()
{
    qDebug() << "This will output to the spawned console!";
}


/*void MainWindow::on_point_a_x_textEdited(const QString &arg1)
{
    int arg = arg1.toInt();
    qDebug() << arg;
    if (!(arg >=0))
    {
        ui->point_a_x->text() = "0";
    }
    else if (!(arg <= 512))
    {
        ui->point_a_x->text() = "512";
    }
}*/

void MainWindow::on_point_b_coords_editingFinished()
{
    QString value = ui->point_b_coords->text();
    int counter = 0;
    for (int i = 0; i < value.size(); i++)
    {
        if(value[i] == ',')
            counter ++;
    }
    if (counter != 1 || mapLoaded == false)
    {
        ui->point_b_coords->setText("0,0");
        wrongPointB = true;
        return;
    }

    QStringList coords = ui->point_b_coords->text().split(',');
    int x = coords[0].toInt(), y = coords[1].toInt();

    qDebug() << x << "  " << y;
    wrongPointB = set_point_b(x,y, lista2d);

    if (wrongPointB == true)
    {
        ui->point_b_coords->setText("0,0");
    }
}


void MainWindow::on_point_a_coords_editingFinished()
{
    QString value = ui->point_a_coords->text();
    int counter = 0;

    for (int i = 0; i < value.size(); i++)
    {
        if(value[i] == ',')
            counter ++;
    }

    if (counter != 1 || mapLoaded == false)
    {
        ui->point_a_coords->setText("0,0");
        return;
    }

    QStringList coords = ui->point_a_coords->text().split(',');
    int x = coords[0].toInt(), y = coords[1].toInt();

    qDebug() << x << "  " << y;
    wrongPointA = set_point_a(x,y, lista2d);

    if (wrongPointA == true)
    {
        ui->point_a_coords->setText("0,0");
    }
}

int MainWindow::get_point_a_X()
{
    QStringList coords = ui->point_a_coords->text().split(',');
    return coords[0].toInt();
}

int MainWindow::get_point_a_Y()
{
    QStringList coords = ui->point_a_coords->text().split(',');
    return coords[1].toInt();
}

int MainWindow::get_point_b_X()
{
    QStringList coords = ui->point_b_coords->text().split(',');
    return coords[0].toInt();
}

int MainWindow::get_point_b_Y()
{
    QStringList coords = ui->point_b_coords->text().split(',');
    return coords[1].toInt();
}

bool MainWindow::set_point_a(int x, int y, DoubleList::list2d * list2d)
{
    if(mapLoaded == false)
    {
        return true;
    }

    int type = list2d->check_type(x,y,list2d);

    if (type != 0)
        return true;
    else
    {
        ui->point_a->setGeometry(x,y+52,5,5);
        return false;
    }
}

bool MainWindow::set_point_b(int x, int y, DoubleList::list2d * list2d)
{
    if(mapLoaded == false)
    {
        return true;
    }

    int type = list2d->check_type(x,y,list2d);

    if (type != 0)
        return true;
    else
    {
        ui->point_b->setGeometry(x,y+52,5,5);
        return false;
    }
}


void MainWindow::on_distance_clicked()
{
    QString pointA = ui->point_a_coords->text(), pointB = ui->point_b_coords->text();

    if(!(wrongPointA || wrongPointB))
    {
        QStringList coordsA = pointA.split(','), coordsB = pointB.split(',');
        int xa = coordsA[0].toInt(), ya = coordsA[1].toInt(),
                xb = coordsB[0].toInt(), yb = coordsB[1].toInt();

        //double distance = sqrt(x*x + y*y);       // EUCLIDEAN DISTANCE

        //int distance = abs(xb - xa) + abs(yb - ya);     // MANHATTAN DISTANCE

        qDebug() << Algorithm::distance_between_points(xa,ya,xb,yb);
    }
}


void MainWindow::on_show_path_clicked()
{
    if (wrongPointA || wrongPointB || mapLoaded == false)
        return;

    //QStringList coordsA = ui->point_a_coords->text().split(','), coordsB = ui->point_b_coords->text().split(',');;

    pm = Algorithm::create_path(search_for_path());

    ui->path->setPixmap(pm);
}




void MainWindow::on_search_for_path_clicked()
{
    search_for_path();

    // searchList->add_element(0,get_point_b_X(),get_point_b_Y()); //  PUNKT KONCOWY




}

QImage MainWindow::search_for_path()
{
    QImage pathImage(512, 512, QImage::Format_ARGB32);
    pathImage.fill(qRgba(0,0,0,0));

    DoubleList::list2d * paths = new DoubleList::list2d;
    paths->add_list();

    paths = Algorithm::find_ways_2(get_point_a_X(), get_point_a_Y(), get_point_b_X(),
                                        get_point_b_Y(), 1, lista2d, paths);

    DoubleList::pathElement * element;
    DoubleList::list * path = paths->listHead;
    path = paths->listHead;

    element = path->head;
    for (int i = 0; i < paths->listHead->counter; i++)
    {
        pathImage.setPixel(element->x, element->y, qRgba(30,255,0,255));
        element = element->next;
    }

    if (paths->listCounter > 1)
    {
        for (int j = 2; j <= paths->listCounter; j++)
        {
            path = path->nextList;
            {
                element = path->head;
                for (int i = 0; i < path->counter; i++)
                {
                    pathImage.setPixel(element->x, element->y, qRgba(30,255,0,255));
                    element = element->next;
                }
            }
        }
    }

    qDebug() << "GOTOWA SCIEZKA: ";
    //paths->show_all();

    return pathImage;

}


void MainWindow::on_new_algorithm_clicked()
{
    Algorithm * algorithm = new Algorithm();

    QImage pathImage(512, 512, QImage::Format_ARGB32);
    pathImage.fill(qRgba(0,0,0,0));

    DoubleList::list * open_list = new DoubleList::list;
    DoubleList::list * closed_list = new DoubleList::list;

    closed_list->add_element(0, get_point_a_X(), get_point_a_Y());

    int counter = 1, counter_threshold = 5500;

    bool no_path_available = false;

    while (/*counter <= counter_threshold &&*/ !(closed_list->tail->x == get_point_b_X() && closed_list->tail->y == get_point_b_Y()) && !(counter > 2 && open_list->counter < 1))
    {
        //qDebug() << "KROK NR " << counter;

        DoubleList::list * new_elements_list = algorithm->new_elements(closed_list->tail, lista2d, open_list, closed_list, pathImage);
        //qDebug() << "NOWE ELEMENTY POSZLY";
        algorithm->new_elements_values(get_point_a_X(), get_point_a_Y(), get_point_b_X(), get_point_b_Y(), closed_list->tail, new_elements_list, closed_list, open_list);
        //qDebug() << "ICH WARTOSCI TEZ";
        algorithm->next_element(open_list, closed_list, pathImage);
        //qDebug() << "WYBRANO NOWY ELEMENT - POSZLO";

        pm = Algorithm::create_path(pathImage);

        //qDebug() << "STAWIAM PIXMAPE...";
        ui->path->setPixmap(pm);
        //qDebug() << "USTAWIONO PIXMAPE";

        //open_list->show_list(open_list->counter);

        qDebug() << counter;
        counter += 1;

        //  DODAJ MULTITHREADING ZEBY WYSWIETLALO PO KOLEI KROKI
    }

    qDebug() << "UKONCZONO";

    qDebug() << closed_list->counter << ",      " << open_list->counter;


    if (!(closed_list->tail->x == get_point_b_X() && closed_list->tail->y == get_point_b_Y()))
        no_path_available = true;

    if (no_path_available)
        qDebug() << "NIE MA SCIEZKI";
    else
    {
        DoubleList::list * path_1 = algorithm->set_path(get_point_b_X(), get_point_b_Y(), get_point_a_X(), get_point_a_Y(), closed_list, lista2d, pathImage, 0, 30, 255);

        pm = Algorithm::create_path(pathImage);

        //qDebug() << "STAWIAM PIXMAPE...";
        ui->path->setPixmap(pm);

        qDebug() << "GOTOWA SCIEZKA: ";
        //closed_list->show_list(closed_list->counter);

        /*
        DoubleList::list * path_2 = algorithm->set_path(get_point_a_X(), get_point_a_Y(), get_point_b_X(), get_point_b_Y(), path_1, lista2d, pathImage, 255, 30, 255);

        pm = Algorithm::create_path(pathImage);

        //qDebug() << "STAWIAM PIXMAPE...";
        ui->path->setPixmap(pm);


        qDebug() << "GOTOWA SCIEZKA 2: ";
        */

        qDebug() << "FILTROWANIE...";
        DoubleList::pathElement * starting_element = path_1->head, * ending_element = path_1->tail;
        DoubleList::list * filtrated_path = new DoubleList::list, * path_2;

        do
        {
            path_2 = algorithm->shortest_path_possible(starting_element, lista2d, ending_element);

            if (path_2 != 0)
            {
                starting_element = ending_element;
                ending_element = path_1->tail;
                algorithm->draw_path(path_2, pathImage, 255, 255, 0);
                pm = Algorithm::create_path(pathImage);
                ui->path->setPixmap(pm);

                if (!(path_2->tail->x == path_1->tail->x && path_2->tail->y == path_1->tail->y))
                    path_2 = 0;

            }
            else
            {
                ending_element = ending_element->prev;
            }
        }
        while (path_2 == 0);


         qDebug() << "PRZEFILTROWANO";

    }



    delete algorithm;

}

