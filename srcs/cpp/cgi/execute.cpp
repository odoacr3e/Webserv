#include "../../hpp/Cgi.hpp"


void	vect_split_free(std::vector<char *> &vect, size_t size);

/// @brief	 exec cgi using pipe, fork, dup2 and execve. cgi data is saved in server 
///	@section implementation			
///				1)	forks. If it's a fastcgi and a process already exist, skip
///				2)	STDIN/STDOUT are dup in the child. pipe are saved in s_cgi
///				3)	s_cgi data are binded with the client in server::fd_data vector
/// @param srv 
/// @param client 
void	s_cgi::exec(Server &srv, Client &client)
{
	std::vector<char *> argv;

	get_argv(client, argv);
	if (client.getLocConf().fastcgi_bool == true)
		exec_fastcgi(srv, client, *this, argv);
	else
		exec_cgi(srv, client, *this, argv);
}

/// @brief exec a normal cgi. These cgi process MUST finish when output is done 
/// @param srv 
/// @param client 
/// @param cgi_data 
/// @param argv reference to a std::vector<char *>, that is sent to execve
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
		srv.isChildProcessBool() = true;
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

/// @brief exec a fastcgi. These cgi process MUST run in a infinite loop
///			in Server::suppressSocket, all fastcgi processes are killed
/// @param srv 
/// @param client 
/// @param cgi_data 
/// @param argv reference to a std::vector<char *>, that is sent to execve
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
			srv.isChildProcessBool() = true;
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
			cgi_data.clear(false);
			delete cgi_ptr;
			return (std::cerr << "run_script fatal error: no cookies\n", (void)0);
		}
		cgi_ptr->poll_index[0] = srv.addSocket(cgi_data.pipe[0], FD_PIPE_RD);
	}
	client.bindCgiSocket(srv, *cgi_ptr);
	vect_split_free(argv, argv.size() - 1);
}