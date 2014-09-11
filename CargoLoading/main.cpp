#include "cargoloading.h"
#include <QtWidgets/QApplication>
#include "GUIMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //CargoLoading w;
    //w.show();

    a.setLayoutDirection(Qt::RightToLeft);
    CGUIMainWindow w;
    w.show();


    return a.exec();
}
