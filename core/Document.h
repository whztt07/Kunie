#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>
#include <XCAFDoc_ShapeTool.hxx>

class Application;
class OccView;
class QTreeWidget;
class QActionGroup;

class Document : public QObject
{
    Q_OBJECT

public:
    QString name();
    QString path();
    OccView* view();
    QTreeWidget* tree();

    Handle(TDocStd_Document) ocafDoc();
    Handle(TDocStd_Application) ocafApp();
    Handle(V3d_Viewer) viewer();
    Handle(AIS_InteractiveContext) context();

    QActionGroup* modelingActions();

    bool open(const QString& file);
    bool import(const QString& file);
    bool save();
    bool saveAs(const QString& file);

    void createBottle();
    void createCylinder();
    void createSphere();
    void createCut();

    void display(const TopoDS_Shape& shape);
    void display(const Handle(TopTools_HSequenceOfShape)& shapes);

signals:
    void error(const QString& msg);

private:
    Document(Handle(TDocStd_Document) doc);
    ~Document();

    void insert(const TopoDS_Shape& shape);
    void initActions();
    void displayPrs();
    Quantity_NameOfColor nextColor();
    void loadShapes();
    void loadShapes(const TDF_Label& label);

    Handle(TDocStd_Document) m_ocafDoc;
    Handle(XCAFDoc_ShapeTool) m_shapeTool;
    Handle(V3d_Viewer) m_viewer;
    OccView* m_view;
    QTreeWidget* m_tree;
    int m_colorIndex;

    QActionGroup* m_actions;

    static Graphic3d_NameOfMaterial s_material;

    friend class Application;
};

#endif // DOCUMENT_H
