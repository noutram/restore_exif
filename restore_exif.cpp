#include "stdio.h"
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

// Depends on rapidjson-dev
// Credit to https://rapidjson.org 

void printChildren(rapidjson::Value& obj);

//#define VERBOSE

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
        cout << "Cannot find corresponding JSON file " << fnjpg << endl;
        return -1;
    }

    //TBD - Read JSON file
    string ln;
    while (getline(ip, ln)) {
        // Append the text from the file
        jsonString += ln;
    }   
    #ifdef VERBOSE
    cout << jsonString << endl;
    #endif    
    
     //Done with the JSON file
    ip.close();

    //Parse JSON
    Document doc;
    doc.Parse(jsonString.c_str());

    //Find best timestamp
    string tag;
    if (!doc.HasMember(tag.c_str())) {
        return 0; 
    }

    //Get the photoTakenTime
    rapidjson::Value& v = doc["photoTakenTime"];
    string timeStampString = v["timestamp"].GetString();
    string timeFormatted = v["formatted"].GetString();
    cout << timeFormatted << endl;
    cout << "@" << timeStampString << endl;
    
    //Perform timestamp fix
    string cmd = "touch --date=@" + timeStampString + " " + fn;
    cout << cmd << endl;
    system(cmd.c_str());

    #ifdef VERBOSE
    printChildren(v);
    #endif   

    return 0;
}

void printChildren(rapidjson::Value& obj)
{
    static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
    for (Value::ConstMemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr)
    {
        printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
    }
}