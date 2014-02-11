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

#include "cppw_htmlparser.h"

void EscapeHTMLString(string* pHTML)
{
    ReplaceSubString(pHTML, "<", "&lt;");
    ReplaceSubString(pHTML, ">", "&gt;");
    ReplaceSubString(pHTML, "&", "&amp;");
    ReplaceSubString(pHTML, "\"", "&quot;");
    ReplaceSubString(pHTML, "\'", "&#39;");
    ReplaceSubString(pHTML, "\r", "&#13;");
    ReplaceSubString(pHTML, "\n", "&#10;");
}

string EscapeHTMLString(const string& sHTML)
{
    string res = sHTML;
    EscapeHTMLString(&res);
    return res;
}

CWNode* CreateEmptyPage(string sDocType, string sLang, string sCharset, string sTitle)
{
    CWNode* root = new CWNode();
    root->AddChild("!doctype");
    if(sDocType == "")
        root->Attr->Add("html");
    else
        root->Attr->Add(sDocType);

    CWNode* html = root->AddChild("html");
    if(sLang != "")
        html->Attr->SetValue("lang", sLang);

    CWNode* head = html->AddChild("head");
    if(sCharset != "")
    {
        CWNode* meta_charset = head->AddChild("meta");
        meta_charset->Attr->SetValue("charset", sCharset);

    }
    head->AddChild("title", sTitle);

    html->AddChild("body");
    return root;
}

bool IsVoidElement(string sName)
{
    if((strcasecmp(sName.c_str(), "!doctype") == 0) ||
        (strcasecmp(sName.c_str(), "area") == 0) ||
        (strcasecmp(sName.c_str(), "base") == 0) ||
        (strcasecmp(sName.c_str(), "br") == 0) ||
        (strcasecmp(sName.c_str(), "col") == 0) ||
        (strcasecmp(sName.c_str(), "command") == 0) ||
        (strcasecmp(sName.c_str(), "embed") == 0) ||
        (strcasecmp(sName.c_str(), "hr") == 0) ||
        (strcasecmp(sName.c_str(), "img") == 0) ||
        (strcasecmp(sName.c_str(), "input") == 0) ||
        (strcasecmp(sName.c_str(), "keygen") == 0) ||
        (strcasecmp(sName.c_str(), "link") == 0) ||
        (strcasecmp(sName.c_str(), "meta") == 0) ||
        (strcasecmp(sName.c_str(), "param") == 0) ||
        (strcasecmp(sName.c_str(), "source") == 0) ||
        (strcasecmp(sName.c_str(), "track") == 0) ||
        (strcasecmp(sName.c_str(), "wbr") == 0))
            return true;
    return false;
}


bool IsInlineElement(string sName)
{
    if((strcasecmp(sName.c_str(), "b") == 0) ||
        (strcasecmp(sName.c_str(), "big") == 0) ||
        (strcasecmp(sName.c_str(), "i") == 0) ||
        (strcasecmp(sName.c_str(), "small") == 0) ||
        (strcasecmp(sName.c_str(), "tt") == 0) ||
        (strcasecmp(sName.c_str(), "abbr") == 0) ||
        (strcasecmp(sName.c_str(), "acronym") == 0) ||
        (strcasecmp(sName.c_str(), "cite") == 0) ||
        (strcasecmp(sName.c_str(), "code") == 0) ||
        (strcasecmp(sName.c_str(), "dfn") == 0) ||
        (strcasecmp(sName.c_str(), "em") == 0) ||
        (strcasecmp(sName.c_str(), "kbd") == 0) ||
        (strcasecmp(sName.c_str(), "strong") == 0) ||
        (strcasecmp(sName.c_str(), "samp") == 0) ||
        (strcasecmp(sName.c_str(), "var") == 0) ||
        (strcasecmp(sName.c_str(), "a") == 0) ||
        (strcasecmp(sName.c_str(), "bdo") == 0) ||
        (strcasecmp(sName.c_str(), "br") == 0) ||
        (strcasecmp(sName.c_str(), "img") == 0) ||
        (strcasecmp(sName.c_str(), "map") == 0) ||
        (strcasecmp(sName.c_str(), "object") == 0) ||
        (strcasecmp(sName.c_str(), "q") == 0) ||
        (strcasecmp(sName.c_str(), "script") == 0) ||
        (strcasecmp(sName.c_str(), "span") == 0) ||
        (strcasecmp(sName.c_str(), "sub") == 0) ||
        (strcasecmp(sName.c_str(), "sup") == 0) ||
        (strcasecmp(sName.c_str(), "button") == 0) ||
        (strcasecmp(sName.c_str(), "input") == 0) ||
        (strcasecmp(sName.c_str(), "label") == 0) ||
        (strcasecmp(sName.c_str(), "select") == 0) ||
        (strcasecmp(sName.c_str(), "textarea") == 0))
			return true;

	return false;
}

bool IsBlockElement(string sName)
{
    if((strcasecmp(sName.c_str(), "address") == 0) ||
        (strcasecmp(sName.c_str(), "figcaption") == 0) ||
        (strcasecmp(sName.c_str(), "ol") == 0) ||
        (strcasecmp(sName.c_str(), "article") == 0) ||
        (strcasecmp(sName.c_str(), "figure") == 0) ||
        (strcasecmp(sName.c_str(), "output") == 0) ||
        (strcasecmp(sName.c_str(), "aside") == 0) ||
        (strcasecmp(sName.c_str(), "footer") == 0) ||
        (strcasecmp(sName.c_str(), "p") == 0) ||
        (strcasecmp(sName.c_str(), "audio") == 0) ||
        (strcasecmp(sName.c_str(), "form") == 0) ||
        (strcasecmp(sName.c_str(), "pre") == 0) ||
        (strcasecmp(sName.c_str(), "blockquote") == 0) ||
        (strcasecmp(sName.c_str(), "h1") == 0) ||
        (strcasecmp(sName.c_str(), "h2") == 0) ||
        (strcasecmp(sName.c_str(), "h3") == 0) ||
        (strcasecmp(sName.c_str(), "h4") == 0) ||
        (strcasecmp(sName.c_str(), "h5") == 0) ||
        (strcasecmp(sName.c_str(), "h6") == 0) ||
        (strcasecmp(sName.c_str(), "section") == 0) ||
        (strcasecmp(sName.c_str(), "canvas") == 0) ||
        (strcasecmp(sName.c_str(), "header") == 0) ||
        (strcasecmp(sName.c_str(), "table") == 0) ||
        (strcasecmp(sName.c_str(), "dd") == 0) ||
        (strcasecmp(sName.c_str(), "hgroup") == 0) ||
        (strcasecmp(sName.c_str(), "tfoot") == 0) ||
        (strcasecmp(sName.c_str(), "div") == 0) ||
        (strcasecmp(sName.c_str(), "hr") == 0) ||
        (strcasecmp(sName.c_str(), "ul") == 0) ||
        (strcasecmp(sName.c_str(), "dl") == 0) ||
        (strcasecmp(sName.c_str(), "video") == 0) ||
        (strcasecmp(sName.c_str(), "fieldset") == 0) ||
        (strcasecmp(sName.c_str(), "noscript") == 0))
			return true;

	return false;
}
CWNode::CWNode(const string& sName, const string& sText)
{
    Attr = new CWStringList();
    Text = new CWStringList();
    Childs = new CWNodeList();

    Reset(sName, sText);
}

CWNode::~CWNode()
{
    delete Childs;
    delete Text;
    delete Attr;
}

void CWNode::Clear()
{
    Name = "";
    SelfClosing = false;
    Void = false;
    Attr->Clear();
    Text->Clear();
    Text->Add("");
    Childs->Clear();
}

void CWNode::Reset(const string& sName, const string& sText)
{
    Clear();
    Name = sName;
    Void = IsVoidElement(Name);
    SetText(sText);
}

void CWNode::CopyFrom(CWNode* pCopyFrom)
{
    Clear();
    if(pCopyFrom == NULL)
        return;

    Name = pCopyFrom->Name;
    SelfClosing = pCopyFrom->SelfClosing;
    Void = pCopyFrom->Void;
    Attr->Assign(pCopyFrom->Attr);
    Text->Assign(pCopyFrom->Text);
    Childs->CopyFrom(pCopyFrom->Childs);
}

void CWNode::ParseTag(const char* cInput, int iPos, int* pTagEnd)
{
    int pos = iPos;
    *pTagEnd = -1;

    CWStringList tokens;
    int word_start = iPos;
    bool inside_double_quotes = false;
    bool inside_single_quotes = false;
    bool tag_end = false;
    while(cInput[pos] != 0 && !tag_end)
    {
        // ignore text under quotes
        if(cInput[pos] == '\"' && !inside_single_quotes) inside_double_quotes = !inside_double_quotes;
        if(cInput[pos] == '\'' && !inside_double_quotes) inside_single_quotes = !inside_single_quotes;

        // space, tab, '<', '>' and '=' is end of word
        if(!inside_double_quotes && !inside_single_quotes && (cInput[pos] == ' ' || cInput[pos] == '\t' || cInput[pos] == '\r' || cInput[pos] == '\n' || cInput[pos] == '<' || cInput[pos] == '>' || cInput[pos] == '=' || cInput[pos] == '/'))
        {
            // add word
            if(word_start < pos)
            {
                int cstart = word_start;
                int cpos = pos;
                int clen = pos - word_start;

                if(cInput[cstart] == '\"' || cInput[cstart] == '\'')
                {
                    cstart++;
                    if(cInput[word_start + (clen - 1)] == '\"' || cInput[word_start + (clen - 1)] == '\'')
                        cpos--;
                }
                tokens.Add(string(cInput + cstart, cpos - cstart));
            }

            if(cInput[pos] == '<' || cInput[pos] == '>' || cInput[pos] == '=' || cInput[pos] == '/')
            {
                tokens.Add(string(cInput + pos, 1));
                if(cInput[pos] == '>')
                {
                    tag_end = true;
                    *pTagEnd = pos;
                }
                pos++;
            }
            else
            {
                // remove duplicate spaces, tabs and newlines
                while(cInput[pos] == ' ' || cInput[pos] == '\t' || cInput[pos] == '\r' || cInput[pos] == '\n')
                    pos++;
            }
            word_start = pos;
        }
        else
            pos++;
    }

    // find name and attributes
    int token_count = tokens.Count();
    int name_found = false;
    for(int i = 0; i < token_count; i++)
    {
        string token = tokens.Strings[i];

        string minus1, plus1, plus2 = "";
        if(i > 0) minus1 = tokens.Strings[i - 1];
        if(i < token_count - 1) plus1 = tokens.Strings[i + 1];
        if(i < token_count - 2) plus2 = tokens.Strings[i + 2];

        if(token != "<" && token != ">" && token != "/")
        {
            if(!name_found)
            {
                Name = token;
                name_found = true;
            }
            else
            {
                if(plus1 == "=")
                {
                    Attr->Add(token + "=" + plus2);
                    i+= 2;
                }
                else
                    Attr->Add(tokens.Strings[i]);
            }
        }
        // self closing or void?
        if(token == ">")
        {
            if(minus1 == "/")
            {
                // self closing
                SelfClosing = true;
            }
            else
            {
                Void = IsVoidElement(Name);
            }
        }

    }
}

bool CWNode::Parse(const char* cInput, int* pPos, int iLen, string* pErrorMessage)
{
    const char* input = cInput;
    int pos = 0;
    if(pPos != NULL) pos = *pPos;
    int len = iLen;
    string text = "";

    bool tag_closed = false;

    bool inside_comment = false;
    bool inside_double_quotes = false;
    bool inside_single_quotes = false;
    bool inside_script = strcasecmp(Name.c_str(), "script") == 0 || strcasecmp(Name.c_str(), "style") == 0;
    while(pos < len)
    {
        char current = 0;
        char plus1 = 0;
        char plus2 = 0;
        char plus3 = 0;
        char minus1 = 0;
        char minus2 = 0;

        current = input[pos];
        if(pos < len - 1) plus1 = input[pos + 1];
        if(pos < len - 2) plus2 = input[pos + 2];
        if(pos < len - 3) plus3 = input[pos + 3];

        if(pos > 0) minus1 = input[pos - 1];
        if(pos > 1) minus2 = input[pos - 2];

        // begin of string?
        if(inside_script && !inside_comment && !inside_single_quotes && current == '\"' && minus1 != '\\') inside_double_quotes = !inside_double_quotes;
        if(inside_script && !inside_comment && !inside_double_quotes && current == '\'' && minus1 != '\\') inside_single_quotes = !inside_single_quotes;
        // comment start?
        if(!inside_script && !inside_double_quotes && !inside_single_quotes && !inside_comment && current == '<' && plus1 == '!' && plus2 == '-' && plus3 == '-') inside_comment = true;
        // comment end?
        if(!inside_script && !inside_double_quotes && !inside_single_quotes && inside_comment && current == '>' && minus1 == '-' && minus2 == '-') inside_comment = false;

        if(!inside_comment && !inside_double_quotes && !inside_single_quotes)
        {
            if(!inside_script && current == '<' && plus1 != '/')
            {
                CWNode* node = new CWNode();
                AddChild(node, text, "");
                text = "";

                int tag_end = 0;
                node->ParseTag(input, pos, &tag_end);
                if(tag_end < 0)
                {
                    if(pErrorMessage != NULL) *pErrorMessage = "Malformed input.";
                    return false;
                }

                pos = tag_end;

                if(!node->SelfClosing && !node->Void)
                {
                    pos++;
                    if(!node->Parse(input, &pos, len, pErrorMessage))
                        return false;
                }
            }
            else
            {
                if(current == '<' && plus1 == '/')
                {
                    int tag_end = FindChar(input, '>', pos);
                    if(tag_end < 0)
                    {
                        if(pErrorMessage != NULL) *pErrorMessage = "Malformed input.";
                        return false;
                    }
                    pos = tag_end;
                    tag_closed = true;
                    break;
                }
                else
                    text.append(input + pos, 1); // SLOW!
            }
        }
        else
            text.append(input + pos, 1); // SLOW!

        pos++;
    }
    AppendText(text);

    if(pPos != NULL) *pPos = pos;

    if(pPos != NULL && !tag_closed)
    {
        if(pErrorMessage != NULL) *pErrorMessage = "Malformed input.";
        return false;
    }
    return true;
}

bool CWNode::ParseHTML(const string& sInput, string* pErrorMessage)
{
    Clear();
    int input_len = sInput.size();
    const char* input = sInput.c_str();
    return Parse(input, NULL, input_len, pErrorMessage);
}

string CWNode::GetHTML(bool bMinify)
{
    string text = "";
    if(Name != "")
    {
        text.append("<");
        text.append(Name);
        int attr_count = Attr->Count();
        for(int i = 0; i < attr_count; i++)
        {
            if(Attr->IsNameValue(i))
            {
                string escaped_value = Attr->GetValue(i);
                ReplaceSubString(&escaped_value, "\"", "&quot;");
                text.append(" " + Attr->GetName(i) + "=\"" + escaped_value + "\"");

            }
            else
                text.append(" " + Attr->Strings[i]);
        }

        if(!SelfClosing && !Void) text.append(">");
    }

    bool minify_childs = bMinify && Name != "pre" && Name != "textarea" && Name != "code" && Name != "style";

    int child_count = Childs->Count();
    for(int i = 0; i < child_count; i++)
    {
        if(minify_childs)
        {
            string prev_child_name = "";
            string next_child_name = "";
            if(i > 0) prev_child_name = Childs->Items[i - 1]->Name;
            if(i < child_count - 1) next_child_name = Childs->Items[i + 1]->Name;

            bool no_trim_spaces = Name == "p";
            // bootstrap - glyph spacing - preserve spaces if previous or next child is "span"
            no_trim_spaces = no_trim_spaces || prev_child_name == "span" || next_child_name == "span";

            string tmp = Trim(Text->Strings[i], !no_trim_spaces, true);
            ReplaceSubString(&tmp, "\r\n", "");
            ReplaceSubString(&tmp, "\r", "");
            ReplaceSubString(&tmp, "\n", "");
            text.append(tmp);
        }
        else
            text.append(Text->Strings[i]);

        text.append(Childs->Items[i]->GetHTML(minify_childs));
    }

    if(minify_childs)
    {
        string prev_child_name = "";
        if(child_count > 0) prev_child_name = Childs->Items[child_count - 1]->Name;
        bool no_trim_spaces = Name == "p";
        // bootstrap - glyph spacing - preserve spaces if previous child is "span"
        no_trim_spaces = no_trim_spaces || prev_child_name == "span";
        string tmp = Trim(Text->Strings[child_count], !no_trim_spaces, true);
        ReplaceSubString(&tmp, "\r\n", "");
        ReplaceSubString(&tmp, "\r", "");
        ReplaceSubString(&tmp, "\n", "");
        text.append(tmp);
    }
    else
        text.append(Text->Strings[child_count]);

    if(Name != "")
    {
        if(SelfClosing)
            text.append(" />");
        else
        {
            if(Void)
                text.append(">");
            else
            {
                text.append("</");
                text.append(Name);
                text.append(">");
            }
        }

        // bootstrap - add whitespace becouse button spacing
        if(Name == "div")
            text.append(" ");
    }

    return text;
}

int CWNode::IndexOfChild(CWNode* pNode)
{
    return Childs->IndexOf(pNode);
}

string CWNode::GetText()
{
    string res = "";
    int count = Text->Count();
    for(int i = 0; i < count; i++)
    {
        if(i > 0)
            res.append(LINE_TERM);
        res.append(Text->Strings[i]);
    }
    return res;
}

void CWNode::SetText(const string& sText)
{
    Text->Strings[Childs->Count()] = sText;
}

void CWNode::SetTextBeforeChild(const string& sText, int iChildIndex)
{
    // set text
    Text->Strings[iChildIndex] = sText;
}

void CWNode::SetTextBeforeChild(const string& sText, CWNode* pChild)
{
    // find child
    int index = IndexOfChild(pChild);

    // set text
    Text->Strings[index] = sText;
}

void CWNode::SetTextAfterChild(const string& sText, int iChildIndex)
{
    // set text
    Text->Strings[iChildIndex + 1] = sText;
}

void CWNode::SetTextAfterChild(const string& sText, CWNode* pChild)
{
    // find child
    int index = IndexOfChild(pChild);

    // set text
    Text->Strings[index + 1] = sText;
}

void CWNode::AppendText(const string& sText)
{
    Text->Strings[Childs->Count()].append(sText);
}

void CWNode::AppendTextBeforeChild(const string& sText, int iChildIndex)
{
    // set text
    Text->Strings[iChildIndex].append(sText);
}

void CWNode::AppendTextBeforeChild(const string& sText, CWNode* pChild)
{
    // find child
    int index = IndexOfChild(pChild);

    // set text
    Text->Strings[index].append(sText);
}

void CWNode::AppendTextAfterChild(const string& sText, int iChildIndex)
{
    // set text
    Text->Strings[iChildIndex + 1].append(sText);
}

void CWNode::AppendTextAfterChild(const string& sText, CWNode* pChild)
{
    // find child
    int index = IndexOfChild(pChild);

    // set text
    Text->Strings[index + 1].append(sText);
}

void CWNode::AddClass(const string& sClass)
{
    if(sClass == "")
        return;

    CWStringList new_list;
    StringToList(sClass, ' ', &new_list);

    CWStringList old_list;
    StringToList(Attr->GetValue("class"), ' ', &old_list);

    int new_count = new_list.Count();
    for(int i = 0; i < new_count; i++)
    {
        if(new_list.Strings[i] != "" && old_list.Find(new_list.Strings[i], true) < 0)
            old_list.Add(new_list.Strings[i]);
    }
    Attr->SetValue("class", ListToString(&old_list, ' '));
}

void CWNode::RemoveClass(const string& sClass)
{
    if(sClass == "")
        return;

    CWStringList new_list;
    StringToList(sClass, ' ', &new_list);

    CWStringList old_list;
    StringToList(Attr->GetValue("class"), ' ', &old_list);

    int new_count = new_list.Count();
    for(int i = 0; i < new_count; i++)
    {
        if(new_list.Strings[i] != "")
        {
            int index = old_list.Find(new_list.Strings[i], true);
            if(index >= 0)
                old_list.Delete(index);
        }
    }
    Attr->SetValue("class", ListToString(&old_list, ' '));
}

void CWNode::AddChild(CWNode* pNode, const string& sTextBefore, const string& sTextAfter)
{
    int child_count = Childs->Count();
    Text->Strings[child_count].append(sTextBefore);
    Childs->Add(pNode);
    Text->Add(sTextAfter);
}

CWNode* CWNode::AddChild(const string& sName, const string& sText)
{
    CWNode* child = new CWNode(sName, sText);
    AddChild(child);
    return child;
}

void CWNode::InsertChild(CWNode* pNode, int iIndex, const string& sTextBefore, const string& sTextAfter)
{
    Text->Strings[iIndex].append(sTextBefore);
    Childs->Insert(pNode, iIndex);
    Text->Insert(sTextAfter, iIndex + 1);
}

CWNode* CWNode::InsertChild(const string& sName, int iIndex, const string& sText)
{
    CWNode* child = new CWNode();
    child->Name = sName;
    child->Void = IsVoidElement(child->Name);
    child->SetText(sText);
    InsertChild(child, iIndex);
    return child;
}

void CWNode::DeleteChild(int iIndex)
{
    Childs->Delete(iIndex);
    Text->Strings[iIndex].append(Text->Strings[iIndex + 1]);
    Text->Delete(iIndex + 1);
}

bool CWNode::DeleteChild(CWNode* pNode, bool bRecoursive)
{
    int child_id = IndexOfChild(pNode);
    if(child_id < 0)
    {
        if(bRecoursive)
        {
            if(Childs->Delete(pNode, bRecoursive))
                return true;
        }
        return false;
    }
    DeleteChild(child_id);
    return true;
}

CWNode* CWNode::DetachChild(int iIndex)
{
    CWNode* item = Childs->Detach(iIndex);
    if(item == NULL)
        return NULL;

    Text->Strings[iIndex].append(Text->Strings[iIndex + 1]);
    Text->Delete(iIndex + 1);
    return item;
}

bool CWNode::DetachChild(CWNode* pNode, bool bRecoursive)
{
    int child_id = IndexOfChild(pNode);
    if(child_id < 0)
    {
        if(bRecoursive)
        {
            if(Childs->Detach(pNode, bRecoursive))
                return true;
        }
        return false;
    }
    DetachChild(child_id);
    return true;
}

CWNode* CWNode::FindChildByName(const string& sName, bool bCaseSensitive, bool bRecoursive)
{
    return Childs->FindItemByName(sName, bCaseSensitive, bRecoursive);
}

CWNode* CWNode::FindChildByNameAndAttr(const string& sName, const string& sAttrName, const string& sAttrValue, bool bCaseSensitive, bool bRecoursive)
{
    return Childs->FindItemByNameAndAttr(sName, sAttrName, sAttrValue, bCaseSensitive, bRecoursive);
}

CWNode* CWNode::FindChildByID(const string& sID, bool bCaseSensitive, bool bRecoursive)
{
    return Childs->FindItemByID(sID, bCaseSensitive, bRecoursive);
}

CWNode* CWNode::FindChildByClass(const string& sClass, bool bCaseSensitive, bool bRecoursive)
{
    return Childs->FindItemByClass(sClass, bCaseSensitive, bRecoursive);
}

CWNode* CWNode::FindChild(const string& sSelector, bool bCaseSensitive, bool bRecoursive)
{
    if(sSelector == "")
        return NULL;

    if(sSelector[0] == '.') return FindChildByClass(sSelector.substr(1, sSelector.size() - 1), bCaseSensitive, bRecoursive);

    if(sSelector[0] == '#') return FindChildByID(sSelector.substr(1, sSelector.size() - 1), bCaseSensitive, bRecoursive);

    return FindChildByName(sSelector, bCaseSensitive, bRecoursive);
}

CWNode* CWNode::FindParentOfChild(CWNode* pChild)
{
    if(pChild == NULL) return NULL;

    int child_count = Childs->Count();
    for(int i = 0; i < child_count; i++)
    {
        if(Childs->Items[i] == pChild)
            return this;

        CWNode* parent = Childs->Items[i]->FindParentOfChild(pChild);
        if(parent != NULL) return parent;
    }
    return NULL;
}

void CWNode::FindChildsByName(const string& sName, bool bCaseSensitive, bool bRecoursive, CWNodeList* pResult, bool bClearResults)
{
    Childs->FindItemsByName(sName, bCaseSensitive, bRecoursive, pResult, bClearResults);
}

void CWNode::FindChildsByID(const string& sID, bool bCaseSensitive, bool bRecoursive, CWNodeList* pResult, bool bClearResults)
{
    Childs->FindItemsByID(sID, bCaseSensitive, bRecoursive, pResult, bClearResults);
}

// --------------


CWNodeList::CWNodeList(bool bOwnsObjects)
{
    OwnsObjects = bOwnsObjects;
}

CWNodeList::~CWNodeList()
{
    Clear();
}

int CWNodeList::Count()
{
    return Items.size();
}

void CWNodeList::Clear()
{
    if(OwnsObjects)
    {
        int item_count = Count();
        for(int i = 0; i < item_count; i++)
            delete Items[i];
    }

    Items.clear();
}

void CWNodeList::CopyFrom(CWNodeList* pCopyFrom)
{
    Clear();
    int child_count = pCopyFrom->Count();
    for(int i = 0; i < child_count; i++)
    {
        CWNode* child = new CWNode();
        child->CopyFrom(pCopyFrom->Items[i]);
        Add(child);
    }
}

int CWNodeList::IndexOf(CWNode* pItem)
{
    int item_count = Count();
    for(int i = 0; i < item_count; i++)
        if(Items[i] == pItem)
            return i;
    return -1;
}

void CWNodeList::Add(CWNode* pItem)
{
    Items.push_back(pItem);
}

void CWNodeList::Insert(CWNode* pItem, int iIndex)
{
    Items.insert(Items.begin() + iIndex, pItem);
}

void CWNodeList::Delete(int iIndex)
{
    if(OwnsObjects)
        delete Items[iIndex];
    Items.erase(Items.begin() + iIndex);
}

bool CWNodeList::Delete(CWNode* pNode, bool bRecoursive)
{
    int item_index = IndexOf(pNode);
    if(item_index < 0)
    {
        if(bRecoursive)
        {
            int item_count = Count();
            for(int i = 0; i < item_count; i++)
            {
                if(Items[i]->DeleteChild(pNode, bRecoursive))
                    return true;
            }
        }
        return false;
    }
    Delete(item_index);
    return true;
}

CWNode* CWNodeList::Detach(int iIndex)
{
    CWNode* item = Items[iIndex];
    Items.erase(Items.begin() + iIndex);
    return item;
}

bool CWNodeList::Detach(CWNode* pNode, bool bRecoursive)
{
    int item_index = IndexOf(pNode);
    if(item_index < 0)
    {
        if(bRecoursive)
        {
            int item_count = Count();
            for(int i = 0; i < item_count; i++)
            {
                if(Items[i]->DetachChild(pNode, bRecoursive))
                    return true;
            }
        }
        return false;
    }
    Detach(item_index);
    return true;
}

CWNode* CWNodeList::FindItemByName(const string& sName, bool bCaseSensitive, bool bRecoursive)
{
    int item_count = Count();
    for(int i = 0; i < item_count; i++)
    {
        if(bCaseSensitive)
        {
            if(Items[i]->Name == sName) return Items[i];
        }
        else
        {
            if(strcasecmp(Items[i]->Name.c_str(), sName.c_str()) == 0) return Items[i];
        }
    }

    if(bRecoursive)
    {
        for(int i = 0; i < item_count; i++)
        {
            CWNode* item = Items[i]->FindChildByName(sName, bCaseSensitive, bRecoursive);
            if(item != NULL) return item;
        }
    }

    return NULL;
}

CWNode* CWNodeList::FindItemByNameAndAttr(const string& sName, const string& sAttrName, const string& sAttrValue, bool bCaseSensitive, bool bRecoursive)
{
    CWNodeList list;
    list.OwnsObjects = false;

    FindItemsByName(sName, bCaseSensitive, bRecoursive, &list, true);

    int item_count = list.Count();
    for(int i = 0; i < item_count; i++)
    {
        CWNode* node = list.Items[i];
        if(bCaseSensitive)
        {
            if(node->Attr->GetValue(sAttrName) == sAttrValue) return node;
        }
        else
        {
            if(strcasecmp(node->Attr->GetValue(sAttrName).c_str(), sAttrValue.c_str()) == 0) return node;
        }
    }

    return NULL;
}

CWNode* CWNodeList::FindItemByID(const string& sID, bool bCaseSensitive, bool bRecoursive)
{
    int item_count = Count();
    for(int i = 0; i < item_count; i++)
    {
        if(bCaseSensitive)
        {
            if(Items[i]->Attr->GetValue("id") == sID) return Items[i];
        }
        else
        {
            if(strcasecmp(Items[i]->Attr->GetValue("id").c_str(), sID.c_str()) == 0) return Items[i];
        }

        if(bRecoursive)
        {
            CWNode* item = Items[i]->FindChildByID(sID, bCaseSensitive, bRecoursive);
            if(item != NULL) return item;
        }
    }

    return NULL;
}

CWNode* CWNodeList::FindItemByClass(const string& sClass, bool bCaseSensitive, bool bRecoursive)
{
    int item_count = Count();
    for(int i = 0; i < item_count; i++)
    {
        CWStringList class_list;
        StringToList(Items[i]->Attr->GetValue("class"), ' ', &class_list);
        if(class_list.Find(sClass, bCaseSensitive) >= 0)
            return Items[i];

        if(bRecoursive)
        {
            CWNode* item = Items[i]->FindChildByClass(sClass, bCaseSensitive, bRecoursive);
            if(item != NULL) return item;
        }
    }

    return NULL;
}

CWNode* CWNodeList::FindItem(const string& sSelector, bool bCaseSensitive, bool bRecoursive)
{
    if(sSelector == "") return NULL;

    if(sSelector[0] == '.') return FindItemByClass(sSelector.substr(1, sSelector.size() - 1), bCaseSensitive, bRecoursive);

    if(sSelector[0] == '#') return FindItemByID(sSelector.substr(1, sSelector.size() - 1), bCaseSensitive, bRecoursive);

    return FindItemByName(sSelector, bCaseSensitive, bRecoursive);
}

void CWNodeList::FindItemsByName(const string& sName, bool bCaseSensitive, bool bRecoursive, CWNodeList* pResult, bool bClearResults)
{
    if(bClearResults) pResult->Clear();

    int item_count = Count();
    for(int i = 0; i < item_count; i++)
    {
        if(bCaseSensitive)
        {
            if(Items[i]->Name == sName) pResult->Add(Items[i]);
        }
        else
        {
            if(strcasecmp(Items[i]->Name.c_str(), sName.c_str()) == 0) pResult->Add(Items[i]);
        }

        if(bRecoursive)
            Items[i]->FindChildsByName(sName, bCaseSensitive, bRecoursive, pResult, false);
    }
}

void CWNodeList::FindItemsByID(const string& sID, bool bCaseSensitive, bool bRecoursive, CWNodeList* pResult, bool bClearResults)
{
    if(bClearResults) pResult->Clear();

    int item_count = Count();
    for(int i = 0; i < item_count; i++)
    {
        if(bCaseSensitive)
        {
            if(Items[i]->Attr->GetValue("id") == sID) pResult->Add(Items[i]);
        }
        else
        {
            if(strcasecmp(Items[i]->Attr->GetValue("id").c_str(), sID.c_str()) == 0) pResult->Add(Items[i]);
        }

        if(bRecoursive)
            Items[i]->FindChildsByID(sID, bCaseSensitive, bRecoursive, pResult, false);
    }
}
