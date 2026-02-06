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
# include <sys/stat.h>
# include <fcntl.h>
# include <ctime>

# include "../srcs/hpp/Conf.hpp"
# include "../srcs/hpp/Request.hpp"
# include "../srcs/utils/print.tpp"
# include "../srcs/utils/string/string.tpp"

# define COLOR_RESET "\033[0m"
# define DEBUG 0

#if defined(DEBUG) && DEBUG
  #define DBG_MSG(msg) (std::cout << "\033[31m" << (msg) << COLOR_RESET << std::endl)
#else
  #define DBG_MSG(msg) ((void)0)
#endif

class Request;
class Client;
class Server;
typedef struct s_conf_server    t_conf_server;
typedef struct s_conf_location	t_conf_location;
typedef std::pair<std::string, int>	IpPortPair;
typedef std::map<IpPortPair, t_conf_server> SrvNameMap;

int	requestParsing(Client &client, char *input, int bytes);

//SEXTION - utils
std::string removeWhitespaces(std::string line);
size_t	    find_first_not_whitespace(std::string line);
size_t      find_first_whitespace(std::string line);
bool        is_there_whitespaces(std::string line);
bool        charFinder(const std::string literal, int(*check)(int));
std::string file_opener(std::fstream &file);
std::string	file_opener(std::fstream &file, std::string msg);

size_t      find_first_special_char(std::string line);
bool        is_there_special_chars(std::string line);
size_t      find_first_not_special_char(std::string line);

void			trim_from(std::string &s, size_t from);
bool		    trim_equal_left(std::string &s, char c);
bool		    trim_diff_left(std::string &s, char c);
bool		    trim_equal_right(std::string &s, char c);
bool		    trim_diff_right(std::string &s, char c);

bool		    valid_directory(std::string directory);
bool		    valid_file(std::string file);

std::string		url_rooting(std::string url, t_conf_server &srv);
std::string		url_rooting(std::string url, t_conf_location &loc);
void			url_normalize(std::string *url);
std::string		url_normalize(std::string url);
std::string		url_arg_remove(std::string url, char limiter);
std::string		url_arg_get(std::string url, char limiter);

int			    valid_ip_address(std::string addr);
int			    atohex(std::string s);
int 		    checkValidCode(int code);

dirent	*findUrlDirectory(std::string url);
void	  ft_ls(std::string path, std::string &files);

// int			ft_recv(int fd, std::vector<char *> &packets, size_t packet_size);
int	ft_recv(int fd, Request &request, char *buf, int bytes);

void	run_script(Server &srv, Client &client, std::string &body);
std::string	env_value(const char **env, std::string key);
void		vect_split(std::vector<std::string> &vect, std::string s, char c);

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