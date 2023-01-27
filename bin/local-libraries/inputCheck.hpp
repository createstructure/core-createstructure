/**
 * @file inputCheck.hpp
 *
 * @brief This library is used to check the user input(s).
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

// Class prototype
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

#endif
