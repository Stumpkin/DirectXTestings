#pragma once
#include <exception>
#include <string>

using namespace std;

class Catcher : public exception //inherits from std::exception
{
public:
	Catcher(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* getType() const noexcept;
	int getLine() const noexcept;
	const string& getFile() const noexcept;
	string getOriginString() const noexcept;

private:
	int line;
	string file;

protected:
	mutable string whatBuffer;
};