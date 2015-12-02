#include "KunieTest.h"
#include <QTest>
#include <QApplication>

#include "MainWindow.h"

void KunieTest::initTestCase()
{
    qApp->setAttribute(Qt::AA_NativeWindows);
    m_win = new MainWindow;
    m_win->show();
    QVERIFY( QTest::qWaitForWindowActive(m_win));
}

void KunieTest::myFirstTest()
{
    qInfo() << "myFirstTest";
    QVERIFY(1 == 1);
}

void KunieTest::cleanupTestCase()
{
    QTest::qWait(5000);
    m_win->close();
    delete m_win;
}
