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
#include <VrmlData_Scene.hxx>
#include <VrmlData_DataMapOfShapeAppearance.hxx>

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

Handle(TopTools_HSequenceOfShape) Translator::importVRML(const QString& file)
{
    Handle(TopTools_HSequenceOfShape) sequence;
    TCollection_AsciiString filePath = file.toUtf8().data();

    VrmlData_Scene vrmlScene;
    filebuf fb;

    fb.open(filePath.ToCString(), ios::in);
    if(fb.is_open() == false)
        return sequence;

    Standard_IStream input(&fb);

    vrmlScene << input;
    fb.close();

    if(vrmlScene.Status() != VrmlData_StatusOK)
        return sequence;

    VrmlData_DataMapOfShapeAppearance M;
    TopoDS_Shape shape = vrmlScene.GetShape(M);

    if(shape.IsNull()) return sequence;

    sequence = new TopTools_HSequenceOfShape();
    sequence->Append(shape);

    return sequence;
}

/*
bool OCCViewer::ExportVrml2(char* filename)
{
    TraceDebugIndent("Writing VRML 2.0 file...");
    TraceDebugFormattedStr("File: ", "%s", ".", filename);

    VrmlData_Scene vrmlScene;
    VrmlData_ShapeConvert shapeConvert(vrmlScene);

    TraceDebug("Grouping objects to export...");
    int iShape = 0;
    for ( myAISContext->InitCurrent(); myAISContext->MoreCurrent(); myAISContext->NextCurrent() )
    {
        Handle_AIS_InteractiveObject anIO = myAISContext->Current();
        Handle_AIS_Shape anIS = Handle_AIS_Shape::DownCast(anIO);

        if(anIS.IsNull() == Standard_False)
        {
            TopoDS_Shape shape = anIS->Shape();
            if ( shape.IsNull() )
            {
                TraceDebugUnIndent("Failed to export VRML 2.0 file. Couldn't access object.");
                return false;
            }

            Quantity_Color color = myAISContext->Current()->Color();
            Standard_Real transparency = myAISContext->Current()->Transparency();

            // Give a name to the shape.
            TCollection_AsciiString name("Shape");
            name += TCollection_AsciiString(iShape++);
            shapeConvert.AddShape(shape, name.ToCString());

            // Check presence of faces in the shape.
            TopExp_Explorer expl(shape, TopAbs_FACE);
            if (expl.More())
                shapeConvert.Convert(true, false, 0.01); // faces only
            else
                shapeConvert.Convert(false, true, 0.01); // edges only

            // Name of the color & transparency.
            // It will be uniquely saved in VRML file.
            TCollection_AsciiString cname = Quantity_Color::StringName(color.Name());
            cname += transparency;

            // Make the appearance (VRML attribute)
            Handle(VrmlData_Appearance) appearance = Handle(VrmlData_Appearance)::DownCast(vrmlScene.FindNode(cname.ToCString()));
            if (appearance.IsNull())
            {
                // Not found ... create a new one.
                Handle(VrmlData_Material) material = new VrmlData_Material(vrmlScene, cname.ToCString(), 0.2, 0.2, transparency);
                material->SetDiffuseColor(color);
                material->SetEmissiveColor(color);
                material->SetSpecularColor(color);
                vrmlScene.AddNode(material, false);
                appearance = new VrmlData_Appearance(vrmlScene, cname.ToCString());
                appearance->SetMaterial(material);
                vrmlScene.AddNode(appearance, false);
            }

            // Apply the material to the shape of entity.
            Handle(VrmlData_Group) group = Handle(VrmlData_Group)::DownCast(vrmlScene.FindNode(name.ToCString()));
            if (!group.IsNull())
            {
                VrmlData_ListOfNode::Iterator itr = group->NodeIterator();
                for (; itr.More(); itr.Next())
                {
                    Handle(VrmlData_Node) node = itr.Value();
                    if (node->DynamicType() == STANDARD_TYPE(VrmlData_ShapeNode))
                    {
                        Handle(VrmlData_ShapeNode) shape = Handle(VrmlData_ShapeNode)::DownCast(node);
                        shape->SetAppearance(appearance);
                    }
                    else if (itr.Value()->DynamicType() == STANDARD_TYPE(VrmlData_Group))
                    {
                        Handle(VrmlData_Group) groupc = Handle(VrmlData_Group)::DownCast(itr.Value());
                        VrmlData_ListOfNode::Iterator itrc = groupc->NodeIterator();
                        for (; itrc.More(); itrc.Next())
                        {
                            Handle(VrmlData_Node) nodec = itrc.Value();
                            if (nodec->DynamicType() == STANDARD_TYPE(VrmlData_ShapeNode))
                            {
                                Handle(VrmlData_ShapeNode) shapec = Handle(VrmlData_ShapeNode)::DownCast(nodec);
                                shapec->SetAppearance(appearance);
                            }
                        } // for of group nodes...
                    } // if (it is a shape node...
                } // for of group nodes...
            } // if (!group.IsNull...
        }
        else
            TraceWarning("Impossible to gain object representation.");
    }

    // Call VRML writer
    Standard_CString aFileName = (Standard_CString) filename;
    ofstream writer(aFileName);
    if(writer.is_open() == false)
    {
        TraceWarning("Problem exporting file: could not open file buffer.");
        TraceDebugUnIndent("Failed to process the file.");
        return false;
    }
    writer writer.close();

    TraceDebugUnIndent("VRML 2.0 file successfully exported.");
    return true;
}
*/
