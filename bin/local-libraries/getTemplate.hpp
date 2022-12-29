/**
 * gettemplateURL.hpp
 * 
 * Library for getting template URL.
 * 
 * @author Castellani Davide (@DavideC03)
 */

#ifndef GET_TEMPLATE
// If not imported yet
#define GET_TEMPLATE

// Dependencies
#include <iostream>
#include <string>
#include "../global-libraries/bin/json.hpp"
#include "../global-libraries/bin/rest.hpp"
#include "repoInfoCheck.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Classes prototype(s)
class GetTemplate {
	private:
		string username;
		string token;
		json data;

		bool hasEnding(string original, string ending);
		string ending(string original, string ending);
	public:
		GetTemplate(json repoInfo);
		string get();

		static string get(json repoInfo);
};

#endif
