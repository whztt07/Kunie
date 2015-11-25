#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "TopTools_HSequenceOfShape.hxx"

class QString;

class Translator
{
public:
    static Handle(TopTools_HSequenceOfShape) importBREP(const QString& file);
};

#endif // TRANSLATOR_H
