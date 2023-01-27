/**
 * @file getUploadURL.cpp
 *
 * @brief Library for getting the upload URL for the repo.
 *
 * @author Castellani Davide (@DavideC03)
 */

// Dependencies
#include "getUploadURL.hpp"

// Definitions
// #define DEBUG

/**
 * @brief Constructor
 *
 * @param repoInfo Json object containing the repository information
 */
GetUploadURL::GetUploadURL(json repoInfo)
{
	GetUploadURL::username = repoInfo["username"].get<string>();
	GetUploadURL::token = repoInfo["token"].get<string>();
	GetUploadURL::data = RepoInfoCheck::sanitize(repoInfo["answers"]);
}

/**
 * @brief Get the template
 *
 * @return string The template URL
 */
string GetUploadURL::get()
{
	return "https://" +
		   GetUploadURL::username +
		   ":" +
		   GetUploadURL::token +
		   "@github.com/" +
		   (GetUploadURL::data["isOrg"].get<bool>() ? GetUploadURL::data["org"].get<string>() : GetUploadURL::username) +
		   "/" +
		   (GetUploadURL::data["prefix"].get<string>() == "" ? GetUploadURL::data["repo_path"].get<string>() : GetUploadURL::data["prefix"].get<string>() + "-" + GetUploadURL::data["repo_path"].get<string>());
}

/**
 * @brief Get, without constructor
 *
 * @param repoInfo Json object containing the repository information
 * @return string The template URL
 */
string GetUploadURL::get(json repoInfo)
{
	return GetUploadURL(repoInfo).get();
}

#undef DEBUG
