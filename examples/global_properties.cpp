#include <iostream>
#include <OpenGP/SurfaceMesh/SurfaceMesh.h>
using namespace OpenGP;
using namespace std;

int main(int /*argc*/, char** /*argv*/) {
    /// Base mesh
    SurfaceMesh m;

    /// Example #1: adding simple properties
    {
        //--- Add an int property called "prop_int" with the specified value
        m.add_property<int>("prop_int",10);

        //--- Retrieve a reference to the property called "prop_int"
        int& val = m.get_property<int>("prop_int");
        cout << "prop_int: " << val << endl;

        //--- Using the wrong type results in a runtime exception
        // float& valf = m.get_property<float>("prop_int");
    }

    /// Example #2: you can also get a reference directly with add_property
    {
        string& texfile = m.add_property<string>("texture_file","texture.png");
        cout << "texture path: " << texfile << endl;
    }
}
