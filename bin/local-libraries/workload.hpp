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
#include <sys/reboot.h>
#include <unistd.h>
#include "inputCheck.hpp"
#include "../global-libraries/bin/json.hpp"
#include "../global-libraries/bin/rest.hpp"
#include "../global-libraries/bin/cryptation.hpp"

// using ...
using namespace std;
using namespace std::this_thread;
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
};

// Function(s)
Workload::Workload(json settings)
{
	/**
	 * Constructor
	 *
	 * @param input: json object containing the input data
	 */

	Workload::settings = InputCheck(settings).sanitize();
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
	/*
	// Usefull for debugging
	response["code"] = 200;
	response["repo_id"] = 1;
	*/

	switch (response["code"].get<int>())
	{
	case 200: // New job
		return Workload::decodeWorkload(response["repo_id"].get<int>());

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

/*
// Declared functions
json decodeWork();
bool existsInJson(const json j, const string key);
json getWork();
void superWork(string work, json workInfo);
json getSettings();

// Function(s)
bool invalidChar(char c)
{
	/* Invalid char: check if a char is valid or not
	 *
	 * input:
	 *      - c: the char to analyze
	 *
	 * output:
	 *      - true if the given char is invalid
	 *
	return !(c >= 32 && c < 128);
}

void stripUnicode(string &str)
{
	/* Strip Unicode: strip a string to the readible chars
	 *
	 * input:
	 *      - the string to adjust
	 *
	str.erase(remove_if(str.begin(), str.end(), invalidChar), str.end());
}

json decodeWork()
{
	/* Decode Work: get decoded work instructions
	 *
	 * output:
	 *      - decoded work instructions
	 *

	// Check if it's time to reboot
	chrono::duration<double, milli> tm = chrono::high_resolution_clock::now() - start; // milliseconds
	if (tm.count() > 21600000)
	{ // 43200000 = 12h in millisecond; 21600000 = 6h in millisecond; 10800000 = 3h in millisecond
		sleep_for(60s);
		sync();
		reboot(RB_AUTOBOOT);
		exit(0); // Stop the manager to avoid work interruption
	}
//	cout << "dec" << endl;
#ifdef DEBUG
	cout << textRequest(
				"https:\u002F\u002Fwww.castellanidavide.it/other/rest/product/give_work.php",
				"",
				getSettings(),
				"POST")
		 << endl;
#endif // DEBUG

	try
	{
		json message(jsonRequest(
			"https:\u002F\u002Fwww.castellanidavide.it/other/rest/product/give_work.php",
			"",
			getSettings(),
			"POST"));

#ifdef DEBUG
		cout << "--->" << message.dump() << endl;
#endif // DEBUG

		if (!existsInJson(message, "message"))
			return message;

		string decoded_message;
		json workInfo;

		switch (messageTypeConvert[message["message"].get<string>()])
		{
		case messageType::newWork:
#ifdef DEBUG
			cout << "New work" << endl;
#endif // DEBUG
			decoded_message = "";
			for (auto &element : message["data"])
			{
				decoded_message += decrypt(element.get<string>());
				// cout << decoded_message << endl;
			}

			stripUnicode(decoded_message);

#ifdef DEBUG
			cout << "Decoded message: " << decoded_message << endl;
#endif // DEBUG

			try
			{
				message["data"] = json::parse(decoded_message);
			}
			catch (...)
			{
				message["data"] = decoded_message;
			}

			return message;
			break;

		case messageType::noWork:
#ifdef DEBUG
			cout << "No work" << endl;
#endif // DEBUG
			sleep_for(1s);
			throw "Try again";
			break;

		case messageType::superWork:
#ifdef DEBUG
			cout << "Superwork" << endl;
#endif // DEBUG
			workInfo["server_id"] = getSettings()["server_id"].get<string>();
			workInfo["server_code"] = getSettings()["server_code"].get<string>();
			workInfo["work_id"] = message["priority_id"].get<string>();
			superWork(message["priority_message"].get<string>(), workInfo);

			throw "Finished superwork";
			break;

		default:
#ifdef DEBUG
			cout << "Ununderstanded message" << endl;
#endif // DEBUG
			cout << "Unknown message: " << message["message"] << endl;
		}
	}
	catch (...)
	{
		return decodeWork();
	}

	cout << "Error taking job work" << endl;
	return nullptr;
}

bool existsInJson(const json j, const string key)
{
	/* Exist In Json: check if key exist in json
	 *
	 * input:
	 *	- j: the json object
	 *	- key: the key to search
	 *
	 * output:
	 *      - if disponible, work instructions
	 *
	return j.find(key) != j.end();
}

json getWork()
{
	/* Get Work: if disponible get work instructions
	 *
	 * output:
	 *      - if disponible, work instructions
	 *
	// Return if new work is assigned
	json d(decodeWork());
	if (d.empty())
	{
		sleep_for(5s);
		return getWork();
	}
	return d;
}

void superWork(string work, json workInfo)
{
	/* Get Work: if disponible get work instructions
	 *
	 * input:
	 *      - work: the work tag
	 *      - workInfo: the information(s) to set the work as done
	 *
	// Local variabile(s)
	unordered_map<string, string> works = {
		{"test", "echo test"},																							// Test print
		{"update", "apt update; apt full-upgrade -y; docker pull ghcr.io/createstructure/core-createstructure:latest"}, // Update apt packages
		{"shutdown", "sleep 1m; shutdown"}																				//,		// Wait and shudown the server
																														//{"reboot", "sleep 1m; reboot"}			// Wait and reboot the server
	};

	// Mark the instruction as done
	request(
		"https:\u002F\u002Fwww.castellanidavide.it/other/rest/product/finished_priority.php",
		"",
		workInfo,
		"POST");

	// Run instruction
	if (work == "reboot")
	{
		sleep_for(60s);
		sync();
		reboot(RB_AUTOBOOT);
	}
	else
	{
		system(works[work].c_str());
	}
}

json getSettings()
{
	/* Get Settings: return the server settings
	 *
	 * output:
	 *      - the server settings
	 *
	if (settings.empty())
	{
		ifstream t("server.settings");
		string tmp((istreambuf_iterator<char>(t)),
				   istreambuf_iterator<char>());
		settings = json::parse(tmp);
#ifdef DEBUG
		cout << "Loaded settings: " << settings.dump() << endl;
#endif // DEBUG
	}

	assert(
		settings["server_id"].get<string>() != "" &&
		settings["server_code"].get<string>() != "");

	return settings;
}
*/
#endif
