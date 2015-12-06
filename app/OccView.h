#ifndef OCCVIEW_H
#define OCCVIEW_H

#include <QOpenGLWindow>
#include <QPoint>
#include <QMap>
#include <V3d_View.hxx>

class Document;
class QAction;

class OccView : public QOpenGLWindow
{
public:
    OccView(Document* document);
    ~OccView();

    QWidget* widget();
    static Document* document(QWidget* widget);

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

    Document* m_document;
    QWidget* m_widget;
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
    static QMap<QWidget*, Document*> m_map;
};

#endif // OCCVIEW_H
