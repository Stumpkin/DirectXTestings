#include "Catcher.h"
#include <sstream>

using namespace std;

Catcher::Catcher(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* Catcher::what() const noexcept
{
	ostringstream oss;
	oss << getType() << endl;
	oss << getOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Catcher::getType() const noexcept
{
	return "Catcher Exception";
}

int Catcher::getLine() const noexcept
{
	return line;
}

const string& Catcher::getFile() const noexcept
{
	return file;
}

string Catcher::getOriginString() const noexcept
{
	ostringstream oss;
	oss << "[File] " << file << endl;
	oss << "[Line] " << line;
	return oss.str();
}