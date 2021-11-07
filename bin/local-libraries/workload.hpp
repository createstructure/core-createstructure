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

	json decodeWorkload(int repo_id);
	json getPriority();
	json getNormal();

public:
	Workload(json settings);

	json getWorkload();

	static json getWorkload(json settings);
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

	json priority(Workload::getPriority());

	if (!priority.empty())
		return {{"type", 0},
				{"workload", priority}};

	json normal(Workload::getNormal());

	if (!normal.empty())
		return {{"type", 1},
				{"workload", normal}};

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
		return response;

	case 201: // No priority
		return {};

	default:
		cerr << "Error: " << response["message"].get<string>() << endl;
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
	
	// Usefull for debugging
	response["code"] = 200;
	response["repo_id"] = 2;

	json tmp;

	switch (response["code"].get<int>())
	{
	case 200: // New job
		tmp = Workload::decodeWorkload(response["repo_id"].get<int>());
		if (!tmp["token"].is_string() || !tmp["username"].is_string())
		{
			cerr << "Error: invalid data" << endl;
			return {};
		}
		tmp["answers"] = RepoInfoCheck::sanitize(tmp["answers"]);
		return tmp;

	case 204: // No job
		return {};

	default:
		cerr << "Error: " << response["message"].get<string>() << endl;
		return {};
	}
}

json Workload::decodeWorkload(int repo_id)
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
		 {"repo_id", repo_id}},
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
	 * @param input: json object containing the input data
	 * @return: string containing the workload
	 */

	return Workload(settings).getWorkload();
}
#endif
