
#include "../../../includes/ether.hpp"
#include "../../hpp/Conf.hpp"

void	parseMain(Conf &conf, std::ifstream &fd) 
{
	std::string					line;
	std::vector<std::string>	token;
	int i = 0;

	(void)conf;
	while (std::getline(fd, line)) //ciclo finche non trovo http o events come parola
	{
		i++;
		while (!line.empty())
		{
			std::cout << "\033[33mLine " << i << ": " << line << "\033[0m" << std::endl;
			line = removeWhitespaces(line);
			if (!line.empty())
				token.push_back(line.substr(0, find_first_whitespace(line)));
			std::cout << "\033[34mToken: " << (*token.rbegin()) << "\033[0m" << std::endl;
			//se trovi caratteri speciali, resetti vector e analizzi
			line = line.substr(find_first_whitespace(line));
			//std::cout << "Line 2: " << line << std::endl;
		}
	}
	for (size_t i = 0; i < token.size(); ++i) 
    	std::cout << token[i] << " \n";
}

// http
// {
// 	server
// 	{
		
// 	}	
// }

// http    xvzv   dsdcsdf 
// {
//  	server
// 	{
// 		location
// 		{
		
// 		}
// 		location
// 		{
		
// 		}
// 	}
// 	server
// 	{
		
// 	}
// }*/