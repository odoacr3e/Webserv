#include "../../../includes/ether.hpp"
#include "../../hpp/Client.hpp"
#include "../../hpp/Server.hpp"

typedef std::vector<char *>	argvVector;

static int 		hex_value(char c);
static void		convert_hexa(std::vector<char*> &input);
void			createArgvCrypter(std::string &args, argvVector &argv);
void			createArgvWeaksleep(std::string &args, argvVector &argv_data);


void		get_argv(Client &client, argvVector &argv)
{
	std::string	url;
	std::string	cmd;
	std::string	args;
	char		separator;
	char		*temp;

	url = client.getRequest().getUrl();
	url.find('?') != std::string::npos ? separator = '?' : separator = '/';
	if (separator == '?')
		cmd = url.substr(0, url.find_last_of('?'));
	else if (client.getLocConf().cgi_path.length() == 0)
		cmd = "CGI_PATH_NOT_FOUND";
	else
		cmd = client.getLocConf().cgi_path;
	if (client.getRequest().getMethodEnum() == POST)
	{
		client.getRequest().getBinBody().push_back('\0');
		args = client.getRequest().getBinBody().data();
		while (find_and_replace(args, "+", " "));
	}
	else
		args = url.substr(url.find_last_of(separator) + 1, url.length());
	std::cout << "get_argv(): args --->" << args << "\n";
	temp = new char[cmd.length() + 1];
	temp[cmd.length()] = 0;
	std::memcpy(temp, cmd.c_str(), cmd.length());
	argv.push_back(temp);
	if (client.getLocConf().script_type == "crypter")
		createArgvCrypter(args, argv);
	else if (client.getLocConf().script_type == "weaksleep")
		createArgvWeaksleep(args, argv);
	else
	{
		find_and_erase(args, "value=");//FIXME - per pokedex
		vect_split_new(argv, args, separator);
	}
	argv.push_back(NULL);
	std::cout << "cmd: " << argv[0] << std::endl;
	convert_hexa(argv);
	for (size_t i = 1; i < argv.size() - 1; i++)
		std::cout << "arg " << i << ": " << argv[i] << std::endl;
}

static int hex_value(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return -1;
}

void convert_hexa(std::vector<char*> &input)
{
    for (size_t idx = 0; idx < input.size(); ++idx)
    {
        char *original = input[idx];
		if(!original)
			continue ;

        size_t len = std::strlen(original);
        char *decoded = new char[len + 1];
        size_t read = 0;
        size_t write = 0;

        while (read < len)
        {
            if (original[read] == '%' && read + 2 < len)
            {
                int high = hex_value(original[read + 1]);
                int low  = hex_value(original[read + 2]);
                if (high != -1 && low != -1)
                {
                    decoded[write++] = static_cast<char>((high << 4) | low);
                    read += 3;
                    continue;
                }
            }
            decoded[write++] = original[read++];
        }
        decoded[write] = '\0';

        delete[] original;
        input[idx] = decoded;
    }
}

