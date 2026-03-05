
#include "../../../includes/ether.hpp"
#include "../../../includes/status_codes.hpp"

/** 
 * @brief trim a string from a specific position (included)
 * 
 * @param s string to trim
 * @param from start position for trim, included in trim
 */
void	trim_from(std::string &s, size_t from)
{
	int	i;

	if (s.length() < from)
		return ;
	else if (from == 0)
		return (s.clear());
	i = s.length() - from;
	s.erase(from, i);
}

/** 
 * @brief find first occurence different from \p `c` and trim until \p `c` position, starting from 'left'
 * @param s string to trim
 * @param c char to trim from s
 */
bool	trim_equal_left(std::string &s, char c)
{
	size_t	i;

	if (!c)
		return (1);
	i = s.find_first_not_of(c);
	if (i == std::string::npos)
		return (1);
	s.erase(0, i);
	return (0);
}

/** 
 * @brief find first occurence of \p `c` and trim until \p `c` position, starting from 'left'
 * @param s string to trim
 * @param c char to trim from s
 */
bool	trim_diff_left(std::string &s, char c)
{
	size_t	i;

	if (!c)
		return (1);
	i = s.find_first_of(c);
	if (i == std::string::npos)
		return (1);
	s.erase(0, i);
	return (0);
}

/** 
 * @brief find first occurence different from \p `c` and trim until \p `c` position, starting from 'right'
 * @param s string to trim
 * @param c char to trim from s
 */
bool	trim_equal_right(std::string &s, char c)
{
	size_t	start;
	size_t	end;

	if (!c)
		return (1);
	start = s.find_first_of(c);
	if (start == std::string::npos)
		return (1);
	start = s.find_first_not_of(c, start);
	if (start == std::string::npos)
		return (1);
	end = s.substr(start).length();
	end = start + end;
	s.erase(start, end);
	return (0);
}

/** 
 * @brief find first occurence of \p `c` and trim until \p `c` position, starting from 'right'
 * @param s string to trim
 * @param c char to trim from s
 */
bool	trim_diff_right(std::string &s, char c)
{
	size_t	start;
	size_t	end;

	if (!c)
		return (1);
	start = s.find_first_not_of(c);
	if (start == std::string::npos)
		return (1);
	start = s.find_first_of(c, start);
	if (start == std::string::npos)
		return (trim_from(s, s.find_first_not_of(c)), 1);
	end = s.substr(start).length();
	end = start + end;
	s.erase(start, end);
	return (0);
}

/** 
 * @brief trim \p `left`, create a substr and, from that, it trims \p `right` 
 * 
 * @param str original string
 * @param left left substr
 * @param right right substr (the next after left)
 * @return 0 on success, 1 on error
 * 
 * Example:
 * ```
 * ft_strtrim("LeftContenutoRight", "Left", "Right") ---> "Contenuto"
 * ```
 */
bool	ft_strtrim(std::string &str, std::string left, std::string right)
{
	size_t	i;
	size_t	j;

	i = str.find(left);
	if (i == str.npos)
		return (1);
	j = str.find(right, i + left.length());
	if (j == str.npos)
		str = str.substr(i + left.length());
	else
		str = str.substr(i + left.length(), j);
	return (0);
}

/**
 * @brief splits a \p string into a \p `vector<std::string>` splitting by a \p char
 * 
 * @note \p `vector<std::string>` is not created in this function, user has to create it out of function scope
 * and pass it by reference
 * 
 * @param vect vector<std::string> passed by reference
 * @param s string to split
 * @param c char used for splitting \p `s`
 * 
 */
void	vect_split(std::vector<std::string> &vect, std::string s, char c)
{
	std::string	temp;
	size_t		i;

	while (s.empty() == false && s[0] != c)
	{
		i = s.find(c);
		if (i == std::string::npos)
		{
			vect.push_back(s);
			return ;
		}
		temp = s.substr(0, i);
		s.erase(0, temp.length());
		s.erase(0, s[0] == c);
		vect.push_back(temp);
	}
}

/**
 * @brief splits a \p string into a \p `vector<std::string>` splitting by a \p char
 * 
 * @note function allocs memory: every element MUST BE DELETED by user
 * 
 * @param vect vector of char * 
 * @param s string to split
 * @param c char used for splitting \p `s`
 */
void	vect_split_new(std::vector<char *> &vect, std::string s, char c)
{
	std::string	temp;
	size_t		i;
	char		*temp_char;

	while (s.empty() == false && s[0] != c)
	{
		i = s.find(c);
		if (i == std::string::npos)
		{
			temp_char = new char[s.length() + 1];
			temp_char[s.length()] = 0;
			std::memcpy(temp_char, s.c_str(), s.length());
			vect.push_back(temp_char);
			return ;
		}
		temp = s.substr(0, i);
		s.erase(0, temp.length());
		s.erase(0, s[0] == c);
		temp_char = new char[temp.length() + 1];
		temp_char[temp.length()] = 0;
		std::memcpy(temp_char, temp.c_str(), temp.length());
		vect.push_back(temp_char);
	}
}

/** 
 * @brief free function for \p ```vect_split_new```
 * 
 * @param vect vector of char * allocated before
 * @param size size of vector
 */
void	vect_split_free(std::vector<char *> &vect, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		delete [] vect[i];
	}
}

/**
 * @brief conversion from hexadecimal to char for string conversion in URL browser
 * 
 * @param input input string to modify
 * @return string modified with hexadecimal chars converted to char
 */
std::string		hex_to_char(std::string& input)
{
    std::string result;

    result.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i) 
	{
        if (input[i] == '%' &&
            i + 2 < input.size() &&
            std::isxdigit(static_cast<unsigned char>(input[i + 1])) &&
            std::isxdigit(static_cast<unsigned char>(input[i + 2]))) 
		{

            char hex[3];
            hex[0] = input[i + 1];
            hex[1] = input[i + 2];
            hex[2] = '\0';

            char* endptr = 0;
            long value = std::strtol(hex, &endptr, 16);

            result += static_cast<char>(value);
            i += 2; 
        } 
		else
        	result += input[i];
    }
	return (result);
}

/**
 * @brief conversion from hexadecimal to char for string conversion in URL browser
 * 
 * @param input input string to modify
 * @return string modified with hexadecimal chars converted to char
 */
std::string		hex_to_char(std::string& input)
{
    std::string result;

    result.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i) 
	{
        if (input[i] == '%' &&
            i + 2 < input.size() &&
            std::isxdigit(static_cast<unsigned char>(input[i + 1])) &&
            std::isxdigit(static_cast<unsigned char>(input[i + 2]))) 
		{

            char hex[3];
            hex[0] = input[i + 1];
            hex[1] = input[i + 2];
            hex[2] = '\0';

            char* endptr = 0;
            long value = std::strtol(hex, &endptr, 16);

            result += static_cast<char>(value);
            i += 2; 
        } 
		else
        	result += input[i];
    }
	return (result);
}

std::string		hex_to_char(std::string& input)
{
    std::string result;

    result.reserve(input.size());
    for (std::size_t i = 0; i < input.size(); ++i) 
	{
        if (input[i] == '%' &&
            i + 2 < input.size() &&
            std::isxdigit(static_cast<unsigned char>(input[i + 1])) &&
            std::isxdigit(static_cast<unsigned char>(input[i + 2]))) 
		{

            char hex[3];
            hex[0] = input[i + 1];
            hex[1] = input[i + 2];
            hex[2] = '\0';

            char* endptr = 0;
            long value = std::strtol(hex, &endptr, 16);

            result += static_cast<char>(value);
            i += 2; 
        } 
		else
        	result += input[i];
    }
	return (result);
}