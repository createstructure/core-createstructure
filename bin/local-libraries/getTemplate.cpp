/**
 * @file gettemplateURL.cpp
 *
 * @brief Library for getting template URL.
 *
 * @author Castellani Davide (@DavideC03)
 */

// Dependencies
#include "getTemplate.hpp"

// Definitions
// #define DEBUG

/**
 * @brief Constructor
 *
 * @param repoInfo Json object containing the repository information
 */
GetTemplate::GetTemplate(json repoInfo)
{
	// Get the data
	GetTemplate::username = repoInfo["username"].get<string>();
	GetTemplate::token = repoInfo["token"].get<string>();
	GetTemplate::data = RepoInfoCheck::sanitize(repoInfo["answers"]);
}

/**
 * @brief Get the template
 *
 * @return string The template URL
 */
string GetTemplate::get()
{
	string templateURL(GetTemplate::data["template"].get<string>());

	templateURL = GetTemplate::ending(templateURL, "-template");
	replace(templateURL.begin(), templateURL.end(), ' ', '-');

#ifdef DEBUG
	cout << "Given template request: " << templateURL << endl;
#endif // DEBUG

	try
	{
		if (templateURL.find("/") != string::npos)
		{
			// External template
			json response = Rest::jsonRequest(
				Rest::GITHUB_REST_API + "repos/" + templateURL,
				GetTemplate::token,
				NULL,
				false);
#ifdef DEBUG
			cout << "Response: " << response.dump(4) << endl;
#endif // DEBUG
			assert(response["private"].get<bool>());
			return "https://" +
				   GetTemplate::username +
				   ":" +
				   GetTemplate::token +
				   "@github.com/" +
				   templateURL;
		}
		else
		{
			// Internal template
			json response = Rest::jsonRequest(
				Rest::GITHUB_REST_API + "repos/createstructure/" + templateURL,
				GetTemplate::token,
				NULL,
				false);
			assert(!response["private"].get<bool>());
			return "https://" +
				   GetTemplate::username +
				   ":" +
				   GetTemplate::token +
				   "@github.com/createstructure/" +
				   templateURL;
		}
	}
	catch (...)
	{
		// If the template doesn't exist or there is an error
		return "https://github.com/createstructure/default-template";
	}
}

/**
 * @brief Get if "original" ends with "ending"
 *
 * @param original The string to evaluate
 * @param ending The "end" string
 * @return True if "original" ends with "ending", False otherwise
 */
bool GetTemplate::hasEnding(string original, string ending)
{
	if (original.length() >= ending.length())
	{
		return (0 == original.compare(original.length() - ending.length(), ending.length(), ending));
	}
	else
	{
		return false;
	}
}

/**
 * @brief Add a string to the end of a string
 *
 * @param original The original string
 * @param ending The string to add
 *
 * @return string The original string with the ending
 */
string GetTemplate::ending(string original, string ending)
{
	if (GetTemplate::hasEnding(original, ending))
		return original;
	else
		return original + ending;
}

/**
 * @brief Get the template
 *
 * @return The template URL
 */
string GetTemplate::get(json repoInfo)
{
	return GetTemplate(repoInfo).get();
}

#undef DEBUG
