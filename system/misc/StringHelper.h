//---------------------------------------------------------------------------
#ifndef StringHelperH
#define StringHelperH
//---------------------------------------------------------------------------
#include "PTypes.h"
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <stdlib.h>
//---------------------------------------------------------------------------

class QString;


/// Convert the string given to a int. Returns true if successful.
bool  strToInt(std::string s, int& num);
/// Convert the string given to a float. Returns true if successful.
bool  strToFloat(std::string s, float& num);
/// Converts num to a string
std::string  intToStr(int64 i);
/// Converts float to a string
std::string  floatToStr(float f);
/// Trims the string (deletes spaces at the beginning and the end)
std::string  trimStr(std::string s);
/// Deletes the leading spaces in the string and returns the result
std::string  lTrimStr(std::string s);
/// Deletes the trailing spaces in the string and returns the result
std::string  rTrimStr(std::string s);
/// Converts the string into lower case and returns the result
std::string  strLCase(std::string s);
/// Converts the string into upper case and returns the result
std::string  strUCase(std::string s);
/// Removes all double spaces from the string (i.e. makes them single spaces)
std::string  removeDblSpace(std::string s);
/// Extracts the characters up to a certain character (see detailed descr.)
std::string  extractToChar(std::string& s, char c);

/// Splits s into substrings, splitted by splitchars
/// Example: splitString( "hello world!\n bye", " \n" ) ----> "hello", "world!", "bye"
std::vector<std::string>  splitString( const std::string &s, const std::string &splitchars);

/// Example: "dir/test.abc" -> "test.abc"
std::string  removePathFromFilename( const std::string &filename );
/// Example: "dir/test.abc" -> "dir/"
std::string  removeFilenameFromPath( const std::string &filename );

/// Cuts string ( by removing all characters behind maxLength), if length > maxLength 
std::string  cutString( const std::string str, card32 maxLength );

/// Replaces every substring in str with rStr
std::string  replaceSubStrings( const std::string str, const std::string substring, const std::string rStr );

/// returns a string with format [y years][d days ][h:][m:]s.ms calculated from time, time in ms
std::string  getTimeString( card32 time, bool milliseconds=false );

/// returns string with the file name ending or "." if no ending exists, e.g. input:"dir/myFile.txt" output:".txt"
std::string  extractFileExt( const std::string &s );

std::string  qString2STLString(const QString &str);

/// converts all '/' characters to '\'
std::string  allDirSeperatorsToDos(const std::string filename);

/// converts all '\' characters to '/'
std::string  allDirSeperatorsToUnix(const std::string filename);

/// pads string with zeros in front if length is smaller than numZeroes
std::string  fillNumberStringWithZeros(const std::string number, unsigned numZeros);

/// converts time (in ms) into "[h hours] [m minutes] [s seconds] ms ms"
std::string  convertTimeToString( card32 delta_time );



#endif
