/**
 * inputCheck.cpp
 *
 * This library is used to check the user input(s).
 *
 * @author: Castellani Davide (@DavideC03)
 */

// Dependencies
#include "inputCheck.hpp"

// Definitions
// #define DEBUG

InputCheck::InputCheck(json data)
{
	/**
	 * Constructor
	 *
	 * @param data: json object with the input data
	 */
	InputCheck::data = data;
}

bool InputCheck::check()
{
	/**
	 * Check the input passed by the user
	 *
	 * @return: true if the input is valid, false otherwise
	 */
	try
	{
		for (string &item : InputCheck::required)
			InputCheck::data[item].get<string>();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

json InputCheck::sanitize()
{
	/**
	 * Sanitize the input passed by the user
	 *
	 * @return: json object with the sanitized input data
	 */
	assert(InputCheck::check());

	json sanitized;

	for (string &item : InputCheck::required)
		sanitized[item] = InputCheck::data[item].get<string>();

	return sanitized;
}

json InputCheck::sanitize(json data)
{
	/**
	 * Sanitize the input passed by the user
	 *
	 * @param data: json object with the input data
	 * @return: json object with the sanitized input data
	 */
	return InputCheck(data).sanitize();
}

#undef DEBUG
