/**
 * @file workload.cpp
 *
 * @brief Library to get the workload of a process.
 *
 * @author Castellani Davide (@DavideC03)
 */

// Dependencies
#include "workload.hpp"

// Definitions
#define DEBUG

/**
 * Constructor
 *
 * @param input: json object containing the input data
 */
Workload::Workload(json settings)
{
	Workload::settings = InputCheck::sanitize(settings);
	Workload::cryptation = Cryptation(Workload::settings["server_gpg_key"], false);
}

/**
 * Get the workload of the process
 *
 * @return: string containing the workload
 */
json Workload::getWorkload()
{
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

/**
 * Get the priority of the process
 *
 * @return: json containing the priority
 */
json Workload::getPriority()
{
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
			return Workload::getNormal();
		else
			exit(406);
	}
}

/**
 * Get the normal workload of the process
 *
 * @return: json containing the normal workload
 */
json Workload::getNormal()
{
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
		if (Workload::attempts-- > 0)
			return Workload::getNormal();
		else
			exit(406);
	}
}

/**
 * @brief Get the normal workload of the process
 *
 * @return json Json containing the normal workload
 */
json Workload::getNormalRepoInfo()
{
	try
	{
		json tmp = Workload::decodeWorkload(Workload::ID);
		if (!tmp["token"].is_string() || !tmp["username"].is_string())
		{
			cerr << "Error: invalid data (1)" << endl;
			return {};
		}
		tmp["answers"] = RepoInfoCheck::sanitize(tmp["answers"]);
		return tmp;
	}
	catch (...)
	{
		cerr << "Error: invalid data (2)" << endl;
		if (Workload::attempts-- > 0)
			return Workload::getNormalRepoInfo();
		else
			return {};
	}
}

/**
 * @brief Set the finished status of the process
 */
void Workload::setDone()
{
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
			true);
#ifdef DEBUG
		cout << response.dump(4) << endl;
#endif // DEBUG
	}
	else
	{
		json request = {
			{"request", "server_set_job_done"},
			{"server_name", Workload::settings["server_name"]},
			{"server_password", Workload::settings["server_password"]},
			{"repoID", Workload::ID}};

		json response = Rest::jsonRequest(
			Rest::CREATESTRUCTURE_REST_API,
			"",
			request,
			true);
#ifdef DEBUG
		cout << response.dump(4) << endl;
#endif // DEBUG
	}
}

/**
 * @brief Set the ID and priority of the process
 *
 * @param ID ID of the process
 * @param priority Priority of the process
 */
void Workload::setter(int ID, bool priority)
{
	Workload::ID = ID;
	Workload::priority = priority;
}

/**
 * @brief Decode the workload
 *
 * @return json Object containing the workload
 */
json Workload::decodeWorkload(int repoID)
{
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
	cout << rest.textRequest() << endl;
	cout << rest.jsonRequest().dump(4) << endl;
	cout << rest.jsonRequest()["repo_info"].get<string>() << endl;
#endif // DEBUG

	// Decrypt the repo info
	string decrypted = "";
	if (!json::accept(rest.jsonRequest()["repo_info"].get<string>()))
	{
		if (rest.jsonRequest()["repo_info"].is_string())
		{
			cerr << "Can't read repo_info as json" << rest.jsonRequest()["repo_info"].get<string>() << endl;
			exit(502);
		}
		else
		{
			cerr << "Can't read repo_info" << endl;
			exit(502);
		}
	}

	for (auto &i : json::parse(rest.jsonRequest()["repo_info"].get<string>()))
	{
#ifdef DEBUG
		cout << i.dump(4) << endl;
#endif // DEBUG
		decrypted += Workload::cryptation.decrypt(i.get<string>());
	}

	if (!json::accept(decrypted))
	{
		cerr << "Can't read decrypted as json" << decrypted << endl;
		exit(502);
	}

	return json::parse(decrypted);
}

/**
 * @brief Get the workload of the process
 *
 * @param settings Settings of the process
 * @return string String containing the workload
 */
json Workload::getWorkload(json settings)
{
	return Workload(settings).getWorkload();
}

/**
 * @brief Set the finished status of the process
 *
 * @param settings Settings of the process
 * @param ID ID of the process
 * @param priority True if the process is a priority, False otherwise
 */
void Workload::setDone(json settings, int ID, bool priority)
{
	Workload workload(settings);
	workload.setter(ID, priority);
	workload.setDone();
}

#undef DEBUG
