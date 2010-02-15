/***************************************************************************
                          seriesIconView.cc  -  description
                             -------------------
    begin                : 1999
    copyright            : (C) 2001 by Denis RiviÃ¯Â¿Â½e
    email                : nudz@free.fr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <roll/gui/seriesIconView.h>
#include <roll/gui/mainWin.h>
#include <roll/game/vars.h>
#include <roll/struct/series.h>
#include <qcursor.h>

using namespace roll;
using namespace std;


LevelsDrag::LevelsDrag( QWidget * dragSource, const char * name )
  : QIconDrag( dragSource, name )
{
}


LevelsDrag::~LevelsDrag()
{
  //cout << "deleting clipboard levels...\n";
  // cleanup stored levels
  /*map<unsigned, SimpleLevel*>::iterator	il, el = _levels.end();
  for( il=_levels.begin(); il!=el; ++il )
  delete il->second;*/
}


const char* LevelsDrag::format ( int i ) const
{
  if( i == 0 )
    return( "application/x-qiconlist" );
  else if( i == 1 )
    return( "Rock'n'Roll/levels-list" );
  else
    return( 0 );
}


bool LevelsDrag::canDecode( QMimeSource* ms )
{
  return( ms->provides( "application/x-qiconlist" ) ||
	  ms->provides( "Rock'n'Roll/levels-list" ) );
}


bool LevelsDrag::decode( QMimeSource* ms, 
			 map<unsigned, SimpleLevel> & levels )
{
  if( !canDecode( ms ) )
    return( false );

  try	// Microsoft VC++ throws an exception if a dynamic_cast fails
    {
      LevelsDrag	*ld = dynamic_cast<LevelsDrag*>( ms );
      if( ld )
	{
	  levels = ld->_levels;
	  //out << "levels decoded via direct LevelsDrag\n";
	  return( true );
	}
    }
  catch( exception & )
    {
    }

  QByteArray	ba( ms->encodedData( "Rock'n'Roll/levels-list" ) );

  unsigned	inum;
#if QT_VERSION >= 0x040000
  QDataStream	bstr( &ba, IO_ReadOnly );
#else
  QDataStream	bstr( ba, IO_ReadOnly );
#endif

  while( !bstr.atEnd() && bstr.device()->state() == IO_Open 
	 && bstr.device()->status() == IO_Ok 
	 && ba.size() - bstr.device()->at() > 50 )
    {	// under windows, QDataStream end dfoesn't seem correct
      /*out << "qdatastream: at " << bstr.device()->at() << " / " << ba.size() 
	<< endl;*/
      bstr >> inum;
      if( bstr.atEnd() ||  bstr.device()->state() != IO_Open 
	  || bstr.device()->status() != IO_Ok)
	break;	// unser windows atEnd() is not detected earlier
      //out << "decode level " << inum << "..." << endl;

      SimpleLevel	& sl = levels[ inum ];
      if( !sl.read( bstr ) )
	{
	  levels.erase( inum );
	  err << "could not decode level " << inum << endl;
	}
    }

  return( true );
}


void LevelsDrag::append( const QIconDragItem &item, unsigned n,
			 SimpleLevel* sl, const QRect & r1, const QRect & r2 )
{
  //cout << "append level " << sl << " in clipboard\n";
  _levels[ n ] = *sl;
  QIconDrag::append( item, r1, r2 );
}


QByteArray LevelsDrag::encodedData( const char* mime ) const
{
  if( QString( mime ) == "Rock'n'Roll/levels-list" )
    {
      //cout << "encodedData recognized\n";
      QByteArray	ba;
#if QT_VERSION >= 0x040000
      QDataStream	bstr( &ba, IO_WriteOnly );
#else
      QDataStream	bstr( ba, IO_WriteOnly );
#endif
      map<unsigned, SimpleLevel>::const_iterator	il, el = _levels.end();

      for( il=_levels.begin(); il!=el; ++il )
	{
	  bstr << il->first;
	  il->second.write( bstr );
	}

      return( ba );
    }
  else
    return( QIconDrag::encodedData( mime ) );
}


// -------------


SeriesIconView::SeriesIconView( QWidget * parent, const char * name,
                                Qt::WFlags f )
  : QIconView( parent, name, f )
{
  setAcceptDrops( true );
  //connect( this, SIGNAL( moved() ), SLOT( levelsMoved() ) );
}


SeriesIconView::~SeriesIconView()
{
}


QDragObject * SeriesIconView::dragObject()
{
  return( copySelection( this ) );
}


QDragObject * SeriesIconView::copySelection( QWidget* source )
{
  //cout << "SeriesIconView::copySelection( " << source << " )\n";
  vector<unsigned>	lvls;
  LevelsDrag	*ld = new LevelsDrag( source, "levelsDrag" );
  unsigned	i = 0;

  if( currentItem() )
    ld->setPixmap( *currentItem()->pixmap(),
		   QPoint( currentItem()->pixmapRect().width() / 2, 
			   currentItem()->pixmapRect().height() / 2 ) );

  QPoint 
    orig = viewportToContents( viewport()->mapFromGlobal( QCursor::pos() ) );

  for( QIconViewItem *ii = firstItem(); ii; ii=ii->nextItem(), ++i )
    if( ii->isSelected() )
      {
	SimpleLevel		*sl = new SimpleLevel;
	ser->unpack( i, *sl );
	QRect	r1( ii->pixmapRect( false ).x() - orig.x(),
		    ii->pixmapRect( false ).y() - orig.y(),
		    ii->pixmapRect().width(), ii->pixmapRect().height() );
	QRect	r2( ii->textRect( false ).x() - orig.x(),
		    ii->textRect( false ).y() - orig.y(),
		    ii->textRect().width(), ii->textRect().height() );
	QIconDragItem id;
	ld->append( id, i, sl, r1, r2 );
      }

  return( ld );
}


void SeriesIconView::levelsMoved( unsigned insertedpos, 
				  const set<unsigned> & lvls )
{
  //cout << "SeriesIconView::levelsMoved - delete selection\n";
  unsigned			n = lvls.size(), x, i = count() - 1;
  set<unsigned>::const_reverse_iterator	il, el = lvls.rend();
  QIconViewItem			*item = lastItem(), *item2;
  bool				levelchanged = false;

  for( il=lvls.rbegin(); il!=el; ++il )
    {
      if( *il >= insertedpos )
	x = *il + n;
      else
	x = *il;
      if( x <= (unsigned) game.tb )
	{
	  theQRWin->storeLevel();
	  if( x < (unsigned) game.tb 
	      || (unsigned) game.tb == ser->numLevels() - 1 )
	    --game.tb;
	  levelchanged = true;
	}
      ser->deleteLevel( x );
      for( ; i>x; --i )
	item = item->prevItem();
      item2 = item;
      --i;
      item = item->prevItem();
      delete item2;
    }
  for( i=0, item=firstItem(); item; ++i, item=item->nextItem() )
    item->setText( tr( "Level " ) + QString::number( i ) );
  if( levelchanged )
    theQRWin->changeLevel( game.tb );
}


void SeriesIconView::levelsDropped( int, QDropEvent* e, 
				    const QValueList_QIconDragItem & lst )
{
  emit dropped( e, lst );
}


// --------------------


SeriesIconViewItem::SeriesIconViewItem( SeriesIconView* parent, 
					const QString & text, 
					const QPixmap & icon )
  : QIconViewItem( parent, text, icon ), _parent( parent )
{
  setDropEnabled( true );
}


SeriesIconViewItem::SeriesIconViewItem( SeriesIconView* parent, 
					QIconViewItem* after, 
					const QString & text, 
					const QPixmap & icon )
  : QIconViewItem( parent, after, text, icon ), _parent( parent )
{
  setDropEnabled( true );
}


SeriesIconViewItem::~SeriesIconViewItem()
{
}


bool SeriesIconViewItem::acceptDrop( const QMimeSource * mime ) const
{
  if( mime->provides( "Rock'n'Roll/levels-list" ) && dropEnabled() )
    return( true );
  return( false );
}


void SeriesIconViewItem::dropped( QDropEvent* e, 
				  const QValueList_QIconDragItem & lst )
{
  _parent->levelsDropped( index(), e, lst );
}


