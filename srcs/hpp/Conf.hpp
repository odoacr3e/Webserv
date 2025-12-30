#ifndef CONF_HPP
# define CONF_HPP

# include "../../includes/ether.hpp"
# define DEFAULT_CONF_PATH "default.conf"
# define DEFAULT_CONF_ROOT "/www/var"
# define DEFAULT_CONF_PORT 8080
# define DEFAULT_CONF_IP "127.0.0.1"
# define DEFAULT_CONF_SERVNAME "localhost"
# define DEFAULT_CONF_BODYSIZE 1024

class	Conf;
struct s_conf_server;
struct s_conf_location;
typedef std::map<std::pair<std::string, int>, t_conf_server> SrvNameMap;

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
	CONF_MISSING_BLOCK,
	CONF_PATH_INVALID,
};

struct s_conf_server
{
	void	set(void);
	void	set_if_empty(Conf &conf);

	std::map<std::string, t_conf_location>	location; // <"/pippo", struct *>
	std::string								root;//root /var/www/html;
	std::string								index;//index /var/www/html;
	std::vector<std::string>				server_names;//server_name example.com www.example.com *example.com;
	int										client_max_body_size;//client_max_body_size 10m;
	bool									listen_set;
	// std::map<>							error_pages;//error_page 404 /404.html;	error_page 500 502 503 504 /50x.html;
	// std::string							access_log;//access_log /var/log/nginx/access.log;
	// std::string							error_log;//error_log /var/log/nginx/access.log;
};

struct s_conf_location
{
	void	set(std::string path);
	void	set_if_empty(Conf &conf);

	std::string		path;//da togliere
	std::string		root;// fa append su URI (root + URI)
	std::string		alias;// sostituisce parola (alias + URI senza parola) -> sempre con / alla fine del path di alias
	std::string		proxy_pass;
};

class Conf
{
	private:
		const std::string			_file;
		bool						_events;
		bool						_http;
		bool						_server;
		bool						_location;

		int							_nevents;
		int							_nhttp;
		int							_nserver;

		int							_nipport;

		//SECTION - settings got from parsing
		//SECTION - main block
		std::string									_user;
		t_conf_server								_srvblock; // temporanea per blocchi server gestiti attualmente
		t_conf_location								_locblock; // temporanea per blocchi location gestiti attualmente
		std::vector<t_conf_server>					_srv_conf; // vettore di blocchi server
		std::map<std::string, std::string>			_server_names;
		SrvNameMap									_srvnamemap; // mappa ipport/server_data
		std::vector<std::pair<std::string, int> >	_ipport; // vettore con tutti ip/porte
		// logica -> nel blocco server tmp mettiamo nella mappa di server block attuale l'URI della location con valore la struct riempita della location
		// usciamo da blocco location e alla fine del blocco server pushamo blocco server temporaneo in _srv_conf

	//canonic
	public:
		Conf(std::string filepath);
		~Conf();
		Conf(const Conf &other);
		Conf	&operator=(const Conf &other);

		// getters
		bool		getEvents() const;
		bool		getHttp() const;
		bool		getServer() const;
		bool		getLocation() const;
		int			getIpPortNumber() const;
		std::string	getCurrLocation() const;
		SrvNameMap	&getSrvNameMap();

		bool	checkIpPort(std::string ip, int port) const;
		// setters
		void	setEvents(bool val);
		void	setHttp(bool val);
		void	setServer(bool val);
		void	setLocation(bool val);
		void	setCurrLocation(std::string curr);
		void	incrementIpPortNumber(void);
		void	setIpPort(std::string ip, int port);
		
		void	updateBlock(int block_type);
		int		getBlockNumber(int block_type);
		
		std::vector<t_conf_server>					&getConfServer(void);
		t_conf_server								&getServerBlock(void);
		t_conf_location								&getLocationBlock();
		t_conf_location								getCopyLocationBlock();
		std::vector<std::pair<std::string, int> >	&getIpPort();
		std::pair<std::string, int>					&getPairIpPort(int i);
		
		std::string	checkOpenBlock(void) const;
		std::string	missingBlock() const;
		
		// main block
		std::string	getMainUser(void) const;
		void		setMainUser(std::string);
		
		void		addServerName(std::string name);
		bool		findServerName(std::string name);

		

		bool		checkDuplicateServerName();
		bool		checkDuplicateIpPort();
		
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

//std::ostream &operator<<(std::ostream &os, Conf &c);

void	confParse(Conf &conf, std::ifstream &fd);

/*
	QUALE SERVER SCEGLIERE?

	//SECTION -	server_name
	-	prendiamo richiesta: leggiamo la richiesta
	-	recuperiamo host dall'header
	-	confronto host con server_name

	1)	se host == server_name
	2)	wildcard
	3)	default server (puo contenere lettere, numeri e punto, in altri casi da errore)
*/



#endif