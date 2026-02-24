#include "../../includes/ether.hpp"
#include "../../srcs/hpp/Server.hpp"

typedef std::vector<char *>	argvVector;

void		createArgvCrypter(std::string &args, argvVector &argv_data)
{
	size_t		first_to_delete;
	size_t		decrypt_instruction;
	size_t		crypt_instruction;
	size_t		last_to_delete;

	first_to_delete = args.find("body=") + 5;
	decrypt_instruction = args.find("&decrypt_string=end") + 15;
	crypt_instruction = args.find("&crypt_string=end") + 13;
	crypt_instruction != std::string::npos ? last_to_delete = crypt_instruction : last_to_delete = decrypt_instruction;
	if (first_to_delete != std::string::npos)
	{
		find_and_erase(args, "body=");
		if(last_to_delete != std::string::npos)
			find_and_erase(args, "=end");
		else
			std::cerr << "Bad crypter format: missing endpart\n";
		std::cout << "get_argv(): Post processing BODY: |" << args << "|\n";
	}
	else
		std::cerr << "Bad crypter format missing beginpart\n";
	vect_split_new(argv_data, args, '&');
}

std::string createHtmlCrypter(t_cgi &cgi_ptr)
{
	std::fstream file("www/cgi-bin/crypter/index.html");
	std::string body;

	body = file_opener(file, "");
	std::cout << "RESULT: " << cgi_ptr.output << std::endl;
	find_and_replace(body, "<!-- {RESULT} -->", cgi_ptr.output);
	return (body);
}