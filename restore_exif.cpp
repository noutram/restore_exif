#include "stdio.h"
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include <chrono>
#include <ctime> 

using namespace std;
using namespace rapidjson;

// Depends on rapidjson-dev
// Credit to https://rapidjson.org 

void printChildren(rapidjson::Value& obj);
string preferredTag(Document& d);
//#define VERBOSE

int main(int argc, char* argv[])
{
    //UTC
    time_t now = time(0);
    tm* gmtm = gmtime(&now);
    char *dt = asctime(gmtm);

    ofstream log("restore_exif.log",ofstream::out | ofstream::app);
    #ifdef VERBOSE
    cout << "restore_exif " << argc << " parameters\n";
    #endif
    cout << "Job Started " << dt;
    log << "Job started " << gmtm->tm_hour << ":" << gmtm->tm_min << ":" << gmtm->tm_sec << " " << gmtm->tm_mday << "/" << gmtm->tm_mon << "/" << gmtm->tm_year << ",";

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
    log << "," << fn << "," << fnjpg;

    //Open JSON File and parse
    string jsonString;
    ifstream ip;
    ip.open(fnjpg);
    if (!ip) {
        cout << "Cannot find corresponding JSON file " << fnjpg << endl;
        log << ",No JSON file found" << endl;
        log.close();
        return -1;
    }

    //Read JSON file
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
    string tag = preferredTag(doc);
    if (tag == "") {
        cout << "No suitable JSON tag for " << fnjpg << endl;
        log << ",No JSON timestamp was found" <<  endl;
        log.close();
        return 0; 
    }

    //Get the photoTakenTime
    rapidjson::Value& v = doc["photoTakenTime"];
    string timeStampString = v["timestamp"].GetString();
    string timeFormatted = v["formatted"].GetString();
    cout << timeFormatted << endl;
    cout << "@" << timeStampString << endl;
    log << "," << timeStampString << "," << timeFormatted; 
        
    //Perform timestamp fix
    string cmd = "touch --date=@" + timeStampString + " " + fn;
    cout << cmd << endl;
    int res = system(cmd.c_str());
    if (res != 0) {
        log << ",Failed to touch file" << endl;
    } else {
        log << ",OK" << endl;
    }
    #ifdef VERBOSE
    printChildren(v);
    #endif   

    log.close();
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

string preferredTag(Document& d)
{
    const string preferred[] = {"photoTakenTime", "creationTime"};
    for (string s : preferred) {
        if (d.HasMember(s.c_str())) {
            return s;
        }
    }
    return "";
}

