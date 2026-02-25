#include "../../includes/ether.hpp"
#include "../../srcs/hpp/Server.hpp"

# define GIANLUIGI_BUFFON "\
<!DOCTYPE html>\n\
<html>\n\
<head>\n\
    <title>{NAME}</title>\n\
    <meta http-equiv=\"refresh\" content=\"0; url={LINK}\">\n\
</head>\n\
<body>\n\
    </body>\n\
</html>"

std::string	createHtmlYouTube(t_cgi &cgi_data)
{
	std::string body(GIANLUIGI_BUFFON);

	std::cout << "youtube link: " << cgi_data.output << "\n";
	find_and_replace(body, "{NAME}", "YouTube");
	find_and_replace(body, "{LINK}", cgi_data.output);
	return (body);
}

typedef std::vector<char *>	argvVector;

std::string createHtmlWeaksleep(t_cgi &cgi_data)
{
	(void)cgi_data;
	std::fstream file("www/cgi-bin/weaksleep/success.html");
	std::string body;

	body = file_opener(file, "");
	return (body);
}

void	createArgvWeaksleep(std::string &args, argvVector &argv_data)
{
	
	find_and_erase(args, "number=");
	args = "www/cgi-bin/weaksleep/weaksleep.py&" + args;
	vect_split_new(argv_data, args, '&');
}
