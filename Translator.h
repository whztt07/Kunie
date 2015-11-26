#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "TopTools_HSequenceOfShape.hxx"

class QString;

class Translator
{
public:
    static Handle(TopTools_HSequenceOfShape) importBREP(const QString& file);
    static Handle(TopTools_HSequenceOfShape) importIGES(const QString& file);
    static Handle(TopTools_HSequenceOfShape) importSTEP(const QString& file);
    static Handle(TopTools_HSequenceOfShape) importCSFDB(const QString& file);
    static Handle(TopTools_HSequenceOfShape) importSTL(const QString& file);
};

#endif // TRANSLATOR_H
