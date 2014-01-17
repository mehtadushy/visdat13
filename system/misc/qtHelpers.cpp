//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "qtHelpers.h"
//---------------------------------------------------------------------------
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtGui/QMessageBox>
#include <QtGui/QBitmap>
//---------------------------------------------------------------------------



map<QString,QPixmap*> QTHelpers::m_Pixmaps;

string QTHelpers::toSTLString(const QString &str)
{
	if( str.isEmpty() )
		return "";
	else
		return string(str.toLatin1().constData());
}

QString QTHelpers::toQString(const std::string &str)
{
	return QString( str.c_str() );
}

QPixmap * QTHelpers::createPixmap(const QString &filename)
{
	QString id = QString("T") + filename;
	if( m_Pixmaps.find( id ) != m_Pixmaps.end() )
		return m_Pixmaps[id];

	QPixmap * pixmap = new QPixmap( filename );
	QImage image = pixmap->toImage();
	QColor transpColor = image.pixel(0,0);
	pixmap->setMask( pixmap->createMaskFromColor( transpColor ) );

	m_Pixmaps[id]=pixmap;

	return pixmap;
}

void QTHelpers::cleanup()
{
	for( map<QString,QPixmap*>::iterator iter = m_Pixmaps.begin(); iter != m_Pixmaps.end(); ++iter )
		delete iter->second;
	m_Pixmaps.clear();
}

bool QTHelpers::question( QWidget * parent, const QString &question_str )
{
	return QMessageBox::question(parent,"question", question_str, QMessageBox::Yes,QMessageBox::No ) == QMessageBox::Yes;
}
