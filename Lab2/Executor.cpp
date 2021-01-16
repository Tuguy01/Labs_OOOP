#include "Executor.h"

Read_file::Read_file(string filename)
{
	file_name = filename;
}

Write_file::Write_file(string filename)
{
	file_name = filename;
}

Grep::Grep(string input_word)
{
	word = input_word;
}

Replace::Replace(string word1, string word2)
{
	old_word = word1;
	new_word = word2;
}

Dump::Dump(string filename)
{
	file_name = filename;
}

vector<string>Read_file::execute(vector<string>& text)
{

	string string_temp;
	ifstream input_file;

	input_file.open(this->file_name);
	while (!input_file.eof() && getline(input_file, string_temp, '\n'))
		text.push_back(string_temp);

	input_file.close();

	return text;
}

vector<string>Write_file::execute(vector<string>& text)
{
	ofstream out_file;
	out_file.open(this->file_name);

	for (int i = 0; i < text.size(); i++)
		out_file << text.at(i) << endl;

	out_file.close();
	return text;
}

vector<string>Grep::execute(vector<string>& text)
{
	int text_size = text.size();
	for (int i = 0; i < text_size; i++)
	{
		if (text.at(i).find(word) == string::npos)
		{
			text.erase(text.begin() + i);
			i--;
		}
	}

	return text;
}

vector<string>Replace::execute(vector<string>& text)
{
	for (int i = 0; i < text.size(); i++)
	{
		size_t start = text.at(i).find(old_word);
		if (start != string::npos)
			text.at(i).replace(start, old_word.size(), new_word);
	}

	return text;
}

vector<string>Dump::execute(vector<string>& text)
{
	ofstream fout;
	fout.open(this->file_name);

	string temp_string;
	for (int i = 0; i < text.size(); i++)
		fout << text.at(i) << endl;

	fout.close();
	return text;
}

vector<int> get_sequence(string sequence_string)
{
	int i = 0;
	int temp_begin = 0;
	vector<int> sequence;

	do
	{
		if (sequence_string.at(i) == ' ' && sequence_string.at(i - 1) != '>')
		{
			string str_tmp = sequence_string.substr(temp_begin, i - temp_begin);
			int number = stoi(str_tmp);
			sequence.push_back(number);
		}
		i++;
	} while (i < sequence_string.size());

	string str_tmp = sequence_string.substr(temp_begin, i - temp_begin);
	int number = stoi(str_tmp);
	sequence.push_back(number);

	return sequence;
}

pair<int, string> get_block(string block_string)
{

	int i = 0;
	while (block_string.at(i) != ' ')
		i++;

	string str_number = block_string.substr(0, i);
	int number = stoi(str_number);

	i++;
	while (block_string.at(i) != ' ')
		i++;
	i++;

	int start = i;
	while (block_string.at(i) != '\n' && block_string.at(i) != '\r')
		i++;

	string command = block_string.substr(start, i - start);

	pair<int, string> result(number, command);
	return result;
}

void execute_command(string line, vector<string>& text)
{
	int i = 0;
	while (line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r')
	{
		i++;
		if (i >= line.size())
			break;
	}
	string current_command = line.substr(0, i);

	int temp_begin = i + 1;
	string first_argument_str;
	char* first_argument = (char*)"";
	if (i < line.size() && line.at(i) != '\n' && line.at(i) != '\r')
	{
		i++;
		while (line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r')
		{
			i++;
			if (i >= line.size())
				break;
		}
		first_argument_str = line.substr(temp_begin, i - temp_begin);

		first_argument = (char*)first_argument_str.c_str();
	}

	temp_begin = i + 1;
	string second_argument_str;
	char* second_argument = (char*)"";
	if (i < line.size() && line.at(i) != '\n' && line.at(i) != '\r')
	{
		i++;
		while (line.at(i) != ' ' && line.at(i) != '\n' && line.at(i) != '\r')
		{
			i++;
			if (i >= line.size())
				break;
		}
		second_argument_str = line.substr(temp_begin, i - temp_begin);

		second_argument = (char*)second_argument_str.c_str();
	}

	if (!current_command.compare("readfile"))
	{
		Read_file command(first_argument);
		command.execute(text);
	}
	else if (!current_command.compare("writefile"))
	{
		Write_file command(first_argument);
		command.execute(text);
	}
	else if (!current_command.compare("grep"))
	{
		Grep command(first_argument);
		command.execute(text);
	}
	else if (!current_command.compare("sort") || !current_command.compare("sort\n") || !current_command.compare("sort\r"))
	{
		Sort command;
		command.execute(text);
	}
	else if (!current_command.compare("replace") || !current_command.compare("replace\n") || !current_command.compare("replace\r"))
	{
		Replace command(first_argument, second_argument);
		command.execute(text);
	}
	else if (!current_command.compare("dump"))
	{
		Dump command(first_argument);
		command.execute(text);
	}
}

void workflow_execute(string filename) {
	Validator validator;
	try
	{
		validator.is_valid(filename);
	}
	catch (Workflow_exception & error)
	{
		cerr << error.number() << endl;
		return;
	}

	ifstream fin;
	string temp_string;
	fin.open(filename.c_str());

	getline(fin, temp_string, '\n');
	getline(fin, temp_string, '\n');

	map<int, string> commands_map;

	while (temp_string.compare("csed\n") && temp_string.compare("csed\r"))
	{
		pair<int, string> to_map = get_block(temp_string);
		commands_map.insert(to_map);
		getline(fin, temp_string, '\n');
	}

	getline(fin, temp_string, '\n');
	vector<int> sequence = get_sequence(temp_string);

	vector<string> text;
	for (int i = 0; i < sequence.size(); i++)
		do_command(commands_map[sequence.at(i)], text);

	return;
}

void Validator::is_valid(string filename)
{
	ifstream input_file;
	input_file.open(filename.c_str());
	string temp_string;

	getline(input_file, temp_string, '\n');
	string string_desc = temp_string.substr(0, 4);

	if (string_desc.compare("desc"))
	{
		string str("Error: no desc");
		throw Workflow_exception(str);
		return;
	}

	vector<int> numbers;
	const int max_commands = 1000;
	int commands = 0;
	getline(input_file, temp_string, '\n');
	while (temp_string.compare("csed\n") && temp_string.compare("csed\r") && temp_string.compare("csed") && commands < max_commands)
	{
		int i = 0;
		while (i < temp_string.size() && temp_string.at(i) != ' ')
			i++;
		string str_number = temp_string.substr(0, i);
		if (isdigit(str_number.at(0)))
		{
			int number = stoi(str_number);
			numbers.push_back(number);
		}
		for (int j = 0; j < str_number.size(); j++)
		{
			if (!isdigit(str_number.at(j)))
			{
				string str("Error: sequence is not all numbers");
				throw Workflow_exception(str);
				return;
			}
		}

		i++;
		if (i < temp_string.size() && temp_string.at(i) != '=')
		{
			string str("Error: no =");
			throw Workflow_exception(str);
			return;
		}
		i++;
		i++;

		int start = i;
		if (i >= temp_string.size())
		{
			string str("Error: no command");
			throw Workflow_exception(str);
			return;
		}
		while (i < temp_string.size() && temp_string.at(i) != ' ' && temp_string.at(i) != '\n' && temp_string.at(i) != '\r')
		{
			i++;
			if (i >= temp_string.size())
				break;
		}
		string command = temp_string.substr(start, i - start);

		if (command.compare("readfile") && command.compare("writefile") && command.compare("grep") && command.compare("sort") && command.compare("replace") && command.compare("dump"))
		{
			string str("Error: invalid command");
			throw Workflow_exception(str);
			return;
		}

		commands++;
		if (input_file.eof())
		{
			string str("Error: no csed");
			throw Workflow_exception(str);
			return;
		}

		getline(input_file, temp_string, '\n');
	}

	getline(input_file, temp_string, '\n');
	for (int j = 0; j < temp_string.size(); j++)
	{
		if (temp_string.at(j) == '>' && temp_string.at(j - 1) != '-')
		{
			string str("Error: invalid sequence");
			throw Workflow_exception(str);
		}
		else if (!isdigit(temp_string.at(j)) && temp_string.at(j) != ' ' && temp_string.at(j) != '\r' && temp_string.at(j) != '\n' && temp_string.at(j) != '-' && temp_string.at(j) != '>')
		{
			string str("Error: invalid sequence");
			throw Workflow_exception(str);
		}
	}

	vector<int> seq = get_sequence(temp_string);
	for (int j = 0; j < seq.size(); j++) {
		bool is_in = false;
		for (int k = 0; k < numbers.size(); k++)
		{
			if (numbers.at(k) == seq.at(j))
				is_in = true;
		}
		if (!is_in)
		{
			string str("Error: invalid sequence");
			throw Workflow_exception(str);
		}
	}
	input_file.close();
}

Workflow_exception::Workflow_exception(string& err_massage) : error{ err_massage } {}

const char* Workflow_exception::number()
{
	return error.c_str();
}
