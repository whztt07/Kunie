#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <AppStd_Application.hxx>

class Application : public QApplication, public AppStd_Application
{
public:
    Application(int & argc, char ** argv);
};

#endif // APPLICATION_H
