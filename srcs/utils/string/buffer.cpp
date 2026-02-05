
#include "../../../includes/ether.hpp" 


// static void	alloc_packets(std::vector<char *> &packets, size_t pos, size_t len);
// void		print_packets(std::vector<char *> &packets, size_t &size);

/* 
** 1. Prendere content length
** 2. Cercare boundary e salvarlo (se c'e)
** 3. ciclare finche byte letti (nel totale) sono diversi da content length
		OPPURE bytes != 0
** 4. Controllare boundary finale (-- + boundary + --) oppure continuazione (-- + boundary)
** 5. 
*/

int	ft_recv(int fd, Request &request)
{
	std::cout << "ft_recv, fd is: " << fd << "\n";
	// std::cout << "INCHINATEVI DINANZI A MEGA GABIBBO " << CHARIZARD << "\n";
	if (fd < 0)
		return (-69);

	size_t				tot = 0;
	size_t				bodyLength = request.getBodyLen();
	char				buf[2048] = {0};
	std::vector<char>	body;
	body.reserve(bodyLength + 1);
	std::string			bound;
	std::string			contType = request.getHeaderVal("Content-Type");
	// int					DEBUG_COUNTER = 0;
	// int					left = bodyLength;
	int bytes;

	std::fstream	destroyer("REQUEST", std::ios_base::trunc);
	print_file("REQUEST", request.getBody());
	if (contType.find("boundary") != std::string::npos)
	{
		bound = contType.substr(contType.find("boundary=") + std::string("boundary=").length());
		std::cout << "BOUNDARY TROVATO: " << bound << std::endl;
	}
	std::cout << "2 - ft_recv, fd is: " << fd << "\n";
	// while (tot < bodyLength)
	// {
	// 	/* std::cout << "left: " << left << "\n";
	// 	if (left >= 2048)
	// 		bytes = recv(fd, buf, 2048, 0);
	// 	else
	// 		bytes = recv(fd, buf, bodyLength - tot, 0);
	// 	std::cout << "Bytes: " << bytes << std::endl;
	// 	std::cout << "Tot bytes: " << tot << std::endl;
	// 	if (std::string(buf).find(bound) != std::string::npos)
	// 	{
	// 		std::cout << "HO TROVATO IL ONE PIECE" << std::endl;
	// 	}
	// 	print_file("REQUEST", buf);
	// 	if (bytes <= 0)
	// 	{
	// 		std::cout << "left: " << left << "\n";
	// 		std::cout << "ft_recv: exit with bytes " << bytes << ", counter " << DEBUG_COUNTER << "\n";
	// 		std::cout << "ft_recv: fd is " << fd << "\n";
	// 		break ; 
	// 	}
	// 	left -= bytes;
	// 	tot += bytes;
	// 	body.insert(body.end(), buf, buf + bytes); */
	// 	size_t to_read = std::min(bodyLength - tot, (size_t)2048);
	// 	bytes = recv(fd, buf, to_read, 0);
		
	// 	if (bytes <= 0)
	// 		break;
		
	// 	tot += bytes;
	// 	body.insert(body.end(), buf, buf + bytes);
	// }
	std::cout << "=== INIZIO RECV ===" << std::endl;
	std::cout << "Content-Length: " << bodyLength << std::endl;

	while (tot < bodyLength)
	{
		size_t to_read = std::min(bodyLength - tot, (size_t)2048);
		std::cout << "Provo a leggere " << to_read << " bytes (tot finora: " << tot << ")" << std::endl;
		
		bytes = recv(fd, buf, to_read, 0);
		std::cout << "recv() ha ritornato: " << bytes << std::endl;
		
		if (bytes <= 0)
			break;
		
		tot += bytes;
	}

	std::cout << "=== FINE RECV: tot=" << tot << ", bodyLength=" << bodyLength << " ===" << std::endl;
	// for (size_t i = 0; i != body.size(); i++)
	// {
	// 	print_file("PORNO_EMMA_WATSON.ico", body[i]);
	// }
	// std::remove("PORNO_EMMA_WATSON.ico");
	// print_file("PORNO_EMMA_WATSON.ico", body.data());
	std::cout << "MEGA GABIBBO\n";
	exit(0);
	return (69 - 69);
}

/*
int	ft_recv(int fd, std::vector<char *> &packets, size_t packet_size)
{
	int		bytes = 1;
	size_t	total = 0;
	size_t	i = 0;

	if (fd == -1)
		return (-1);
	alloc_packets(packets, 0, packet_size);
	bytes = recv(fd, packets[0], packet_size, 0); 
	while (1)
	{
		if (bytes < 0)
			return (-1);
		total += bytes;
		std::string pck = packets[i];
		// std::cout << "find boundary: " << (pck.find("--") == std::string::npos ? "not found" : "found") << std::endl;
		// boundary=--------------abc
		// -- + --------------abc
		// -- + --------------abc + --
		if (pck.find("Content-Length") != std::string::npos)
		{
			size_t contlen = std::atol(pck.substr(pck.find("Content-Length")).c_str());
			std::cout << "SIZE: " << contlen << std::endl << std::endl;
		}
		// print_file("REQUEST", "------BEGIN-----\n");
		print_file("REQUEST", packets[i]);
		// print_file("REQUEST", "\n------END-----\n");
		if ((size_t)bytes <= 0)
			break ;
		alloc_packets(packets, ++i, packet_size);
		bytes = recv(fd, packets[i], packet_size, 0);
	}
	packets[i][bytes] = 0;
	++i;
	if (i < packets.size() && packets[i] != NULL)
		packets[i][0] = 0;
	print_packets(packets, total);
	print_file("REQUEST", "---DELIMITATORE LEGGIBILE DA UN UMANO---");
	return (total);
}
*/

void	ft_to_string(std::vector<char *> &packets, std::string &request_buff)
{
	request_buff.clear();
	for (std::vector<char *>::iterator it = packets.begin(); it != packets.end(); it++)
	{
		if ((*it)[0] == 0)
			break ;
		request_buff += (*it);
	}
}

//allocs only when needed
// static void	alloc_packets(std::vector<char *> &packets, size_t pos, size_t len)
// {
// 	if (pos < packets.size() && packets[pos] != NULL)
// 	{
// 		packets[pos][0] = 0;
// 		return ;
// 	}
// 	if (pos < packets.size())
// 		packets[pos] = new char[len + 2];
// 	else
// 		packets.push_back(new char[len + 2]);
// }

void	print_packets(std::vector<char *> &packets, size_t &size)
{
	int	n_package = 0;

	std::cout << "size: " << size << std::endl;
	for (std::vector<char *>::iterator it = packets.begin(); it != packets.end(); it++)
	{
		if ((*it)[0] == 0)
			break ;
		std::cout << "\n\033[33mpackage number \033[0m" << n_package++ << std::endl;
		for (int i = 0; (*it)[i]; i++)
			std::cout << (*it)[i];
	}
	std::cout << "\033[33mPRINT END \033[0m\n";
}
