//---------------------------------------------------------------------------
#ifndef FileDialogsH 
#define FileDialogsH
//---------------------------------------------------------------------------
#include "Persistent.h"
//---------------------------------------------------------------------------
#include <string>
#include <vector>   
//---------------------------------------------------------------------------

using namespace std;

class QWidget;
	
class StandardDirectoriesConfig {
 public:
	string openDir;
	string saveDir;
	string importDir;
	string exportDir; 
};

extern StandardDirectoriesConfig standardDirectoriesConfig;
	
/// encapsulates QT file dialogs	
class  FileDialogs {
 public:
	/// default directories to initialize file dialog with
	/// stored in configuration (i.e. per user / computer / app)
	enum InitDirType {																															 
		CURRENT_DIR,		
		OPEN_DIR,
		SAVE_DIR,
		IMPORT_DIR,
		EXPORT_DIR
	};

 private:
	static void getDir(string &dir, InitDirType t);
	static void storeDir(string dir, InitDirType t);
	static string getFileNameIntern(QWidget *parent, string caption, 
                                   string filter, string *selectedFilter, string dir, 
                                   InitDirType initDir, bool open, string defaultName);
	
 public:
	/// get file name to open 
	/// returns "" if canceled
	/// if dir == "" the directory is initialized to the selected default localtion
	static string getOpenFileName(QWidget *parent, string caption, 
                                 string filter, string *selectedFilter = NULL, string dir = "", 
                                 InitDirType initDir = OPEN_DIR, string defaultName = "");
	/// get multiple file names to open 
	/// returns empty list if canceled
	/// if dir == "" the directory is initialized to the selected default localtion
	static vector<string> getOpenFileNames(QWidget *parent, string caption, 
													   string filter, string *selectedFilter = NULL, string dir = "", 
														InitDirType initDir = OPEN_DIR, string defaultName = "");
	/// get file name to open 
	/// returns "" if canceled
	/// if dir == "" the directory is initialized to the selected default localtion
	static string getSaveFileName(QWidget *parent, string caption, 
                                 string filter, string *selectedFilter = NULL, string dir = "", 
                                 InitDirType initDir = SAVE_DIR, string defaultName = "");
	
};



#endif
