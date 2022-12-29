/**
 * repo.hpp
 *
 * Library for managing repositories.
 *
 * @author Castellani Davide (@DavideC03)
 */

#ifndef REPO
// If not imported yet
#define REPO

// Dependencies
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <filesystem>
#include "getTemplate.hpp"
#include "getUploadURL.hpp"
#include "../global-libraries/bin/json.hpp"
#include "../global-libraries/bin/rest.hpp"

// using ...
using namespace std;
using json = nlohmann::json;
using chrono::duration;
using chrono::duration_cast;
using chrono::high_resolution_clock;
using chrono::milliseconds;

// Class prototype(s)
class Repo
{
private:
	json data;
	string path;
	tm *date;
	vector<pair<string, string>> changes;

	string getDate(string pattern);
	vector<pair<string, string>> getChanges();
	vector<pair<string, string>> getSpecialChanges();
	string replace(string original, vector<pair<string, string>> changes);
	bool endsWith(string const &value, string const &ending);

public:
	Repo(json data);
	void all();
	void download();
	void create();
	void elaborate();
	void upload();
	void remove();

	static void all(json data);
	static void download(json data);
	static void create(json data);
	static void elaborate(json data);
	static void upload(json data);
	static void remove(json data);
};

#endif
