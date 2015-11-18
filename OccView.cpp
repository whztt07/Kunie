#include <OpenGl_GraphicDriver.hxx>

#undef Bool
#undef CursorShape
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose

#include "OccView.h"

#include <QWidget>

#if defined(Q_OS_WIN32)
#include <WNT_Window.hxx>
#elif defined(Q_OS_OSX)
#include <Cocoa_Window.hxx>
#elif defined(Q_OS_LINUX)
#include <Xw_Window.hxx>
#endif

Handle(Aspect_DisplayConnection) OccView::s_display;
Handle(Graphic3d_GraphicDriver) OccView::s_graphicDriver;

QWidget* OccView::create(QWidget *parent, Qt::WindowFlags flags)
{
    return QWidget::createWindowContainer(new OccView, parent, flags);
}

OccView::OccView()
{   
    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(16);
    setFormat(format);

    if (s_graphicDriver.IsNull()) {
        s_display = new Aspect_DisplayConnection;
        s_graphicDriver = new OpenGl_GraphicDriver(s_display);
    }

    m_viewer = new V3d_Viewer(s_graphicDriver, (short* const)"viewer");
    m_view = m_viewer->CreateView();
    m_context = new AIS_InteractiveContext(m_viewer);
}

OccView::~OccView()
{
}

void OccView::initializeGL()
{
    WId handle = winId();

#if defined(Q_OS_WIN32)
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle)handle);
#elif defined(Q_OS_OSX)
    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView*)handle);
#elif defined(Q_OS_LINUX)
    Handle(Xw_Window) wind = new Xw_Window(s_display, handle);
#endif

    m_view->SetWindow(wind);

    // Set up lights etc
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    m_view->SetBackgroundColor(Quantity_NOC_BLACK);
    m_view->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    m_context->SetDisplayMode(AIS_Shaded);
}

void OccView::paintGL()
{
    m_view->Redraw();
}

void OccView::resizeGL(int, int)
{
    m_view->MustBeResized();
}
