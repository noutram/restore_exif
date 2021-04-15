#include "stdio.h"
#include <iostream>
using namespace std;

#define VERBOSE
int main(int argc, char* argv[])
{
    #ifdef VERBOSE
    cout << "restore_exif " << argc << " parameters\n";
    #endif

    //Base filename
    string fn;

    //Has a filename been passed as a parameter?
    if (argc < 2) {
        //Assume stdin
        cin >> fn;
    } else {
        //argv[1] is the filename
        fn = string(argv[1]);
    }

    #ifdef VERBOSE
    cout << "Processing " << fn << endl;
    #endif

    
    return 0;
}
