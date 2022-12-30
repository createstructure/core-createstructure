/**
 * getUploadURL.cpp
 *
 * Library for getting the upload URL for the repo.
 *
 * @author Castellani Davide (@DavideC03)
 */

// Dependencies
#include "getUploadURL.hpp"

// Definitions
// #define DEBUG

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
		   (GetUploadURL::data["prefix"].get<string>() == "" ? GetUploadURL::data["repo_path"].get<string>() : GetUploadURL::data["prefix"].get<string>() + "-" + GetUploadURL::data["repo_path"].get<string>());
}

string GetUploadURL::get(json repoInfo)
{

	return GetUploadURL(repoInfo).get();
}

#undef DEBUG
