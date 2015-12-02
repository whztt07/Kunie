#include "OccView.h"
#include <QWheelEvent>
#include <QDebug>
#include <QCursor>
#include <QApplication>
#include <QMouseEvent>
#include <QWidget>
#include <QAction>

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

OccView::OccView(Handle(AIS_InteractiveContext) context, QWidget* parent):
    m_context(context),
    m_mode(Default)
{   
    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(1);
    setFormat(format);

    initCursors();

    m_view = m_context->CurrentViewer()->CreateView();
    m_widget = QWidget::createWindowContainer(this, parent);

    m_hlr = new QAction("&Hidden line removal", this);
    m_hlr->setCheckable(true);
    connect(m_hlr, &QAction::toggled, this, &OccView::setHLR);
    m_widget->addAction(m_hlr);

    m_rayTracing = new QAction("&Ray tracing", this);
    m_rayTracing->setCheckable(true);
    m_rayTracing->setChecked(false);
    connect(m_rayTracing, &QAction::toggled, this, &OccView::setRayTracing);
    m_widget->addAction(m_rayTracing);

    m_shadow = new QAction("&Shadow", this);
    m_shadow->setCheckable(true);
    m_shadow->setChecked(m_view->ChangeRenderingParams().IsShadowEnabled);
    connect(m_shadow, &QAction::toggled, this, &OccView::setShadow);
    m_widget->addAction(m_shadow);

    m_reflection = new QAction("Re&flection", this);
    m_reflection->setCheckable(true);
    m_reflection->setChecked(m_view->ChangeRenderingParams().IsReflectionEnabled);
    connect(m_reflection, &QAction::toggled, this, &OccView::setReflection);
    m_widget->addAction(m_reflection);

    m_antialiasing = new QAction("&Antialiasing", this);
    m_antialiasing->setCheckable(true);
    m_antialiasing->setChecked(m_view->ChangeRenderingParams().IsAntialiasingEnabled);
    connect(m_antialiasing, &QAction::toggled, this, &OccView::setAntialiasing);
    m_widget->addAction(m_antialiasing);
}

OccView::~OccView()
{
}

void OccView::fitAll()
{
    m_view->FitAll();
    m_view->ZFitAll();
}

void OccView::setHLR(bool enabled)
{
    m_view->SetComputedMode(enabled);
}

void OccView::setRayTracing(bool enabled)
{
    Graphic3d_RenderingMode method = enabled ? Graphic3d_RM_RAYTRACING : Graphic3d_RM_RASTERIZATION;
    m_view->ChangeRenderingParams().Method = method;
    m_view->Redraw();
}

void OccView::setShadow(bool enabled)
{
    m_view->ChangeRenderingParams().IsShadowEnabled = enabled;
    m_view->Redraw();
}

void OccView::setReflection(bool enabled)
{
    m_view->ChangeRenderingParams().IsReflectionEnabled = enabled;
    m_view->Redraw();
}

void OccView::setAntialiasing(bool enabled)
{
    m_view->ChangeRenderingParams().IsAntialiasingEnabled = enabled;
    m_view->Redraw();
}

QWidget *OccView::widget()
{
    return m_widget;
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
