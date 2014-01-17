//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "VerticalScrollView.h"
//---------------------------------------------------------------------------



VerticalScrollView::VerticalScrollView(QWidget * parent)
{
  m_Content = NULL;
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setWidgetResizable(true);
}

void VerticalScrollView::resizeEvent( QResizeEvent * e )
{
  m_Content = viewport()->childAt(1,1);
  if( m_Content == NULL )
    return;

  int content_height = m_Content->height();
  int scrollbarWidth = 20;

  if( content_height+3 > height() )
  {
    m_Content->resize( width() - scrollbarWidth, content_height );
  }
  else
  {
    m_Content->resize( width(), content_height );
  }
  QScrollArea::resizeEvent(e);
}

#ifndef CMAKE_INTDIR
#ifdef _WIN32
#include "moc/VerticalScrollView_moc.cpp"
#else
#ifndef CMAKE_UNIX_INCLUDE_MOC
#include "moc_VerticalScrollView.cpp"
#endif
#endif
#endif
