#include <QWidget>
#include <QFileInfo>
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
#undef Status

#include "Application.h"
#include "Document.h"
#include "OccView.h"
#include "Translator.h"
#include "CylinderCommand.h"

#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TNaming_NamedShape.hxx>

TopoDS_Shape MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);

Quantity_Color Document::s_colors[] = {
    Quantity_Color(1,       0,      0,      Quantity_TOC_RGB),
    Quantity_Color(0,       1,      0,      Quantity_TOC_RGB),
    Quantity_Color(0,       0,      1,      Quantity_TOC_RGB),
    Quantity_Color(0.5,     0.5,    0,      Quantity_TOC_RGB),
    Quantity_Color(0,       0.5,    0.5,    Quantity_TOC_RGB),
    Quantity_Color(0.5,     0,      0.5,    Quantity_TOC_RGB),
    Quantity_Color(0.25,    0.75,   0,      Quantity_TOC_RGB),
    Quantity_Color(0,       0.25,   0.75,   Quantity_TOC_RGB),
    Quantity_Color(0.75,    0,      0.25,   Quantity_TOC_RGB),
    Quantity_Color(0,       0.75,   0.25,   Quantity_TOC_RGB),
    Quantity_Color(0.25,    0,      0.75,   Quantity_TOC_RGB),
    Quantity_Color(0.75,    0.25,   0,      Quantity_TOC_RGB),
    Quantity_Color(0,       0.125,  0.875,  Quantity_TOC_RGB),
    Quantity_Color(0.875,   0,      0.125,  Quantity_TOC_RGB),
    Quantity_Color(0.125,   0.875,  0,      Quantity_TOC_RGB),
    Quantity_Color(0.625,   0,      0.375,  Quantity_TOC_RGB),
    Quantity_Color(0.375,   0.625,  0,      Quantity_TOC_RGB),
    Quantity_Color(0,       0.375,  0.625,  Quantity_TOC_RGB),
    Quantity_Color(0.875,   0.125,  0,      Quantity_TOC_RGB),
    Quantity_Color(0.0,     0.875,  0.125,  Quantity_TOC_RGB),
    Quantity_Color(0.125,   0,      0.875,  Quantity_TOC_RGB),
    Quantity_Color(0.625,   0.375,  0,      Quantity_TOC_RGB),
    Quantity_Color(0,       0.625,  0.375,  Quantity_TOC_RGB),
    Quantity_Color(0.375,   0,      0.625,  Quantity_TOC_RGB)
};

int Document::s_maxColor = sizeof(Document::s_colors) / sizeof(Quantity_Color);

static Handle(Graphic3d_GraphicDriver) graphicDriver;

Document::Document(const QString& title, Application *app):
    QObject(app),
    m_title(title),
    m_colorNum(0)
{
    if (graphicDriver.IsNull()) {
        Handle(Aspect_DisplayConnection) display = new Aspect_DisplayConnection;
        graphicDriver = new OpenGl_GraphicDriver(display);
    }

    m_viewer = new V3d_Viewer(graphicDriver, (short* const)"viewer");
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    app->ocafApp()->NewDocument("MDTV-Standard", m_document);
    TPrsStd_AISViewer::New(m_document->Main(), m_viewer);
    TPrsStd_AISViewer::Find(m_document->Main(), m_context);
    m_context->SetDisplayMode(AIS_Shaded);

    m_view = new OccView(m_context);
}

Document::~Document()
{
    delete m_view->widget();
}

QString Document::title()
{
    return m_title;
}

OccView* Document::view()
{
    return m_view;
}

void Document::display(const TopoDS_Shape& shape)
{
    insert(shape);
    m_context->UpdateCurrentViewer();
}

void Document::display(const Handle(TopTools_HSequenceOfShape)& shapes)
{
    if(shapes.IsNull() || !shapes->Length()) return;

    for(int i=1; i<=shapes->Length(); i++)
        insert(shapes->Value(i));

    m_context->UpdateCurrentViewer();
}

void Document::createBottle()
{
    TopoDS_Shape bottle = MakeBottle(50, 70, 30);
    display(bottle);
}

void Document::createCylinder(double x, double y, double z, double r, double h)
{
    m_document->NewCommand();
    CylinderCommand cmd(m_document->Main());
    TDF_Label label = cmd.createCylinder(x, y, z, r, h, "Cylindre");
    Handle(TPrsStd_AISPresentation) prs = TPrsStd_AISPresentation::Set(label, TNaming_NamedShape::GetID());
    prs->SetMaterial(Graphic3d_NOM_BRASS);
    prs->SetColor(Quantity_NOC_MATRABLUE);
    prs->Display(1);
    m_context->UpdateCurrentViewer();
    m_document->CommitCommand();
}

void Document::import(const QString& file)
{
    QString ext = QFileInfo(file).suffix();
    Handle(TopTools_HSequenceOfShape) shapes;

    if(ext == "brep" || ext == "rle") {
        shapes = Translator::importBREP(file);
    } else if(ext == "igs" || ext == "iges") {
        shapes = Translator::importIGES(file);
    } else if(ext == "stp" || ext == "step") {
        shapes = Translator::importSTEP(file);
    } else if(ext == "csfdb") {
        shapes = Translator::importCSFDB(file);
    } else if(ext == "vrml") {
        shapes = Translator::importVRML(file);
    } else if(ext == "stl") {
        shapes = Translator::importSTL(file);
    } else {
        emit error(QFileInfo(file).fileName() + " unknown file type");
    }

    if(shapes.IsNull() || !shapes->Length()) {
        emit error(QFileInfo(file).fileName() + " read error");
    } else {
        display(shapes);
        m_view->fitAll();
    }
}

void Document::insert(const TopoDS_Shape& shape)
{
    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    m_context->Display(ais, false);
    m_context->SetMaterial(ais, Graphic3d_NOM_BRASS);
    m_context->SetColor(ais, s_colors[m_colorNum++]);
    m_colorNum %= s_maxColor;
}
