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

#ifndef CPPW_HTMLPARSER_H
#define CPPW_HTMLPARSER_H

#include "cppw_string.h"


class CWNode;
class CWNodeList;

bool IsVoidElement(string sName);
bool IsInlineElement(string sName);
bool IsBlockElement(string sName);

CWNode* CreateEmptyPage(string sDocType = "html", string sLang = "en", string sCharset = "UTF-8", string sTitle = "");
void EscapeHTMLString(string* pHTML);
string EscapeHTMLString(const string& sHTML);

/// HTML parser (DOM node)
class CWNode
{
        void ParseTag(const char* cInput, int iPos, int* iTagEnd);
        bool Parse(const char* cInput, int* pPos, int iLen, string* pErrorMessage);
    public:
        /// Tag name for example: body
        string Name;
        /// true if this is self-closing tag for example: <br />
        bool SelfClosing;
        /// true if this is void tag (no closing) for example: <meta ... >
        bool Void;

        /// String list of attributes
        CWStringList* Attr;

        /// String list of text inside tag. Has (child_count + 1) strings: every N-th string in list is text before N-th child. Last string is text after last child.
        CWStringList* Text;

        /// List of childs elements
        CWNodeList* Childs;

        /// Constructor
        CWNode(const string& sName = "", const string& sText = "");

        /// Destructor
        ~CWNode();

        /// Deletes all childs and text and resets element name to empty string
        void Clear();

        /// Deletes all childs and text and resets element Name to sName and Text to sText
        void Reset(const string& sName = "", const string& sText = "");

        /// Copy another node to this node
        void CopyFrom(CWNode* pCopyFrom);

        /// Function parses input HTML string and creates DOM tree
        bool ParseHTML(const string& sInput, string* pErrorMessage);

        /// Reverse from ParseHTML: creates HTML string from DOM tree
        string GetHTML(bool bMinify);

        /// Returns index of direct child given by argument pNode
        int IndexOfChild(CWNode* pNode);

        /// Get text contained in node
        string GetText();

        /// set text after last child
        void SetText(const string& sText);
        /// set text before child which index is given. returns false if index is invalid.
        void SetTextBeforeChild(const string& sText, int iChildIndex);
        /// set text before given child. returns false if child is not found.
        void SetTextBeforeChild(const string& sText, CWNode* pChild);
        /// set text after child which index is given. returns false if index is invalid.
        void SetTextAfterChild(const string& sText, int iChildIndex);
        /// set text after given child. returns false if child is not found.
        void SetTextAfterChild(const string& sText, CWNode* pChild);

        /// append text after last child
        void AppendText(const string& sText);
        /// append text before child which index is given. returns false if index is invalid
        void AppendTextBeforeChild(const string& sText, int iChildIndex);
        /// append text before given child. returns false if child is not found.
        void AppendTextBeforeChild(const string& sText, CWNode* pChild);
        /// append text after child which index is given. returns false if index is invalid
        void AppendTextAfterChild(const string& sText, int iChildIndex);
        /// append text after given child. returns false if child is not found.
        void AppendTextAfterChild(const string& sText, CWNode* pChild);

        /// Add class (or list of classes) from given string to attribute "class"). For example AddClass("button green") will add classes "button" and "green" to attribute class.
        void AddClass(const string& sClass);
        /// Remove class (or list of classes) given by string from attribute "class"). For example RemoveClass("button green") will remove classes "button" and "green" from attribute class.
        void RemoveClass(const string& sClass);

        /// Add given child at the end of list of direct childs
        void AddChild(CWNode* pNode, const string& sTextBefore = "", const string& sTextAfter = "");
        /// Add given child at the end of list of direct childs
        CWNode* AddChild(const string& sName, const string& sText = "");

        /// Insert given child into index position in list of direct childs
        void InsertChild(CWNode* pNode, int iIndex, const string& sTextBefore = "", const string& sTextAfter = "");
        /// Insert given child into index position in list of direct childs
        CWNode* InsertChild(const string& sName, int iIndex, const string& sText = "");

        /// Delete direct child with given index.
        void DeleteChild(int iIndex);
        /// Delete given child. If bRecoursive==true searches subtree until child is found. Returns true if child is found and deleted.
        bool DeleteChild(CWNode* pNode, bool bRecoursive);

        /// Remove child but don't destroy it. Returns pointer to removed child.
        CWNode* DetachChild(int iIndex);
        /// Remove child but don't destroy it. Returns true if child is found and removed
        bool DetachChild(CWNode* pNode, bool bRecoursive);

        /// Find first child matching sName. if bRecoursive == true searches entire tree.
        CWNode* FindChildByName(const string& sName, bool bCaseSensitive, bool bRecoursive);
        /// Find child matching sName with attribute name sAttrName and attribute value sAttrValue. if bRecoursive==true searches entire tree.
        CWNode* FindChildByNameAndAttr(const string& sName, const string& sAttrName, const string& sAttrValue, bool bCaseSensitive, bool bRecoursive);
        /// Find first child by attribute "id". if bRecoursive == true searches entire tree.
        CWNode* FindChildByID(const string& sID, bool bCaseSensitive, bool bRecoursive);
        /// Find first child by attribute "class". if bRecoursive == true searches entire tree.
        CWNode* FindChildByClass(const string& sClass, bool bCaseSensitive, bool bRecoursive);
        /// Find first child by selector ".class", "#id" or "name" if bRecoursive == true searches entire tree.
        CWNode* FindChild(const string& sSelector, bool bCaseSensitive, bool bRecoursive);

        CWNode* FindParentOfChild(CWNode* pChild);

        /// Find all childs matching sName. if bRecoursive==true searches entire tree.
        void FindChildsByName(const string& sName, bool bCaseSensitive, bool bRecoursive, CWNodeList* pResult, bool bClearResults);
        /// Find all childs by attribute "id". if bRecoursive==true searches entire tree.
        void FindChildsByID(const string& sID, bool bCaseSensitive, bool bRecoursive, CWNodeList* pResult, bool bClearResults);
};

/// List od CWNode objects (DOM nodes)
class CWNodeList
{
    public:
        vector<CWNode*> Items;
        /// If OwnsObjects==true then destroying list will destroy elements.
        bool OwnsObjects;

        CWNodeList(bool bOwnsObjects = true);
        ~CWNodeList();

        int Count();
        /// Delete all items from list. If OwnsObjects==true all items will be destroyed.
        void Clear();

        void CopyFrom(CWNodeList* pCopyFrom);

        int IndexOf(CWNode* pItem);

        void Add(CWNode* pItem);
        void Insert(CWNode* pItem, int iIndex);
        /// Remove item from list. If OwnsObjects==true item is destroyed.
        void Delete(int iIndex);
        /// Remove item from list. If bRecoursive search entire subtree until item is found. If OwnsObjects==true item is destroyed. Returns true if child is found and deleted.
        bool Delete(CWNode* pItem, bool bRecoursive);
        /// Remove item from list. Item is not destroyed. Returns pointer to item.
        CWNode* Detach(int iIndex);
        /// Remove item from list. If bRecoursive search entire subtree until item is found. Item is not destroyed. Returns true if child is found and deleted.
        bool Detach(CWNode* pItem, bool bRecoursive);

        /// Find item by name.
        CWNode* FindItemByName(const string& sName, bool bCaseSensitive, bool bRecoursive);
        /// Find item by name and attribute.
        CWNode* FindItemByNameAndAttr(const string& sName, const string& sAttrName, const string& sAttrValue, bool bCaseSensitive, bool bRecoursive);
        /// Find item by attribute "id".
        CWNode* FindItemByID(const string& sID, bool bCaseSensitive, bool bRecoursive);
        /// Find item by attribute "class".
        CWNode* FindItemByClass(const string& sClass, bool bCaseSensitive, bool bRecoursive);
        /// Find item by selector. For example ".element_class", "#element_id", "element_name".
        CWNode* FindItem(const string& sSelector, bool bCaseSensitive, bool bRecoursive);

        /// Populates pResult list with all elements matching sName.
        void FindItemsByName(const string& sName, bool bCaseSensitive, bool bRecoursive, CWNodeList* pResult, bool bClearResults);
        /// Populates pResult list with all elements matching sID.
        void FindItemsByID(const string& sID, bool bCaseSensitive, bool bRecoursive, CWNodeList* pResult, bool bClearResults);

};

#endif
