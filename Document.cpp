#include <QApplication>
#include <Aspect_DisplayConnection.hxx>
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

#include "Document.h"
#include "OccView.h"

#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>

TopoDS_Shape MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);

static Handle(Graphic3d_GraphicDriver) graphicDriver;

Document::Document(QObject *parent):
    QObject(parent)
{
    if (graphicDriver.IsNull()) {
        Handle(Aspect_DisplayConnection) display = new Aspect_DisplayConnection;
        graphicDriver = new OpenGl_GraphicDriver(display);
    }

    m_viewer = new V3d_Viewer(graphicDriver, (short* const)"viewer");
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    m_context = new AIS_InteractiveContext(m_viewer);
    m_context->SetDisplayMode(AIS_Shaded);

    m_view = new OccView(m_context);
}

OccView* Document::view()
{
    return m_view;
}

void Document::onMakeBottle()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    TopoDS_Shape aBottle = MakeBottle(50, 70, 30);
    Handle(AIS_Shape) AISBottle = new AIS_Shape(aBottle);
    m_context->SetMaterial(AISBottle, Graphic3d_NOM_GOLD);
    m_context->Display(AISBottle);
    QApplication::restoreOverrideCursor();
}
