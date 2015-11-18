#ifndef OCCVIEW_H
#define OCCVIEW_H

#include <QOpenGLWindow>
#include <V3d_Viewer.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Graphic3d_GraphicDriver.hxx>

class OccView : public QOpenGLWindow
{
public:
    static QWidget* create(QWidget* parent=0, Qt::WindowFlags flags=0);

protected:
    OccView();
    ~OccView();

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    Handle(V3d_Viewer)                      m_viewer;
    Handle(V3d_View)                        m_view;
    Handle(AIS_InteractiveContext)          m_context;
    static Handle(Aspect_DisplayConnection) s_display;
    static Handle(Graphic3d_GraphicDriver)  s_graphicDriver;
};

#endif // OCCVIEW_H
