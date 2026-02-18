#include "../../includes/ether.hpp"
#include "../hpp/Server.hpp"

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
