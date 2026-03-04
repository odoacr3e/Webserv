#include "../../hpp/Cgi.hpp"


void	vect_split_free(std::vector<char *> &vect, size_t size);

void		exec_cgi(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv)
{
	if (pipe(cgi_data.pipe) != 0)
		return (std::cout << "run_script fatal error: pipe\n", (void)0);
	fcntl(cgi_data.pipe[0], FD_CLOEXEC);
	fcntl(cgi_data.pipe[1], FD_CLOEXEC);
	cgi_data.pid = fork();
	if (cgi_data.pid == -1)
		return (std::cout << "run_script fatal error: pid\n", (void)0);
	else if (cgi_data.pid == 0)
	{
		dup2(cgi_data.pipe[1], STDOUT_FILENO);
		close(cgi_data.pipe[0]);
		close(cgi_data.pipe[1]);
		srv.suppressSocket();
		execve(argv[0], argv.data(), NULL);
		vect_split_free(argv, argv.size() - 1);
		std::cerr << "run_script fatal error: execve failed!\n";
		std::exit(1);
	}
	vect_split_free(argv, argv.size() - 1);
	close_fd(&cgi_data.pipe[1]);
	s_cgi	*cgi_ptr = new s_cgi(cgi_data);
	client.bindCgiSocket(srv, *cgi_ptr);
}

void		exec_fastcgi(Server &srv, Client &client, t_cgi &cgi_data, argvVector &argv)
{
	s_cgi	*cgi_ptr;

	if (client.getCookieData().exist && client.getCookieData().cgi != NULL)
	{
		cgi_ptr = client.getCookieData().cgi;
		if (srv.getAddrs()[cgi_ptr->poll_index[0]].events & POLLIN)
		{
			vect_split_free(argv, argv.size() - 1);
			std::cout << "goodbye\n";
				return ;
		}
		write(cgi_ptr->pipe[1], argv[1], std::strlen(argv[1]));
	}
	else
	{
		int		pipes[2][2];

		if (pipe(pipes[0]) != 0 || pipe(pipes[1]) != 0)
			return (std::cout << "run_script fatal error: pipe\n", (void)0);
		cgi_data.pipe[0] = pipes[0][0];
		cgi_data.pipe[1] = pipes[1][1];
		fcntl(cgi_data.pipe[0], FD_CLOEXEC);
		fcntl(cgi_data.pipe[1], FD_CLOEXEC);
		cgi_data.pid = fork();
		if (cgi_data.pid == -1)
			return (std::cout << "run_script fatal error: fork\n", (void)0);
		else if (cgi_data.pid == 0)
		{
			dup2(pipes[0][1], STDOUT_FILENO);
			dup2(pipes[1][0], STDIN_FILENO);
			close(pipes[0][0]);
			close(pipes[1][1]);
			srv.suppressSocket();
			execve(argv[0], argv.data(), NULL);
			perror("execve");
			vect_split_free(argv, argv.size() - 1);
			std::cerr << "run_script fatal error: execve\n";
			std::exit(1);
		}
		close(pipes[0][1]);
		close(pipes[1][0]);
		cgi_ptr = new s_cgi(cgi_data);
		if (client.getCookieData().exist == true)
			srv.getCookieMap()[client.getCookieData().id].cgi = cgi_ptr;
		else
		{
			std::cerr << "run_script fatal error: fastcgi needs cookies\n";
			cgi_data.clear();
			delete cgi_ptr;
			return (std::cerr << "run_script fatal error: no cookies\n", (void)0);
		}
		cgi_ptr->poll_index[0] = srv.addSocket(cgi_data.pipe[0], FD_PIPE_RD);
	}
	client.bindCgiSocket(srv, *cgi_ptr);
	vect_split_free(argv, argv.size() - 1);
}