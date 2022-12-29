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
#include <iostream>
#include <string>
#include <thread>
#include <sys/reboot.h>
#include <unistd.h>
#include "../global-libraries/bin/json.hpp"
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

#endif
