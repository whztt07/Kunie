#ifndef TEST_H
#define TEST_H

#include <QObject>

class Application;

class Test : public QObject
{
    Q_OBJECT

public:
    Test(Application* app);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void defaultDocument();
    void addDocument();
    void openDocument();
    void saveDocument();
    void checkSavedDocument();

private:
    Application* m_app;
};

#endif // KUNIETEST_H
