#include "stdio.h"
#include <iostream>
#include <fstream>
using namespace std;

#define VERBOSE
int main(int argc, char* argv[])
{
    #ifdef VERBOSE
    cout << "restore_exif " << argc << " parameters\n";
    #endif

    //Base filename
    string fn;
    string fnjpg;

    //Has a filename been passed as a parameter?
    if (argc < 2) {
        //Assume stdin
        cin >> fn;
    } else {
        //argv[1] is the filename
        fn = string(argv[1]);
    }


    //Name of JSON file
    fnjpg = fn + ".json";

    #ifdef VERBOSE
    cout << "Processing " << fn << " using " << fnjpg << endl;
    #endif

    //Open JSON File and parse
    string jsonString;
    ifstream ip;
    ip.open(fnjpg);
    if (!ip) {
        
        return -1;
    }

    return 0;
}
