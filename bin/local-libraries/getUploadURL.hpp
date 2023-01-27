/**
 * @file getUploadURL.hpp
 *
 * @brief Library for getting the upload URL for the repo.
 *
 * @author Castellani Davide (@DavideC03)
 */

#ifndef GET_UPLOAD_URL
// If not imported yet
#define GET_UPLOAD_URL

// Dependencies
#include <iostream>
#include <string>
#include "../global-libraries/bin/json.hpp"
#include "../global-libraries/bin/rest.hpp"
#include "repoInfoCheck.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Class prototype
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

#endif
