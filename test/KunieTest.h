#ifndef KUNIETEST_H
#define KUNIETEST_H

#include <QObject>

class MainWindow;

class KunieTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void myFirstTest();
    void cleanupTestCase();

private:
    MainWindow* m_win;
};

#endif // KUNIETEST_H
