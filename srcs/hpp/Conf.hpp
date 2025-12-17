#ifndef CONF_HPP
# define CONF_HPP

# include "../../includes/ether.hpp"
# define CONF_DEFAULT_PATH "default.conf"

typedef struct s_conf_server	t_conf_server;

typedef struct s_conf_location	t_conf_location;

enum	e_conf_error
{
	CONF_BLOCK_OPEN,
	CONF_BLOCK_CLOSE,
	CONF_BLOCK_FORMAT,
	CONF_BLOCK_EMPTY,
	CONF_BLOCK_INVALID,
	CONF_MULT_BLOCK,
	CONF_INSTRUCTION_UNFINISHED,
	CONF_INSTRUCTION_EMPTY,
	CONF_MISSING_BLOCK
};

// #events
// #http
// # server
// #  location

// #events minimo 0, massimo 1
// #http 1 e uno solo
// #server minimo 1, massimo INFINITI
// #location minimo 0, massimo INFINITI

//getline
//find(something different from ISSPACE)
// if npos
//	IGNORE
//1)	prendi prima cosa che non e ISSPACE fino a ISSPACE successiva
//2)	push back in un vector
//3)	togliere dalla linea letta
//4.1)	quando trovi ';', analizzi la stringa, ripeti punto 1
//4.2)	quando trovi '{', entri strato/dai errore, ripeti punto 1
//4.3)	quando trovi '}', esci strato/dai errore, ripeti punto 1.


/*
	CONF: salviamo indice struct server corrente
  Struttura configuration 
  
  std::vector<struct server>
  
  struct s_server
  {
	std::map<std::string (name), struct location>;
  }
  struct s_location
  {
	
  }
*/

class Conf
{
	private:
		const std::string	_file;
		bool				_events;
		bool				_http;
		bool				_server;
		bool				_location;

		int					_nevents;
		int					_nhttp;
		int					_nserver;

		//SECTION - settings got from parsing
		//SECTION - main block
		std::string			_user;

	//canonic
	public:
		Conf(std::string filepath);
		~Conf();
		Conf(const Conf &other);
		Conf	&operator=(const Conf &other);

		// getters
		bool	getEvents() const;
		bool	getHttp() const;
		bool	getServer() const;
		bool	getLocation() const;

		// setters
		void	setEvents(bool val);
		void	setHttp(bool val);
		void	setServer(bool val);
		void	setLocation(bool val);

		void	updateBlock(int block_type);
		int		getBlockNumber(int block_type);

		std::string	checkOpenBlock(void) const;
		std::string	missingBlock() const;

		// main block
		std::string	getMainUser(void) const;
		void		setMainUser(std::string);

		enum	e_block_type
		{
			B_EVENTS,
			B_HTTP,
			B_SERVER,
		};

		// exception
		class ConfException: public std::exception
		{
			private:
				std::string _msg;
			public:
				ConfException(std::string msg) : _msg(msg) {};
				virtual const char *what() const throw() {return (_msg.c_str());}
				~ConfException() throw() {};
		};
};

std::ostream &operator<<(std::ostream &os, Conf &c);

void	confParse(Conf &conf, std::ifstream &fd);

/*
	{struttura server}	->	push ARRAY SERVER
*/

struct s_conf_server
{
	std::map<std::string, t_conf_location>	location; // <"/pippo", struct *>
//	std::map<>								error_pages;//error_page 404 /404.html;	error_page 500 502 503 504 /50x.html;
	std::vector<int>						ports;//listen 80; listen 127.0.0.1:8080; listen 443 ssl;
	// std::pair<>								server_name;//server_name example.com www.example.com;
	std::string								root;//root /var/www/html;
	std::string								access_log;//access_log /var/log/nginx/access.log;
	std::string								error_log;//error_log /var/log/nginx/access.log;
	int										client_max_body_size;//client_max_body_size 10m;
};

struct s_conf_location
{
	
};


#endif