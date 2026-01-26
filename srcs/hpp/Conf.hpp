#ifndef CONF_HPP
# define CONF_HPP

# include "../../includes/ether.hpp"
# include "../../includes/status_codes.hpp"

class	Conf;
typedef struct s_conf_server	t_conf_server;
typedef struct s_conf_location	t_conf_location;

typedef std::vector<t_conf_server>							ServerVector;
typedef std::vector<std::pair<std::string, int> >			IpPortVector;
typedef std::pair<std::string, int>							IpPortPair;
typedef std::map<IpPortPair, t_conf_server> 				SrvNameMap;
typedef std::vector<std::pair<std::string, std::string> >	CgiParam;
typedef std::map<int, std::string> 							errPages;

//SECTION - default conf settings

# define DEFAULT_CONF_PATH "default.conf"
# define DEFAULT_CONF_INDEX "index.html"
# define DEFAULT_CONF_ROOT "/www/var"
# define DEFAULT_CONF_PORT 8080
# define DEFAULT_CONF_IP "127.0.0.1"
# define DEFAULT_CONF_SERVNAME "localhost"
# define DEFAULT_CONF_BODYSIZE 1024

//SECTION - s_conf_server: the data structure for server block

/*NOTE - structure used to store server configuration.
Stored in:
| VARIABLE_NAME  ||	   GETTER_NAME     || SCOPE
|Conf::_srvblock ||Conf::getServerBlock||data of current server block
|Conf::_srv_conf ||Conf::getConfServer ||vector of all server blocks
|Conf::_srvnamemap|Conf::getSrvNameMap ||map ip pair/server conf
*/
struct s_conf_server
{
	void	set(void);//reset the variables to zero
	void	set_if_empty(Conf &conf);//set empty variables to default values

	std::map<std::string, t_conf_location>	location; // <"/pippo", struct *>
	std::string								root;//root /var/www/html;
	std::string								index;//index /var/www/html;
	std::vector<std::string>				server_names;//server_name example.com www.example.com *example.com;
	int										client_max_body_size;//client_max_body_size 10m;
	bool									listen_set;//is listen istruction set?
	bool									autoindex;//is autoindex tette?
	std::map<int, std::string> 				err_pages;//error codes associated to their error page
};

//SECTION - s_conf_location: the data structure for location block

/*NOTE - structure used to store location configuration.
Stored in:
| VARIABLE_NAME  		||	   GETTER_NAME   || SCOPE
|Conf::_locblock 		||Conf::getLocBlock* ||data of current location block
|t_conf_server::location||		   -		 ||server block location map

_______________________________
*Conf::_locblock has 3 GETTERS.
1)	Conf::getLocBlock		->	returns a REFERENCE of current locblock
2)	Conf::getCopyLocBlock	->	returns a reference of current locblock
3)	Conf::getCurrLocation	->	returns current locblock path variable
*/
struct s_conf_location
{
	void						set(std::string path);
	void						set_if_empty(Conf &conf);
	std::string 				getRetUri() const;
	int							getRetCode() const;
	void						setRetUri(std::string uri);
	void						setRetCode(int code);

	CgiParam					cgiparam;
	std::string					path; //location /images {}
	std::string					root; // fa append su URI (root + URI)
	std::string					alias; // sostituisce parola (alias + URI senza parola) -> sempre con / alla fine del path di alias
	std::string					index;
	std::string					ret_uri;
	std::string					ret_text;
	int							ret_code;
	bool						autoindex; //set autoindex mode on
	std::map<int, std::string>	err_pages;
};

class Conf
{
	private:
		const std::string			_file; //configuration file path
		bool						_events;
		bool						_http;
		bool						_server;
		bool						_location;

		int							_nevents;
		int							_nhttp;
		int							_nserver;

		int							_nipport;

		std::string		_user;
		t_conf_server	_srvblock; // temporanea per blocchi server gestiti attualmente
		t_conf_location	_locblock; // temporanea per blocchi location gestiti attualmente
		ServerVector	_srv_conf; // vettore di blocchi server
		std::map<std::string, std::string>			_server_names;
		SrvNameMap		_srvnamemap; // mappa ipport/server_data
		IpPortVector	_ipport; // vettore con tutti ip/porte
		// logica -> nel blocco server tmp mettiamo nella mappa di server block attuale l'URI della location con valore la struct riempita della location
		// usciamo da blocco location e alla fine del blocco server pushamo blocco server temporaneo in _srv_conf

	//canonic
	public:
		Conf(std::string filepath);
		~Conf();
		Conf(const Conf &other);
		Conf	&operator=(const Conf &other);

		//SECTION - getters (utils/conf/getter.cpp)
		
		//SECTION - block type
		bool			getEvents() const;
		bool			getHttp() const;
		bool			getServer() const;
		bool			getLocation() const;
		int				getBlockNumber(int block_type) const;
		int				getBlockType(void) const;
		
		//SECTION - main block
		std::string		getMainUser(void) const; // TODO - da togliere parsing blocco main
		
		//SECTION - server block
		ServerVector	&getConfServer(void);
		t_conf_server	&getServerBlock(void);
		
		//SECTION - location block
		t_conf_location	&getLocationBlock();
		t_conf_location	getCopyLocationBlock() const;
		std::string		getCurrLocation() const;
		

		//SECTION - ip port
		int				getIpPortNumber() const;
		IpPortPair		&getPairIpPort(int i);
		IpPortVector	&getIpPort();
		
		//SECTION - SrvNameMap
		SrvNameMap		&getSrvNameMap();

		//SECTION - getErrorPages
		const errPages	&getErrPages() const;
		
		//SECTION - setters (utils/conf/setter.cpp)
		
		//SECTION - block type
		void			setEvents(bool val);
		void			setHttp(bool val);
		void			setServer(bool val);
		void			setLocation(bool val, std::string path);
		void			updateBlock(int block_type);

		//SECTION - main type
		void			setMainUser(std::string);

		//SECTION - location block
		void			setCurrLocation(std::string curr);

		
		//SECTION - ip port
		void			incrementIpPortNumber(void);
		void			setIpPort(std::string ip, int port);
		
		//SECTION - Server Name
		void			addServerName(std::string name);
		
		//SECTION - checks (cpp/Conf.cpp)
		bool			checkIpPort(std::string ip, int port) const;
		bool			findServerName(std::string name);//is name already saved?
		bool			checkDuplicateIpPort();
		std::string		checkOpenBlock(void) const;//what is the current block?
		std::string		missingBlock() const;//is a block missing? ("" == no)
		int				checkBlockType(std::string &type) const;
		
		enum	e_block_type
		{
			B_NONE,
			B_EVENTS,
			B_HTTP,
			B_SERVER,
			B_LOCATION,
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

//SECTION - overload

std::ostream &operator<<(std::ostream &os, Conf &c);
std::ostream &operator<<(std::ostream &os, t_conf_server &srv);
std::ostream &operator<<(std::ostream &os, t_conf_location &loc);
std::ostream &operator<<(std::ostream &os, SrvNameMap &map);

//SECTION - enum conf error

//these enum are given to istructionError function to print debug messages
enum	e_conf_error
{
	CONF_VALID,
	CONF_BLOCK_OPEN = Conf::B_LOCATION + 1,
	CONF_BLOCK_CLOSE,
	CONF_BLOCK_UNIFINISHED,
	CONF_BLOCK_FORMAT,
	CONF_BLOCK_EMPTY,
	CONF_BLOCK_INVALID,
	CONF_MULT_BLOCK,
	CONF_INSTRUCTION_UNFINISHED,
	CONF_INSTRUCTION_EMPTY,
	CONF_MISSING_BLOCK,
	CONF_PATH_INVALID,
	CONF_MULT_LOCATION,
};

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