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
#include <iostream>
#include <string>
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

#endif
