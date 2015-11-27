#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>

class OccView;

class Document : public QObject
{
    Q_OBJECT

public:
    explicit Document(QObject *parent=0);
    OccView* view();

    void display(const TopoDS_Shape& shape);
    void display(const Handle(TopTools_HSequenceOfShape)& shapes);

    void makeBottle();
    void import(const QString& file);

signals:
    void error(const QString& msg);

private:
    Handle(V3d_Viewer) m_viewer;
    Handle(AIS_InteractiveContext) m_context;
    OccView* m_view;
};

#endif // DOCUMENT_H
