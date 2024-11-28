#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timerDuration = 3 * 60;  
    timeLeft = timerDuration;

  
    ui->lcdNumber->setDigitCount(5); 
    updateLCD(timeLeft);

   
    connect(ui->Order, &QPushButton::clicked, this, &MainWindow::addOrder);
    connect(ui->Queue, &QPushButton::clicked, this, &MainWindow::startQueue);
    connect(ui->Serve, &QPushButton::clicked, this, &MainWindow::serveOrder);
    connect(ui->Delete, &QPushButton::clicked, this, &MainWindow::deleteOrder);
    connect(ui->RecentOrders, &QPushButton::clicked, this, &MainWindow::updatePendingOrdersCount);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLCD(int timeInSeconds)
{
    int minutes = timeInSeconds / 60;
    int seconds = timeInSeconds % 60;
    QString timeString = QString::asprintf("%02d:%02d", minutes, seconds);
    ui->lcdNumber->display(timeString);
}

void MainWindow::updateTimer()
{
    if (timeLeft > 0) {
        timeLeft--;
        updateLCD(timeLeft);
    } else {
        timer->stop();
        
        if (ui->pending_orders->count() > 0) {
            QListWidgetItem* item = ui->pending_orders->takeItem(0);
            ui->completed_orders->addItem(item);
            
            updateCompletedOrdersCount();
          
            timeLeft = timerDuration;
            updateLCD(timeLeft);
          
            if (ui->pending_orders->count() > 0) {
                timer->start(1000);
            }
        }
    }
}

void MainWindow::addOrder()
{
    QString orderText = ui->lineEdit->text();
    if (!orderText.isEmpty()) {
      
        QString orderNumber = QString::asprintf("#%03d", ui->pending_orders->count() + 1);


        QString orderWithNumber = orderNumber + ": " + orderText;

       
        ui->pending_orders->addItem(orderWithNumber);

       
        ui->lineEdit->clear();
    }
}

void MainWindow::startQueue()
{
    if (!timer->isActive() && ui->pending_orders->count() > 0) {
        timeLeft = timerDuration;  
        updateLCD(timeLeft);
        timer->start(1000); 
    }
}

void MainWindow::serveOrder()
{
   
    if (ui->pending_orders->count() > 0) {
        QListWidgetItem* item = ui->pending_orders->takeItem(0);
        ui->completed_orders->addItem(item);
        
        updateCompletedOrdersCount();
        
        timeLeft = timerDuration;
        updateLCD(timeLeft);
       
        if (ui->pending_orders->count() > 0) {
            timer->start(1000);
        } else {
            timer->stop();
        }
    }
}

void MainWindow::deleteOrder()
{
   
    QListWidgetItem* item = ui->pending_orders->currentItem();
    if (item) {
        delete item;
    }
}


void MainWindow::updateCompletedOrdersCount()
{
    int completedOrderCount = ui->completed_orders->count();
    QString countText = QString("Completed Orders: %1").arg(completedOrderCount);
    ui->completed_orders_2->clear();
    ui->completed_orders_2->addItem(countText); 
}

void MainWindow::updatePendingOrdersCount()
{
    int pendingOrderCount = ui->pending_orders->count();
    QString countText = QString("Pending Orders: %1").arg(pendingOrderCount);
    ui->completed_orders_3->clear();
    ui->completed_orders_3->addItem(countText);  
}


