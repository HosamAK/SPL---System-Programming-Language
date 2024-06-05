#pragma once

#include "../include/ConnectionHandler.h"
#include <fstream>
#include <map>

using namespace std;

// TODO: implement the STOMP protocol
class StompProtocol
{
private:
    int id;
    int receipt;
    string username;
    string passcode;
    bool isLogged;
    map<string, int> idMap;
    map<string, map<string, vector<string>>> SummaryEvents;

public:
    StompProtocol();
    void insertHashm(string des);
    int getIDM(string s);
    void deleteID(string s);
    void updateisLogged();
    string createFrame(string input);
    void StringtoArguments(string input, char split, vector<string> &command);
    string helpFunction(string command, vector<string> arguments);
    string HashmaptoString(map<string, string> ConvertMap);
    void insertReport(string report);
    string VectoStr(vector<string> v);
};
