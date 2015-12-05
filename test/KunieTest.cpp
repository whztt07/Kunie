#include "KunieTest.h"
#include "Application.h"
#include <QTest>

#include "MainWindow.h"

KunieTest::KunieTest(Application *app)
{
    m_app = app;
}

void KunieTest::initTestCase()
{
    QVERIFY( QTest::qWaitForWindowActive(m_app->window()));
}

void KunieTest::myFirstTest()
{
    qInfo() << "myFirstTest";
    QVERIFY(1 == 1);
}

void KunieTest::cleanupTestCase()
{
    QTest::qWait(5000);
    m_app->window()->close();
}
