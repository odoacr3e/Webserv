#ifndef CONF_HPP
# define CONF_HPP

# include "../../includes/ether.hpp"


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

class Conf
{
	private:

	//canonic
	public:
		Conf();
		~Conf();
		Conf(const Conf &other);
		Conf	operator=(const Conf &other);

};

#endif