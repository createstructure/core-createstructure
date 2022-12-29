// Dependencies
#include <iostream>
#include <string>
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
string getFile(string path);

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
	bool byArg = false;

#ifdef DEBUG
	cout << "argc: " << argc << endl;
	for (int i = 0; i < argc; i++)
		cout << "argv[" << i << "]: " << argv[i] << endl;
#endif // DEBUG

	// Save the given input data
	if (argc == 2 && json::accept(string(argv[1])))
	{
			inputs = json::parse(string(argv[1]));
			byArg = true;
	}
	
	if (!byArg)
	{
#ifdef DEBUG
		system("ls /etc/auth");
#endif // DEBUG
		vector<string> credentials = {
			"server_gpg_key",
			"server_name",
			"server_password"};

		for (auto &credential : credentials)
		{
			inputs[credential] = getFile("/etc/auth/" + credential);
		}
	}
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

#ifdef DEBUG
		cout << workloadData.dump(4) << endl;
#endif // DEBUG

		// Elaborate the workload
		switch (workloadData["type"].get<int>())
		{
		case 0:
// Run priority
#ifdef DEBUG
			cout << "Running priority: " << workloadData.dump(4) << endl;
#endif // DEBUG
			Sleep::sleep(15); // Sleep for 15 seconds
			// Set the workload as done
			workload.setDone();
			Priority::execute(
				inputs,
				workloadData["workload"]["priority_instruction"].get<string>(),
				workloadData["workload"]["priorityID"].get<int>());
			break;
		case 1:
// Create a repo
#ifdef DEBUG
			cout << "Creating a repo: " << workloadData.dump(4) << endl;
#endif // DEBUG
			Repo::all(workloadData["workload"]);
			// Set the workload as done
			workload.setDone();
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

#ifdef DEBUG
		cout << "Workload done" << endl;
		return 0; // Exit the program
#endif			  // DEBUG
	}
	return 0;
}

string getFile(string path)
{
	/**
	 * getFile: get the content of a file
	 *
	 * @param path: the path of the file
	 * @return: the content of the file
	 */
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}

#undef DEBUG
