//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "SeparatorClassProperty.h"
//---------------------------------------------------------------------------
#include "SeparatorPropertyEditor.h"
//---------------------------------------------------------------------------


IMPLEMENT_GEOX_CLASS( SeparatorClassProperty ,0)
{
	BEGIN_CLASS_INIT( SeparatorClassProperty );
}

SeparatorClassProperty::SeparatorClassProperty(const string name)
: SingleClassProperty(name, 0) {
	addPropertyEditor(SeparatorPropertyEditor::getClass(), true);
}

ClassProperty* SeparatorClassProperty::copyClassProperty() {
	return new SeparatorClassProperty(getCName());
}



