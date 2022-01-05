/**
 * getUploadURL.hpp
 *
 * Library for getting the upload URL for the repo.
 *
 * @author Castellani Davide (@DavideC03)
 */

#ifndef GET_UPLOAD_URL
// If not imported yet
#define GET_UPLOAD_URL

// Dependencies
#include <bits/stdc++.h>
#include "../global-libraries/bin/json.hpp"
#include "../global-libraries/bin/rest.hpp"
#include "repoInfoCheck.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Definitions
// #define DEBUG

// Classes prototype(s)
class GetUploadURL
{
private:
	string username;
	string token;
	json data;

public:
	GetUploadURL(json repoInfo);
	string get();

	static string get(json repoInfo);
};

GetUploadURL::GetUploadURL(json repoInfo)
{
	/**
	 * Constructor
	 *
	 * @param repoInfo: json object containing the repository information
	 */
	// Get the data
	GetUploadURL::username = repoInfo["username"].get<string>();
	GetUploadURL::token = repoInfo["token"].get<string>();
	GetUploadURL::data = RepoInfoCheck::sanitize(repoInfo["answers"]);
}

string GetUploadURL::get()
{
	/**
	 * Get the template
	 *
	 * @return: the template URL
	 */
	return "https://" +
		   GetUploadURL::username +
		   ":" +
		   GetUploadURL::token +
		   "@github.com/" +
		   (GetUploadURL::data["isOrg"].get<bool>() ? GetUploadURL::data["org"].get<string>() : GetUploadURL::username) +
		   "/" +
		   (GetUploadURL::data["prefix"].get<string>() == "" ? GetUploadURL::data["name"].get<string>() : GetUploadURL::data["prefix"].get<string>() + "-" + GetUploadURL::data["name"].get<string>());
}

string GetUploadURL::get(json repoInfo)
{

	return GetUploadURL(repoInfo).get();
}

#undef DEBUG
#endif
