/**
 * workload.hpp
 *
 * Library to get the workload of a process.
 *
 * @author Castellani Davide (@DavideC03)
 */

#ifndef WORKLOAD
// If not imported yet
#define WORKLOAD

// Dependencies
#include <bits/stdc++.h>
#include "inputCheck.hpp"
#include "repoInfoCheck.hpp"
#include "../global-libraries/bin/json.hpp"
#include "../global-libraries/bin/rest.hpp"
#include "../global-libraries/bin/cryptation.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Definitions
// #define DEBUG

// Class prototype
class Workload
{
private:
	json settings;
	Cryptation cryptation;
	int attempts;
	int ID;
	bool priority;

	json decodeWorkload(int repoID);
	json getPriority();
	json getNormal();

	json getNormalRepoInfo();
public:
	Workload(json settings);

	json getWorkload();
	void setDone();
	void setter(int ID, bool priority);

	static json getWorkload(json settings);
	static void setDone(json settings, int ID, bool priority);
};

// Function(s)
Workload::Workload(json settings)
{
	/**
	 * Constructor
	 *
	 * @param input: json object containing the input data
	 */

	Workload::settings = InputCheck::sanitize(settings);
	Workload::cryptation = Cryptation(Workload::settings["server_gpg_key"], false);
}

json Workload::getWorkload()
{
	/**
	 * Get the workload of the process
	 *
	 * @return: string containing the workload
	 */
	// Initialize variables
	Workload::attempts = 5;
	Workload::priority = true;
	Workload::ID = -1;

	// Try to get the priority workload
	json priority(Workload::getPriority());

	if (!priority.empty())
		return {{"type", 0},
				{"workload", priority}};

	Workload::priority = false;

	// Try to get the normal workload
	json normal(Workload::getNormal());

	if (!normal.empty())
		return {{"type", 1},
				{"workload", normal}};

	// Otherwise
	return {{"type", 2}};
}

json Workload::getPriority()
{
	/**
	 * Get the priority of the process
	 *
	 * @return: json containing the priority
	 */

	json request = {
		{"request", "server_get_priority"},
		{"server_name", Workload::settings["server_name"]},
		{"server_password", Workload::settings["server_password"]}};

	Rest rest(
		Rest::CREATESTRUCTURE_REST_API,
		"",
		request,
		true);

	json response = rest.jsonRequest();
#ifdef DEBUG
	cout << response.dump(4) << endl;
#endif // DEBUG

	switch (response["code"].get<int>())
	{
	case 200: // New priority
		Workload::ID = response["priorityID"].get<int>();
		return response;

	case 201: // No priority
		return {};

	default:
		cerr << "Error: " << response["message"].get<string>() << endl;
		if (Workload::attempts-- > 0)
			return Workload::getPriority();
		else
			return {};
	}
}

json Workload::getNormal()
{
	/**
	 * Get the normal workload of the process
	 *
	 * @return: json containing the normal workload
	 */

	json request = {
		{"request", "server_reserve_job"},
		{"server_name", Workload::settings["server_name"]},
		{"server_password", Workload::settings["server_password"]}};

	Rest rest(
		Rest::CREATESTRUCTURE_REST_API,
		"",
		request,
		true);

	json response = rest.jsonRequest();
#ifdef DEBUG
	cout << response.dump(4) << endl;
#endif // DEBUG
	
	/*
	// Usefull for debugging
	response["code"] = 200;
	response["repoID"] = 2;
	*/

	switch (response["code"].get<int>())
	{
	case 200: // New job
		Workload::ID = response["repoID"].get<int>();
		
		return Workload::getNormalRepoInfo();

	case 204: // No job
		return {};

	default:
		cerr << "Error: " << response["message"].get<string>() << endl;
		return {};
	}
}

json Workload::getNormalRepoInfo()
{
	/**
	 * Get the normal workload of the process
	 *
	 * @return: json containing the normal workload
	 */
	try {
		json tmp = Workload::decodeWorkload(Workload::ID);
		if (!tmp["token"].is_string() || !tmp["username"].is_string())
		{
			cerr << "Error: invalid data" << endl;
			return {};
		}
		tmp["answers"] = RepoInfoCheck::sanitize(tmp["answers"]);
		return tmp;
	} catch (...) {
		cerr << "Error: invalid data" << endl;
		if (Workload::attempts-- > 0)
			return Workload::getNormalRepoInfo();
		else
			return {};
	}
}

void Workload::setDone()
{
	/**
	 * Set the finished status of the process
	 */
	if (Workload::ID == -1)
		return;

	if (Workload::priority)
	{
		json request = {
			{"request", "server_set_priority_done"},
			{"server_name", Workload::settings["server_name"]},
			{"server_password", Workload::settings["server_password"]},
			{"priorityID", Workload::ID}};

		json response = Rest::jsonRequest(
			Rest::CREATESTRUCTURE_REST_API,
			"",
			request,
			true
		);
#ifdef DEBUG
		cout << response.dump(4) << endl;
#endif // DEBUG
	} else {
		json request = {
			{"request", "server_set_job_done"},
			{"server_name", Workload::settings["server_name"]},
			{"server_password", Workload::settings["server_password"]},
			{"repoID", Workload::ID}};

		json response = Rest::jsonRequest(
			Rest::CREATESTRUCTURE_REST_API,
			"",
			request,
			true
		);
#ifdef DEBUG
		cout << response.dump(4) << endl;
#endif // DEBUG
	}
}

void Workload::setter(int ID, bool priority)
{
	/**
	 * Set the ID and priority of the process
	 *
	 * @param ID: ID of the process
	 * @param priority: priority of the process
	 */
	Workload::ID = ID;
	Workload::priority = priority;
}

json Workload::decodeWorkload(int repoID)
{
	/**
	 * Decode the workload
	 *
	 * @return: json object containing the workload
	 */
	// Get the repo info
	Rest rest = Rest(
		Rest::CREATESTRUCTURE_REST_API,
		"",
		{{"request", "server_get_job_info"},
		 {"server_name", Workload::settings["server_name"]},
		 {"server_password", Workload::settings["server_password"]},
		 {"repoID", repoID}},
		true);

#ifdef DEBUG
	cout << rest.jsonRequest().dump(4) << endl;
	cout << rest.jsonRequest()["repo_info"].get<string>() << endl;
#endif // DEBUG

	// Decrypt the repo info
	string decrypted = "";
	for (auto &i : json::parse(rest.jsonRequest()["repo_info"].get<string>()))
	{
#ifdef DEBUG
		cout << i.dump(4) << endl;
#endif // DEBUG
		decrypted += Workload::cryptation.decrypt(i.get<string>());
	}

	return json::parse(decrypted);
}

json Workload::getWorkload(json settings)
{
	/**
	 * Get the workload of the process
	 *
	 * @param settings: settings of the process
	 * @return: string containing the workload
	 */

	return Workload(settings).getWorkload();
}

void Workload::setDone(json settings, int ID, bool priority)
{
	/**
	 * Set the finished status of the process
	 *
	 * @param settings: settings of the process
	 * @param ID: ID of the process
	 * @param priority: true if the process is a priority, false otherwise
	 */

	Workload workload(settings);
	workload.setter(ID, priority);
	workload.setDone();
}

#undef DEBUG
#endif
