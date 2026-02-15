/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lparolis <lparolis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 17:10:42 by lparolis          #+#    #+#             */
/*   Updated: 2026/01/14 15:54:13 by lparolis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int line = 0;
bool	dateChecker(long int Year, long int Month, long int Day);
bool	isLeapYear(long int year);
bool	checkValueChars(std::string const &value);
bool	checkValueLimits(std::string const &value);
bool	checkInputValueLimits(std::string const &value);
float	convertValue(std::string value);

BitcoinExchange::BitcoinExchange()
{
	DBG_MSG("BitcoinExchange constructor called");
	
	std::ifstream	table("data.csv");
	std::string		buffer;

	if (table.fail())
		throw ParsingException("Failed to open the .csv file!");
	line = 0;
	while (std::getline(table, buffer) && line < 10)
	{
		lineParsing(buffer);
		++line;
	}
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &obj)
{
	DBG_MSG("BitcoinExchange copy constructor called");
	(void)obj;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &obj)
{
	DBG_MSG("BitcoinExchange copy operator called");
	(void)obj;
	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
	DBG_MSG("BitcoinExchange destructor called");
}

void	BitcoinExchange::lineParsing(std::string buffer)
{
	if (buffer.compare("date,exchange_rate") == 0)
		return;
	if(buffer.empty() || buffer.find(',') == std::string::npos)
	{
		std::cout << "Skipped line [" << line << "] Cause of bad data format\n";
		return;
	}
	// Qui mi separo la data e il valore di btc per ogni riga
	size_t		commaPos = buffer.find(',');
	std::string	date = buffer.substr(0, commaPos);
	std::string	value = buffer.substr(commaPos + 1, buffer.length() - (commaPos + 1));
	// std::cout << date << " | " << value << std::endl;
	
	if (date.find('-') == std::string::npos || date.find_first_of('-') == date.find_last_of('-'))
	{
		std::cout << "Skipped line [" << line << "] Cause of bad date format\n";
		return;
	}
	// Qui invece mi vado a separare la data di ogni riga nei valori singoli di giorno mese anno
	// li ho messi come long int per controllare anche se mi dai come input valori insensati
	size_t		firstDateSeparator = date.find('-');
	size_t		lastDateSeparator = date.find_last_of('-');
	std::string	sYear = date.substr(0, firstDateSeparator);
	std::string	sMonth = date.substr(firstDateSeparator + 1, lastDateSeparator - (firstDateSeparator + 1));
	std::string	sDay = date.substr(lastDateSeparator + 1, date.size());
	// std::cout << sYear << "|" << sMonth << "|" << sDay << std::endl;
	
	long int	iYear = std::atol(sYear.c_str());
	long int	iMonth = std::atol(sMonth.c_str());
	long int	iDay = std::atol(sDay.c_str());
	// Questa funzione fa un ulteriore check che la data inserita sia valida
	if (dateChecker(iYear, iMonth, iDay) == false)
		return;
	
	// Qui faccio i controlli sul value
	// std::cout << value << std::endl;
	if (checkValueChars(value) == false || checkValueLimits(value) == false)
		return;

	this->exchangeTable[date] = convertValue(value);
	if (DEBUG)
		std::cout << "Added line " << line << " in content map" << std::endl;
	
}

bool	dateChecker(long int Year, long int Month, long int Day)
{
	if (Year > 2022 || Year < 2009 || Month > 12 || Month < 1 || Day > 31 || Day < 1)
	{
		std::cout << "Skipped line [" << line << "] Cause of bad date values\n";
		return false;
	}
	if ((Month == 4 || Month == 6 || Month == 9 || Month == 11) && Day == 31)
	{
		std::cout << "Skipped line [" << line << "] Cause of bad day values ... (31 not compatible with current month!)\n";
		return false;
	}
	if ((isLeapYear(Year) == true && Month == 2 && Day > 29) || (isLeapYear(Year) == false && Month == 2 && Day > 28))
	{
		std::cout << "Skipped line [" << line << "] Cause of bad Day/Month values ... (Leap years dude!)\n";
		return false;
	}
	return true;
}

bool	isLeapYear(long int Year)
{
    if (Year % 400 == 0)
        return true;
    if (Year % 100 == 0)
        return false;
    if (Year % 4 == 0)
        return true;
    return false;
}

bool	checkValueChars(std::string const &value)
{	
	if (value.empty())
	{
		std::cout << "Passed value at line " << line << " is empty!" << std::endl;
		return false;
	}
	
	bool dot_seen = false;
		
	for (size_t i = 0; i < value.size(); i++)
	{
		if (value[i] == '.')
		{
			if (dot_seen == true)
			{
				std::cout << "Too many dots at line " << line << std::endl;
				return false;
			}
			if (i == 0 || i == value.size() - 1)
			{
				std::cout << "Misplaced dots at line " << line << std::endl;
				return false;
			}
			dot_seen = true;
		}
		else if (std::isdigit(value[i]) == 0)
		{
			std::cout << "One of line " << line << " value chars is not a number!" << std::endl;
			return false;
		}
	}
	return true;
}

bool	checkValueLimits(std::string const &value)
{
	char *end = 0;
	long int convertedValue = std::strtol(value.c_str(), &end, 10);
	
	if (end == value.c_str() || (end && *end != '\0'))
	{
		std::cout << "Error during value conversion at line " << line << std::endl;
		return false;
	}
	if (convertedValue > 47115.93 || convertedValue < 0)
	{
		std::cout << "Value of main database out of the record at line " << line << std::endl;
		return false;
	}
	return true;
}

bool	checkInputValueLimits(std::string const &value)
{
	char *end = 0;
	long int convertedValue = std::strtol(value.c_str(), &end, 10);
	
	if (end == value.c_str() || (end && *end != '\0'))
	{
		std::cout << "Error during input value conversion at line " << line << std::endl;
		return false;
	}
	if (convertedValue > 2147483648 || convertedValue < 0)
	{
		std::cout << "Value of input database out of acceptable range at line " << line << std::endl;
		return false;
	}
	return true;
}

float	convertValue(std::string value)
{
	char *end = 0;
	float convertedValue = std::strtof(value.c_str(), &end);
	
	if (end == value.c_str() || (end && *end != '\0'))
		throw BitcoinExchange::ParsingException("Error during conversion of value conversion!");
	return convertedValue;
}

void	BitcoinExchange:: inputParsing(std::string buffer)
{
	if (buffer.compare("date,exchange_rate") == 0)
		return;
	if(buffer.empty() || buffer.find('|') == std::string::npos)
	{
		std::cout << "Skipped line [" << line << "] Cause of bad data format\n";
		return;
	}
	// Qui mi separo la data e il nunmero di btc per ogni riga
	size_t		pipePos = buffer.find('|');
	std::string	date = buffer.substr(0, pipePos - 1);
	std::string	value = buffer.substr(pipePos + 2, buffer.length() - (pipePos + 2));
	// std::cout << date << " | " << value << std::endl;
	
	if (date.find('-') == std::string::npos || date.find_first_of('-') == date.find_last_of('-'))
	{
		std::cout << "Skipped line [" << line << "] Cause of bad date format\n";
		return;
	}
	// Qui invece mi vado a separare la data di ogni riga nei valori singoli di giorno mese anno
	// li ho messi come long int per controllare anche se mi dai come input valori insensati
	size_t		firstDateSeparator = date.find('-');
	size_t		lastDateSeparator = date.find_last_of('-');
	std::string	sYear = date.substr(0, firstDateSeparator);
	std::string	sMonth = date.substr(firstDateSeparator + 1, lastDateSeparator - (firstDateSeparator + 1));
	std::string	sDay = date.substr(lastDateSeparator + 1, date.size());
	// std::cout << sYear << "|" << sMonth << "|" << sDay << std::endl;
	
	long int	iYear = std::atol(sYear.c_str());
	long int	iMonth = std::atol(sMonth.c_str());
	long int	iDay = std::atol(sDay.c_str());
	// Questa funzione fa un ulteriore check che la data inserita sia valida
	if (dateChecker(iYear, iMonth, iDay) == false)
		return;
	
	// Qui faccio i controlli sul value
	// std::cout << value << std::endl;
	if (checkValueChars(value) == false || checkInputValueLimits(value) == false)
		return;

	this->input[date] = convertValue(value);
	if (DEBUG)
		std::cout << "Added line " << line << " in input map" << std::endl;
	
}

void	BitcoinExchange::inputProcess(std::string inputPath)
{
	std::ifstream	input(inputPath.c_str());
	std::string		buffer;

	if (input.fail())
		throw ParsingException("Failed to open the .txt input file!");
	line = 0;
	while (std::getline(input, buffer))
	{
		inputParsing(buffer);
		++line;
	}
	std::map<std::string, float>::const_iterator it = this->input.begin();
	while (it != this->input.end())
	{
		std::cout << it->first << " | " << it->second << std::endl;
		++it;
	}
	
}
