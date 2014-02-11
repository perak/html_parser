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

#include "cppw_string.h"
#include <sstream>
#include <cstdio>
#include <cstdlib>

string IntToStr(int iNum)
{
    char res[128];
    sprintf(res, "%d", iNum);
    return res;
}

int StrToIntDef(string str, int iDefaultValue)
{
    if(str == "" || str.find_first_not_of("0123456789+-") != string::npos)
        return iDefaultValue;
    return(atoi(str.c_str()));
}

int ExtractIntDef(string str, int iDefaultValue)
{
    string s = "";
    const char* buff = str.c_str();
    int i = 0;
    while(buff[i] != '\0')
    {
        if(isdigit(buff[i]))
            s.push_back(buff[i]);
        i++;
    }

    return StrToIntDef(s, iDefaultValue);
}

char* _ftoa(double val, char* str)
{
  sprintf(str, "%f", val);
  return str;
}

char* _ftoa(double fValue, int iScale, char* pBuffer)
{
    char fmt[10];
    sprintf(fmt, "%%.%dlf", iScale);
    sprintf(pBuffer, fmt, fValue);
    return pBuffer;
}

string FloatToStr(double fValue)
{
    char tmp[81];
    // convert to float
    _ftoa(fValue, tmp);

    // remove trailing zeroes
    int count = strlen(tmp);
    while((count > 0) && (tmp[count - 1] == '0'))
    {
        tmp[count - 1] = '\0';
        count--;
    }

    // at least one trailing zero after decimal point
    if(tmp[strlen(tmp) - 1] == '.')
        strcat(tmp, "0");
    return tmp;
}

string FloatToStr(double fValue, int iScale)
{
    char tmp[81];
    return _ftoa(fValue, iScale, tmp);
}

string FloatToStr(double fValue, char cDecimalSeparator, char cThousandSeparator)
{
    if(cDecimalSeparator == cThousandSeparator)
    {
        if(cDecimalSeparator == '.')
            cThousandSeparator = ',';
        else
            cThousandSeparator = '.';
    }

    string res = FloatToStr(fValue);
    // change decimal separator
    int len = res.size();
    int decimal_pos = -1;
    for(int i = len - 1; i >= 0; i--)
    {
        if(res[i] == '.')
        {
            res[i] = cDecimalSeparator;
            decimal_pos = i;
        }
    }

    if(decimal_pos < 0)
        decimal_pos = len;

    if(cThousandSeparator != '\0')
    {
        int first_num_pos = 0;
        if(fValue < 0)
            first_num_pos = 1;
        for(int i = decimal_pos - 3; i > first_num_pos; i -= 3)
            res.insert(i, 1, cThousandSeparator);
    }
    return res;
}

string FloatToStr(double fValue, int iScale, char cDecimalSeparator, char cThousandSeparator, int iMaxTrailingZeroes)
{
    if(cDecimalSeparator == cThousandSeparator)
    {
        if(cDecimalSeparator == '.')
            cThousandSeparator = ',';
        else
            cThousandSeparator = '.';
    }

    string res = FloatToStr(fValue, iScale);
    // change decimal separator
    int len = res.size();
    int decimal_pos = -1;
    for(int i = len - 1; i >= 0; i--)
    {
        if(res[i] == '.')
        {
            res[i] = cDecimalSeparator;
            decimal_pos = i;
        }
    }

    if(decimal_pos < 0)
        decimal_pos = len;

    if(iMaxTrailingZeroes >= 0)
    {
        for(int i = len - 1; i > decimal_pos; i--)
        {
            if(res[i] == '0' && (i - decimal_pos) > iMaxTrailingZeroes)
                res.erase(i, 1);
            else
                break;
        }
        len = res.size();
        if(decimal_pos == len - 1)
            res.erase(decimal_pos, 1);
    }

    if(cThousandSeparator != '\0')
    {
        int first_num_pos = 0;
        if(fValue < 0)
            first_num_pos = 1;

        for(int i = decimal_pos - 3; i > first_num_pos; i -= 3)
            res.insert(i, 1, cThousandSeparator);
    }
    return res;
}

void ReplaceSubString(string* pString, string *pOld, string *pNew)
{
    if(pString == NULL || pOld == NULL || pNew == NULL)
        return;

    int old_size = pOld->size();
    int new_size = pNew->size();
    size_t pos = 0;
    while((pos = pString->find(*pOld, pos)) != string::npos)
    {
        pString->replace(pos, old_size, *pNew);
        pos = pos + new_size;
    }
}

void ReplaceSubString(string* pString, string sOld, string sNew)
{
    ReplaceSubString(pString, &sOld, &sNew);
}

string ReplaceSubString(const string& sString, string sOld, string sNew)
{
    string s(sString);
    ReplaceSubString(&s, &sOld, &sNew);
    return s;
}


string EnsureFirstChar(const string& sString, char cChar)
{
    string res = sString;
    int len = res.size();
    if(len == 0 || res[0] != cChar)
        return res.insert(0, 1, cChar);

    return res;
}

string EnsureLastChar(const string& sString, char cChar)
{
    string res = sString;
    int len = res.size();
    if(len == 0 || res[len - 1] != cChar)
        return res.append(1, cChar);

    return res;
}

string RemoveFirstChar(const string& sString, char cChar)
{
    string res = sString;
    int len = res.size();
    if(len > 0 && res[0] == cChar)
        return res.erase(0, 1);

    return res;
}

string RemoveLastChar(const string& sString, char cChar)
{
    string res = sString;
    int len = res.size();
    if(len > 0 && res[len - 1] == cChar)
        return res.erase(len - 1, 1);

    return res;
}

int FindChar(const char* str, char c, int pos)
{
    while(str[pos] != 0)
    {
        if(str[pos] == c) return pos;
        pos++;
    }
    return -1;
}


string Trim(string sString, bool bSpaces, bool bNewLines)
{
    string invalid_chars = "";
    if(bSpaces) invalid_chars.append(" \t");
    if(bNewLines) invalid_chars.append("\r\n");

    if(sString == "")
        return sString;

    string res(sString);
    while(res.find_first_of(invalid_chars) == 0)
        res.erase(0, 1);

    if(res == "")
        return res;

    while(res.find_last_of(invalid_chars) == res.size() - 1)
        res.erase(res.size() - 1, 1);

    return res;
}

void StringToList(const string* pText, char cSeparator, CWStringList* pList)
{
    pList->Clear();
    if(*pText == "") return;

    size_t pos = 0;
    int newline = -1;
    while((newline = FindChar(pText->c_str(), cSeparator, pos)) >= 0)
    {
        pList->Add(pText->substr(pos, newline - pos));
        pos = newline + 1;
    }
    unsigned int len = pText->size();
    if(pos <= len)
        pList->Add(pText->substr(pos, len - pos));
}

void StringToList(const string& sText, char cSeparator, CWStringList* pList)
{
	return StringToList(&sText, cSeparator, pList);
}



void StringToList(const string& sText, char cSeparator, char cIgnoreSepStartChar, char cIgnoreSepEndChar, CWStringList* pList)
{
	char* text = (char*)sText.c_str();
	int last = 0;
	int pos = 0;
	bool ignore = false;
	while(text[pos] != 0)
	{
		if(text[pos] == cIgnoreSepStartChar && !ignore)
			ignore = true;
		else if(text[pos] == cIgnoreSepEndChar && ignore)
				ignore = false;
		if(text[pos] == cSeparator && !ignore)
		{
			pList->Add(string(text + last, pos - last));
			last = pos + 1;
		}
		pos++;
	}
	pList->Add(string(text + last, pos - last));
}

string ListToString(CWStringList* pList, char cSeparator)
{
    string res = "";
    int count = pList->Count();
    for(int i = 0; i < count; i++)
    {
        if(i > 0) res.append(1, cSeparator);
        res.append(pList->Strings[i]);
    }
    return res;
}

string EscapeJSON(const string& input) {
    std::ostringstream ss;
    for (string::const_iterator iter = input.begin(); iter != input.end(); iter++) {
    //C++98/03:
    //for (std::string::const_iterator iter = input.begin(); iter != input.end(); iter++) {
        switch (*iter) {
            case '\\': ss << "\\\\"; break;
            case '"': ss << "\\\""; break;
            case '/': ss << "\\/"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default: ss << *iter; break;
        }
    }
    return ss.str();
}


bool IsEMailAddress(string sEMail)
{
    size_t pos1 = sEMail.find("@");
    if(pos1 == string::npos)
        return false;

    size_t pos2 = sEMail.rfind(".");
    if(pos2 == string::npos)
        return false;

    if(pos1 > pos2)
        return false;

    return true;
}

int StrCmp(const string& str1, const string& str2, bool bOpposite)
{
    if(str1 == str2)
        return 0;

    if(bOpposite)
    {
        if(str1 > str2)
            return -1;
        return 1;
    }

    if(str1 > str2)
        return 1;
    return -1;
}

int StrCmpi(const string& str1, const string& str2, bool bOpposite)
{
	int res = strcasecmp(str1.c_str(), str2.c_str());

	if(bOpposite)
		return 0 - res;

	return res;
}

string ByteToBinString(uint8_t iByte, char cFalseChar, char cTrueChar)
{
    char b[9];
    b[8] = '\0';

    uint8_t counter = 0;
    for (uint8_t z = 128; z > 0; z >>= 1)
    {
        b[counter] = ((iByte & z) == z) ? cTrueChar : cFalseChar;
        counter++;
    }

    return b;
}

uint32_t UTF8_charlen(const char* cUTF8, uint32_t* pPos)
{
	int pos = 0;
	if(pPos != NULL) pos = *pPos;

    uint8_t ch = cUTF8[pos];
    // empty string
    if(ch == 0)
        return 0;

    if(ch <= 0x7F) /* 0XXX XXXX one byte */
    {
		if(pPos != NULL) (*pPos) += 1;
        return 1;
    }
    else if((ch & 0xE0) == 0xC0)  /* 110X XXXX  two bytes */
    {
		if(pPos != NULL) (*pPos) += 2;
        return 2;
    }
    else if((ch & 0xF0) == 0xE0)  /* 1110 XXXX  three bytes */
    {
		if(pPos != NULL) (*pPos) += 3;
        return 3;
    }
    else if((ch & 0xF8) == 0xF0)  /* 1111 0XXX  four bytes */
    {
		if(pPos != NULL) (*pPos) += 4;
        return 4;
    }

	if(pPos != NULL) (*pPos) += 1;
    return 1;
}

uint32_t UTF8_strlen(const char* cUTF8)
{
	uint32_t len = 0;
	uint32_t pos = 0;
	while(UTF8_charlen(cUTF8, &pos)) len++;

	return len;
}

uint32_t UTF8_to_UTF32_char(const char* cUTF8, uint32_t* pPos, int* pLen)
{
    *pLen = 0;

    uint8_t ch = cUTF8[*pPos];
    // empty string
    if(ch == 0)
    {
        return 0;
    }

    if(ch <= 0x7F) /* 0XXX XXXX one byte */
    {
        *pLen = 1;
        (*pPos) += 1;
        return ch;
    }
    else if((ch & 0xE0) == 0xC0)  /* 110X XXXX  two bytes */
    {
        int c1 = (uint8_t)ch;
        int c2 = (uint8_t)cUTF8[(*pPos) + 1];

        uint32_t res = (c1 - 192) * 64 + c2 - 128;
        *pLen = 2;
        (*pPos) += 2;
        return res;
    }
    else if((ch & 0xF0) == 0xE0)  /* 1110 XXXX  three bytes */
    {
        int c1 = (uint8_t)ch;
        int c2 = (uint8_t)cUTF8[(*pPos) + 1];
        int c3 = (uint8_t)cUTF8[(*pPos) + 2];

        uint32_t res = (c1 - 224) * 4096 + (c2 - 128) * 64 + c3 - 128;
        *pLen = 3;
        (*pPos) += 3;
        return res;
    }
    else if((ch & 0xF8) == 0xF0)  /* 1111 0XXX  four bytes */
    {
        int c1 = (uint8_t)ch;
        int c2 = (uint8_t)cUTF8[(*pPos) + 1];
        int c3 = (uint8_t)cUTF8[(*pPos) + 2];
        int c4 = (uint8_t)cUTF8[(*pPos) + 2];

        uint32_t res = (c1 - 240) * 262144 + (c2 - 128) * 4096 + (c3 - 128) * 64 + c4 - 128;
        *pLen = 4;
        (*pPos) += 4;
        return res;
    }

    *pLen = -1;
    return 0;
}



CWStringList::CWStringList()
{
    Name = "";
}

CWStringList::~CWStringList()
{
    Clear();
}

void CWStringList::Clear()
{
    Strings.clear();
}

int CWStringList::Count()
{
    return Strings.size();
}

void CWStringList::Append(CWStringList* pCopyFrom)
{
    if(pCopyFrom == NULL)
        return;
    int count = pCopyFrom->Count();
    for(int i = 0; i < count; i++)
        Add(pCopyFrom->Strings[i]);
}

void CWStringList::Assign(CWStringList* pCopyFrom)
{
    Clear();
    Append(pCopyFrom);
}

void CWStringList::Append(string sText)
{
    if(sText == "") return;

    // space-backslash-lineterm is continuation sequence
    ReplaceSubString(&sText, " \\\r\n", "\f");
    ReplaceSubString(&sText, " \\\n", "\f");
    ReplaceSubString(&sText, " \\\r", "\f");
    // convert MSDOS & Mac lineterms to \n
    ReplaceSubString(&sText, "\r\n", "\n");

    size_t newline = 0;
    size_t pos = 0;
    while((newline = sText.find_first_of("\r\n", pos)) != string::npos)
    {
        string s = sText.substr(pos, newline - pos);
        ReplaceSubString(&s, "\f", LINE_TERM);
        Add(s);
        pos = newline + 1;
    }
    unsigned int len = sText.size();
    if(pos <= len)
    {
        string s = sText.substr(pos, len - pos);
        ReplaceSubString(&s, "\f", LINE_TERM);
        Add(s);
    }
}

void CWStringList::SetText(string sText)
{
    Clear();
	Append(sText);
}

string CWStringList::GetText()
{
    string res = "";
    string continuation = " \\" + string(LINE_TERM);
    int count = Count();
    for(int i = 0; i < count; i++)
    {
        if(i > 0)
            res.append(LINE_TERM);
        string s(Strings[i]);
        ReplaceSubString(&s, LINE_TERM, continuation);
        res.append(s);
    }
    return res;
}

void CWStringList::Add(string sString)
{
    Strings.push_back(sString);
}

void CWStringList::Insert(string sString, int iIndex)
{
    Strings.insert(Strings.begin() + iIndex, sString);
}

void CWStringList::Delete(int iIndex)
{
    Strings.erase(Strings.begin() + iIndex);
}

void CWStringList::DeleteEmptyLines()
{
	int count = Count();
	for(int i = count - 1; i >= 0; i--)
		if(Strings[i] == "") Delete(i);
}

int CWStringList::Find(string sString, bool bCaseSensitive, int iStartIndex)
{
    int count = Count();
    for(int i = iStartIndex; i < count; i++)
    {
        if(bCaseSensitive)
        {
            if(Strings[i] == sString)
                return i;
        }
        else
        {
            if(strcasecmp(Strings[i].c_str(), sString.c_str()) == 0)
                return i;
        }
    }
    return -1;
}

bool CWStringList::IsNameValue(int iIndex)
{
    return Strings[iIndex].find("=") != string::npos;
}

int CWStringList::FindName(string sName, int iStartIndex)
{
    int count = Count();
    for(int i = iStartIndex; i < count; i++)
    {
        if(GetName(i) == sName)
            return i;
    }
    return -1;
}

void CWStringList::DeleteNameValue(string sName)
{
    int index = FindName(sName);
    if(index < 0)
        return;
    Delete(index);
}

string CWStringList::GetValue(string sName, int iStartIndex)
{
    int pos = FindName(sName, iStartIndex);
    if(pos < 0)
        return "";

    return Strings[pos].substr(sName.size() + 1, Strings[pos].size() - (sName.size() + 1));
}

string CWStringList::GetName(int iIndex)
{
    string str = Strings[iIndex];
    size_t eq_pos = str.find("=");
    if(eq_pos == string::npos)
        return str;

    return str.substr(0, eq_pos);
}

string CWStringList::GetValue(int iIndex)
{
    string str = Strings[iIndex];
    size_t eq_pos = str.find("=");
    if(eq_pos == string::npos)
        return str;

    return str.substr(eq_pos + 1, str.size() - (eq_pos + 1));
}

void CWStringList::GetValues(string sName, CWStringList* pValues)
{
    pValues->Clear();
    int pos = 0;
    while((pos = FindName(sName, pos)) >= 0)
    {
        pValues->Add(GetValue(pos));
        pos++;
    }
}

void CWStringList::SetValue(string sName, string sValue)
{
    int pos = FindName(sName);
    if(pos < 0)
    {
        Add(sName + "=" + sValue);
        return;
    }

    Strings[pos] = sName + "=" + sValue;
}

void CWStringList::Swap(int iIndex1, int iIndex2)
{
    std::swap(Strings[iIndex1], Strings[iIndex2]);
}

void CWStringList::Sort(bool bDescending, int iStart, int iEnd)
{
    int i = iStart;
    int k = iEnd;

    if(iEnd - iStart >= 1)
    {
        while(k > i)
        {
            while(StrCmp(Strings[i], Strings[iStart], bDescending) <= 0 && i <= iEnd && k > i)
                i++;
            while(StrCmp(Strings[k], Strings[iStart], bDescending) > 0 && k >= iStart && k >= i)
                k--;

            if(k > i)
            {
                if(Strings[i] != Strings[k])
                    Swap(i, k);
            }
        }

        if(Strings[iStart] != Strings[k])
            Swap(iStart, k);

        if(k > iStart)
            Sort(bDescending, iStart, k - 1);
        if(k < iEnd)
            Sort(bDescending, k + 1, iEnd);
    }
}

void CWStringList::Sort(bool bDescending)
{
    Sort(bDescending, 0, Count() - 1);
}

// ------------
// ------------

CWStringListArray::CWStringListArray()
{
    OwnsObjects = true;
}

CWStringListArray::~CWStringListArray()
{
    Clear();
}

void CWStringListArray::Clear()
{
    if(OwnsObjects)
    {
        int count = Count();
        for(int i = 0; i < count; i++)
            delete Items[i];
    }
    Items.clear();
}

int CWStringListArray::Count()
{
    return Items.size();
}

void CWStringListArray::Add(CWStringList* pList)
{
    Items.push_back(pList);
}

void CWStringListArray::Delete(int iIndex)
{
    if(OwnsObjects)
        delete Items[iIndex];
    Items.erase(Items.begin() + iIndex);
}

int CWStringListArray::IndexOf(CWStringList* pList)
{
    int count = Count();
    for(int i = 0; i < count; i++)
    {
        if(Items[i] == pList)
            return i;
    }
    return -1;
}

// ------------
// ------------

CWStringTable::CWStringTable()
{

}

CWStringTable::~CWStringTable()
{
    Clear();
}

void CWStringTable::Clear()
{
    int column_count = ColCount();
    for(int i = 0; i < column_count; i++)
        delete Columns[i];
    Columns.clear();
}

int CWStringTable::ColCount()
{
    return Columns.size();
}

int CWStringTable::RowCount()
{
    if(ColCount() == 0)
        return 0;

    return Columns[0]->Count();
}

void CWStringTable::AddRow()
{
    int col_count = ColCount();
    for(int i = 0; i < col_count; i++)
        Columns[i]->Add("");
}

void CWStringTable::AddCol(string sName, string sDefaultValue)
{
    CWStringList* col = new CWStringList();
    col->Name = sName;
    int row_count = RowCount();
    for(int i = 0; i < row_count; i++)
        col->Add(sDefaultValue);
    Columns.push_back(col);
}

int CWStringTable::GetColIndex(string sColName)
{
    int col_count = ColCount();
    for(int i = 0; i < col_count; i++)
        if(Columns[i]->Name == sColName)
            return i;
    return -1;
}

string CWStringTable::GetValue(int iColIndex, int iRowIndex)
{
    if(iColIndex < 0 || iColIndex >= ColCount())
        return "";

    if(iRowIndex < 0 || iRowIndex >= RowCount())
        return "";

    return Columns[iColIndex]->Strings[iRowIndex];
}

string CWStringTable::GetValue(string sColName, int iRowIndex)
{
    int col_index = GetColIndex(sColName);
    if(col_index < 0)
        return "";
    return GetValue(col_index, iRowIndex);
}

void CWStringTable::SetValue(int iColIndex, int iRowIndex, string sValue, bool bAutoExpand)
{
    if(iColIndex < 0 || iRowIndex < 0)
        return;


    if(iColIndex >= ColCount())
    {
        if(!bAutoExpand)
            return;
        while(ColCount() <= iColIndex)
            AddCol("Column_" + IntToStr(ColCount()));
    }

    if(iRowIndex >= RowCount())
    {
        if(!bAutoExpand)
            return;
        while(RowCount() <= iRowIndex)
            AddRow();
    }

    Columns[iColIndex]->Strings[iRowIndex] = sValue;
}

void CWStringTable::SetValue(string sColName, int iRowIndex, string sValue, bool bAutoExpand)
{
    int col_index = GetColIndex(sColName);
    if(col_index < 0)
        return;
    SetValue(col_index, iRowIndex, sValue, bAutoExpand);
}

void CWStringTable::DeleteRow(int iRowIndex)
{
    if(iRowIndex < 0 || iRowIndex >= RowCount())
        return;

    int col_count = ColCount();
    for(int i = 0; i < col_count; i++)
        Columns[i]->Delete(iRowIndex);
}

void CWStringTable::DeleteCol(int iColIndex)
{
    if(iColIndex < 0 || iColIndex >= ColCount())
        return;

    delete Columns[iColIndex];
    Columns.erase(Columns.begin() + iColIndex);
}

void CWStringTable::DeleteCol(string sColName)
{
    int col_index = GetColIndex(sColName);
    if(col_index < 0)
        return;

    DeleteCol(col_index);
}

void CWStringTable::SwapCols(int iIndex1, int iIndex2)
{
    std::swap(Columns[iIndex1], Columns[iIndex2]);
}

void CWStringTable::SwapCols(string sColName1, string sColName2)
{
    SwapCols(GetColIndex(sColName1), GetColIndex(sColName2));
}

void CWStringTable::SwapRows(int iIndex1, int iIndex2)
{
    int col_count = ColCount();
    for(int i = 0; i < col_count; i++)
        Columns[i]->Swap(iIndex1, iIndex2);
}

void CWStringTable::Sort(int iColIndex, bool bDescending, int iStart, int iEnd)
{
    int i = iStart;
    int k = iEnd;

    CWStringList* col = Columns[iColIndex];

    if(iEnd - iStart >= 1)
    {
        while(k > i)
        {
            while(StrCmp(col->Strings[i], col->Strings[iStart], bDescending) <= 0 && i <= iEnd && k > i)
                i++;
            while(StrCmp(col->Strings[k], col->Strings[iStart], bDescending) > 0 && k >= iStart && k >= i)
                k--;

            if(k > i)
            {
                if(col->Strings[i] != col->Strings[k])
                    SwapRows(i, k);
            }
        }

        if(col->Strings[iStart] != col->Strings[k])
            SwapRows(iStart, k);

        if(k > iStart)
            Sort(iColIndex, bDescending, iStart, k - 1);
        if(k < iEnd)
            Sort(iColIndex, bDescending, k + 1, iEnd);
    }
}

void CWStringTable::Sort(int iColIndex, bool bDescending)
{
    Sort(iColIndex, bDescending, 0, RowCount() - 1);
}

void CWStringTable::Sort(string sColName, bool bDescending)
{
    Sort(GetColIndex(sColName), bDescending, 0, RowCount() - 1);
}
