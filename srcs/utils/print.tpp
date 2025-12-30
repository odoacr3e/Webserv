/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 15:11:03 by alerusso          #+#    #+#             */
/*   Updated: 2025/12/30 16:12:28 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_TPP
# define PRINT_TPP

template <typename F, typename S>
void	print_map(std::ostream &os, std::map<F, S> map, std::string name, const char *color)
{
	os << color << "PRINTING MAP '" << color << name << "\033[0m':\n";
	if (map.empty())
	{
		os << "map" << name << " vuota.\n";
		return ;
	}
	for (typename std::map<F, S>::iterator it = map.begin();\
		it != map.end(); it++)
	{
		os << "key '" << color << (*it).first << "\033[0m': ";
		os << color << (*it).second << "\033[0m'\n";
	}
}

template <typename F, typename S>
void	print_map(std::map<F, S> map, std::string map_name)
{
	print_map(std::cout, map, map_name, "\033[0m");
}

template <typename F, typename S>
void	print_map(std::ostream &os, std::map<F, S> map, std::string map_name)
{
	print_map(os, map, map_name, "\033[0m");
}

template <typename F, typename S>
void	print_pair(std::ostream &os, std::pair<F, S> &pair)
{
	os << "1°: " << pair.first << " 2°: " << pair.second;	
}

template <typename F, typename S>
void	print_pair(std::pair<F, S> &pair)
{
	print_pair(std::cout, pair);
}

#endif