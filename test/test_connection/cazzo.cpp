
#include "../../includes/ether.hpp"
#include <signal.h>
#include <stdio.h>

int	server_fd = -1;
int client_fd = -1;

void	print_addr(struct sockaddr_in &p)
{
	std::cout << "print of " << &p << ":\n";
	std::cout << "sin_addr\t" << p.sin_addr.s_addr << "\n";
	std::cout << "sin_port\t" << p.sin_port << " ossia: " << LISTENING_PORT << std::endl;
	std::cout << "sin_family\t" << p.sin_family << std::endl << std::endl;
}

void	print_client(struct pollfd *p)
{
	for (int i = 1; i < 6; i++)
	{
		std::cout << i << std::endl;
		std::cout << "fd:\t" << p[i].fd << "\nevents:\t" << p[i].events;
		std::cout << "revents:\t" << p[i].revents << std::endl << std::endl;
	}
}

std::string	create_html(std::string body)
{
	std::string	html;

	html += "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: ";
	html += std::to_string(body.length() + 1);
	html += "\r\n\r\n";
	html += body + "\n";
	std::cout << html << std::endl;
	return (html + "\n");
}

static void	spread_democracy(void);

int	error(std::string err)
{
	perror(err.c_str());
	spread_democracy();
	return (1);
}

void	stop(int sig)
{
	(void)sig;
	spread_democracy();
	error("control C\n");
	exit(1);
}


static void	spread_democracy(void)
{
	if (server_fd != -1)
		close(server_fd);
	if (client_fd != -1)
		close(client_fd);
}

int main()
{
	signal(SIGINT, stop);
	// s.sa_handler = stop;
	// @brief socket(DOMAIN, TYPE, PROTOCOL)
	// se PROTOCOL == 0 allora viene scelto in automatico
	// restituisce il file descriptor oppure -1 in caso di errori
	server_fd = socket(AF_INET, SOCK_STREAM, 0); // La flag CALZA_STREAM è importante -> crea un socket che ascolta in TCP
	if (server_fd == -1)
		return (error("Palle sudate non e andato a buon fine"));

	// @brief struct sockaddr_in address serve a creare una struttura
	struct sockaddr_in address; // https://man7.org/linux/man-pages/man3/sockaddr.3type.html
	address.sin_family = AF_INET; //definisce la famiglia dell'indirizzo
	address.sin_addr.s_addr = INADDR_ANY; //accetta connessione su tutte le interfacce di rete -> ascolta su 0.0.0.0 quindi qualsiasi indirizzo viene ascolt
	address.sin_port = htons(LISTENING_PORT); //converte la porta nell'ordine di byte di rete corrisponde (host to network short) -> converte il numero del formato giusto per la rete
	
	std::cout << "server fd:\t" << server_fd << std::endl;
	print_addr(address);
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) != 0)
		return (error("vaffanculo 1:"));
	
	if (listen(server_fd, 5) != 0)
		return (error("vaffanculo 69:"));

	struct pollfd fds[6];
	for (int i = 0; i < 6; i++)
	{
		// fds[i].revents = 0;
		fds[i].fd = -1;
		fds[i].events = POLLIN;
	}
	fds[0].fd = server_fd;
	std::cout << "Server listening on port: " << LISTENING_PORT << std::endl;
	while (1)
	{
		//La funzione poll aggiorna tutti gli eventi all'interno delle strutture 
		// relative ad ogni socket, andando a modificare le variabili della 
		// struttura per definire se sono stati eseguiti eventi o no
		int ready = poll(fds, 6, -1);
		if (ready < 0)
			return (error("poll"));
		if (fds[0].revents & POLLIN)
		{
			client_fd = accept(server_fd, NULL, NULL);
			if (client_fd == -1)
				return (error("il client è gay"));
			std::cout << "connessione trovata, client: " << client_fd << std::endl;
			for (int i = 1; i <= 5; i++)
			{
				if (fds[i].fd == -1)// il primo libero che trova lo assegna
				{
					fds[i].fd = client_fd;
					fds[i].events = POLLIN;
					std::cout << "fd client in array: " << fds[i].fd << std::endl;
					break;
				}
			}
		}
		for (int i = 1; i <= 5; i++)
		{
			std::cout << "revents Fd[" << i << "]:" << fds[i].revents << std::endl;
			if (fds[i].fd != -1 && (fds[i].revents & (POLLIN) || \
			(fds[i].revents & (POLLOUT)) || (fds[i].revents & (POLLERR))))
			{
				std::cout << "Entrato:\t" << fds[i].fd << std::endl;
				char buffer[1024];
				int bytes = read(fds[i].fd, buffer, sizeof(buffer));
				if (bytes <= 0)
				{
					printf("Client %d disconnesso\n", fds[i].fd);
					close(fds[i].fd);
					fds[i].fd = -1;
				}
				else
				{
					// Rispondo
					std::cout << " ----Sent message----" << std::endl;
					std::string	html = create_html("mega gay");
					send(fds[i].fd, html.c_str(), html.length(), 0);
					close(fds[i].fd);
					fds[i].fd = -1;
				}
			}
			// std::cout << "connessione trovata, client: " << client_fd << std::endl;
			// send(client_fd, html.c_str(), html.length(), 0);
			// close(fds[i].fd);
			// std::cout << "Client chiuso: " << fds[i].fd << std::endl;
			// fds[i].fd = -1;
			// break ;
		}
	}
	spread_democracy();
}

// void	edogay(void)
// {
// 	struct sockaddr_in address; //il nonno di ale è fortissimo

// 	address.sin_addr;
// 	address.sin_family; //edogaycheusaeportegiàusate
// 	address.sin_addr;
// 	address.sin_zero; //---> dal punto di vista funzionale questo elemento della struttura è inutile, si tratta solo di cenvezione per passaggio tra funzioni di questa struttura
// }