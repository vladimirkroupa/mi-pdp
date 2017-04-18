#ifndef LOGGER_H_
#define LOGGER_H_

#include <sstream>

class Logger {
public:
	static void log(std::stringstream * ss);
	static void log(std::string str);
	static void logLn(std::string str);
	static void ln();
	static void setRank(int rank);
	static bool TO_FILE;
private:
	static int rank;
};

#endif /* LOGGER_H_ */