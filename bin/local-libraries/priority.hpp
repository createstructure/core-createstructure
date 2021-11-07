/**
 * priority.hpp
 *
 * Library for priority queue.
 *
 * @author Castellani Davide (@DavideC03)
 */

#ifndef PRIORITY
// If not imported yet
#define PRIORITY

// Dependencies
#include <bits/stdc++.h>
#include <sys/reboot.h>
#include <unistd.h>
#include "../global-libraries/bin/json.hpp"
#include "../global-libraries/bin/sleep.hpp"
#include "inputCheck.hpp"

// using ...
using namespace std;
using namespace std::this_thread;
using json = nlohmann::json;

// Definitions
// #define DEBUG

// Class prototype(s)
class Priority
{
private:
	inline static const vector<string> priority = {"test", "update", "shutdown", "reboot"};
	json settings;

public:
	Priority(json settings);
	void execute(string priorityType, int id);

	static void execute(json settings, string priorityType, int id);
	static void execute(json settings, string priorityType);
};

// Function(s)
Priority::Priority(json settings)
{
	/**
	 * Constructor
	 *
	 * @param settings - json object with the settings
	 */
	this->settings = InputCheck::sanitize(settings);
}

void Priority::execute(string priorityName, int id)
{
	/**
	 * Execute the priority queue
	 *
	 * @param priorityName - name of the priority queue
	 * @param id - id of the process
	 */

	if (find(Priority::priority.begin(), Priority::priority.end(), priorityName) != Priority::priority.end())
	{
		switch (find(Priority::priority.begin(), Priority::priority.end(), priorityName) - Priority::priority.begin())
		{
		case 0: // test
#ifdef DEBUG
			cout << "Priority: " << priorityName << endl;
#endif // DEBUG
			cout << "test" << endl;
			break;
		case 1: // update
#ifdef DEBUG
			cout << "Priority: " << priorityName << endl;
#endif // DEBUG
			system("apt update; apt full-upgrade -y; docker pull ghcr.io/createstructure/core-createstructure:latest");
			break;
		case 2: // shutdown
#ifdef DEBUG
			cout << "Priority: " << priorityName << endl;
#endif // DEBUG
			Sleep::sleep(60);
			system("shutdown -h now");
			break;
		case 3: // reboot
#ifdef DEBUG
			cout << "Priority: " << priorityName << endl;
#endif // DEBUG
			Sleep::sleep(60);
			sync();
			reboot(RB_AUTOBOOT);
			break;
		default:
			break;
		}
	}
}

void Priority::execute(json settings, string priorityType, int id)
{
	/**
	 * Execute the priority queue
	 *
	 * @param settings - json object with the settings
	 * @param priorityType - name of the priority queue
	 * @param id - id of the process
	 */
	Priority(settings).execute(priorityType, id);
}

#undef DEBUG
#endif
