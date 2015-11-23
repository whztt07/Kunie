#ifndef OCCVIEW_H
#define OCCVIEW_H

#include <QOpenGLWindow>
#include <QPoint>
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

    void wheelEvent(QWheelEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);

private:
    enum Mode {
        Default,
        Rotation,
        Panning
    };

    static void initCursors();

    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_View) m_view;
    Mode m_mode;
    QPoint m_precPos;

    static QCursor* s_rotate;
    static QCursor* s_zoom;
};

#endif // OCCVIEW_H
