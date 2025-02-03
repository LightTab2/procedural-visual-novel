#include "NAMSC_editor.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("PVN");
    QCoreApplication::setApplicationName("PVN Editor");
    QApplication a(argc, argv);
    NAMSC_editor w;
    w.show();
    return a.exec();
}
