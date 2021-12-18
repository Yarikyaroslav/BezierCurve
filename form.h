#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>


#include <vector>
#include <utility>
#include <random>


class Form : public QWidget
{
    Q_OBJECT

public:
    Form(QWidget *parent = nullptr);
    ~Form();

public slots:
    void start();
    void stop();
    void makeStep();

private:
    QLabel *labelN;

    QSpinBox *SpinBoxN;

    QPushButton *pushButtonStart;
    QPushButton *pushButtonStop;


    QGraphicsScene *Scene;
    QGraphicsView *GraphicsView;
    std::vector<QGraphicsLineItem*>* lines;
    QGraphicsEllipseItem* EI;

    QTimer * timer;

    double t;
    double dt = 0.005;

    std::vector<std::pair<double,double>> v;
    std::pair<double,double>* po;

};
#endif // FORM_H
