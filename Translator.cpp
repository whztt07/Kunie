#include "Translator.h"

#include <QString>
#include <TopoDS_Shape.hxx>
#include <BRep_Builder.hxx>
#include <TCollection_AsciiString.hxx>
#include <BRepTools.hxx>

Handle(TopTools_HSequenceOfShape) Translator::importBREP(const QString &file)
{
    Handle(TopTools_HSequenceOfShape) sequence;
    TopoDS_Shape shape;
    BRep_Builder builder;

    TCollection_AsciiString  filePath = file.toUtf8().data();
    Standard_Boolean result = BRepTools::Read(shape, filePath.ToCString(), builder);

    if(result) {
        sequence = new TopTools_HSequenceOfShape();
        sequence->Append(shape);
    }

    return sequence;
}

