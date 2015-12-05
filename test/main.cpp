#include <QTest>
#include "Application.h"
#include "KunieTest.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    KunieTest test(&app);
    return QTest::qExec(&test, argc, argv);
}
