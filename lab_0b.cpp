#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctype.h>
#include <vector>
#include <algorithm>
using namespace std;

bool comp(pair<string, int> a, pair<string, int> b)
{
	return a.second > b.second;
}

vector <pair<string, int>> creating_vector(map <string, int> *frequency_table)
{
	vector<pair<string, int>> vector_words;
	int map_len = frequency_table->size();
	map <string, int> :: iterator map_it = frequency_table->begin();

	for (int i = 0; i < map_len; i++)
	{
		vector_words.push_back(make_pair(map_it->first, map_it->second));
		map_it++;
	}

	sort(vector_words.begin(), vector_words.end(), comp);
	return vector_words;
}

void create_output_file(ofstream* output_file, vector<pair<string, int>>* vector_words, int number_of_words)
{
	vector<pair<string, int>> ::iterator vector_it = vector_words->begin();
	while (1)
	{
		*output_file << vector_it->first << "," << to_string(vector_it->second) << "," << to_string(((double)vector_it->second / (double)number_of_words) * 100) << "%" << endl;
		vector_it++;
		if (vector_it == vector_words->end())
		{
			return;
		}
	}
}

void reading_file(ifstream* input_file, ofstream* output_file)
{
	string temp_string;
	string temp_word;
	int number_of_words = 0;
	int str_len = 0;
	map <string, int> frequency_table;

	while (!input_file->eof())
	{
		getline(*input_file, temp_string);
		str_len = temp_string.length();

		for (int i = 0; i < str_len; i++)
		{
			if (isalnum(temp_string[i]) != 0)
			{
				temp_word += temp_string[i];
				if (i == str_len - 1)
				{
					number_of_words++;
					frequency_table[temp_word]++;
					temp_word = "";
				}
			}
			else if(i > 0)
			{
				if (isalnum(temp_string[i - 1]) != 0)
				{
					number_of_words++;
					frequency_table[temp_word]++;
					temp_word = "";
				}
			}
		}
	}
	vector<pair<string, int>> vector_words = creating_vector(&frequency_table);
	create_output_file(output_file, &vector_words, number_of_words);
	return;
}

int main(int argc, char** argv) 
{
	if (argc != 3)
	{
		cout << "Wrong input\nTry again\n";
		return 0;
	}

	ifstream input_file;
	input_file.open(argv[1], ios::in);
	if(!input_file.is_open())
	{
		cout << "Input file isn't open\nTry again\n";
		return 0;
	}

	ofstream output_file;
	output_file.open(argv[2], ios::out);
	if (!output_file.is_open())
	{
		cout << "Output file isn't open\nTry again\n";
		return 0;
	}

	reading_file(&input_file, &output_file);

	input_file.close();
	output_file.close();
	return 0;
}
