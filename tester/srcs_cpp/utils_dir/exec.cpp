#include "../tester.hpp"

void	run_cmd(char *const argv[], std::string &output, bool kill_bool);

void	run_cmd(char *const argv[], std::string &output, bool kill_bool)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) != 0)
		return (std::cout << "run_script fatal error\n", (void)0);
	pid = fork();
	if (pid == -1)
		return (std::cout << "run_script fatal error\n", (void)0);
	else if (pid == 0)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		execve(argv[0], argv, NULL);
		std::cerr << "run_cmd fatal error\n";
		std::exit(1);
	}
	close(pipe_fd[1]);
	if (kill_bool)
	{
		sleep(1);
		kill(pid, SIGINT);
	}
	std::string	filename("/dev/fd/" + ft_to_string(pipe_fd[0]));
	std::ifstream	output_fd(filename.c_str(), std::ios_base::in);
	std::getline(output_fd, output, '\0');
	close(pipe_fd[0]);
}

static int			get_file_input(std::string filename, std::string &input);
static int			get_file_output(std::string filename, std::string &output);
static std::string	ft_diff(std::string &output, std::string &correct);

int		update_result(std::string filename, std::string &output)
{
	std::fstream	fd;
	std::string		temp;
	std::string		file_input;

	if (get_file_input(filename, file_input) != 0)
		return (1);
	fd.open(filename.c_str(), std::ios::out | std::ios_base::trunc);
	if (fd.fail())
	{
		std::cerr << "cannot update " << filename << std::endl;
		return (1);
	}
	erase_ansi(output);
	fd << file_input << output;
	return (0);
}

int		compare_result(std::string filename, std::string &output)
{
	std::fstream	fd;
	std::string		temp[2];
	std::string		file_input;
	std::string		file_output;

	if (get_file_output(filename, file_output) != 0)
		return (1);
	erase_ansi(file_output);
	erase_ansi(output);
	temp[0] = file_output;
	temp[1] = output;
	erase_space(file_output);
	erase_space(output);
	if (output == file_output)
		return (0);
	std::cout << "ERROR LOG: test " << filename << "\n";
	std::cout << ft_diff(temp[0], temp[1]);
	return (2);
}

static std::string	ft_diff(std::string &output, std::string &correct)
{
	std::string		diff;
	std::istringstream	fd_out(output);
	std::istringstream	fd_corr(correct);

	output.clear();
	correct.clear();
	diff += "\n";
	while (std::getline(fd_out, output, '\n'))
	{
		std::getline(fd_corr, correct, '\n');
		if (output == correct)
			diff += output + "\n";
		else
		{
			diff += "|KO|" + output + "|\n";
			diff += "|OK|" + correct + "|\n";
		}
	}
	diff += "\n";
	return (diff);
}

static int	get_file_input(std::string filename, std::string &input)
{
	std::fstream	fd(filename.c_str());
	std::string		temp;

	if (fd.fail())
	{
		std::cout << getcwd(NULL, 0) << "\0";
		std::cerr << "fail to open " << filename << std::endl;
		return (1);
	}
	std::getline(fd, temp, '!');
	input += temp + "!";
	std::getline(fd, temp, '\n');
	input += temp + "\n";
	return (0);
}

static int	get_file_output(std::string filename, std::string &output)
{
	std::fstream	fd(filename.c_str());
	std::string		temp;

	if (fd.fail())
	{
		std::cerr << "fail to open " << filename << std::endl;
		return (1);
	}
	std::getline(fd, temp, '!');
	std::getline(fd, temp, '\n');
	std::getline(fd, output, '\0');
	return (0);
}
