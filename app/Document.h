#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_HSequenceOfShape.hxx>

class OccView;
class QWidget;

class Document : public QObject
{
    Q_OBJECT

public:
    explicit Document(const QString& title, QWidget* parent=0);
    QString title();
    OccView* view();

    void display(const TopoDS_Shape& shape);
    void display(const Handle(TopTools_HSequenceOfShape)& shapes);

    void makeBottle();
    void import(const QString& file);

signals:
    void error(const QString& msg);

private:
    void insert(const TopoDS_Shape& shape);

    QString m_title;
    Handle(V3d_Viewer) m_viewer;
    Handle(AIS_InteractiveContext) m_context;
    OccView* m_view;
    int m_colorNum;

    static Quantity_Color s_colors[];
    static int s_maxColor;
};

#endif // DOCUMENT_H
