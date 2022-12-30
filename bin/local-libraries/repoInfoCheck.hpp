/**
 * repoInfoCheck.hpp
 *
 * This library is used to check the user input(s).
 *
 * @author: Castellani Davide (@DavideC03)
 */

#ifndef REPO_INFO_CHECK
// If not imported yet
#define REPO_INFO_CHECK

// Dependencies
#include <iostream>
#include <string>
#include "../global-libraries/bin/json.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Definitions
// #define DEBUG

// Classes prototype(s)
class RepoInfoCheck
{
	/**
	 * Check the input passed by the user
	 */
private:
	vector<string> required = {"name"};
	json data;
	json sanitized;

public:
	RepoInfoCheck(json data);

	bool check();
	json sanitize();
	static json sanitize(json data);
};

#endif
