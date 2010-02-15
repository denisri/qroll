#include <iostream>
#include <string>
#include <cstdio>
#include <stdlib.h>

using namespace std;

int main( int argc, char** argv )
{
  if( argc != 2 )
    {
      cerr << "Usage : " << argv[0] << "file.wav\n";
      cerr << "\nConverts WAV file to raw sound data (by cutting the WAV\n" 
	   << "header). Saves it in file.raw.\n\n";
      exit( 1 );
    }

  char	*fnin = argv[1];

  FILE	*fin;

  fin = fopen( fnin, "rb" );

  if( !fin )
    {
      cerr << "Cannot open " << fnin << endl;
      exit( 1 );
    }

  fseek( fin, 44, SEEK_SET );

  string	fnout( fnin );
  size_t	pos = fnout.rfind( ".wav" );
  if( pos == string::npos && (pos=fnout.rfind( ".WAV" ) ) == string::npos )
    {
      cerr << "warning: cannot find '.wav' extension.\n";
      cerr << "appening '.raw' at end of filename.\n";
    }
  else
    fnout.erase( pos, 4 );
  fnout += ".raw";
  cout << "output file : " << fnout << endl;

  FILE	*fout;
  fout = fopen( fnout.c_str(), "wb" );
  if( !fout )
    {
      cerr << "Cannot write " << fnout << ".\n";
      exit( 1 );
    }

  unsigned char	c;
  unsigned	i = 0;

  while( !feof( fin ) )
    {
      ++i;
      fscanf( fin, "%c", &c );
      fprintf( fout, "%c", c );
    }
  cout << i << " bytes copied.\n";

  fclose( fin );
  fclose( fout );

  return( 0 );
}


