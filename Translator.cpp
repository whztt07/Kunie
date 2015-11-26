#include "Translator.h"

#include <QString>
#include <TopoDS_Shape.hxx>
#include <BRep_Builder.hxx>
#include <TCollection_AsciiString.hxx>
#include <BRepTools.hxx>
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include <FSD_File.hxx>
#include <ShapeSchema.hxx>
#include <Storage_Data.hxx>
#include <Storage_HSeqOfRoot.hxx>
#include <Storage_Root.hxx>
#include <PTopoDS_HShape.hxx>
#include <PTColStd_PersistentTransientMap.hxx>
#include <MgtBRep.hxx>
#include <StlAPI_Reader.hxx>

Handle(TopTools_HSequenceOfShape) Translator::importBREP(const QString& file)
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

Handle(TopTools_HSequenceOfShape) Translator::importIGES(const QString& file)
{
    Handle(TopTools_HSequenceOfShape) sequence;
    TCollection_AsciiString filePath = file.toUtf8().data();

    IGESControl_Reader reader;
    int status = reader.ReadFile(filePath.ToCString());

    if(status == IFSelect_RetDone) {
        sequence = new TopTools_HSequenceOfShape();
        reader.TransferRoots();
        TopoDS_Shape shape = reader.OneShape();
        sequence->Append(shape);
    }

    return sequence;
}

Handle(TopTools_HSequenceOfShape) Translator::importSTEP(const QString& file)
{
    Handle(TopTools_HSequenceOfShape) sequence;
    TCollection_AsciiString  filePath = file.toUtf8().data();

    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filePath.ToCString());

    if(status == IFSelect_RetDone) {
        bool failsonly = false;
        reader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);

        int nbr = reader.NbRootsForTransfer();
        reader.PrintCheckTransfer(failsonly, IFSelect_ItemsByEntity);

        for(Standard_Integer n=1; n<=nbr; n++) {
            bool ok = reader.TransferRoot(n);
            int nbs = reader.NbShapes();
            if(ok==true && nbs>0) {
                sequence = new TopTools_HSequenceOfShape();
                for(int i=1; i<=nbs; i++) {
                    TopoDS_Shape shape = reader.Shape(i);
                    sequence->Append(shape);
                }
            }
        }
    }

    return sequence;
}

Handle(TopTools_HSequenceOfShape) Translator::importCSFDB(const QString& file)
{
    Handle(TopTools_HSequenceOfShape) sequence;

    // Check file type
    if(FSD_File::IsGoodFileType((Standard_CString)file.toLatin1().constData()) != Storage_VSOk)
        return sequence;

    FSD_File fileDriver;
    TCollection_AsciiString name((Standard_CString)file.toLatin1().constData());
    if(fileDriver.Open(name, Storage_VSRead) != Storage_VSOk)
        return sequence;

    Handle(ShapeSchema) schema = new ShapeSchema();
    Handle(Storage_Data) data = schema->Read(fileDriver);
    if(data->ErrorStatus() != Storage_VSOk)
        return sequence;

    fileDriver.Close();

    sequence = new TopTools_HSequenceOfShape();
    Handle(Storage_HSeqOfRoot) roots = data->Roots();
    for(int i=1; i<=roots->Length(); i++) {
        Handle(Storage_Root) r = roots->Value(i);
        Handle(Standard_Persistent) p = r->Object();
        Handle(PTopoDS_HShape) pShape = Handle(PTopoDS_HShape)::DownCast(p);

        if(!pShape.IsNull()) {
            PTColStd_PersistentTransientMap map;
            TopoDS_Shape tShape;
            MgtBRep::Translate(pShape, map, tShape, MgtBRep_WithTriangle);
            sequence->Append(tShape);
        }
    }

    return sequence;
}

Handle(TopTools_HSequenceOfShape) Translator::importSTL(const QString& file)
{
    Handle(TopTools_HSequenceOfShape) sequence;
    TCollection_AsciiString filePath = file.toUtf8().data();

    StlAPI_Reader reader;
    TopoDS_Shape shape;
    reader.Read(shape, filePath.ToCString());

    sequence = new TopTools_HSequenceOfShape();
    sequence->Append(shape);

    return sequence;
}
