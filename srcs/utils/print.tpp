/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 15:11:03 by alerusso          #+#    #+#             */
/*   Updated: 2025/12/30 18:27:40 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_TPP
# define PRINT_TPP

template <typename F, typename S>
std::ostream	&operator<<(std::ostream &stream, const std::pair<F, S> &pair);

template <typename F, typename S>
void	print_map(std::ostream &os, std::map<F, S> &map, std::string name, const char *color, const char *firstline)
{
	if (firstline)
		os << color << firstline << ": " << name << "\033[0m\n";
	if (map.empty())
	{
		os << "map vuota.\n";
		return ;
	}
	os << std::endl;
	for (typename std::map<F, S>::iterator it = map.begin();\
		it != map.end(); it++)
	{
		os << name << " \033[3;37m" << (*it).first << "\033[0m\n    {";
		os << color << (*it).second << "\033[0m\n    }";
		if (it != --map.end())
			os << std::endl;
	}
	os << "\n";
}

template <typename F, typename S>
void	print_map(std::map<F, S> map)
{
	print_map(std::cout, map, "", "\033[0m", NULL);
}

template <typename F, typename S>
std::ostream	&operator<<(std::ostream &stream, const std::pair<F, S> &pair)
{
	stream << pair.first << ", " << pair.second;
	return (stream);
}

template <typename F, typename S>
std::ostream	&operator<<(std::ostream &stream, const std::map<F, S> &map)
{
	print_map(stream, map, "", "\033[0m", NULL);
	return (stream);
}

/*
	for (size_t i = 0; i < srv.server_names.size(); i++)
	{
		if (i != 0)
			os << "\t\t\t";
		os << srv.server_names[i] << std::endl;
	}
*/

template <typename T>
std::ostream	&operator<<(std::ostream &stream, const std::vector<T> &vec)
{
	stream << "\t\t\t";
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (i != 0)
			stream << "\t\t\t";
		stream << vec[i];
		if (i != vec.size() - 1)
			stream << std::endl;
	}
	return (stream);
}

#endif