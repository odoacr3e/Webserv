#ifndef MACRO_HPP
# define MACRO_HPP

# define COLOR_RESET "\033[0m"
# define DEBUG true
# define CONF false
# define SERVER true
# define RED     "\033[1;91m"
# define BLUE    "\033[1;94m"
# define WHITE   "\033[1;97m"
# define RESET   "\033[0m"

# if defined(DEBUG) && DEBUG
	#define DBG_MSG(msg) (std::cout << "\033[31m" << (msg) << COLOR_RESET << std::endl)
# else
	#define DBG_MSG(msg) ((void)0)
# endif

# if defined(CONF) && CONF
	#define DBG_CNF(msg) (std::cout << "\033[31m" << (msg) << COLOR_RESET << std::endl)
# else
	#define DBG_CNF(msg) ((void)0)
# endif

# if defined(SERVER) && SERVER
	#define DBG_SRV(msg) (std::cout << "\033[31m" << (msg) << COLOR_RESET << std::endl)
# else
	#define DBG_SRV(msg) ((void)0)
# endif

# if DEBUG == true
#  define LOG_REQUEST(str, ...)	print_file("logs/request.md", str, ##__VA_ARGS__)
#  define LOG_RESPONSE(str)		print_file("logs/response.html", str)
#  define LOG_CGI(char_vector)	std::remove("logs/cgi");print_file("logs/cgi", char_vector)
#  define LOG_HISTORY()			this->printPollInfo("logs/history.md")	
#  define LOG_RESET() 			eraseDirectory("logs/")
#  define LOG_ALL(str)			print_file("logs/request.md", str);\
								print_file("logs/response.html", str);\
								print_file("logs/history.md", str);
# else
#  define LOG_REQUEST(str, ...)	(void)0
#  define LOG_RESPONSE(str)		(void)0
#  define LOG_CGI(str, bytes)	(void)0
#  define LOG_HISTORY()			(void)0	
#  define LOG_RESET() 			eraseDirectory("logs/")
#  define LOG_ALL()				(void)0
# endif

#endif