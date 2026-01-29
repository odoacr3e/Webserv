#include "../../includes/ether.hpp"
#include "../hpp/Client.hpp"
#include <sys/wait.h>

static void	run_cmd(char *const argv[], std::string &output);

void	run_script(Client &client, std::string &body)
{
	std::string	output;
	std::cout << "run_script\n";

	const char	*cmd = client.getLocConf().cgiparam[0].second.c_str();
	std::string	url = client.getRequest().getUrl();
	std::string	arg_string(url.substr(url.find_last_of('/') + 1, url.length()));
	const char	*arg = arg_string.c_str();
	char const	*argv[3] = {cmd, arg, NULL};
	run_cmd((char *const *)argv, output);
	std::cout << output << "\n";
	(void)body;
}

static void	run_cmd(char *const argv[], std::string &output)
{
	std::string			url;
	int					pipe_fd[2];

	pid_t	pid;
	if (pipe(pipe_fd) != 0)
		return (std::cout << "run_script fatal error\n", (void)0);
	pid = fork();
	if (pid == -1)//errore
		return (std::cout << "run_script fatal error\n", (void)0);
	else if (pid == 0)//figlio
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		execve(argv[0], argv, NULL);
		std::cout << "run_script fatal error\n";
		exit(1);
	}//sugar daddy
	close(pipe_fd[1]);
	wait(NULL);
	std::string	filename("/dev/fd/" + ft_to_string(pipe_fd[0]));
	std::cout << filename << std::endl;
	//std::fstream	output_fd(filename.c_str(), std::ios_base::in);
	std::ifstream	output_fd(filename.c_str(), std::ios_base::in);
	std::getline(output_fd, output, '\0');
	std::cout << filename << std::endl;
	close(pipe_fd[0]);
}
