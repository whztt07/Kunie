#ifndef OCCVIEW_H
#define OCCVIEW_H

#include <QOpenGLWindow>
#include <QPoint>
#include <AIS_InteractiveContext.hxx>
#include <V3d_View.hxx>

class QAction;

class OccView : public QOpenGLWindow
{
public:
    OccView(Handle(AIS_InteractiveContext) context, QWidget* parent=0);
    ~OccView();

    QWidget* widget();

    void fitAll();
    void setHLR(bool enabled);
    void setRayTracing(bool enabled);
    void setShadow(bool enabled);
    void setReflection(bool enabled);
    void setAntialiasing(bool enabled);

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

    QWidget* m_widget;
    Handle(AIS_InteractiveContext) m_context;
    Handle(V3d_View) m_view;
    Mode m_mode;
    QPoint m_precPos;

    QAction* m_hlr;
    QAction* m_rayTracing;
    QAction* m_shadow;
    QAction* m_reflection;
    QAction* m_antialiasing;

    static QCursor* s_rotate;
    static QCursor* s_zoom;
};

#endif // OCCVIEW_H
