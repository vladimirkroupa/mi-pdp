#ifndef LOGGER_H_
#define LOGGER_H_

#include <sstream>

class Logger {
public:
	static void log(std::stringstream * ss, int rank);
	static void log(std::string str, int rank);
	static void logLn(std::string str, int rank);
	static void ln(int rank);
	static bool TO_FILE;
private:
};

#endif /* LOGGER_H_ */