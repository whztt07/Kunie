#include "OccView.h"
#include "Document.h"
#include <QWheelEvent>
#include <QDebug>
#include <QCursor>
#include <QApplication>
#include <QMouseEvent>
#include <QWidget>
#include <QAction>
#include <QActionGroup>

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
QMap<QWidget*, Document*> OccView::m_map;

OccView::OccView(Document* document):
    m_document(document),
    m_mode(Default)
{   
    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(1);
    setFormat(format);

    m_view = m_document->viewer()->CreateView();

    m_widget = QWidget::createWindowContainer(this);
    m_map.insert(m_widget, m_document);

    initCursors();
    initViewActions();
    initRenderActions();
}

OccView::~OccView()
{
    m_map.remove(m_widget);
}

void OccView::initViewActions()
{
    m_viewActions = new QActionGroup(this);

    QAction* fitAll = new QAction(QPixmap(":/icons/FitAll.png"), "Fit &All", m_viewActions);
    connect(fitAll, &QAction::triggered, this, &OccView::fitAll);

    QAction* axo = new QAction(QPixmap(":/icons/Axo.png"), "A&xio", m_viewActions);
    connect(axo, &QAction::triggered, this, &OccView::axo);

    QAction* front = new QAction(QPixmap(":/icons/Front.png"), "&Front", m_viewActions);
    connect(front, &QAction::triggered, this, &OccView::front);

    QAction* back = new QAction(QPixmap(":/icons/Back.png"), "&Back", m_viewActions);
    connect(back, &QAction::triggered, this, &OccView::back);

    QAction* top = new QAction(QPixmap(":/icons/Top.png"), "&Top", m_viewActions);
    connect(top, &QAction::triggered, this, &OccView::top);

    QAction* bottom = new QAction(QPixmap(":/icons/Bottom.png"), "&Bottom", m_viewActions);
    connect(bottom, &QAction::triggered, this, &OccView::bottom);

    QAction* left = new QAction(QPixmap(":/icons/Left.png"), "&Left", m_viewActions);
    connect(left, &QAction::triggered, this, &OccView::left);

    QAction* right = new QAction(QPixmap(":/icons/Right.png"), "&Right", m_viewActions);
    connect(right, &QAction::triggered, this, &OccView::right);
}

void OccView::initRenderActions()
{
    m_renderActions = new QActionGroup(this);
    m_renderActions->setExclusive(false);

    QAction* hlr = new QAction("&Hidden line removal", m_renderActions);
    hlr->setCheckable(true);
    hlr->setChecked(m_view->ComputedMode());
    connect(hlr, &QAction::toggled, this, &OccView::setHLR);

    QAction* rayTracing = new QAction("&Ray tracing", m_renderActions);
    rayTracing->setCheckable(true);
    rayTracing->setChecked(m_view->ChangeRenderingParams().Method == Graphic3d_RM_RAYTRACING);
    connect(rayTracing, &QAction::toggled, this, &OccView::setRayTracing);

    QAction* shadow = new QAction("&Shadow", m_renderActions);
    shadow->setCheckable(true);
    shadow->setChecked(m_view->ChangeRenderingParams().IsShadowEnabled);
    connect(shadow, &QAction::toggled, this, &OccView::setShadow);

    QAction* reflection = new QAction("Re&flection", m_renderActions);
    reflection->setCheckable(true);
    reflection->setChecked(m_view->ChangeRenderingParams().IsReflectionEnabled);
    connect(reflection, &QAction::toggled, this, &OccView::setReflection);

    QAction* antialiasing = new QAction("&Antialiasing", m_renderActions);
    antialiasing->setCheckable(true);
    antialiasing->setChecked(m_view->ChangeRenderingParams().IsAntialiasingEnabled);
    connect(antialiasing, &QAction::toggled, this, &OccView::setAntialiasing);
}

Document *OccView::document(QWidget *widget)
{
    return m_map.value(widget);
}

QWidget *OccView::widget()
{
    return m_widget;
}

QActionGroup *OccView::viewActions()
{
    return m_viewActions;
}

void OccView::fitAll()
{
    m_view->FitAll();
    m_view->ZFitAll();
    m_view->SetViewMappingDefault();
}

void OccView::axo()
{
    m_view->SetProj( V3d_XposYnegZpos );
    fitAll();
}

void OccView::front()
{
    m_view->SetProj( V3d_Xpos );
    fitAll();
}

void OccView::back()
{
    m_view->SetProj( V3d_Xneg );
    fitAll();
}

void OccView::top()
{
    m_view->SetProj( V3d_Zpos );
    fitAll();
}

void OccView::bottom()
{
    m_view->SetProj( V3d_Zneg );
    fitAll();
}

void OccView::left()
{
    m_view->SetProj( V3d_Ypos );
    fitAll();
}

void OccView::right()
{
    m_view->SetProj( V3d_Yneg );
    fitAll();
}

QActionGroup *OccView::renderActions()
{
    return m_renderActions;
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
    Handle(Xw_Window) wind = new Xw_Window(m_document->viewer()->Driver()->GetDisplayConnection(), handle);
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
