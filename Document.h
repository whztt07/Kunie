#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>

class QWidget;
class OccView;

class Document : public QObject
{
public:
    explicit Document(QObject *parent=0);
    QWidget* createView(QWidget* parent=0, Qt::WindowFlags flags=0);

    void onMakeBottle();

private:
    Handle(V3d_Viewer) m_viewer;
    Handle(AIS_InteractiveContext) m_context;
    OccView* m_view;
};

#endif // DOCUMENT_H
