/**
 * @file inputCheck.cpp
 *
 * @brief This library is used to check the user input(s).
 *
 * @author: Castellani Davide (@DavideC03)
 */

// Dependencies
#include "inputCheck.hpp"

// Definitions
// #define DEBUG

/**
 * @brief Constructor
 *
 * @param data Json object with the input data
 */
InputCheck::InputCheck(json data)
{
	InputCheck::data = data;
}

/**
 * @brief Check the input passed by the user
 *
 * @return bool True if the input is valid, False otherwise
 */
bool InputCheck::check()
{
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

/**
 * @brief Sanitize the input passed by the user
 *
 * @return json Object with the sanitized input data
 */
json InputCheck::sanitize()
{
	assert(InputCheck::check());

	json sanitized;

	for (string &item : InputCheck::required)
		sanitized[item] = InputCheck::data[item].get<string>();

	return sanitized;
}

/**
 * @brief Sanitize the input passed by the user
 *
 * @param data Json object with the input data
 * @return json Object with the sanitized input data
 */
json InputCheck::sanitize(json data)
{
	return InputCheck(data).sanitize();
}

#undef DEBUG
