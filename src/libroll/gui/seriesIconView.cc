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
#include <QMouseEvent>
#include <QApplication>

using namespace roll;
using namespace std;


LevelsDrag::LevelsDrag( QWidget * /* dragSource */, const char * /* name */ )
  : QMimeData( /*dragSource, name*/ )
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


// const char* LevelsDrag::format ( int i ) const
// {
//   if( i == 0 )
//     return( "application/x-qiconlist" );
//   else if( i == 1 )
//     return( "Rock'n'Roll/levels-list" );
//   else
//     return( 0 );
// }
// 
// 
// bool LevelsDrag::canDecode( QMimeSource* ms )
// {
//   return( ms->provides( "application/x-qiconlist" ) ||
// 	  ms->provides( "Rock'n'Roll/levels-list" ) );
// }


bool LevelsDrag::decode( const QMimeData* ms, 
			 map<unsigned, SimpleLevel> & levels )
{
//   if( !canDecode( ms ) )
//     return( false );

  try	// Microsoft VC++ throws an exception if a dynamic_cast fails
  {
    const LevelsDrag	*ld = dynamic_cast<const LevelsDrag*>( ms );
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

  QByteArray	ba( ms->data( "Rock'n'Roll/levels-list" ) );

  unsigned	inum;
  QDataStream	bstr( &ba, QIODevice::ReadOnly );

  while( !bstr.atEnd() && bstr.device()->isOpen()
	 && bstr.status() == QDataStream::Ok 
	 && ba.size() - bstr.device()->pos() > 50 )
    {	// under windows, QDataStream end dfoesn't seem correct
      /*out << "qdatastream: at " << bstr.device()->at() << " / " << ba.size() 
	<< endl;*/
      bstr >> inum;
      if( bstr.atEnd() ||  !bstr.device()->isOpen()
	  || bstr.status() != QDataStream::Ok )
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


void LevelsDrag::append( const QIcon &icon, unsigned n,
			 SimpleLevel* sl )
{
  cout << "append level " << sl << " in clipboard\n";
  _levels[ n ] = *sl;
  setImageData( icon );
  setData( "Rock'n'Roll/levels-list", 
           encodedData( "Rock'n'Roll/levels-list" ) );
}


QByteArray LevelsDrag::encodedData( const char* mime ) const
{
  if( QString( mime ) == "Rock'n'Roll/levels-list" )
    {
      //cout << "encodedData recognized\n";
      QByteArray	ba;
      QDataStream	bstr( &ba, QIODevice::WriteOnly );
      map<unsigned, SimpleLevel>::const_iterator	il, el = _levels.end();

      for( il=_levels.begin(); il!=el; ++il )
	{
	  bstr << il->first;
	  il->second.write( bstr );
	}

      return( ba );
    }
  else
    return( QMimeData::data( mime ) );
}


// -------------

SeriesIconView::SeriesIconView( QWidget * parent )
  : QListWidget( parent )
{
  //connect( this, SIGNAL( moved() ), SLOT( levelsMoved() ) );
  setViewMode( IconMode );
  setSortingEnabled( false );
  setMovement( Static );
//   setMovement( Free );
//   setResizeMode( Fixed );
  setDragDropMode( DragDrop );
  setAcceptDrops( true );
  setDragEnabled( true );
  setDefaultDropAction( Qt::MoveAction );
  setIconSize( QSize( 300, 300 ) );
  setDropIndicatorShown( true );
  setDragDropOverwriteMode( false );
}


SeriesIconView::~SeriesIconView()
{
}


QMimeData * SeriesIconView::dragObject()
{
  return copySelection( this );
}


QMimeData * SeriesIconView::copySelection( QWidget* source )
{
  //cout << "SeriesIconView::copySelection( " << source << " )\n";
  vector<unsigned>	lvls;
  LevelsDrag	*ld = new LevelsDrag( source, "levelsDrag" );

  if( currentItem() )
    ld->setImageData( currentItem()->icon() );

  int i, n = count();
  for( i=0; i<n; ++i )
  {
    QListWidgetItem *ii = item( i );
    if( ii->isSelected() )
    {
      SimpleLevel		*sl = new SimpleLevel;
      ser->unpack( i, *sl );
      ld->append( ii->icon(), i, sl );
    }
  }

  return ld;
}


// void SeriesIconView::mousePressEvent( QMouseEvent* event )
// {
//   if( event->button() == Qt::LeftButton )
//     _dragStart = event->pos();
//   QListWidget::mousePressEvent( event );
// }
// 
// 
// void SeriesIconView::mouseMoveEvent( QMouseEvent* event )
// {
//   if( !( event->buttons() & Qt::LeftButton ) )
//     return;
//   if( ( event->pos() - _dragStart ).manhattanLength()
//       < QApplication::startDragDistance() )
//     return;
// 
//   QDrag *drag = new QDrag(this);
//   QMimeData *mimeData = dragObject();
// 
//   drag->setMimeData(mimeData);
// 
//   Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
//   cout << "dropAction: " << dropAction << endl;
// }


void SeriesIconView::levelsMoved( unsigned insertedpos, 
				  const set<unsigned> & lvls )
{
  //cout << "SeriesIconView::levelsMoved - delete selection\n";
  unsigned			n = lvls.size(), x, i = count() - 1;
  set<unsigned>::const_reverse_iterator	il, el = lvls.rend();
  QListWidgetItem		*item1, *item2;
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
      item1 = item( x );
      item2 = item1;
      --i;
      item1 = item( x-1 );
      delete item2;
    }

  unsigned cnt = count();
  for( i=0; i<cnt; ++i )
  {
    item1 = item( i );
    item1->setText( tr( "Level " ) + QString::number( i ) );
  }
  if( levelchanged )
    theQRWin->changeLevel( game.tb );
}


void SeriesIconView::levelsDropped( QListWidgetItem *, QDropEvent* e )
{
  emit dropped( e );
}


// void SeriesIconView::dragEnterEvent( QDragEnterEvent* event )
// {
//   cout << "SeriesIconView::dragEnterEvent\n";
//   cout << "donc: " << event->mimeData()->hasFormat( "Rock'n'Roll/levels-list" ) << endl;
//   if( event->mimeData()->hasFormat( "Rock'n'Roll/levels-list" ) )
//   {
//     cout << "accept.\n";
//     event->acceptProposedAction();
//     event->accept();
//   }
//   else
//     event->ignore();
// }
// 
// 
// /*
// void SeriesIconView::dropEvent( QDropEvent* e )
// {
//   cout << "SeriesIconView::dropEvent\n";
//   levelsDropped( itemAt( e->pos() ), e );
//   e->acceptProposedAction();
// }
// */
// 
bool SeriesIconView::dropMimeData( int index, const QMimeData* data, 
                                  Qt::DropAction action )
{
  cout << "dropMimeData\n";
  return QListWidget::dropMimeData( index, data, action );
}
// 
// 
// Qt::DropActions SeriesIconView::supportedDropActions() const
// {
//   return Qt::CopyAction | Qt::MoveAction;
// }


// --------------------


SeriesIconViewItem::SeriesIconViewItem( SeriesIconView* parent,
                                        const QString & text,
                                        const QIcon & icon )
  : QListWidgetItem( icon, text, parent ), _parent( parent )
{
//   setFlags( flags() | Qt::ItemIsDropEnabled );
}


SeriesIconViewItem::SeriesIconViewItem( SeriesIconView* parent,
                                        QListWidgetItem* after,
                                        const QString & text,
                                        const QIcon & icon )
  : QListWidgetItem( icon, text, 0 ), _parent( parent )
{
  parent->insertItem( parent->row( after ), this );
//   setFlags( flags() | Qt::ItemIsDropEnabled );
}


SeriesIconViewItem::~SeriesIconViewItem()
{
}


void SeriesIconViewItem::dragEnterEvent( QDragEnterEvent* event )
{
  if( event->mimeData()->hasFormat( "Rock'n'Roll/levels-list" ) )
    event->acceptProposedAction();
}


void SeriesIconViewItem::dropEvent( QDropEvent* e )
{
  _parent->levelsDropped( this, e );
}



