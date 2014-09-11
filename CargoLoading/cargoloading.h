#ifndef CARGOLOADING_H
#define CARGOLOADING_H

#include <QtWidgets/QMainWindow>
#include "ui_cargoloading.h"

class CargoLoading : public QMainWindow
{
    Q_OBJECT

public:
    CargoLoading(QWidget *parent = 0);
    ~CargoLoading();

private:
    Ui::CargoLoadingClass ui;
};

#endif // CARGOLOADING_H
