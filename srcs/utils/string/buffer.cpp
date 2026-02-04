#include "../../../includes/ether.hpp"

static void	alloc_packets(std::vector<char *> &packets, size_t pos, size_t len);
void		print_packets(std::vector<char *> &packets, size_t &size);

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
		// print_file("REQUEST", "------BEGIN-----\n");
		print_file("REQUEST", packets[i]);
		// print_file("REQUEST", "\n------END-----\n");
		if ((size_t)bytes < packet_size)
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
static void	alloc_packets(std::vector<char *> &packets, size_t pos, size_t len)
{
	if (pos < packets.size() && packets[pos] != NULL)
	{
		packets[pos][0] = 0;
		return ;
	}
	if (pos < packets.size())
		packets[pos] = new char[len + 2];
	else
		packets.push_back(new char[len + 2]);
}

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
