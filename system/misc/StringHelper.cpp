//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "StringHelper.h"
#include <sstream>
#include <iostream>
#include <QtCore/QString>
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

using namespace std;


bool strToInt(std::string s, int& num){
  istringstream is(s);
  is >> num;
  if (!is.eof()) return false;
  bool result = !(is.fail());
  return result;
}


bool strToFloat(std::string s, float& num){
  istringstream is(s);
  is >> num;
  if (!is.eof()) return false;
  bool result = !(is.fail());
  return result;
}

std::string intToStr( int64 i )
{
  std::string s;
  ostringstream os;
  os << i;
  s = os.str();
  return s;
}

std::string floatToStr( float f )
{
  std::string s;
  ostringstream os;
  os << f;
  s = os.str();
  return s;
}

std::string lTrimStr(std::string s)
{
  while (s[0] == ' ') s.erase(0, 1);
  return s;
}

std::string rTrimStr(std::string s)
{
  if (s.size() > 0) {
    while (s[s.size()-1] == ' ') {
      s.erase(s.size()-1, 1);
      if (s.size() == 0) break;
    }
  }
  return s;
}

std::string trimStr(std::string s)
{
  s = lTrimStr(rTrimStr(s));
  return s;
}

std::string strLCase(std::string s)
{
  for (unsigned i = 0; i != s.size(); ++i)
    s[i] = (unsigned char)tolower(s[i]);
  return s;
}

std::string strUCase(std::string s)
{
  for (unsigned i = 0; i != s.size(); ++i)
    s[i] = (unsigned char)toupper(s[i]);
  return s;
}

std::string removeDblSpace(std::string s)
{
  unsigned pos = 0;
  bool lastSpace = false;
  while (pos < s.size()) {
    if (s[pos] == ' ') {
      if (lastSpace) {
        s.erase(pos, 1);
      } else {
        lastSpace = true;
        ++pos;
      }
    } else {
      lastSpace = false;
      ++pos;
    }
  }

  return s;
}

/**
 * Extracts the first part from the string s until but excluding c
 * if c is not part of the string, s will be returned and s will be empty
 * Example:
 * a = "a.b";
 * b = extractToChar(a, '.');
 * Result: a is "a", b is "b" */
std::string extractToChar(std::string& s, char c)
{
  std::string word;
  for (unsigned i = 0; i < s.size(); ++i) {
    if (s[i] != c) word += s[i];
      else break;
  }
  s.erase(0, word.size()+1);
  return word;
}

/// splits s into substring
std::vector<std::string> splitString(const std::string &s, const std::string &splitchars)
{
	std::vector<std::string>	ret;
	std::string tmp;
	for( unsigned int i=0;i<s.length();i++ )
	{
		bool splitter = false;
		for( unsigned int j=0;j<splitchars.length();j++ )
			if( s[i] == splitchars[j] )
			{
				splitter = true;
				break;
			}
		if( splitter )
		{
			if( tmp.length() )
			{
				ret.push_back( tmp );
				tmp = "";
			}
		}
		else
			tmp += s[i];
	}

	if( tmp.length() )
		ret.push_back( tmp );

	return ret;
}

std::string removePathFromFilename( const std::string &s )
{
  std::string ret = "";
  for (int i=(int)s.length()-1; i >= 0; i--)
    if ((s[i] == '/') || (s[i] == '\\'))
      return s.substr(i+1, s.length()-i-1);

  return s;
}

std::string removeFilenameFromPath( const std::string &s )
{
  std::string ret = "";
  for (int i=(int)s.length()-1; i >= 0; i--)
    if ((s[i] == '/') || (s[i] == '\\'))
      return s.substr(0, i+1);

  return s;
}

std::string cutString( const std::string str, card32 maxLength )
{
   if( str.length() <= maxLength )
      return str;

   std::string ret = str;
   ret.resize( maxLength );
   return ret;
}

std::string  replaceSubStrings( const std::string str, const std::string substring, const std::string rStr )
{
   std::string ret = str;
   if( substring == rStr )
      return ret;
   int index = (int)ret.find( substring );
   while( index >= 0 )
   {
      ret = ret.replace( index, substring.length(), rStr );
      index = (int)ret.find( substring );
   }

   return ret;
}

std::string  getTimeString( card32 time, bool milliseconds )
{
   card32 ms = time % 1000;
   card32 seconds = (time / 1000) % 60;
   card32 minutes = (time / 60 / 1000) % 60;
   card32 hours = (time / 60 / 60 / 1000) % 24;
   card32 days = (time / 60 / 60 / 1000 / 24 ) % 365;
   card32 years = (time / 60 / 60 / 1000 / 24 / 365 );

   std::string ret;
   bool b = false;
   if( years > 0 )
   {
      ret += intToStr(years) + " years ";
      b = true;
   }
   if( days > 0 || b)
   {
      ret += intToStr(days) + " days";
      b = true;
   }
   if( hours > 0 || b )
   {
      ret += intToStr(hours) + ":";
      b = true;
   }
   if( minutes > 0 || b )
   {
      std::string tmpMin = intToStr(minutes);
      if( tmpMin.length() < 2 )
         tmpMin = std::string("0") + tmpMin;
      ret += tmpMin + ":";
      b = true;
   }
   std::string tmpSec = intToStr(seconds);
   if( tmpSec.length() < 2 )
      tmpSec = std::string("0") + tmpSec;
   ret += tmpSec;
   if( milliseconds )
   {
      ret += ".";
      ret += intToStr(ms);
   }
   return ret;
}


/// returns string with the file name ending or "." if no ending exists, e.g. input:"dir/myFile.txt" output:".txt"
std::string extractFileExt( const std::string &s ) {
	int index = (int)s.find_last_of(".");
	if (index>0) {
		return s.substr(index,(int)s.size()-index);
	}
	return ".";
}

std::string qString2STLString(const QString &str) {
	QByteArray temp = str.toLatin1();
	const char *data = temp.constData();
	return string(data);
}

std::string allDirSeperatorsToDos(const string filename) {
	string result = filename;
	for (unsigned i=0; i<result.size(); i++) {
		if (result[i] == '/') {
			result[i] = '\\';
		}
	}
	return result;
}

std::string allDirSeperatorsToUnix(const string filename) {
	string result = filename;
	for (unsigned i=0; i<result.size(); i++) {
		if (result[i] == '\\') {
			result[i] = '/';
		}
	}
	return result;
}

std::string  fillNumberStringWithZeros(const string number, unsigned numZeros) {
	std::string result = number;
	while (result.size() < numZeros) {
		result = string("0") + result;
	}
	return result;
}

std::string  convertTimeToString( card32 delta_time )
{
	string ret;
	card32 h = delta_time / 1000 / 60 / 60;
	card32 m = (delta_time / 1000 / 60) % 60;
	card32 s = (delta_time / 1000 ) % 60;
	card32 ms = delta_time % 1000;
	if( h > 0 )
		ret += intToStr(h) + "h ";
	if( m > 0 || h > 0)
		ret += intToStr(m) + "m ";
	if( s > 0 || h > 0 || m > 0 )
		ret += intToStr(s) + "s ";
	ret += intToStr(ms) + "ms";
	return ret;
}
