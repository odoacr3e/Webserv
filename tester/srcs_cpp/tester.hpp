#ifndef TESTER_HPP
# define TESTER_HPP

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
# include <sys/wait.h>
# include <fcntl.h>
# include <ctime>

# include "../../includes/ether.hpp"

# define PROG_NAME "webserv"
# define PROG_PATH ".."

void	run_cmd(char *const argv[], std::string &output, bool kill_bool);
void	erase_space(std::string &s);
void	erase_ansi(std::string &s);
int		update_result(std::string file, std::string &output);
int		compare_result(std::string file, std::string &output);

#endif