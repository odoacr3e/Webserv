/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alerusso <alessandro.russo.frc@gmail.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 19:44:42 by alerusso          #+#    #+#             */
/*   Updated: 2025/10/05 18:03:30 by alerusso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "BitcoinExchange.hpp"

static void		database_insert_entry(std::map<Date, double> &map, std::string entry);
static double	check_bitcoin(std::string entry, std::string sep, bool check_max);
static void		print_bitcoin_value(std::map<Date, double> &map, std::string entry);

//REVIEW - read_database
/*
	@des:	open database and fill the std::map passed as parameter.
	!EXCEPTIONS
	!FIRST LINE IGNORED
	!IF THROWS EXCEPTION, STOPS INCREMENTING MAP

	@param: map =>				the data structure of the data;
			database_name =>	the name of the database.
	@ret:	void

	1)	Open database. If fails, tries to open it inside database dir.
		If fails again, !EXCEPTION;
	2)	First line is skipped. If file is empty, !EXCEPTION;
	3)	Using Date class and casting value as double, the new
		pair is put in the map.
		If Date is invalid, or double is invalid/negative, !EXCEPTION;
	4)	All file is looped.
*/
void	read_database(std::map<Date, double> &map, std::string database_name)
{
	std::fstream	database(database_name.c_str(), std::ios::in);
	std::string		line;

	if (database.fail() == true)
	{
		database_name.insert(0, DATABASE_DIR"/");
		database.open(database_name.c_str(), std::ios::in);
		if (database.fail() == true)
			throw (Error(EX_OPEN_DATABASE));
	}
	std::getline(database, line);
	if (line.empty() == true)
		throw (Error(EX_EMPTY_DATABASE));
	while (std::getline(database, line))
		database_insert_entry(map, line);
}

//REVIEW - print_input
/*
	@des:	open input file and print bitcoin values, 
			using database to find the prices over time.
	!EXCEPTIONS
	!FIRST LINE IGNORED

	@param: map =>				the data structure of the data;
			input_name =>		the name of the input file.
	@ret:	void

	1)	If database is empty, just return without doing anything;
	2)	Open input file. If fails, tries to open it inside input file dir.
		If fails again, !EXCEPTION;
	3)	First line is skipped. If file is empty, !EXCEPTION;
	4)	Print bitcoin value, using "print_bitcoin_value".
	5)	All file is looped.
*/
void	print_input(std::map<Date, double> &map, std::string input_name)
{
	std::fstream	input(input_name.c_str(), std::ios::in);
	std::string		line;

	if (map.empty() == true)
		return ;
	if (input.fail() == true)
	{
		input_name.insert(0, INPUT_DIR"/");
		input.open(input_name.c_str(), std::ios::in);
		if (input.fail() == true)
			throw (Error(EX_OPEN_INPUT));
	}
	std::getline(input, line);
	if (line.empty() == true)
		throw (Error(EX_EMPTY_INPUT));
	while (std::getline(input, line))
	{
		try
		{
			print_bitcoin_value(map, line);
		}
		catch(const Error& e)
		{
			e.print();
		}
	}
}

/*
	!EXCEPTION
	
	Date constructor and "check_bitcoin" throws exceptions.
	'#' is considered as a comment.
	
	if no exceptions are thrown,
	map stores in position date the bitcoin price.
*/
static void	database_insert_entry(std::map<Date, double> &map, std::string entry)
{
	double	n;

	if (entry[0] == '#' || valid_input(entry) == false)
		return ;
	try
	{
		Date	date(entry, '-');
		n = check_bitcoin(entry, SEPARATOR_DATABASE, false);
		map[date] = n; 
	}
	catch(const Error& e)
	{
		e.print();
	}
}

/*
	!EXCEPTION

	Bitcoin is invalid when:

	1)	There are not enough fields in the file;
	2)	There are too many fields in the file;
	3)	entry is not a number;
	4)	bitcoin value is under BITCOIN_MIN;
	4)	bitcoin value is over BITCOIN_MAX.
*/
static double	check_bitcoin(std::string entry, std::string sep, bool check_max)
{
	size_t	pos;
	double	bitcoin_value;

	pos	= entry.find(sep);
	if (pos == entry.npos)
		throw (Error(EX_TOO_FEW_FIELDS, 0, entry));
	pos += sep.length();
	if (entry.substr(pos).find(sep) != entry.npos)
		throw (Error(EX_TOO_MANY_FIELDS, 0, entry));
	if (valid_number(entry.substr(pos), true) != 0)
		throw (Error(EX_BAD_VALUE, 0, entry));
	bitcoin_value = std::atof(entry.substr(pos).c_str());
	if (bitcoin_value < BITCOIN_MIN)
		throw (Error(EX_RANGE_LOW, 0, entry));
	else if (check_max == true && bitcoin_value > BITCOIN_MAX)
		throw (Error(EX_RANGE_HIGH, 0, bitcoin_value));
	return (bitcoin_value);
}

/*
	@des:	print bitcoin value, using the database.
	!EXCEPTIONS
	!IF MAP IS EMPTY, CAN SEG_FAULT
	!IF MAP IS NOT SORT, CAN SEG_FAULT

	@param: map =>		the data structure of the data;
			entry =>	the entry to print.
	@ret:	void

	1)	date and bitcoin value are created;
	2)	if date is lower than map first date, !EXCEPTION;
	3)	date is printed;
	4)	if date is bigger than map last date, last date price is used;
	5)	Else, loop one day back until you find a valid date.
*/
static void	print_bitcoin_value(std::map<Date, double> &map, std::string entry)
{
	double	n;

	if (entry[0] == '#' || valid_input(entry) == false)
		return ;
	Date	date(entry, '-');
	n = check_bitcoin(entry, SEPARATOR_INPUT, true);
	if (date < map.begin()->first)
		throw (Error(EX_TOO_OLD));
	date.basic_print(std::cout);
	std::cout << " => " << n << " = ";
	if (date > (map.rbegin()->first))
		std::cout << map.rbegin()->second * n;
	else
	{
		while (map.find(date) == map.end())
			date--;
		std::cout << map.find(date)->second * n;
	}
	std::cout << std::endl;
}
