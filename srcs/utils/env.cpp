#include "../../includes/include.hpp"

std::string	env_value(const char **env, std::string key)
{
	std::string	temp;

	if (!env)
		return ("");
	for (int i = 0; env[i]; i++)
	{
		temp = env[i];
		trim_equal_right(temp, '=');
		if (temp == key)
		{
			temp = env[i];
			trim_diff_left(temp, '=');
			temp.erase(0, 1);
			return (temp);
		}
	}
	return ("");
}
