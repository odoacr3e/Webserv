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