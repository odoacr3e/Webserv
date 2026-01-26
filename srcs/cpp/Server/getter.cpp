/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 12:46:56 by lparolis          #+#    #+#             */
/*   Updated: 2026/01/26 13:10:13 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../hpp/Server.hpp"

// NOTE - restituisce il vettore di strutture di poll sotto forma di array
struct pollfd *Server::getAddrs(void)
{
	return (this->_addrs.data());
}

size_t	Server::getAddrSize(void) const
{
	return (this->_addrs.size());
}

int	Server::getServerNum() const
{
	return (this->_server_num);
}

SrvNameMap		&Server::getSrvNameMap() const
{
	return (*this->_srvnamemap);
}