#include <QTest>
#include "Application.h"
#include "Test.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    Test test(&app);
    return QTest::qExec(&test, argc, argv);
}
