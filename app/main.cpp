#include "Application.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ForceRasterWidgets);
    Application a(argc, argv);
    return a.exec();
}
