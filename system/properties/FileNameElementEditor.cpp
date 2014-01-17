//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "FileNameElementEditor.h"
//---------------------------------------------------------------------------
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QFont>
#include "ClassProperty.h"
#include "StringHelper.h"
#include "FileDialogs.h"
//---------------------------------------------------------------------------



FileNameElementEditor::FileNameElementEditor(QWidget * parent, Qt::WFlags f)
: ElementEditor(parent, f)
{
  m_Edit = new QLineEdit( "",this );
  m_dotdotdot = new QPushButton("...", this);
  m_dotdotdot->setMaximumWidth(16);
  QFont fnt = m_dotdotdot->font();
  fnt.setWeight(QFont::Bold);
  fnt.setFamily("Tahoma");
  m_dotdotdot->setFont(fnt);
  connect( m_Edit, SIGNAL(textChanged(const QString&)), SLOT(sltTextChanged(const QString&)) );
  connect( m_Edit, SIGNAL(returnPressed()), SLOT(sltReturnPressed()) );
  connect( m_dotdotdot, SIGNAL(clicked()), SLOT(sltDotDotDotClicked()) );
  QHBoxLayout * layout = new QHBoxLayout( this );
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_Edit);
  layout->addWidget(m_dotdotdot);
  setMinimumHeight(30);
  setMaximumHeight(30);
}

FileNameElementEditor::~FileNameElementEditor() {}


void FileNameElementEditor::setup(Object *obj, pointer_diff offset, ClassProperty *prop, bool readOnly)
{
  m_Object = obj;
  m_Property = prop;
  setOffset( offset );
  m_Edit->setEnabled( !readOnly );
  updateEditor();
}

void FileNameElementEditor::updateEditor()
{
  string newStr = m_Property->toString(m_Object,getOffset());
  if (newStr != qString2STLString(m_Edit->text())) {
    m_Edit->setText( QString( newStr.c_str() ) );
  }
}

void FileNameElementEditor::sltTextChanged(const QString &text)
{
  if( ! text.isEmpty() )
  {
    m_Property->fromString( m_Object, getOffset(), text.toLatin1().constData() );
    modified();
  }
}

void FileNameElementEditor::sltReturnPressed()
{
  updateEditor();
}

void FileNameElementEditor::sltDotDotDotClicked()
{
  string newText = FileDialogs::getOpenFileName(
    this, "enter filename",
      "*.*", NULL, m_Property->toString(m_Object,getOffset()),
      FileDialogs::IMPORT_DIR
   );
   if (newText != "") {
    m_Edit->setText(QString(newText.c_str()));
    sltTextChanged(QString(newText.c_str()));
   }
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/FileNameElementEditor_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_FileNameElementEditor.cpp"
#endif
#endif
#endif
