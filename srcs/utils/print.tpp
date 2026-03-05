/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpussorolis <lpalerols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 15:11:03 by alerusso          #+#    #+#             */
/*   Updated: 2026/01/14 10:46:56 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_TPP
# define PRINT_TPP

#include "../../includes/ether.hpp"

template <typename F, typename S>
std::ostream	&operator<<(std::ostream &stream, const std::pair<F, S> &pair);

template <typename F, typename S>
void	print_map(std::ostream &os, std::map<F, S> &map, std::string name, const char *color, const char *firstline)
{
	if (firstline)
		os << color << firstline << ": " << name << "\033[0m\n";
	if (map.empty())
		return (os << std::endl, (void)0);
	os << std::endl;
	for (typename std::map<F, S>::iterator it = map.begin();\
		it != map.end(); it++)
	{
		os << name << " \033[3;37m" << (*it).first << "\033[0m\n    {";
		os << color << (*it).second << "\033[0m\n    }";
		if (it != --map.end())
			os << std::endl << std::endl;
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

template <typename T>
std::ostream	&operator<<(std::ostream &stream, std::vector<T> &vec)
{
	stream << "\t";
	for (size_t i = 0; i < vec.size(); i++)
	{
		// if (i != 0)
		// 	stream << "\t\t\t";
		stream << vec[i];
		// if (i != vec.size() - 1)
		// 	stream << std::endl;
	}
	return (stream);
}


// template <typename T>
// void	print_file(std::ofstream &stream, T content)
// {
// 	stream << content;
// }

template <typename T>
void	print_file(std::string filename, T content)
{
	std::ofstream	stream(filename.c_str(), std::ios_base::app);

	if (stream.fail())
	{
		std::cerr << "Errore: " << std::strerror(errno) << std::endl;
		return (std::cerr << "cannot open " + filename + "\n", (void)0);
	}
	stream << content;
}

template <typename T>
void	print_file(std::string filename, T content, int bytes)
{
	std::ofstream	stream(filename.c_str(), std::ios_base::app | std::ios_base::binary);

	if (stream.fail())
	{
		std::cerr << "Errore: " << std::strerror(errno) << std::endl;
		return (std::cerr << "cannot open " + filename + "\n", (void)0);
	}
	stream.write(content, bytes);
}

/*
template <typename T>
void	print_file(std::string filename, T content, std::ios_base::openmode flags)
{
	std::ofstream	stream(filename, flags);

	if (stream.fail())
	{
		return (std::cerr << "cannot open " + filename + "\n", (void)0);
	}
	stream << content;
}*/

#endif