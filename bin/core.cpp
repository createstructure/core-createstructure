// Dependencies
#include <bits/stdc++.h>
#include "global-libraries/bin/json.hpp"
#include "global-libraries/bin/rest.hpp"
#include "global-libraries/bin/sleep.hpp"
#include "local-libraries/inputCheck.hpp"
#include "local-libraries/workload.hpp"
#include "local-libraries/priority.hpp"
#include "local-libraries/repo.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Definitions
// #define DEBUG

// Declared functions
int main(int argc, char *argv[]);

// Code
int main(int argc, char *argv[])
{
	/**
	 * Main: the start point of the code
	 *
	 * @param argc: number of arguments passed by command-line
	 * @param argv: array of arguments passed by command-line
	 * @return: 0 if the program ends successfully
	 */
	// Function viariable(s)
	json inputs;
	string path;

	// Assert the correct number of inputs
	assert(argc == 2); // name_of_program data(json)

	// Save the given input data
	inputs = json::parse(string(argv[1]));
#ifdef DEBUG
	cout << inputs.dump() << endl;
#endif // DEBUG

	// Check the input data
	inputs = InputCheck::sanitize(inputs);
#ifdef DEBUG
	cout << inputs.dump() << endl;
#endif // DEBUG

	// Initialize the workload
	Workload workload(inputs);

	while (true)
	{
		// Take the workload
		json workloadData = workload.getWorkload();

		// Elaborate the workload
		switch (workloadData["type"].get<int>())
		{
		case 0:
// Run priority
#ifdef DEBUG
			cout << "Running priority: " << workloadData.dump(4) << endl;
#endif // DEBUG
			Priority::execute(
				inputs,
				workloadData["priority_instruction"].get<string>(),
				workloadData["priorityID"].get<int>()
			);
			break;
		case 1:
// Create a repo
#ifdef DEBUG
			cout << "Creating a repo: " << workloadData.dump(4) << endl;
#endif // DEBUG
			Repo::all(workloadData["workload"]);
			break;
		case 2:
		default:
// Do nothing
#ifdef DEBUG
			cout << "Doing nothing: " << workloadData.dump(4) << endl;
#endif // DEBUG
			Sleep::sleep(1);
			break;
		}
		
		// Set the workload as done
		workload.setDone();
#ifdef DEBUG
		cout << "Workload done" << endl;
		return 0; // Exit the program
#endif // DEBUG
	}
	return 0;
}

#undef DEBUG
