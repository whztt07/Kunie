#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>

class Application;
class OccView;
class QActionGroup;

class Document : public QObject
{
    Q_OBJECT

public:
    QString title();
    OccView* view();

    Handle(TDocStd_Document) ocafDoc();
    Handle(TDocStd_Application) ocafApp();
    Handle(V3d_Viewer) viewer();
    Handle(AIS_InteractiveContext) context();

    QActionGroup* modelingActions();

    void display(const TopoDS_Shape& shape);
    void display(const Handle(TopTools_HSequenceOfShape)& shapes);

    void createBottle();
    void createCylinder();
    void createSphere();
    void createCut();
    void open(const QString& file);
    void import(const QString& file);
    void saveAs(const QString& file);

signals:
    void error(const QString& msg);

private:
    explicit Document(const QString& title, Application* app);
    ~Document();

    void insert(const TopoDS_Shape& shape);
    void initActions();
    void displayPrs();
    Quantity_NameOfColor nextColor();

    QString m_title;
    Handle(TDocStd_Document) m_document;
    Handle(V3d_Viewer) m_viewer;
    OccView* m_view;
    int m_colorNum;

    QActionGroup* m_actions;

    static Graphic3d_NameOfMaterial s_material;
    static Quantity_NameOfColor s_colors[];
    static int s_maxColor;

    friend class Application;
};

#endif // DOCUMENT_H
