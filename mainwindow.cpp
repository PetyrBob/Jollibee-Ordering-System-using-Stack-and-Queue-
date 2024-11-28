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

    // Initialize the timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTimer);
    timerDuration = 3 * 60;  // 3 minutes in seconds
    timeLeft = timerDuration;

    // LCD number initialization
    ui->lcdNumber->setDigitCount(5); // Format: mm:ss
    updateLCD(timeLeft);

    // Connect buttons to slots
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
        // Move the first order from pending_orders to completed_orders
        if (ui->pending_orders->count() > 0) {
            QListWidgetItem* item = ui->pending_orders->takeItem(0);
            ui->completed_orders->addItem(item);
            // Update the count of completed orders
            updateCompletedOrdersCount();
            // Reset the timer
            timeLeft = timerDuration;
            updateLCD(timeLeft);
            // Start the timer again if there are more pending orders
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
        // Generate an order number
        QString orderNumber = QString::asprintf("#%03d", ui->pending_orders->count() + 1);

        // Format the order with the order number
        QString orderWithNumber = orderNumber + ": " + orderText;

        // Add the order to the pending orders list
        ui->pending_orders->addItem(orderWithNumber);

        // Clear the input field
        ui->lineEdit->clear();
    }
}

void MainWindow::startQueue()
{
    if (!timer->isActive() && ui->pending_orders->count() > 0) {
        timeLeft = timerDuration;  // Reset the timer to 3 minutes
        updateLCD(timeLeft);
        timer->start(1000);  // Start the timer with 1-second intervals
    }
}

void MainWindow::serveOrder()
{
    // Move the first order from pending_orders to completed_orders
    if (ui->pending_orders->count() > 0) {
        QListWidgetItem* item = ui->pending_orders->takeItem(0);
        ui->completed_orders->addItem(item);
        // Update the count of completed orders
        updateCompletedOrdersCount();
        // Reset the timer
        timeLeft = timerDuration;
        updateLCD(timeLeft);
        // Start the timer again if there are more pending orders
        if (ui->pending_orders->count() > 0) {
            timer->start(1000);
        } else {
            timer->stop();
        }
    }
}

void MainWindow::deleteOrder()
{
    // Delete selected order from pending_orders
    QListWidgetItem* item = ui->pending_orders->currentItem();
    if (item) {
        delete item;
    }
}

// New method to update the number of completed orders
void MainWindow::updateCompletedOrdersCount()
{
    int completedOrderCount = ui->completed_orders->count();
    QString countText = QString("Completed Orders: %1").arg(completedOrderCount);
    ui->completed_orders_2->clear();
    ui->completed_orders_2->addItem(countText);  // Update the completed orders counter
}

void MainWindow::updatePendingOrdersCount()
{
    int pendingOrderCount = ui->pending_orders->count();
    QString countText = QString("Pending Orders: %1").arg(pendingOrderCount);
    ui->completed_orders_3->clear();
    ui->completed_orders_3->addItem(countText);  // Update the completed orders counter
}


