/**
 * inputCheck.hpp
 *
 * This library is used to check the user input(s).
 *
 * @author: Castellani Davide (@DavideC03)
 */

#ifndef INPUTCHECK
// If not imported yet
#define INPUTCHECK

// Dependencies
#include <bits/stdc++.h>
#include "../global-libraries/bin/json.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Definitions
// #define DEBUG

// Classes prototype(s)
class InputCheck
{
	/**
	 * Check the input passed by the user
	 */
private:
	vector<string> required = {"server_name", "server_password", "server_gpg_key"};
	json data;

public:
	InputCheck(json data);

	bool check();
	json sanitize();
	static json sanitize(json data);
};

// Function(s)
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
#endif
