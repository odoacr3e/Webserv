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

// "file=@www/var/"

// Content-Disposition: form-data; name="file"; filename="favicon.ico"
// Content-Type: application/octet-stream\r\n


// #include <fstream>
// #include <iostream>

// struct Dato {
//     int id;
//     double valore;
// };

// int main() {
//     Dato d1{42, 3.14};

//     // Scrittura binaria
//     std::ofstream out("dati.bin", std::ios::binary);
//     if (!out) {
//         std::cerr << "Errore apertura file in scrittura\n";
//         return 1;
//     }
//     out.write(reinterpret_cast<const char*>(&d1), sizeof(Dato));
//     out.close();

//     // Lettura binaria
//     Dato d2{};
//     std::ifstream in("dati.bin", std::ios::binary);
//     if (!in) {
//         std::cerr << "Errore apertura file in lettura\n";
//         return 1;
//     }
//     in.read(reinterpret_cast<char*>(&d2), sizeof(Dato));
//     in.close();

//     std::cout << d2.id << " " << d2.valore << "\n";
//     return 0;
// }

void	print_bin(std::ofstream &ofile, const char *content, size_t size)
{
	ofile.write(content, size);
}

int	ft_recv(int fd, Request &request, char *input, int bytes_first_recv)
{
	std::cout << "ft_recv, fd is: " << fd << "\n";
	// std::cout << "INCHINATEVI DINANZI A MEGA GABIBBO " << CHARIZARD << "\n";
	if (fd < 0)
		return (-69);

	size_t				tot = 0;
	size_t				bodyLength = request.getBodyLen() - bytes_first_recv;
	int					left = bodyLength;
	char				buf[2048] = {0};
	std::vector<char>	body;
	int 				bytes;

	body.insert(body.begin(), input, input + bytes_first_recv);
	(void)tot;
	//SECTION - recv
	std::remove("PORNO_EMMA_WATSON.ico");
	std::ofstream		ofile("PORNO_EMMA_WATSON.ico", std::ios_base::binary);
	while (left)
	{
		bytes = recv(fd, buf, 2048, MSG_DONTWAIT);
		if (bytes == -1)
		{
			//print_file("PORNO_EMMA_WATSON.ico", body);
			//print_in_file = false;
			//perror("failed because ");
			//std::cout << "errno is " << errno << "\n";
			std::cout << "Continue: left is " << left << "\n";
			break;
			// continue;
		}
		if (bytes == 0)
		{
			std::cout << "connessione chiusa\n";
			break ;
		}
		if (left < 0)
		{
			std::cout << "muori JOJO! \n";
			std::abort();
		}
		left -= bytes;
		body.insert(body.end(), buf, buf + bytes);
	}
	//SECTION - print result
		// print_file("PORNO_EMMA_WATSON.ico", body[i]);
	ofile.write(body.data(), body.size());
	//print_file("PORNO_EMMA_WATSON.ico", body.data());
	std::cout << "MEGA GABIBBO\n";
	exit(0);
	return (69 - 69);
}


// --------------------------cNMUpdzlv0oCW51YRqJn7s--


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

/*void	ft_to_string(std::vector<char *> &packets, std::string &request_buff)
{
	request_buff.clear();
	for (std::vector<char *>::iterator it = packets.begin(); it != packets.end(); it++)
	{
		if ((*it)[0] == 0)
			break ;
		request_buff += (*it);
	}
}*/

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

/*void	print_packets(std::vector<char *> &packets, size_t &size)
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
}*/

//robbe
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

	// std::cout << "2 - ft_recv, fd is: " << fd << "\n";
	// std::cout << "=== INIZIO RECV ===" << std::endl;
	// std::cout << "Content-Length: " << bodyLength << std::endl;

	// while (tot < bodyLength)
	// {
	// 	size_t to_read = std::min(bodyLength - tot, (size_t)2048);
	// 	std::cout << "Provo a leggere " << to_read << " bytes (tot finora: " << tot << ")" << std::endl;
		
	// 	bytes = recv(fd, buf, to_read, 0);
	// 	std::cout << "recv() ha ritornato: " << bytes << std::endl;
		
	// 	if (bytes <= 0)
	// 		break;
		
	// 	tot += bytes;
	// }

	// std::cout << "=== FINE RECV: tot=" << tot << ", bodyLength=" << bodyLength << " ===" << std::endl;



/*
	if (contType.find("boundary") != std::string::npos)
	{
		bound = contType.substr(contType.find("boundary=") + std::string("boundary=").length());
		std::cout << "BOUNDARY TROVATO: " << bound << std::endl;
	}
*/