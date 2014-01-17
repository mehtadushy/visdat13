//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "FileDialogs.h"
//---------------------------------------------------------------------------
#include "StringClassProperty.h"
#include "StringHelper.h"
//---------------------------------------------------------------------------
#include <QtGui/QFileDialog>
#include <QtCore/QDir>
//---------------------------------------------------------------------------


StandardDirectoriesConfig standardDirectoriesConfig;

// ---

void FileDialogs::getDir(string &dir, InitDirType t) {
	if (t == CURRENT_DIR) {
		dir = qString2STLString(QDir::currentPath());
		return;
	} else {
		switch (t) {
		 case OPEN_DIR: dir = standardDirectoriesConfig.openDir; break;
		 case SAVE_DIR: dir = standardDirectoriesConfig.saveDir; break;
		 case IMPORT_DIR: dir = standardDirectoriesConfig.importDir; break;
		 case EXPORT_DIR: dir = standardDirectoriesConfig.exportDir; break;
		}
	}
	dir = removeFilenameFromPath(dir);
	QDir dirTest(dir.c_str());
	if (!dirTest.exists()) {
		dir = qString2STLString(QDir::currentPath());
	}
}

void FileDialogs::storeDir(string dir, InitDirType t) {
	if (t == CURRENT_DIR) {
		return;
	} else {
		switch (t) {
		 case OPEN_DIR: standardDirectoriesConfig.openDir = dir; break;
		 case SAVE_DIR: standardDirectoriesConfig.saveDir = dir; break;
		 case IMPORT_DIR: standardDirectoriesConfig.importDir = dir; break;
		 case EXPORT_DIR: standardDirectoriesConfig.exportDir = dir; break;
		}
	}
}

string FileDialogs::getFileNameIntern(
	QWidget *parent, string caption, 
   string filter, string *selectedFilter, string dir, InitDirType initDir, bool open, string defaultName)
 {
	QString qcaption(caption.c_str());
	QString qfilter(filter.c_str());
	QString qselectedFilter;
	if (dir == "") {
		getDir(dir, initDir);	
	}
	dir += defaultName;
	QString qdir(dir.c_str());
	QString filename;
	if (open) {
		filename = QFileDialog::getOpenFileName(parent, qcaption, qdir, qfilter, &qselectedFilter );
	} else {
		filename = QFileDialog::getSaveFileName(parent, qcaption, qdir, qfilter, &qselectedFilter );
	}
	if( !filename.isEmpty() ) {
		string result = qString2STLString(filename);
		if (selectedFilter != NULL) *selectedFilter = qString2STLString(qselectedFilter);
		storeDir(removeFilenameFromPath(qString2STLString(filename)), initDir);
		return result;	
	} else {
		return string("");
	}  
 }


string FileDialogs::getOpenFileName(
	QWidget *parent, string caption, 
   string filter, string *selectedFilter, string dir, InitDirType initDir, string defaultName)
 {
	return getFileNameIntern(parent, caption, filter, selectedFilter, dir, initDir, true, defaultName);
 }

string FileDialogs::getSaveFileName(QWidget *parent, string caption, 
	string filter, string *selectedFilter, string dir, InitDirType initDir, string defaultName)
{
	return getFileNameIntern(parent, caption, filter, selectedFilter, dir, initDir, false, defaultName);
}

vector<string> FileDialogs::getOpenFileNames(
	QWidget *parent, string caption, 
	string filter, string *selectedFilter, string dir, 
	InitDirType initDir, string defaultName)
{
	QString qcaption(caption.c_str());
	QString qfilter(filter.c_str());
	QString qselectedFilter;
	if (dir == "") {
		getDir(dir, initDir);	
	}
	dir += defaultName;
	QString qdir(dir.c_str());
	QStringList filenames;
	filenames = QFileDialog::getOpenFileNames(parent, qcaption, qdir, qfilter, &qselectedFilter );
	vector<string> result;
	for( int i=0;i<filenames.size();i++ )
		result.push_back(qString2STLString(filenames[i]));
	if (selectedFilter != NULL) *selectedFilter = qString2STLString(qselectedFilter);
	if (!result.empty())
		storeDir(removeFilenameFromPath(result[0]), initDir);
	return result;	
}
