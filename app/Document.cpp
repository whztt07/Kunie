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
#include "SphereCommand.h"
#include "CutCommand.h"

#include <QActionGroup>
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <TPrsStd_AISViewer.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <TNaming_NamedShape.hxx>
#include <TDF_ChildIterator.hxx>
#include <IGESCAFControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TDF_LabelSequence.hxx>
#include <TNaming_Builder.hxx>


TopoDS_Shape MakeBottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);

Quantity_NameOfColor Document::s_colors[] = {
    Quantity_NOC_MATRABLUE,
    Quantity_NOC_MATRAGRAY,
    Quantity_NOC_AQUAMARINE1,
    Quantity_NOC_AQUAMARINE4,
    Quantity_NOC_BISQUE4,
    Quantity_NOC_BLUE4,
    Quantity_NOC_BLUEVIOLET,
    Quantity_NOC_BROWN,
    Quantity_NOC_BURLYWOOD,
    Quantity_NOC_BURLYWOOD4,
    Quantity_NOC_CADETBLUE3,
    Quantity_NOC_CADETBLUE4,
    Quantity_NOC_CHARTREUSE1,
    Quantity_NOC_CHARTREUSE4,
    Quantity_NOC_CHOCOLATE1,
    Quantity_NOC_CHOCOLATE4,
    Quantity_NOC_CORAL1,
    Quantity_NOC_CORAL4,
    Quantity_NOC_CORNFLOWERBLUE,
    Quantity_NOC_CYAN1,
    Quantity_NOC_CYAN4,
    Quantity_NOC_DARKGOLDENROD1,
    Quantity_NOC_DARKGOLDENROD4,
    Quantity_NOC_DARKGREEN,
    Quantity_NOC_DARKKHAKI,
    Quantity_NOC_DARKOLIVEGREEN1,
    Quantity_NOC_DARKOLIVEGREEN4,
    Quantity_NOC_DARKORANGE1,
    Quantity_NOC_DARKORANGE4,
    Quantity_NOC_DARKORCHID1,
    Quantity_NOC_DARKORCHID4,
    Quantity_NOC_DARKSALMON,
    Quantity_NOC_DARKSEAGREEN1,
    Quantity_NOC_DARKSEAGREEN4,
    Quantity_NOC_DARKSLATEBLUE,
    Quantity_NOC_DARKSLATEGRAY1,
    Quantity_NOC_DARKSLATEGRAY4,
    Quantity_NOC_DARKTURQUOISE,
    Quantity_NOC_DARKVIOLET,
    Quantity_NOC_DEEPPINK,
    Quantity_NOC_DEEPPINK4,
    Quantity_NOC_DEEPSKYBLUE1,
    Quantity_NOC_DEEPSKYBLUE4,
    Quantity_NOC_DODGERBLUE1,
    Quantity_NOC_DODGERBLUE4,
    Quantity_NOC_FIREBRICK1,
    Quantity_NOC_FIREBRICK4,
    Quantity_NOC_FORESTGREEN,
    Quantity_NOC_GAINSBORO,
    Quantity_NOC_GOLD1,
    Quantity_NOC_GOLD4,
    Quantity_NOC_GOLDENROD1,
    Quantity_NOC_GOLDENROD4,
    Quantity_NOC_GREEN1,
    Quantity_NOC_GREEN4,
    Quantity_NOC_GREENYELLOW,
    Quantity_NOC_HOTPINK1,
    Quantity_NOC_HOTPINK4,
    Quantity_NOC_INDIANRED1,
    Quantity_NOC_INDIANRED4,
    Quantity_NOC_KHAKI1,
    Quantity_NOC_KHAKI4,
    Quantity_NOC_LAVENDER,
    Quantity_NOC_LAWNGREEN,
    Quantity_NOC_LIGHTBLUE1,
    Quantity_NOC_LIGHTBLUE4,
    Quantity_NOC_LIGHTCORAL,
    Quantity_NOC_LIGHTGOLDENROD1,
    Quantity_NOC_LIGHTGOLDENROD4,
    Quantity_NOC_LIGHTPINK1,
    Quantity_NOC_LIGHTPINK4,
    Quantity_NOC_LIGHTSALMON1,
    Quantity_NOC_LIGHTSALMON4,
    Quantity_NOC_LIGHTSEAGREEN,
    Quantity_NOC_LIGHTSKYBLUE1,
    Quantity_NOC_LIGHTSKYBLUE4,
    Quantity_NOC_LIGHTSLATEBLUE,
    Quantity_NOC_LIGHTSLATEGRAY,
    Quantity_NOC_LIGHTSTEELBLUE1,
    Quantity_NOC_LIGHTSTEELBLUE4,
    Quantity_NOC_LIGHTYELLOW,
    Quantity_NOC_LIGHTYELLOW4,
    Quantity_NOC_LIMEGREEN,
    Quantity_NOC_LINEN,
    Quantity_NOC_MAGENTA1,
    Quantity_NOC_MAGENTA4,
    Quantity_NOC_MAROON1,
    Quantity_NOC_MAROON4,
    Quantity_NOC_MEDIUMAQUAMARINE,
    Quantity_NOC_MEDIUMORCHID1,
    Quantity_NOC_MEDIUMORCHID4,
    Quantity_NOC_MEDIUMPURPLE1,
    Quantity_NOC_MEDIUMPURPLE4,
    Quantity_NOC_MEDIUMSEAGREEN,
    Quantity_NOC_MEDIUMSLATEBLUE,
    Quantity_NOC_MEDIUMSPRINGGREEN,
    Quantity_NOC_MEDIUMTURQUOISE,
    Quantity_NOC_MEDIUMVIOLETRED,
    Quantity_NOC_MIDNIGHTBLUE,
    Quantity_NOC_MINTCREAM,
    Quantity_NOC_MISTYROSE,
    Quantity_NOC_MISTYROSE4,
    Quantity_NOC_MOCCASIN,
    Quantity_NOC_NAVAJOWHITE1,
    Quantity_NOC_NAVAJOWHITE4,
    Quantity_NOC_NAVYBLUE,
    Quantity_NOC_OLDLACE,
    Quantity_NOC_OLIVEDRAB1,
    Quantity_NOC_OLIVEDRAB4,
    Quantity_NOC_ORANGE1,
    Quantity_NOC_ORANGE4,
    Quantity_NOC_ORANGERED1,
    Quantity_NOC_ORANGERED4,
    Quantity_NOC_ORCHID1,
    Quantity_NOC_ORCHID4,
    Quantity_NOC_PALEGOLDENROD,
    Quantity_NOC_PALEGREEN1,
    Quantity_NOC_PALEGREEN4,
    Quantity_NOC_PALETURQUOISE1,
    Quantity_NOC_PALETURQUOISE4,
    Quantity_NOC_PALEVIOLETRED1,
    Quantity_NOC_PALEVIOLETRED4,
    Quantity_NOC_PAPAYAWHIP,
    Quantity_NOC_PEACHPUFF,
    Quantity_NOC_PEACHPUFF4,
    Quantity_NOC_PERU,
    Quantity_NOC_PINK1,
    Quantity_NOC_PINK4,
    Quantity_NOC_PLUM1,
    Quantity_NOC_PLUM4,
    Quantity_NOC_POWDERBLUE,
    Quantity_NOC_PURPLE1,
    Quantity_NOC_PURPLE4,
    Quantity_NOC_RED1,
    Quantity_NOC_RED4,
    Quantity_NOC_ROSYBROWN1,
    Quantity_NOC_ROSYBROWN4,
    Quantity_NOC_ROYALBLUE1,
    Quantity_NOC_ROYALBLUE4,
    Quantity_NOC_SADDLEBROWN,
    Quantity_NOC_SALMON1,
    Quantity_NOC_SALMON4,
    Quantity_NOC_SANDYBROWN,
    Quantity_NOC_SEAGREEN1,
    Quantity_NOC_SEAGREEN4,
    Quantity_NOC_SEASHELL,
    Quantity_NOC_SEASHELL4,
    Quantity_NOC_BEET,
    Quantity_NOC_TEAL,
    Quantity_NOC_SIENNA1,
    Quantity_NOC_SIENNA4,
    Quantity_NOC_SKYBLUE1,
    Quantity_NOC_SKYBLUE4,
    Quantity_NOC_SLATEBLUE1,
    Quantity_NOC_SLATEBLUE4,
    Quantity_NOC_SLATEGRAY1,
    Quantity_NOC_SLATEGRAY4,
    Quantity_NOC_SPRINGGREEN,
    Quantity_NOC_SPRINGGREEN4,
    Quantity_NOC_STEELBLUE1,
    Quantity_NOC_STEELBLUE4,
    Quantity_NOC_TAN1,
    Quantity_NOC_TAN4,
    Quantity_NOC_THISTLE1,
    Quantity_NOC_THISTLE4,
    Quantity_NOC_TOMATO1,
    Quantity_NOC_TOMATO4,
    Quantity_NOC_TURQUOISE1,
    Quantity_NOC_TURQUOISE4,
    Quantity_NOC_VIOLET,
    Quantity_NOC_VIOLETRED1,
    Quantity_NOC_VIOLETRED4,
    Quantity_NOC_WHEAT1,
    Quantity_NOC_WHEAT4,
    Quantity_NOC_WHITESMOKE,
    Quantity_NOC_YELLOW1,
    Quantity_NOC_YELLOW4,
    Quantity_NOC_YELLOWGREEN
};

int Document::s_maxColor = sizeof(Document::s_colors) / sizeof(Quantity_NameOfColor);
Graphic3d_NameOfMaterial Document::s_material = Graphic3d_NOM_ALUMINIUM;

static Handle(Graphic3d_GraphicDriver) graphicDriver;

Document::Document(const QString& title, Application* app):
    QObject(app),
    m_title(title),
    m_colorNum(0)
{
    if (graphicDriver.IsNull()) {
        Handle(Aspect_DisplayConnection) display = new Aspect_DisplayConnection;
        graphicDriver = new OpenGl_GraphicDriver(display);
    }

    m_viewer = new V3d_Viewer(graphicDriver, (const short*)"viewer");
    m_viewer->SetDefaultLights();
    m_viewer->SetLightOn();

    app->ocafApp()->NewDocument("MDTV-XCAF", m_ocafDoc);
    TPrsStd_AISViewer::New(m_ocafDoc->Main(), m_viewer);
    context()->SetDisplayMode(AIS_Shaded);

    m_shapeTool = XCAFDoc_DocumentTool::ShapeTool(m_ocafDoc->Main());

    m_view = new OccView(this);

    initActions();
}

Document::~Document()
{
}

QString Document::title()
{
    return m_title;
}

OccView* Document::view()
{
    return m_view;
}

QActionGroup *Document::modelingActions()
{
    return m_actions;
}

Handle(V3d_Viewer) Document::viewer()
{
    return context()->CurrentViewer();
}

Handle(AIS_InteractiveContext) Document::context()
{
    Handle(AIS_InteractiveContext) context;
    TPrsStd_AISViewer::Find(m_ocafDoc->Main(), context);
    return context;
}

Handle(TDocStd_Document) Document::ocafDoc()
{
    return m_ocafDoc;
}

Handle(TDocStd_Application) Document::ocafApp()
{
    return Handle(TDocStd_Application)::DownCast(m_ocafDoc->Application());
}

void Document::display(const TopoDS_Shape& shape)
{
    insert(shape);
    context()->UpdateCurrentViewer();
}

void Document::display(const Handle(TopTools_HSequenceOfShape)& shapes)
{
    if(shapes.IsNull() || !shapes->Length()) return;

    for(int i=1; i<=shapes->Length(); i++)
        insert(shapes->Value(i));

    context()->UpdateCurrentViewer();
}

void Document::createBottle()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    TopoDS_Shape bottle = MakeBottle(50, 70, 30);
    display(bottle);
    QApplication::restoreOverrideCursor();
}

void Document::createCylinder()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_ocafDoc->NewCommand();
    CylinderCommand cmd(m_ocafDoc->Main());
    TDF_Label label = cmd.createCylinder(0, 0, 0, 10, 50, "Cylindre");
    Handle(TPrsStd_AISPresentation) prs = TPrsStd_AISPresentation::Set(label, TNaming_NamedShape::GetID());
    prs->SetMaterial(s_material);
    prs->SetColor(nextColor());
    prs->Display(1);
    context()->UpdateCurrentViewer();
    m_ocafDoc->CommitCommand();
    QApplication::restoreOverrideCursor();
}

void Document::createSphere()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_ocafDoc->NewCommand();
    SphereCommand cmd(m_ocafDoc->Main());
    TDF_Label label = cmd.createSphere(0, 0, 0, 0.5, "Sphere");
    Handle(TPrsStd_AISPresentation) prs = TPrsStd_AISPresentation::Set(label, TNaming_NamedShape::GetID());
    prs->SetMaterial(s_material);
    prs->SetColor(nextColor());
    prs->Display(1);
    context()->UpdateCurrentViewer();
    m_ocafDoc->CommitCommand();
    QApplication::restoreOverrideCursor();
}

void Document::createCut()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    m_ocafDoc->NewCommand();
    CutCommand cmd(m_ocafDoc->Main());

    CylinderCommand cylinCmd(m_ocafDoc->Main());
    TDF_Label base = cylinCmd.createCylinder(0, 10, -30, 10, 60, "Cylinder");
    Handle(TPrsStd_AISPresentation) prsBase = TPrsStd_AISPresentation::Set(base, TNaming_NamedShape::GetID());
    prsBase->SetMaterial(s_material);
    prsBase->SetColor(nextColor());
    prsBase->Display(1);
    m_view->fitAll();
    Application::wait(500);

    SphereCommand sphereCmd(m_ocafDoc->Main());
    TDF_Label tool = sphereCmd.createSphere(0, 0, 0, 15, "Sphere");
    Handle(TPrsStd_AISPresentation) prsTool = TPrsStd_AISPresentation::Set(tool, TNaming_NamedShape::GetID());
    prsTool->SetMaterial(s_material);
    prsTool->SetColor(nextColor());
    prsTool->Display(1);
    m_view->fitAll();
    Application::wait(1000);

    TDF_Label cut = cmd.createCut(base, tool, "Cut");
    Handle(TPrsStd_AISPresentation) prsCut = TPrsStd_AISPresentation::Set(cut, TNaming_NamedShape::GetID());
    prsCut->SetMaterial(s_material);
    prsCut->SetColor(nextColor());
    prsCut->SetMode(AIS_Shaded);

    prsBase->Erase();
    prsTool->Erase();
    prsCut->Display(1);
    prsCut->SetDisplayed(Standard_True);
    context()->UpdateCurrentViewer();

    m_ocafDoc->CommitCommand();
    QApplication::restoreOverrideCursor();
}

bool Document::open(const QString& file)
{
    bool success = false;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString ext = QFileInfo(file).suffix();

    if (ext == "igs" || ext == "iges") {
        IGESCAFControl_Reader reader;
        IFSelect_ReturnStatus  status = reader.ReadFile(file.toUtf8().data());
        success =  (status == IFSelect_RetDone);

        if(!success)
            emit error(QFileInfo(file).fileName() + " not open");
        else {
            reader.Transfer(m_ocafDoc);
            loadShapes();
        }
    } else if (ext == "stp" || ext == "step") {
        STEPCAFControl_Reader reader;
        IFSelect_ReturnStatus  status = reader.ReadFile(file.toUtf8().data());
        success =  (status == IFSelect_RetDone);

        if(!success)
            emit error(QFileInfo(file).fileName() + " not open");
        else {
            reader.Transfer(m_ocafDoc);
            loadShapes();
        }
    } else {
        PCDM_ReaderStatus status = ocafApp()->Open(file.toUtf8().data(), m_ocafDoc);
        success =  (status == PCDM_RS_OK);

        if (!success)
            emit error(QFileInfo(file).fileName() + " not open");
        else {
            TPrsStd_AISViewer::New(m_ocafDoc->Main(), m_viewer);
            context()->SetDisplayMode(AIS_Shaded);
            displayPrs();
        }
    }

    QApplication::restoreOverrideCursor();
    return success;
}

bool Document::import(const QString& file)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
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

    QApplication::restoreOverrideCursor();

    return (!shapes.IsNull() && shapes->Length());
}

bool Document::saveAs(const QString &file)
{
    QString ext = QFileInfo(file).suffix();
    Handle(TopTools_HSequenceOfShape) shapes;

    if(ext == "dxc") {
        m_ocafDoc->ChangeStorageFormat("MDTV-XCAF");
    } else if(ext == "xml") {
        m_ocafDoc->ChangeStorageFormat("XmlXCAF");
    } else if(ext == "xbf") {
        m_ocafDoc->ChangeStorageFormat("BinXCAF");
    } else if(ext == "std") {
        m_ocafDoc->ChangeStorageFormat("MDTV-Standard");
    } else if(ext == "cbf") {
        m_ocafDoc->ChangeStorageFormat("BinOcaf");
    } else {
        emit error(QFileInfo(file).fileName() + " unknown file type");
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    TCollection_ExtendedString statusMsg;
    PCDM_StoreStatus status = ocafApp()->SaveAs(m_ocafDoc, file.toUtf8().data(), statusMsg);

    if(status != PCDM_SS_OK)
        emit error(QFileInfo(file).fileName() + " not saved.\n" + QString::fromUtf16((const ushort*)statusMsg.ToExtString()));

    QApplication::restoreOverrideCursor();

    return (status == PCDM_SS_OK);
}

void Document::insert(const TopoDS_Shape& shape)
{
    Handle(AIS_Shape) ais = new AIS_Shape(shape);
    context()->Display(ais, false);
    context()->SetMaterial(ais, s_material);
    context()->SetColor(ais, nextColor());
}

void Document::initActions()
{
    m_actions = new QActionGroup(this);

    QAction* createBottle = new QAction(QPixmap(":/icons/Bottle.png"), "&Bottle", m_actions);
    connect(createBottle, &QAction::triggered, this, &Document::createBottle);

    QAction* createCylinder = new QAction(QPixmap(":/icons/Cylinder.png"), "&Cylinder", m_actions);
    connect(createCylinder, &QAction::triggered, this, &Document::createCylinder);

    QAction* createSphere = new QAction(QPixmap(":/icons/Sphere.png"), "&Sphere", m_actions);
    connect(createSphere, &QAction::triggered, this, &Document::createSphere);

    QAction* createCut = new QAction(QPixmap(":/icons/Cut.png"), "C&ut", m_actions);
    connect(createCut, &QAction::triggered, this, &Document::createCut);
}

void Document::displayPrs()
{
    for(TDF_ChildIterator it(m_ocafDoc->Main()); it.More(); it.Next()) {
        Handle(TPrsStd_AISPresentation) prs;
        if(!it.Value().FindAttribute(TPrsStd_AISPresentation::GetID(), prs)) continue;
        if(prs->IsDisplayed())
            prs->Display(1);
    }

    context()->UpdateCurrentViewer();
}

Quantity_NameOfColor Document::nextColor()
{
    Quantity_NameOfColor color = s_colors[m_colorNum++];
    m_colorNum %= s_maxColor;
    return color;
}

void Document::loadShapes()
{
    // collect sequence of labels to display
    TDF_LabelSequence shapeLabels;
    m_shapeTool->GetFreeShapes(shapeLabels);

    // set presentations and show
    for (Standard_Integer i = 1; i <= shapeLabels.Length(); i++) {
        // get the shapes and attributes
        const TDF_Label& label = shapeLabels.Value(i);
        loadShapes(label);
    }
}

void Document::loadShapes(const TDF_Label& label)
{
    TopoDS_Shape shape;
    if (m_shapeTool->GetShape(label, shape)) {
        TNaming_Builder builder(label);
        builder.Generated(shape);
        Handle(TPrsStd_AISPresentation) prs = TPrsStd_AISPresentation::Set(label, TNaming_NamedShape::GetID());
        prs->SetMaterial(s_material);
        prs->SetColor(nextColor());
        prs->Display(1);

        if (label.HasChild()) {
            TDF_ChildIterator it;
            for (it.Initialize(label); it.More(); it.Next()) {
                loadShapes(it.Value());
            }
        }
    }
}
