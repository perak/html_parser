#include <iostream>

#include "cppw_htmlparser.h"

int main()
{
    string error_message = "";

    // example html input page
    string input = "";
    input.append("<!doctype html>\n");
    input.append("<html>\n");
    input.append("    <head>\n");
    input.append("        <title>HTML parser</title>\n");
    input.append("    </head>\n");
    input.append("    <body>\n");
    input.append("        <img src=\"http://upload.wikimedia.org/wikipedia/commons/a/a9/Example.jpg\" alt=\"\" />\n");
    input.append("    </body>\n");
    input.append("</html>\n");

    // print original page
    cout << "*** ORIGINAL PAGE ***" << endl << endl;
    cout  << input << endl;

    // parse input
    CWNode tree;
    if(!tree.ParseHTML(input, &error_message))
    {
        cout << "Error parsing HTML document: " << error_message << endl;
        return -1;
    }

    // find body element
    CWNode* body = tree.FindChildByName("body", false, true);
    if(body == NULL)
    {
        cout << "\"body\" element not found!" << endl;
        return -1;
    }

    // create page title and insert as first child of body element
    body->InsertChild("h1", 0, "HTML parser example");

    // create new paragraph and add it as last child of body element
    body->AddChild("p", "This page is created using CppWeb HTML parser.");

    // construct page from tree and print to screen
    string output = tree.GetHTML(false);
    cout << "*** MODIFIED PAGE ***" << endl << endl;
    cout << output << endl;

    // extract images from page
    CWNodeList image_list;
    image_list.OwnsObjects = false;
    tree.FindChildsByName("img", false, true, &image_list, true);

    // print links to all images found in page
    int image_count = image_list.Count();
    cout << "*** FOUND " << image_count << " IMAGE(S) ***" << endl << endl;
    for(int i = 0; i < image_count; i++)
    {
        CWNode* image = image_list.Items[i];
        // read src attribute
        cout << "Image #" << i + 1 << ": " << image->Attr->GetValue("src") << endl;
    }


    return 0;
}
