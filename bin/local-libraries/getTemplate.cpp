/**
 * gettemplateURL.cpp
 * 
 * Library for getting template URL.
 * 
 * @author Castellani Davide (@DavideC03)
 */

// Dependencies
#include "getTemplate.hpp"

// Definitions
// #define DEBUG

GetTemplate::GetTemplate(json repoInfo) {
	/**
	 * Constructor
	 * 
	 * @param repoInfo: json object containing the repository information
	 */
	// Get the data
	GetTemplate::username = repoInfo["username"].get<string>();
	GetTemplate::token = repoInfo["token"].get<string>();
	GetTemplate::data = RepoInfoCheck::sanitize(repoInfo["answers"]);
}

string GetTemplate::get() {
	/**
	 * Get the template
	 * 
	 * @return: the template URL
	 */
	string templateURL(GetTemplate::data["template"].get<string>());

	templateURL = GetTemplate::ending(templateURL, "-template");
	replace(templateURL.begin(), templateURL.end(), ' ', '-');

#ifdef DEBUG
	cout << "Given template request: " << templateURL << endl;
#endif // DEBUG
	
	try {
		if(templateURL.find("/") != string::npos) {
			// External template
			json response = Rest::jsonRequest(
				Rest::GITHUB_REST_API + "repos/" + templateURL,
				GetTemplate::token,
				NULL,
				false
			);
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
		else {
			// Internal template
			json response = Rest::jsonRequest(
				Rest::GITHUB_REST_API + "repos/createstructure/" + templateURL,
				GetTemplate::token,
				NULL,
				false
			);
			assert(!response["private"].get<bool>());
			return "https://" + 
				GetTemplate::username + 
				":" +
				GetTemplate::token +
				"@github.com/createstructure/" +
				templateURL;
		}
	} catch (...) {
		// If the template doesn't exist or there is an error
		return "https://github.com/createstructure/default-template";
	}
}

bool GetTemplate::hasEnding (string original, string ending) {
    if (original.length() >= ending.length()) {
        return (0 == original.compare (original.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

string GetTemplate::ending(string original, string ending) {
	/**
	 * Add a string to the end of a string
	 * 
	 * @param original: the original string
	 * @param ending: the string to add
	 * 
	 * @return: the original string with the ending
	 */
	if (GetTemplate::hasEnding(original, ending))
		return original;
	else
		return original + ending;
}

string GetTemplate::get(json repoInfo) {
	/**
	 * Get the template
	 * 
	 * @return: the template URL
	 */
	return GetTemplate(repoInfo).get();
}

#undef DEBUG
