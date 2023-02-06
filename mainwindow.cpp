#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsItem>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <random>
#include <QAction>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <fstream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), pocetBodu(0), pocetCentroidu(0), velikost(10)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup();
    setupActions();
}

MainWindow::~MainWindow()
{
    delete ui;

    for(int i = 0; i < body.length(); i++)
    {
        delete body.takeAt(i);
        pocetBodu--;
    }
    body.clear();

    for(int i = 0; i < centroidy.length(); i++)
    {
        delete centroidy.takeAt(i);
        pocetCentroidu--;
    }
    centroidy.clear();

    for(int i = 0; i < spojnice.length(); i++)
    {
        delete spojnice.takeAt(i);
    }
    spojnice.clear();

    delete scena;
}



void MainWindow::setup()
{
      scena = new QGraphicsScene(0,0,this->width()-22+2, this->height()-100+2);

      /*QGraphicsEllipseItem* item = new QGraphicsEllipseItem(596,256,10,10);
      item->setBrush(QBrush(QColor("green")));

      scena->addItem(item);*/

      ui->graphicsView->setScene(scena);

}

void MainWindow::setupActions()
{
    connect(ui->actionGeneruj_body,&QAction::triggered, this, &MainWindow::generuj_body);
    connect(ui->actionGeneruj_Centroidy,&QAction::triggered, this, &MainWindow::generuj_centroidy);
    connect(ui->actionVyres, &QAction::triggered, this, &MainWindow::vyres);
    connect(ui->actionNacti_body, &QAction::triggered, this, &MainWindow::nacti_body);

    ui->actionVyres->setEnabled(false);
}

void MainWindow::aktualizuj_centroidy()
{
    for(int i = 0; i < spojnice.length(); i++)
    {
        scena->removeItem(spojnice[i]);
    }
    for(int i = 0; i < spojnice.length(); i++)
    {
        delete spojnice.takeAt(i);
    }
    spojnice.clear();

    for(int i = 0; i < pocetCentroidu; i++)
    {
        scena->removeItem(centroidy[i]);
    }
    for(int i = 0; i < centroidy.length(); i++)
    {
        delete centroidy.takeAt(i);
        pocetCentroidu--;
    }
    centroidy.clear();
    scena->update();

    pocetCentroidu = kcentroidy.size();

    for(int i = 0; i < pocetCentroidu; i++)
    {
        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(kcentroidy[i][0],kcentroidy[i][1],velikost,velikost);
        item->setBrush(QBrush(QColor("red")));

        centroidy.push_back(item);

        scena->addItem(item);
    }

    nakresli_linky();
}

void MainWindow::nakresli_linky()       //Kresli spojnice mezi body a centroidem
{
    for(int i = 0; i < kcentroidy.size(); i++)
    {
        for(int j = 0; j < kcentroidy[i].vratPocetBodu(); j++)
        {
            int index = kcentroidy[i].vratPrirazenyBod(j);

            QGraphicsLineItem *item = new QGraphicsLineItem(kcentroidy[i][0]+velikost/2, kcentroidy[i][1]+velikost/2, kbody[index][0]+velikost/2, kbody[index][1]+velikost/2);

            item->setPen(QPen(QColor("green")));

            scena->addItem(item);

            spojnice.push_back(item);
        }
    }
}




void MainWindow::generuj_body()
{
    for(int i = 0; i < spojnice.length(); i++)
    {
        scena->removeItem(spojnice[i]);
    }
    for(int i = 0; i < spojnice.length(); i++)
    {
        delete spojnice.takeAt(i);
    }
    spojnice.clear();


    kbody.clear();
    for(int i = 0; i < pocetBodu; i++)
    {
        scena->removeItem(body[i]);
    }
    for(int i = 0; i < pocetBodu; i++)
    {
        delete body.takeAt(i);
        pocetBodu--;
    }
    body.clear();
    scena->update();


    velikost = ui->spSize->value();

    const double range_from_x = 0.0 + velikost;
    const double range_to_x = scena->width() - velikost;
    const double range_from_y = 0.0 + velikost;
    const double range_to_y = scena->height() - velikost;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double> dist_x(range_from_x, range_to_x);
    std::uniform_real_distribution<double> dist_y(range_from_y, range_to_y);

    //int dimenze = centroidy[0].vratDimenzi();

    pocetBodu = ui->spPoints->value();
    //velikost = ui->spSize->value();

    std::vector<double> pom_souradnice(2);

    for (int i = 0; i < pocetBodu; i++)
    {
        double a = dist_x(generator);
        double b = dist_y(generator);

        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(a,b,velikost,velikost);
        item->setBrush(QBrush(QColor("green")));

        body.push_back(item);

        scena->addItem(item);

        pom_souradnice[0] = a;
        pom_souradnice[1] = b;

        Bod pom(pom_souradnice, 2);

        kbody.push_back(pom);
    }

    if(centroidy.length() > 0)      //Pokud jiz byly i centroidy vygenerovane, tak zpristupni moznost vyresit kmeans
    {
        ui->actionVyres->setEnabled(true);
    }
}

void MainWindow::generuj_centroidy()
{
    for(int i = 0; i < spojnice.length(); i++)
    {
        scena->removeItem(spojnice[i]);
    }
    for(int i = 0; i < spojnice.length(); i++)
    {
        delete spojnice.takeAt(i);
    }
    spojnice.clear();


    kcentroidy.clear();
    for(int i = 0; i < pocetCentroidu; i++)
    {
        scena->removeItem(centroidy[i]);
    }
    for(int i = 0; i < pocetCentroidu; i++)
    {
        delete centroidy.takeAt(i);
        pocetCentroidu--;
    }
    centroidy.clear();
    scena->update();

    velikost = ui->spSize->value();

    const double range_from_x = 0.0 + velikost;
    const double range_to_x = scena->width() - velikost;
    const double range_from_y = 0.0 + velikost;
    const double range_to_y = scena->height() - velikost;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_real_distribution<double> dist_x(range_from_x, range_to_x);
    std::uniform_real_distribution<double> dist_y(range_from_y, range_to_y);

    //int dimenze = centroidy[0].vratDimenzi();

    pocetCentroidu = ui->spCentroids->value();
    //velikost = ui->spSize->value();

    std::vector<double> pom_souradnice(2);

    for (int i = 0; i < pocetCentroidu; i++)
    {
        double a = dist_x(generator);
        double b = dist_y(generator);

        QGraphicsEllipseItem* item = new QGraphicsEllipseItem(a,b,velikost,velikost);
        item->setBrush(QBrush(QColor("red")));

        centroidy.push_back(item);

        pom_souradnice[0] = a;
        pom_souradnice[1] = b;

        Centroid pom(pom_souradnice, 2);
        kcentroidy.push_back(pom);

        scena->addItem(item);
    }

    if(body.length() > 0)       //Pokud byly vygenerovane i body, zpristupni moznost vyreseni problemu
    {
        ui->actionVyres->setEnabled(true);
    }
}

void MainWindow::vyres()
{
    try
    {
        Kmeans k(kcentroidy, kbody, 2);
        k.vyres();
        kcentroidy.clear();
        //bool prazdny_shluk = false;
        for(int i = 0; i < pocetCentroidu; i++)
        {
            kcentroidy.push_back(k[i]);
            /*if(k[i].vratPocetBodu() == 0)     //Kontrola prazdnosti shluku
            {
                prazdny_shluk = true;
            }*/
        }

        /*if(prazdny_shluk)     //Pokud by byl nejaky shluk prazdny, tak vygenerujeme nove centroidy a znovu vypocteme
        {
            generuj_centroidy();
            vyres();
            return;
        }*/

        aktualizuj_centroidy();

        ui->actionVyres->setEnabled(false);
    } catch (MyException ex)
    {
        QMessageBox::warning(this, "Chyba", ex.what());
    }

}

void MainWindow::nacti_body()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "",tr("Files(*.txt);; All files (*.*)"));

    if(!filename.isEmpty())
    {
        precti_body_ze_souboru(filename.toStdString());
    }
    else
    {
        QMessageBox::warning(this, "Chyba", "Nepodarilo se nacist body");
        return;
    }
}


void MainWindow::precti_body_ze_souboru(std::string filename)
{
    for(int i = 0; i < spojnice.length(); i++)
    {
        scena->removeItem(spojnice[i]);
    }
    for(int i = 0; i < spojnice.length(); i++)
    {
        delete spojnice.takeAt(i);
    }
    spojnice.clear();


    kbody.clear();
    for(int i = 0; i < pocetBodu; i++)
    {
        scena->removeItem(body[i]);
    }
    for(int i = 0; i < pocetBodu; i++)
    {
        delete body.takeAt(i);
        pocetBodu--;
    }
    body.clear();
    scena->update();


    std::ifstream file;
    file.open(filename);

    if(file.is_open())
    {
        //file >> velikost;

        while (!file.eof())
        {
            std::vector<double> a;
            double b;
            for (int i = 0; i < 2; i++)
            {
                file >> b;
                a.push_back(b);
            }

            Bod pom(a, a.size());
            kbody.push_back(pom);
            a.clear();
        }
        pocetBodu = kbody.size();

        for(int i = 0; i < pocetBodu; i++)
        {
            QGraphicsEllipseItem* item = new QGraphicsEllipseItem(kbody[i][0],kbody[i][1],velikost,velikost);
            item->setBrush(QBrush(QColor("red")));

            body.push_back(item);

            scena->addItem(item);
        }

    }
    else
    {
        QMessageBox::warning(this, "Chyba", "Nepodarilo se otevrit soubor");
    }
}
