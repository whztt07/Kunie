#ifndef OCCVIEW_H
#define OCCVIEW_H

#include <QOpenGLWindow>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

class OccView : public QOpenGLWindow
{
public:
    OccView(Handle(AIS_InteractiveContext) context);
    ~OccView();

    void fitAll();

protected:

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_View) m_view;
};

#endif // OCCVIEW_H
