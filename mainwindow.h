#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include "Kmeans.h"
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QGraphicsScene *scena;

    QVector<QGraphicsEllipseItem*> body;
    QVector<QGraphicsEllipseItem*> centroidy;
    QVector<QGraphicsLineItem*> spojnice;

    std::vector<Bod> kbody;
    std::vector<Centroid> kcentroidy;

    void setup();
    void setupActions();
    void aktualizuj_centroidy();
    void nakresli_linky();
    void precti_body_ze_souboru(std::string filename);

    int pocetBodu;
    int pocetCentroidu;

    int velikost;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void generuj_body();
    void generuj_centroidy();
    void vyres();
    void nacti_body();
};
#endif // MAINWINDOW_H
