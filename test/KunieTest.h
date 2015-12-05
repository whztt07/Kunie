#ifndef KUNIETEST_H
#define KUNIETEST_H

#include <QObject>

class Application;

class KunieTest : public QObject
{
    Q_OBJECT

public:
    KunieTest(Application* app);

private slots:
    void initTestCase();
    void myFirstTest();
    void cleanupTestCase();

private:
    Application* m_app;
};

#endif // KUNIETEST_H
