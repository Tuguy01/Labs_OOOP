#include <iostream>
#include <exception>
#include <cstring>
#include <string>
#include <iomanip>
#include "Executor.h"

int main(int argc, char* argv[])
{
	if (argc > 1)
  {
		std::string file_name(argv[1]);
		workflow_execute(file_name);
	}

	return 0;
}
