#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <map>
#include <exception>

using namespace std;

static int max_len = 1000;

class Worker
{
public:
	virtual vector<string> execute(vector<string>& text) = 0;
};

class Workflow_exception : std::exception {
public:
	string error;
	Workflow_exception(string& err_massage);
	const char* number();
};

class Read_file: public Worker
{
public:
	string file_name;
	explicit Read_file(string filename);
	virtual vector<string> execute(vector<string>& text);
};

class Write_file : public Worker
{
public:
	string file_name;
	explicit Write_file(string filename);
	virtual vector<string> execute(vector<string>& text);
};

class Grep : public Worker
{
public:
	string word;
	explicit Grep(string input_word);
	virtual vector<string> execute(vector<string>& text);
};

class Sort : public Worker
{
public:
	explicit Sort();
	virtual vector<string> execute(vector<string>& text);
};

class Replace : public Worker
{
public:
	string old_word;
	string new_word;
	explicit Replace(string word1, string word2);
	virtual vector<string> execute(vector<string>& text);
};

class Dump : public Worker
{
public:
	string file_name;
	explicit Dump(string filename);
	virtual vector<string> execute(vector<string>& text);
};
//////////////////
vector<int> get_sequence(string string);

void do_command(string line, vector<string>& text);

void workflow_execute(string filename);

class Validator {
public:
	void is_valid(string filename);
};
