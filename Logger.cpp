#include "Logger.h"

#include <iostream>
#include <fstream>

using namespace std;

int Logger::rank;
bool Logger::TO_FILE = true;

void Logger::log(stringstream * ss) {
    Logger::log(ss->str());
}

void Logger::log(string s) {
    if (TO_FILE) {
        ofstream file;
        stringstream name;
        name << "node" << Logger::rank << ".txt";
        const string tmp = name.str();
        const char * cstr = tmp.c_str();
        file.open(cstr, ios::out | ios::ate | ios::app);
        file << s;
        file.close();
    } else {
        cout << s;
    }
}

void Logger::logLn(string s) {
    stringstream ss;
    ss << s << endl;
    Logger::log(&ss);
}

void Logger::ln() {
    stringstream ss;
    ss << endl;
    Logger::log(&ss);
}

void Logger::setRank(int _rank) {
    Logger::rank = _rank;
}