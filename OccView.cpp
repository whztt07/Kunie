#include "OccView.h"
#include <Graphic3d_GraphicDriver.hxx>

#if defined(Q_OS_WIN32)
#include <WNT_Window.hxx>
#elif defined(Q_OS_OSX)
#include <Cocoa_Window.hxx>
#elif defined(Q_OS_LINUX)
#include <Xw_Window.hxx>
#endif

OccView::OccView(Handle_AIS_InteractiveContext context):
    m_context(context)
{   
    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(16);
    setFormat(format);

    m_view = m_context->CurrentViewer()->CreateView();
}

OccView::~OccView()
{
}

void OccView::fitAll()
{
  m_view->FitAll();
  m_view->ZFitAll();
  m_view->Redraw();
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
