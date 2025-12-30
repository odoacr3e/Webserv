/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.tpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edraccan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 15:11:03 by alerusso          #+#    #+#             */
/*   Updated: 2025/12/30 17:25:02 by edraccan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRINT_TPP
# define PRINT_TPP

template <typename F, typename S>
void	print_map(std::ostream &os, std::map<F, S> map, std::string name, const char *color, const char *firstline)
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
void	print_map(std::ostream &os, std::map<F, S> map, std::string map_name)
{
	print_map(os, map, map_name, "\033[0m", NULL);
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