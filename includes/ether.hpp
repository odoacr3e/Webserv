#ifndef ETHER_HPP
# define ETHER_HPP

# include <iostream>
# include <fstream>
# include <ostream>
# include <sstream>
# include <string>
# include <poll.h>
# include <sys/unistd.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/poll.h>
# include <netinet/in.h>
# include <unistd.h>
# include <vector>
# include <algorithm>
# include <map>
# include <cstring>
# include <cstdlib>
# include <sstream>
# include <dirent.h>
# include <sys/types.h>
# include <fcntl.h>

# include "../srcs/hpp/Request.hpp"
# include "../srcs/utils/print.tpp"

# define COLOR_RESET "\033[0m"
# define DEBUG 1

#if defined(DEBUG) && DEBUG
  #define DBG_MSG(msg) (std::cout << "\033[31m" << (msg) << COLOR_RESET << std::endl)
#else
  #define DBG_MSG(msg) ((void)0)
#endif

class Request;
typedef struct s_conf_server	t_conf_server;
typedef std::pair<std::string, int>	IpPortPair;
typedef std::map<IpPortPair, t_conf_server> SrvNameMap;

template <typename T>
std::string	ft_to_string(T data)
{
	std::ostringstream	stream;

	stream << data;
	return (stream.str());
}

int			requestParsing(Request &request, std::string input);

//SEXTION - utils
std::string	removeWhitespaces(std::string line);
size_t		find_first_not_whitespace(std::string line);
size_t		find_first_whitespace(std::string line);
bool		is_there_whitespaces(std::string line);
bool 		charFinder(const std::string literal, int(*check)(int));

size_t		find_first_special_char(std::string line);
bool		is_there_special_chars(std::string line);
size_t		find_first_not_special_char(std::string line);

bool		valid_directory(std::string directory);
bool		valid_file(std::string file);
int			valid_ip_address(std::string addr);
int			atohex(std::string s);
int 		checkValidCode(int code);

#define CHARIZARD "                 .\"-,.__\n\
                 `.     `.  ,\n\
              .--'  .._,\'\"-' `.\n\
             .    .'         `'\n\
             `.   /          ,'\n\
               `  '--.   ,-\"'\n\
                `\"`   |  \n\
                   -. \\, |\n\
                    `--Y.'      ___.\n\
                         \\     L._, \n\
               _.,        `.   <  <\\                _\n\
             ,' '           `, `.   | \\            ( `\n\
          ../, `.            `  |    .\\`.           \\ \\_\n\
         ,' ,..  .           _.,'    ||\\l            )  \'\".\n\
        , ,'   \\           ,'.-.`-._,'  |           .  _._`.\n\
      ,' /      \\ \\        `' ' `--/   | \\          / /   ..\n\
    .'  /        \\ .         |\\__ - _ ,'` `        / /     `.`.\n\
    |  '          ..         `-...-  |  `-'      / /        . `.\n\
    | /           |L__           |    |          / /          `. `.\n\
   , /            .   .          |    |         / /             ` `\n\
  / /          ,. ,`._ `-_       |    |  _   ,-' /               ` \n\
 / .           \"\"`_/. `-_ \\_,.  ,'    +-' `-'  _,        ..,-.    \\`.\n\
  '         .-f    ,'   `    '.       \\__.---'     _   .'   '     \\ \n\
' /          `.'    l     .' /          \\..      ,_|/   `.  ,'`     L`\n\
|'      _.-\"\"` `.    \\ _,'  `            \\ `.___`.'\"\"`-.  , |   |    | \n\
||    ,'      `. `.   '       _,...._        `  |    `/ '  |   '     .|\n\
||  ,'          `. ;.,.---' ,'       `.   `.. `-'  .-' /_ .'    ;_   ||\n\
|| '              V      / /           `   | `   ,'   ,' '.    !  `. ||\n\
||/            _,-------7 '              . |  `-'    l         /\\    `||\n\
 |          ,' .-   ,' ||               | .-.        `.      .'\\     ||\n\
 `\'        ,'    `\"\".\'    |               |    `.        '. -.'     `'\n\
          /      ,'      |               |,'    \\-.._,.'/'\n\
          .     /        .               .       \\    .''\n\
        .`.    |         `.             /         :_,'.'\n\
          \\ `...\\   _     ,'-.        .'         /_.-'\n\
           `-.__ `,  `'   .  _.>----''.  _  __  /\n\
                .'        /\"\"'          |  \"\"\'   \'_\n\
               /_|.-'\\ ,\"\".             \'.\'`__\'-( \n\
                 / ,\"'\"\"\\,'               `/  `-.|\"\n\
"

#define PIEDI_DELLA_ZIA_DEL_TUO_RAGAZZO "       ;-.               ,\n\
        \\ '.           .'/\n\
         \\  \\ .---. .-' /\n\
          '. '     `\\_.'\n\
            |(),()  |     ,\n\
            (  __   /   .' \\\n\
           .''.___.'--,/\\_,|\n\
          {  /     \\   }   |\n\
           '.\\     /_.'    /\n\
            |'-.-',  `; _.'\n\
            |  |  |   | \n\
            `\"\"`\"\"`\"\"\"   "

#endif