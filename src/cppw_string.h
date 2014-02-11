/*
Copyright (c) 2013 Petar Korponaić

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CPPW_STRING_H
#define CPPW_STRING_H

#include <string>
#include <vector>
#include <string.h>
#include <stdint.h>

#define LINE_TERM "\n"

#ifdef _WIN32
    #ifndef strncasecmp
        #define strncasecmp strnicmp
    #endif

    #ifndef strcasecmp
        #define strcasecmp stricmp
    #endif
#endif

using namespace std;

class CWStringList;

string IntToStr(int iNum);
int StrToIntDef(string str, int iDefaultValue);
int ExtractIntDef(string str, int iDefaultValue);

string FloatToStr(double fValue);
string FloatToStr(double fValue, int iScale);
string FloatToStr(double fValue, char cDecimalSeparator, char cThousandSeparator);
string FloatToStr(double fValue, int iScale, char cDecimalSeparator, char cThousandSeparator, int iMaxTrailingZeroes = -1);


void ReplaceSubString(string* pString, string* pOld, string* pNew);
void ReplaceSubString(string* pString, string sOld, string sNew);
string ReplaceSubString(const string& sString, string sOld, string sNew);

string EnsureFirstChar(const string& sString, char cChar);
string EnsureLastChar(const string& sString, char cChar);

string RemoveFirstChar(const string& sString, char cChar);
string RemoveLastChar(const string& sString, char cChar);

int FindChar(const char* str, char c, int pos);
string Trim(string sString, bool bSpaces = true, bool bNewLines = true);
void StringToList(const string* pText, char cSeparator, CWStringList* pList);
void StringToList(const string& sText, char cSeparator, CWStringList* pList);
void StringToList(const string& sText, char cSeparator, char cIgnoreSepStartChar, char cIgnoreSepEndChar, CWStringList* pList);
string ListToString(CWStringList* pList, char cSeparator);

string EscapeJSON(const string& sText);

bool IsEMailAddress(string sEMail);

int StrCmp(const string& str1, const string& str2, bool bOpposite = false);
int StrCmpi(const string& str1, const string& str2, bool bOpposite = false);

string ByteToBinString(uint8_t iByte, char cFalseChar = '0', char cTrueChar = '1');

uint32_t UTF8_charlen(const char* cUTF8, uint32_t* pPos = NULL);
uint32_t UTF8_strlen(const char* cUTF8);
uint32_t UTF8_to_UTF32_char(const char* cUTF8, uint32_t* pPos, int* pLen);

/// String list object for easy manipulation with list of strings.
class CWStringList
{
    public:
        string Name;

        vector<string> Strings;

        CWStringList();
        ~CWStringList();

        void Clear();
        int Count();

        void Assign(CWStringList* pCopyFrom);
        void Append(CWStringList* pCopyFrom);
		void Append(string sText);
        void SetText(string sText);
        string GetText(); // join all elements into signle string object. Newlines inside elements are converted to "space-backslash-newline" sequence.

        void Add(string sString); // add string to the end of list.
        void Insert(string sString, int iIndex); // insert string into position iIndex.
        void Delete(int iIndex); // delete iIndex-th element from list.
        void DeleteEmptyLines();

        int Find(string sString, bool bCaseSensitive = true, int iStartIndex = 0);

        // name=value functions
        bool IsNameValue(int iIndex);
        int FindName(string sName, int iStartIndex = 0);
        void DeleteNameValue(string sName);
        string GetName(int iIndex);
        string GetValue(string sName, int iStartIndex = 0);
        string GetValue(int iIndex);
        void GetValues(string sName, CWStringList* pValues);
        void SetValue(string sName, string sValue);

        void Swap(int iIndex1, int iIndex2);

        void Sort(bool bDescending, int iStart, int iEnd);
        void Sort(bool bDescending = false);
};

/// Array of CWStringList objects
class CWStringListArray
{
    public:
        vector<CWStringList*>Items;

        bool OwnsObjects;

        CWStringListArray();
        ~CWStringListArray();

        void Clear();
        int Count();

        void Add(CWStringList* pList);
        void Delete(int iIndex);
        int IndexOf(CWStringList* pList);
};

/// String table - list of CWStringList objects (base class for CSV parser CWCSV)
class CWStringTable
{
    public:
        vector<CWStringList*> Columns;

        CWStringTable();
        ~CWStringTable();

        void Clear();

        int RowCount();
        int ColCount();

        void AddRow();
        void AddCol(string sColName = "", string sDefaultValue = "");

        int GetColIndex(string sColName);

        void DeleteRow(int iRowIndex);
        void DeleteCol(int iColIndex);
        void DeleteCol(string sColName);

        string GetValue(int iColIndex, int iRowIndex);
        string GetValue(string sColName, int iRowIndex);

        void SetValue(int iColIndex, int iRowIndex, string sValue, bool bAutoExpand = true);
        void SetValue(string sColName, int iRowIndex, string sValue, bool bAutoExpand = true);

        void SwapCols(int iIndex1, int iIndex2);
        void SwapCols(string sColName1, string sColName2);
        void SwapRows(int iIndex1, int iIndex2);

        void Sort(int iColIndex, bool bDescending, int iStart, int iEnd);
        void Sort(int iColIndex, bool bDescending = false);
        void Sort(string sColName, bool bDescending = false);
};

#endif
