//---------------------------------------------------------------------------
#ifndef QTHelpersH
#define QTHelpersH
//---------------------------------------------------------------------------
#include <string>
#include <map>
#include <QtCore/QString>
//---------------------------------------------------------------------------

class QString;
class QPixmap;
class QWidget;

using namespace std;

class QTHelpers
{
public:

	/// converts a qstring to a std::string
	static string toSTLString( const QString &str );

	/// converts a std::string to a qstring
	static QString toQString( const string &str );

	/// creates an pixmap from file 'filename' and defines the color in pixel(0,0) as transparent
	/// returns NULL if file doesn't exist
	static QPixmap * createPixmap( const QString &filename );

	/// final cleanup
	static void cleanup();

	/// create a question messagebox with yes/no and returns true, if yes was pressed
	static bool question( QWidget * parent, const QString &question_str );

private:
	
	static map<QString,QPixmap*>	m_Pixmaps;

};


#endif
