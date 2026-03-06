#include "../../../hpp/Cgi.hpp"

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

/// @brief parse giorgio output
/// @param cgi_ptr reference to cgi data
/// @return new html
std::string	createHtmlYouTube(t_cgi &cgi_data)
{
	std::string body(GIANLUIGI_BUFFON);

	LOG_TERM << "youtube link: " << cgi_data.output << "\n";
	find_and_replace(body, "{NAME}", "YouTube");
	find_and_replace(body, "{LINK}", cgi_data.output);
	return (body);
}

typedef std::vector<char *>	argvVector;

/// @brief parse weaksleep output
/// @param cgi_ptr reference to cgi data
/// @return new html
std::string createHtmlWeaksleep(t_cgi &cgi_data)
{
	(void)cgi_data;
	std::fstream file("www/cgi-bin/weaksleep/success.html");
	std::string body;

	body = open_and_read(file, "");
	return (body);
}
