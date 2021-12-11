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
#include <bits/stdc++.h>
#include "getTemplate.hpp"
#include "getUploadURL.hpp"
#include "../global-libraries/bin/json.hpp"
#include "../global-libraries/bin/rest.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Definitions
#define DEBUG

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

Repo::Repo(json data)
{
	/**
	 * Constructor
	 *
	 * @param data: json data containing the repository information
	 */
	Repo::data = data;
#ifdef DEBUG
	cout << Repo::data.dump(4) << endl;
#endif // DEBUG
	Repo::path =
#ifdef DEBUG
		"./" +
#else  // DEBUG
		"/media/createstructure/" +
#endif // DEBUG
		Repo::data["username"].get<string>() +
		"+" +
		Repo::data["answers"]["name"].get<string>();

	time_t rawtime;
	time(&rawtime);
	Repo::date = localtime(&rawtime);
}

void Repo::all()
{
	/**
	 * Method for downloading, creating, elaborating and uploading the repository.
	 */
#ifdef DEBUG
	cout << "Repo::all()" << endl;
#endif // DEBUG
	Repo::download();
#ifdef DEBUG
	cout << "Downloaded" << endl;
#endif // DEBUG
	Repo::create();
#ifdef DEBUG
	cout << "Created" << endl;
#endif // DEBUG
	Repo::elaborate();
#ifdef DEBUG
	cout << "Elaborated" << endl;
#endif // DEBUG
	Repo::upload();
#ifdef DEBUG
	cout << "Uploaded" << endl;
#endif // DEBUG
	Repo::remove();
#ifdef DEBUG
	cout << "Removed" << endl;
#endif // DEBUG
}

void Repo::download()
{
	/**
	 * Download the repository
	 */
	system(("git clone " + GetTemplate::get(Repo::data) + " " + Repo::path).c_str());
	Repo::changes = Repo::getChanges();
#ifdef DEBUG
	cout << "Changes: " << Repo::changes.size() << endl;
	for (auto &change : Repo::changes)
		cout << change.first << " -> " << change.second << endl;
#endif // DEBUG
}

void Repo::create()
{
	/**
	 * Create the repository
	 */
	json request = {
		{"name", (Repo::data["answers"]["prefix"].get<string>() == "" ? Repo::data["answers"]["name"].get<string>() : Repo::data["answers"]["prefix"].get<string>() + Repo::data["answers"]["name"].get<string>())},
		{"description", Repo::data["answers"]["descr"].get<string>()},
		{"private", Repo::data["answers"]["private"].get<bool>()}};

	if (
		Repo::data["answers"]["isOrg"].get<bool>() &&
		Repo::data["answers"]["team"].get<string>() != "")
	{
		cout << Rest::GITHUB_REST_API + "orgs/" + Repo::data["answers"]["org"].get<string>() + "/teams" << endl;
		json teams = Rest::jsonRequest(
			Rest::GITHUB_REST_API + "orgs/" + Repo::data["answers"]["org"].get<string>() + "/teams",
			Repo::data["token"].get<string>(),
			NULL,
			false);
#ifdef DEBUG
		cout << "teams: " << teams.dump(4) << endl;
#endif // DEBUG

		long teamID = -1;
		for (auto &team : teams)
			if (team["name"].get<string>() == Repo::data["answers"]["team"].get<string>())
				teamID = team["id"].get<long>();

#ifdef DEBUG
		cout << "teamID: " << teamID << endl;
#endif // DEBUG

		if (teamID == -1)
		{
			json request2 = {{"name", Repo::data["answers"]["team"].get<string>()},
							 {"description", "Team created automatically by createstructure"}};
#ifdef DEBUG
			cout << "request: " << request2.dump(4) << endl;
#endif // DEBUG
			json teamInfo = Rest::jsonRequest(
				Rest::GITHUB_REST_API + "orgs/" + Repo::data["answers"]["org"].get<string>() + "/teams",
				Repo::data["token"].get<string>(),
				request2,
				true);

#ifdef DEBUG
			cout << "teamInfo: " << teamInfo.dump(4) << endl;
#endif // DEBUG

			teamID = teamInfo["id"].get<long>();
		}

		request["team_id"] = teamID;
	}

#ifdef DEBUG
	cout << "request: " << request.dump(4) << endl;
#endif // DEBUG

	json response = Rest::jsonRequest(
		Rest::GITHUB_REST_API +
			(Repo::data["answers"]["isOrg"].get<bool>() ? "orgs/" + Repo::data["answers"]["org"].get<string>() : "user/") +
			"/repos",
		Repo::data["token"].get<string>(),
		request,
		true);

#ifdef DEBUG
	cout << "response: " << response.dump(4) << endl;
#endif // DEBUG
}

bool Repo::endsWith(string const &value, string const &ending)
{
	/**
	 * Check if a string ends with another string
	 *
	 * @param value: string to check
	 * @param ending: string to check against
	 * @return true if the string ends with the other string
	 */
	if (ending.size() > value.size())
		return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void Repo::elaborate()
{
	/**
	 * Elaborate the repository
	 */
	for (const auto &file : filesystem::recursive_directory_iterator(path))
	{
		if (filesystem::is_regular_file(filesystem::status(file.path())) && !Repo::endsWith(file.path().string(), "/.createstructure/change.json"))
		{
			// Get original
			ifstream t(file.path().string());
			string old((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());

#ifdef DEBUG
			cout << "file: " << file.path().string() << "\t" << old.size() << endl;
#endif // DEBUG

			// Put the changed content in the same folder
			ofstream f(file.path().string());
			f << Repo::replace(old, Repo::changes);

			// Check if the file name will become different from the original
			string new_path(Repo::replace(file.path(), Repo::changes));
			if (new_path.compare(file.path()) != 0)
			{
#ifdef DEBUG
				cout << "new_path: " << new_path << endl;
#endif // DEBUG
	   // Create new directory tree
				filesystem::create_directories(((filesystem::path)new_path).remove_filename());

				// Rename file
				filesystem::rename(file.path().c_str(), new_path.c_str());
			}
		}
	}
}

void Repo::upload()
{
	/**
	 * Upload the repository
	 */
	cout << GetUploadURL::get(Repo::data) << endl;
	system((
			   "cd " +
			   Repo::path +
			   "; rm -rf .git; git init; git add *; git commit -m \"Update\"; git branch -M main; git push --set-upstream " +
			   GetUploadURL::get(Repo::data) +
			   " main")
			   .c_str());
}

void Repo::remove()
{
	/**
	 * Remove the repository
	 */
	system(("rm -rf " + Repo::path).c_str());
}

void Repo::all(json data)
{
	/**
	 * Method for downloading, creating, elaborating and uploading the repository.
	 *
	 * @param data The data to be used for the repository.
	 */
	Repo(data).all();
}

void Repo::download(json data)
{
	/**
	 * Download the repository
	 *
	 * @param data: json data containing the repository information
	 */
	Repo(data).download();
}

void Repo::create(json data)
{
	/**
	 * Create the repository
	 *
	 * @param data: json data containing the repository information
	 */
	Repo(data).create();
}

void Repo::elaborate(json data)
{
	/**
	 * Elaborate the repository
	 *
	 * @param data: json data containing the repository information
	 */
	Repo(data).elaborate();
}

void Repo::upload(json data)
{
	/**
	 * Upload the repository
	 *
	 * @param data: json data containing the repository information
	 */
	Repo(data).upload();
}

void Repo::remove(json data)
{
	/**
	 * Remove the repository
	 *
	 * @param data: json data containing the repository information
	 */
	Repo(data).remove();
}

string Repo::getDate(string pattern)
{
	/**
	 * Get the date in the specified pattern
	 *
	 * @param pattern: date pattern
	 * @return string containing the date
	 */
	char buffer[255];

	strftime(buffer, sizeof(buffer), pattern.c_str(), Repo::date);

	return string(buffer);
}

vector<pair<string, string>> Repo::getChanges()
{
	/**
	 * Get the changes to do in the repository
	 *
	 * @return vector containing the changes
	 */
	vector<pair<string, string>> changes;
	vector<pair<string, string>> specialChanges(Repo::getSpecialChanges());
	string changesFile = Repo::path + "/.createstructure/change.json";

#ifdef DEBUG
	cout << "specialChanges: " << specialChanges.size() << endl;
	for (auto &[key, value] : specialChanges)
		cout << "- " << key << ": " << value << endl;
	cout << endl;
#endif // DEBUG

	ifstream t(changesFile);
	string strChanges((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	json jsonChanges = json::parse(strChanges);

#ifdef DEBUG
	cout << "jsonChanges: " << jsonChanges.dump(4) << endl;
#endif // DEBUG

	for (auto &[key, value] : jsonChanges.items())
	{
		changes.push_back(make_pair(key, Repo::replace(value, specialChanges)));
	}

#ifdef DEBUG
	cout << "changes: " << changes.size() << endl;
	for (auto &[key, value] : changes)
		cout << "- " << key << ": " << value << endl;
	cout << endl;
#endif // DEBUG

	return changes;
}

vector<pair<string, string>> Repo::getSpecialChanges()
{
	/**
	 * Get the special changes to do in the repository
	 *
	 * @return vector containing the special changes
	 */
	return {
		// Input special changes
		{"solusernamesol", Repo::data["username"].get<string>()},
		{"solnamesol", Repo::data["answers"]["name"].get<string>()},
		{"soldescrsol", Repo::data["answers"]["descr"].get<string>()},
		{"solprefixsol", Repo::data["answers"]["prefix"].get<string>()},
		{"solisorgsol", (Repo::data["answers"]["isOrg"].get<bool>() ? "true" : "false")},
		{"solownersol", (Repo::data["answers"]["isOrg"].get<bool>() ? Repo::data["answers"]["org"].get<string>() : Repo::data["username"].get<string>())},
		{"solteamsol", Repo::data["answers"]["team"].get<string>()},
		{"soltemplatesol", Repo::data["answers"]["template"].get<string>()},
		{"solprivatesol", (Repo::data["answers"]["private"].get<bool>() ? "true" : "false")},

		// Data special changes
		{"soltime-nowsol", Repo::getDate("%Y-%m-%d")},
		{"soltime_nowsol", Repo::getDate("%Y_%m_%d")},
		{"soltimenowsol", Repo::getDate("%Y%m%d")},
		{"soltimeallsol", Repo::getDate("%c")},
		{"soltimeHMSsol", Repo::getDate("%H%M%s")},
		{"soltimeH-M-Ssol", Repo::getDate("%H-%M-%S")},
		{"soltimeH_M_Ssol", Repo::getDate("%H_%M_%S")},
		{"soltimeyearsol", Repo::getDate("%Y")},
		{"soltimeyearshortsol", Repo::getDate("%y")},
		{"soltimemonthsol", Repo::getDate("%B")},
		{"soltimemonthshortsol", Repo::getDate("%b")},
		{"soltimemonthnumsol", Repo::getDate("%m")},
		{"soltimeweekofyearsol", Repo::getDate("%U")},
		{"soltimedaysol", Repo::getDate("%d")},
		{"soltimedayofyearsol", Repo::getDate("%j")},
		{"soltimedayofweeksol", Repo::getDate("%A")},
		{"soltimedayofweekshortsol", Repo::getDate("%a")},
		{"soltimehoursol", Repo::getDate("%H")},
		{"soltimehour12sol", Repo::getDate("%I")},
		{"soltimeminutesol", Repo::getDate("%M")},
		{"soltimesecondsol", Repo::getDate("%S")},
		{"soltimeoffsetsol", Repo::getDate("%z")}};
}

string Repo::replace(string original, vector<pair<string, string>> changes)
{
	/**
	 * Replace the special changes in the string
	 *
	 * @param str: string to replace
	 * @param changes: vector containing the changes
	 * @return string containing the replaced string
	 */
	size_t pos = 0;

	while (true)
	{
		// Local variable(s)
		pair<string, string> change;
		int bestPos = INT_MAX;

		// For every possible change
		for (size_t i = 0; i < changes.size(); ++i)
		{
			if (original.find(changes[i].first, pos) != string::npos)
			{
				if(original.find(changes[i].first, pos) < bestPos)
				{
					bestPos = original.find(changes[i].first, pos);
					change = changes[i];
				}
			}
		}

		// Check if there is a change to do
		if (bestPos == INT_MAX)
		{
			// Give back the changed string
			return original;
		}
		else
		{
			original.replace(bestPos, change.first.size(), change.second);
#ifdef DEBUG
			cout << "Change: " << change.first << " -> " << change.second << endl;
#endif // DEBUG
			pos = bestPos + change.second.size();
		}
	}

	// Give back the changed string
	return original;
}

#undef DEBUG
#endif
