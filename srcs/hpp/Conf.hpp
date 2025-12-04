#ifndef CONF_HPP
# define CONF_HPP

# include "../../includes/ether.hpp"

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