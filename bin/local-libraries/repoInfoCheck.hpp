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
#include <bits/stdc++.h>
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

public:
	RepoInfoCheck(json data);

	bool check();
	json sanitize();
	static json sanitize(json data);
};

// Function(s)
RepoInfoCheck::RepoInfoCheck(json data)
{
	/**
	 * Constructor
	 *
	 * @param data: json object with the repository data
	 */
	RepoInfoCheck::data = data;
}

bool RepoInfoCheck::check()
{
	/**
	 * Check the repository data
	 *
	 * @return: true if the repository data is valid, false otherwise
	 */
	try
	{
		for (string &item : RepoInfoCheck::required)
			assert(RepoInfoCheck::data[item].is_string());
	}
	catch (...)
	{
		return false;
	}

	return true;
}

json RepoInfoCheck::sanitize()
{
	/**
	 * Sanitize the repo info
	 *
	 * @return: json object with the sanitized repo info
	 */
	assert(RepoInfoCheck::check());

	json sanitized;

	for (string &item : RepoInfoCheck::required)
		sanitized[item] = RepoInfoCheck::data[item].get<string>();

	map<string, string> optional1 = {
		{"descr", "Repository made with the use of createstructure"},
		{"prefix", ""},
		{"template", "default"}};

	for (auto const& [key, value] : optional1)
	{
		if (RepoInfoCheck::data[key].is_string())
			sanitized[key] = RepoInfoCheck::data[key].get<string>();
		else
			sanitized[key] = value;
	}

	map<string, bool> optional2 = {
		{"isOrg", false},
		{"private", false}};

	for (auto const& [key, value] : optional2)
	{
		if (RepoInfoCheck::data[key].is_boolean())
			sanitized[key] = RepoInfoCheck::data[key].get<bool>();
		else
			sanitized[key] = value;
	}

	if (sanitized["isOrg"].get<bool>())
	{
		if (RepoInfoCheck::data["org"].is_string())
		{
			sanitized["org"] = RepoInfoCheck::data["org"].get<string>();

			if (RepoInfoCheck::data["team"].is_string())
				sanitized["team"] = RepoInfoCheck::data["team"].get<string>();
			else
				sanitized["team"] = "";
		}
		else
		{
			RepoInfoCheck::data["isOrg"] = false;
		}

		string team(sanitized["team"].get<string>());
		transform(team.begin(), 
			team.end(), 
			team.begin(), 
			[](unsigned char c){
				return std::tolower(c); 
			}
		);
		sanitized["team"] = team;
	}

#ifdef DEBUG
	cout << "RepoInfoCheck::sanitize()" << endl << sanitized.dump(4) << endl;
#endif // DEBUG

	return sanitized;
}

json RepoInfoCheck::sanitize(json data)
{
	/**
	 * Sanitize the repo info
	 *
	 * @param data: json object with the repo info
	 * @return: json object with the sanitized repo info
	 */
	return RepoInfoCheck(data).sanitize();
}

#undef DEBUG
#endif
