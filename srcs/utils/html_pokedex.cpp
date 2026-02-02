#include "../../includes/ether.hpp"

#define ID_DATA 0
#define ID_FIELD 1
#define ID_HREF 2
#define ID_STATIC 3
#define HTML_NUMBER 4
#define HTML_FILES "data.html", "field.html", "href_val.html", "static_val.html"
#define HTML_PATH "www/cgi-bin/pokedex/html/"

#define CGI_ID_MOVE 0
#define CGI_ID_PKMN 1
#define CGI_ID_UNKNOWN 2

#define CGI_NAMES "moves", "pokemon"
#define IMG_URL \
 "https://raw.githubusercontent.com/partywhale/pokemon-type-icons/master/icons/{ID}.svg", \
 "https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/other/official-artwork/{ID}.png", \
 "https://media.pokemoncentral.it/wiki/0/02/Sprrz0000.png"

static bool			get_files(std::string html_files[4]);
static std::string	get_img(std::string &output);
static std::string	parse_fields(std::string html_files[4], std::string &output);
static std::string	parse_values(std::vector<std::string> &values, std::string html_files[4]);

/*SECTION - data.html structure

<!-- VARIABLES -->
<!-- 
	KEY: 	the argument given to the cgi
	IMG:	the absolute url for the resource image 
	FIELDS:	the dinamic part of the html (list of data from the cgi output)
-->
<head>
    <title>{KEY}</title>		------>	key is the browser page title
    <style>...</style>
</head>
<body>
<img src="{IMG}" alt="{KEY}">	------>	IMG: see IMG_URL macro above
{FIELDS}						------>	multiple field.html (see below)
</body>
</html>
*/

/// @brief parse cgi output, generates html data that will be sent to client
/// @param key the argument given to the cgi
/// @param output the output of the cgi
/// @return html data
std::string	createHtmlPokedex(std::string &key, std::string &output)
{
	std::string		html_files[4];
	std::string		html;

	if (get_files(html_files))
		return ("<p>missing files!</p>");
	html = html_files[ID_DATA];
	while (find_and_replace(html, "{KEY}", key));
	find_and_replace(html, "{IMG}", get_img(output));
	find_and_replace(html, "{FIELDS}", parse_fields(html_files, output));
	std::fstream	s("CGI_HTML_OUTPUT.html");
	s << html << "\n";
	return (html);
}

/// @brief open html files from macro HTML_FILES
/// @param html_files std::string array that stores the html files content
/// @return 
static bool	get_files(std::string html_files[4])
{
	std::string		html_names[] = {HTML_FILES, ""};
	std::ifstream 	html_stream[4];

	for (int i = 0; i != HTML_NUMBER; i++)
	{
		html_stream[i].open(HTML_PATH + html_names[i]);
		if (html_stream[i].fail())
		{
			std::cerr << "cannot open " << html_names[i] << "\n";
			return (1);
		}
		std::getline(html_stream[i], html_files[i], '\0');
		html_stream[i].close();
	}
	return (0);
}

/// @brief parse cgi_id and img_name from cgi output first line
/// @param output the output of the cgi
/// @return an absolute url of an image
static std::string	get_img(std::string &output)
{
	static std::string	img_url_array[] = {IMG_URL, ""};
	std::string			img_url;
	int					cgi_id;
	std::string			img_name;

	if (output.empty() || output[0] != '{')//output:	"{CGI_ID}img_name|..."
	{
		return (img_url_array[CGI_ID_UNKNOWN]);
	}
	output.erase(0, 1);//					output:		"CGI_ID}img_name|..."
	cgi_id = std::atoi(output.c_str());
	if (trim_diff_left(output, '}') == 1)//	output:		"}img_name|..."
		return (img_url_array[CGI_ID_UNKNOWN]);
	output.erase(0, 1);//					output:		"img_name|..."
	if (cgi_id < 0 || cgi_id >= CGI_ID_UNKNOWN)
		return (img_url_array[CGI_ID_UNKNOWN]);
	img_url = img_url_array[cgi_id];
	size_t	i = output.find('|');
	if (i == std::string::npos)
		return (img_url_array[CGI_ID_UNKNOWN]);
	find_and_replace(img_url, "{ID}", output.substr(0, i));
	output.erase(0, i + 1);//				output:		"..."
	std::cout << "get_img: " << img_url << "\n";	
	return (img_url);
}

/*SECTION - field.html structure

<!-- VARIABLES -->
<!-- 
	FIELD_NAME:	the name of the field
	FIELD_DATA:	html of the field (hypertext or static)
-->
<div class="section box">
	<span class="label">{FIELD_NAME}</span>
	<ul>
		{FIELD_DATA}		------>	multiple *_val.html (see below) 
	</ul>
</div>
*/

/// @brief parse output, generates dinamic part of the html
/// @param html_files std::string array that stores the html files content
/// @param output the output of the cgi
/// @return html dinamic fields
static std::string	parse_fields(std::string html_files[4], std::string &output)
{
	std::stringstream			output_stream;
	std::string					line;
	std::string					html_temp;
	std::string					html_fields;
	std::string					field;
	std::vector<std::string>	values;

	output_stream << output;
	while (std::getline(output_stream, line, '|'))
	{
		if (line == "|" || line.find(':') == std::string::npos)
			continue ;
		html_temp = html_files[ID_FIELD];
		field = line.substr(0, line.find(':'));
		line.erase(0, field.length() + 1);
		values.clear();
		vect_split(values, line, ',');
		find_and_replace(html_temp, "{FIELD_NAME}", field);
		find_and_replace(html_temp, "{FIELD_DATA}", parse_values(values, html_files));
		html_fields += html_temp;
	}
	return (html_fields);
}

/*SECTION - href_val.html and static_val.html structure
	
href_val.html:
<li><a href='/script/pokedex/output/{CGI_NAME}.cgi?value={VAL}'>{VAL}</a></li>
----
static_val.html
<li>{VAL}</li>

-	cgi output is like: |FIELD1:val1,val2,@0val3|FIELD2:val1,...
-	values beginning with @ needs href, for cgi @n (see CGI_ID macro above)
-	split into vector:	std::vector<std::string> values = {val1, val2, @0val3}
-	@0val3 takes CGI_ID 0, that is CGI_NAMES "moves"
-	for these values, the result is the following:
<div class="section box">
	<span class="label">FIELD1</span>
	<ul>
		<li>val1</li>
		<li>val2</li>
		<li><a href='/script/pokedex/output/moves.cgi?value=val3'>val3</a></li>
	</ul>
</div>
*/

/// @brief parse values, generates dinamic part of the html
/// @param values vector in which cgi output values are split 
/// @param html_files std::string array that stores the html files content
/// @param html_str the string of the static html files
/// @return 
static std::string	parse_values(std::vector<std::string> &values, std::string html_files[4])
{
	std::string			html_values;
	std::string			html_temp;
	static std::string	cgi_names[] = {CGI_NAMES, ""};
	std::string			cgi;
	int					type;

	for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); it++)
	{
		type = ID_HREF + ((*it)[0] != '@');
		if (type == ID_HREF)
		{
			cgi = cgi_names[std::atoi((*it).c_str() + 1)];
			(*it).erase(0, 2);
		}
		html_temp = html_files[type];
		find_and_replace(html_temp, "{CGI_NAME}", cgi);
		while (find_and_replace(html_temp, "{VAL}", (*it)));
		html_values += html_temp;
		html_values.push_back('\n');
	}
	return (html_values);
}
