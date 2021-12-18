#include "form.h"

#include <cmath>

#define _USE_MATH_DEFINES

#include <QLayout>

std::random_device rd;
std::mt19937 mt(rd());


void Bezier(double t,int n,std::vector<std::pair<double,double>>& v){
    int tmp = v.size();
    if(n==2){
        v.push_back(std::make_pair((1-t)*v[tmp-2].first + t*v[tmp-1].first,(1-t)*v[tmp-2].second + t*v[tmp-1].second));
    }
    else{
        for (int i=n;i>1;i--){
            v.push_back(std::make_pair((1-t)*v[tmp-i].first + t*v[tmp-i+1].first,(1-t)*v[tmp-i].second + t*v[tmp-i+1].second));
        }
        Bezier(t,n-1,v);
    }
}

Form::Form(QWidget *parent)
    : QWidget(parent)
{
    timer = new QTimer(this);
    timer->setInterval(20);

    labelN = new QLabel("Degree n");

    SpinBoxN = new QSpinBox;
    SpinBoxN->setMinimum(2);
    SpinBoxN->setMaximum(50);
    SpinBoxN->setSingleStep(1);
    SpinBoxN->setValue(3);

    pushButtonStart = new QPushButton("start");
    pushButtonStop = new QPushButton("stop");


    Scene = new QGraphicsScene(0,0,1024,720,this);


    GraphicsView = new QGraphicsView(Scene);


    QHBoxLayout *layoutParameters = new QHBoxLayout;
    layoutParameters->addWidget(labelN);
    layoutParameters->addWidget(SpinBoxN);


    QHBoxLayout *layoutButtons = new QHBoxLayout;
    layoutButtons->addWidget(pushButtonStart);
    layoutButtons->addWidget(pushButtonStop);

    QVBoxLayout *layoutMain = new QVBoxLayout;
    layoutMain->addLayout(layoutParameters);
    layoutMain->addLayout(layoutButtons);
    layoutMain->addWidget(GraphicsView);




    this->setLayout(layoutMain);

    connect(pushButtonStart, &QPushButton::clicked, this, &Form::start);
    connect(pushButtonStop, &QPushButton::clicked, this, &Form::stop);
    connect(timer, &QTimer::timeout, this, &Form::makeStep);
}

Form::~Form()
{
}

void Form::start()
{
    Scene->clear();
    v.clear();
    v.shrink_to_fit();
    t = 0;

    int N = SpinBoxN->value();

    for (int i = 0; i<N;i++){
        int x = mt()%1024;
        int y = mt()%720;
        v.push_back(std::make_pair(x,y));
    }

    auto res = v;

    Bezier(t,N,res);

    lines = new std::vector<QGraphicsLineItem*>;
    int j = 0;
    for (int i=N;i>1;i--){
        for (int k=0;k<i-1;k++){
            auto tmp = new QGraphicsLineItem(res[j].first,res[j].second,res[j+1].first,res[j+1].second);
            lines->push_back(tmp);
            j++;
        }
        j++;
    }
    for(int i=0;i<int(lines->size());i++){
        Scene->addItem(lines->operator[](i));
    }

    int rs = res.size();
    EI = new QGraphicsEllipseItem(-4,-4,8,8);
    EI->setBrush(Qt::red);
    EI->setPos(res[rs-1].first,res[rs-1].second);
    Scene->addItem(EI);

    po = new std::pair<double,double>(res[rs-1].first,res[rs-1].second);


    Scene->advance();
    pushButtonStart->setEnabled(false);
    SpinBoxN->setEnabled(false);

    timer->start();
}

void Form::stop()
{
    pushButtonStart->setEnabled(true);
    SpinBoxN->setEnabled(true);

    timer->stop();
}

void Form::makeStep()
{
    int N = SpinBoxN->value();

    t+=dt;

    if(t>1){
        Form::stop();
    }

    auto res = v;

    Bezier(t,N,res);

    int p = 0;
    int j = 0;
    for (int i=N;i>1;i--){
        for (int k=0;k<i-1;k++){
            lines->operator[](p)->setLine(res[j].first,res[j].second,res[j+1].first,res[j+1].second);
            j++;
            p++;
        }
        j++;
    }

    int rs = res.size();
    EI->setPos(res[rs-1].first,res[rs-1].second);

    Scene->addLine(po->first,po->second,res[rs-1].first,res[rs-1].second,QPen(QBrush(Qt::red),5,Qt::SolidLine));

    po = new std::pair<double,double>(res[rs-1].first,res[rs-1].second);

    Scene->advance();

}

