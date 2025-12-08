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
		// bool				_events;
		// bool				_http;
		// bool				_server;
		// bool				_location;

	//canonic
	public:
		Conf(std::string filepath);
		~Conf();
		Conf(const Conf &other);
		Conf	&operator=(const Conf &other);

		// void	validOpenBlock(std::string toFlag) const;
		
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

void	parseMain(Conf &conf, std::ifstream &fd);

#endif