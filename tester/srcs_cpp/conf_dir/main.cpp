#include "../tester.hpp"

int	main(int ac, char **av)
{
	std::string		output;
	int				exit_code = 0;

	if (ac == 1)
		return (1);
	chdir(PROG_PATH);
	run_cmd((char*const[]){(char *)PROG_NAME, av[1], NULL}, output, true);
	if (ac == 3 && av[2] == (std::string)"update")
		exit_code = update_result(av[1], output);
	else
		exit_code = compare_result(av[1], output);
	return (exit_code);
}
