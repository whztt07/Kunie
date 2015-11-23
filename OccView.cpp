#include "OccView.h"
#include <QWheelEvent>
#include <QDebug>
#include <QCursor>
#include <QApplication>
#include <QMouseEvent>

#include <Graphic3d_GraphicDriver.hxx>

#if defined(Q_OS_WIN32)
#include <WNT_Window.hxx>
#elif defined(Q_OS_OSX)
#include <Cocoa_Window.hxx>
#elif defined(Q_OS_LINUX)
#include <Xw_Window.hxx>
#endif


QCursor* OccView::s_rotate = NULL;
QCursor* OccView::s_zoom = NULL;

OccView::OccView(Handle(AIS_InteractiveContext) context):
    m_context(context),
    m_mode(Default)
{   
    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(1);
    setFormat(format);

    initCursors();

    m_view = m_context->CurrentViewer()->CreateView();
}

OccView::~OccView()
{
}

void OccView::fitAll()
{
    m_view->FitAll();
    m_view->ZFitAll();
}

void OccView::wheelEvent(QWheelEvent* ev)
{
    int x = ev->x() + ev->delta() / 8;
    int y = ev->y() + ev->delta() / 8;
    m_view->Zoom(ev->x(), ev->y(), x, y);
}

void OccView::mouseMoveEvent(QMouseEvent*)
{
    QPoint curPos = mapFromGlobal(QCursor::pos());
    if (curPos == m_precPos) return;

    switch (m_mode) {
    case Rotation:
        m_view->Rotation(curPos.x(), curPos.y());
        break;
    case Panning:
        m_view->Pan(curPos.x() - m_precPos.x(), m_precPos.y() - curPos.y());
        break;
    default:
        break;
    }

    m_precPos = curPos;
}

void OccView::mousePressEvent(QMouseEvent* ev)
{
    m_precPos = ev->pos();

    if(ev->button() == Qt::RightButton) {
        if(ev->modifiers() == Qt::NoModifier) {
            QApplication::setOverrideCursor(*s_rotate);
            m_mode = Rotation;
            m_view->StartRotation(ev->x(), ev->y(), 0.4);
        } else if(ev->modifiers() == Qt::ShiftModifier) {
            QApplication::setOverrideCursor(Qt::SizeAllCursor);
            m_mode = Panning;
        }
    }
}

void OccView::mouseReleaseEvent(QMouseEvent*)
{
    m_mode = Default;
    m_view->Redraw();
    QApplication::restoreOverrideCursor();
}

void OccView::initCursors()
{
    if(!s_rotate) s_rotate = new QCursor(QPixmap(":/icons/Rotate.png"));
    if(!s_zoom) s_zoom = new QCursor(QPixmap(":/icons/Zoom.png"));
}

void OccView::initializeGL()
{
    WId handle = winId();

#if defined(Q_OS_WIN32)
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)handle);
#elif defined(Q_OS_OSX)
    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView*)handle);
#elif defined(Q_OS_LINUX)
    Handle(Xw_Window) wind = new Xw_Window(m_context->CurrentViewer()->Driver()->GetDisplayConnection(), handle);
#endif

    m_view->SetWindow(wind);
    m_view->SetBackgroundColor(Quantity_NOC_BLACK);
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);
}

void OccView::paintGL()
{
    m_view->Redraw();
}

void OccView::resizeGL(int, int)
{
    m_view->MustBeResized();
}
