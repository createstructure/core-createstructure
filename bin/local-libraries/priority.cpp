/**
 * @file priority.cpp
 *
 * @brief Library for priority queue.
 *
 * @author Castellani Davide (@DavideC03)
 */

// Dependencies
#include "priority.hpp"

// Definitions
// #define DEBUG

/**
 * @brief Constructor
 *
 * @param settings Json object with the settings
 */
Priority::Priority(json settings)
{
	this->settings = InputCheck::sanitize(settings);
}

/**
 * @brief Execute the priority queue
 *
 * @param priorityName Name of the priority queue
 * @param id Id of the process
 */
void Priority::execute(string priorityName, int id)
{
	vector<string>::const_iterator pos = find(Priority::priority.begin(), Priority::priority.end(), priorityName);

	if (pos != Priority::priority.end())
	{
		switch (pos - Priority::priority.begin())
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
			system("apt update; apt full-upgrade -y");
			break;
		case 2: // shutdown
#ifdef DEBUG
			cout << "Priority: " << priorityName << endl;
#endif // DEBUG
			exit(EXIT_SUCCESS);
			break;
		case 3: // reboot
#ifdef DEBUG
			cout << "Priority: " << priorityName << endl;
#endif							// DEBUG
			exit(EXIT_FAILURE); // Fake an error
			break;
		default:
			break;
		}
	}
}

/**
 * @brief Execute the priority queue
 *
 * @param settings Json object with the settings
 * @param priorityType Name of the priority queue
 * @param id Id of the process
 */
void Priority::execute(json settings, string priorityType, int id)
{
	Priority(settings).execute(priorityType, id);
}

#undef DEBUG
