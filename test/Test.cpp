#include <QTest>
#include <QStandardPaths>
#include "Test.h"
#include "Application.h"
#include "MainWindow.h"
#include "Document.h"
#include "OccView.h"

#define TIMEOUT 500

Test::Test(Application *app)
{
    m_app = app;
}

void Test::initTestCase()
{
    QVERIFY(m_app->window());
    QVERIFY( QTest::qWaitForWindowActive(m_app->window()));
}

void Test::cleanupTestCase()
{
    m_app->window()->close();
}

void Test::init()
{
    if(!m_app->window()->currentDoc()) {
        m_app->window()->newDoc();
    }
    QVERIFY(m_app->window()->currentDoc());
    QTest::qWait(TIMEOUT);
}

void Test::cleanup()
{
    QTest::qWait(TIMEOUT);
    QVERIFY(m_app->window()->closeAll());
    QVERIFY(m_app->window()->currentDoc() == NULL);
}

void Test::defaultDocument()
{
    Document* doc = m_app->window()->currentDoc();
    QVERIFY(doc);
    QVERIFY(doc->view());
    QVERIFY(doc->view()->widget());
    QVERIFY(doc->name() == "Untitled");
}

void Test::addDocument()
{
    Document* doc1 = m_app->window()->currentDoc();

    Document* doc2 = m_app->window()->newDoc();
    QVERIFY(m_app->window()->currentDoc() == doc2);
    QVERIFY(doc2->name() == "Untitled");

    QTest::qWait(TIMEOUT);

    Document* doc3 = m_app->window()->newDoc();
    QVERIFY(m_app->window()->currentDoc() == doc3);
    QVERIFY(doc3->name() == "Untitled");

    QTest::qWait(TIMEOUT);

    m_app->window()->setCurrentDoc(doc1);
    QVERIFY(m_app->window()->currentDoc() == doc1);
}

void Test::openDocument()
{
    QString file = QString(DATA) + QDir::separator() + "cut.xml";
    Document* doc1 = m_app->window()->openDoc(file);
    QVERIFY(doc1);
    QVERIFY(doc1->name() == "cut.xml");
    QVERIFY(m_app->window()->currentDoc() == doc1);

    QTest::qWait(TIMEOUT);

    file = QString(DATA) + QDir::separator() + "sphere.cbf";
    Document* doc2 = m_app->window()->openDoc(file);
    QVERIFY(doc2);
    QVERIFY(doc2->name() == "sphere.cbf");
    QVERIFY(m_app->window()->currentDoc() == doc2);

    QTest::qWait(TIMEOUT);

    QVERIFY(m_app->window()->setCurrentDoc(doc1) == true);

    QTest::qWait(TIMEOUT);

    file = QString(DATA) + QDir::separator() + "cut.dxc";
    Document* doc3 = m_app->window()->openDoc(file);
    QVERIFY(doc3);

    QTest::qWait(TIMEOUT);

    file = QString(DATA) + QDir::separator() + "cut.std";
    Document* doc4 = m_app->window()->openDoc(file);
    QVERIFY(doc4);

    QTest::qWait(TIMEOUT);

    file = QString(DATA) + QDir::separator() + "cut.xbf";
    Document* doc5 = m_app->window()->openDoc(file);
    QVERIFY(doc5);

    QTest::qWait(TIMEOUT);

    file = QString(DATA) + QDir::separator() + "sphere.cbf";
    Document* doc6 = m_app->window()->openDoc(file);
    QVERIFY(doc6 == doc2);
    QVERIFY(m_app->window()->currentDoc() == doc6);
}

void Test::saveDocument()
{
    Document* doc = m_app->window()->currentDoc();
    doc->createCut();

    QString file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.xml";
    QVERIFY(doc->saveAs(file));

    file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.dxc";
    QVERIFY(doc->saveAs(file));

    file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.std";
    QVERIFY(doc->saveAs(file));

    file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.xbf";
    QVERIFY(doc->saveAs(file));

    file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.cbf";
    QVERIFY(doc->saveAs(file));
}

void Test::checkSavedDocument()
{
    QString file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.xml";
    Document* doc = m_app->window()->openDoc(file);
    QVERIFY(doc);

    file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.dxc";
    doc = m_app->window()->openDoc(file);
    QVERIFY(doc);

    file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.std";
    doc = m_app->window()->openDoc(file);
    QVERIFY(doc);

    file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.xbf";
    doc = m_app->window()->openDoc(file);
    QVERIFY(doc);

    file = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator() + "cut.cbf";
    doc = m_app->window()->openDoc(file);
    QVERIFY(doc);
}
