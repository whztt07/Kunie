#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <TDocStd_Document.hxx>

class Application;
class OccView;

class Document : public QObject
{
    Q_OBJECT

public:
    QString title();
    OccView* view();

    Handle(TDocStd_Document) ocafDoc();
    Handle(V3d_Viewer) viewer();

    void display(const TopoDS_Shape& shape);
    void display(const Handle(TopTools_HSequenceOfShape)& shapes);

    void createBottle();
    void createCylinder(double x, double y, double z, double r, double h);
    void import(const QString& file);

signals:
    void error(const QString& msg);

private:
    explicit Document(const QString& title, Application* app);
    ~Document();


    Handle(AIS_InteractiveContext) context();
    void insert(const TopoDS_Shape& shape);

    QString m_title;
    Handle(TDocStd_Document) m_document;
    OccView* m_view;
    int m_colorNum;

    static Quantity_Color s_colors[];
    static int s_maxColor;

    friend class Application;
};

#endif // DOCUMENT_H
