/**
 * @file repoInfoCheck.cpp
 *
 * @brief This library is used to check the user input(s).
 *
 * @author: Castellani Davide (@DavideC03)
 */

// Dependencies
#include "repoInfoCheck.hpp"

// Definitions
// #define DEBUG

/**
 * @brief Constructor
 *
 * @param data Json object with the repository data
 */
RepoInfoCheck::RepoInfoCheck(json data)
{
	RepoInfoCheck::data = data;
}

/**
 * @brief Check the repository data
 *
 * @return bool True if the repository data is valid, False otherwise
 */
bool RepoInfoCheck::check()
{
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

/**
 * @brief Sanitize the repo info
 *
 * @return json Object with the sanitized repo info
 */
json RepoInfoCheck::sanitize()
{
	assert(RepoInfoCheck::check());

	sanitized.clear();

	// Insert data into sanitize
	for (string &item : RepoInfoCheck::required)
		sanitized[item] = RepoInfoCheck::data[item].get<string>();

	// Insert optional arguments
	map<string, string> optional1 = {
		{"descr", "Repository made with the use of createstructure"},
		{"prefix", ""},
		{"template", "default"}};

	for (auto const &[key, value] : optional1)
	{
		if (RepoInfoCheck::data[key].is_string())
			sanitized[key] = RepoInfoCheck::data[key].get<string>();
		else
			sanitized[key] = value;
	}

	// Insert optional arguments for organization
	map<string, bool> optional2 = {
		{"isOrg", false},
		{"private", false}};

	for (auto const &[key, value] : optional2)
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
				  [](unsigned char c)
				  {
					  return std::tolower(c);
				  });
		sanitized["team"] = team;
	}

	// Create repo_path, without spaces
	string repo_path = sanitized["name"];
	for (size_t pos = 0; (pos = repo_path.find(" ", pos) + 1); pos++)
		repo_path.replace(--pos, 1, "-");

	sanitized["repo_path"] = repo_path;

#ifdef DEBUG
	cout << "RepoInfoCheck::sanitize()" << endl
		 << sanitized.dump(4) << endl;
#endif // DEBUG

	return sanitized;
}

/**
 * @brief Sanitize the repo info
 *
 * @param data Json object with the repo info
 * @return json Object with the sanitized repo info
 */
json RepoInfoCheck::sanitize(json data)
{
	return RepoInfoCheck(data).sanitize();
}

#undef DEBUG
