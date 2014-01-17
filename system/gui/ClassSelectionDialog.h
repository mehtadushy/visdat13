#ifndef ClassSelectionDialogH
#define ClassSelectionDialogH
//---------------------------------------------------------------------------
#include <QtGui/QDialog>
#include <string>
#include <vector>
//---------------------------------------------------------------------------

class QTreeWidget;
class QTreeWidgetItem;
class QLineEdit;
class QIcon;

class MetaClass;

using namespace std;


class  ClassSelectionDialog : public QDialog
{
	Q_OBJECT

private:

	void collectClasses( string baseClass, QTreeWidgetItem * parent );
	void tryToFinishDialog( string selectedClass );

	QTreeWidget	*	m_Tree;
	QLineEdit	*	m_DirectTypeIn;
	QIcon			*	m_ClassIcon, * m_AbstractClassIcon;
	bool				m_AllowAbstractClasses;
	MetaClass	*	m_SelectedClass;

public:
	ClassSelectionDialog(QWidget * parent, Qt::WFlags f = 0);
	~ClassSelectionDialog(void);

   void autoMove(QWidget *belowThis);
   void moveOnScreen(int x, int y);

	/// collects all derivates from baseClass.
	/// if allowAbstractClasses is true, then the user is allowed to select abstract classes.
	/// use this method only once !
	void init( const MetaClass * baseClass, bool allowAbstractClasses = false);

	/// returns selected class or null, if dialog was skipped
	MetaClass	*	getSelectedClass() {return m_SelectedClass;}

public slots:

	void sltDirectTypeInChanged( const QString &);
	void sltEnterPressed();
	void sltDoubleClicked( QTreeWidgetItem * item, int column );


protected:

	virtual void showEvent( QShowEvent * e );
};




#endif
