#include "Logger.h"

#include <iostream>
#include <fstream>

using namespace std;

bool Logger::TO_FILE = true;

void Logger::log(stringstream * ss, int rank) {
    Logger::log(ss->str(), rank);
}

void Logger::log(string s, int rank) {
    if (TO_FILE) {
        ofstream file;
        stringstream name;
        name << "node" << rank << ".txt";
        const string tmp = name.str();
        const char * cstr = tmp.c_str();
        file.open(cstr, ios::out | ios::ate | ios::app);
        file << s;
        file.close();
    } else {
        cout << s;
    }
}

void Logger::logLn(string s, int rank) {
    stringstream ss;
    ss << s << endl;
    Logger::log(&ss, rank);
}

void Logger::ln(int rank) {
    stringstream ss;
    ss << endl;
    Logger::log(&ss, rank);
}
