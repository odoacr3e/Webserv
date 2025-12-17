#ifndef CONF_HPP
# define CONF_HPP

# include "../../includes/ether.hpp"
# define CONF_DEFAULT_PATH "default.conf"


// #events
// #http
// # server
// #  location

// #events minimo 0, massimo 1
// #http 1 e uno solo
// #server minimo 1, massimo INFINITI
// #location minimo 0, massimo INFINITI



/*


events{NOME_ELEMENTO VALORE1 VALORE2}

http    xvzv   dsdcsdf 
{
 	server
	{
		location
		{
		
		}
		location
		{
		
		}
	}
	server
	{
		
	}
}*/

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

class Conf
{
	private:
		const std::string	_file;
		bool				_events;
		bool				_http;
		bool				_server;
		bool				_location;\

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

		std::string	checkOpenBlock(void) const;

		// utils
		void	print(void) const;

		// main block
		std::string	getMainUser(void) const;
		void		setMainUser(std::string);
		
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
//conf errors
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
};

void	confParse(Conf &conf, std::ifstream &fd);

#endif