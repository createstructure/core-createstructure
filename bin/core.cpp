// Dependencies
#include <bits/stdc++.h>
#include "global-libraries/bin/json.hpp"
#include "global-libraries/bin/rest.hpp"
#include "global-libraries/bin/sleep.hpp"
#include "local-libraries/inputCheck.hpp"
#include "local-libraries/workload.hpp"
#include "local-libraries/priority.hpp"
#include "local-libraries/repo.hpp"

//#include "libraries/bin/createstructure.hpp"
/*#include "libraries/bin/createstructure_emoji.hpp"
#include "libraries/bin/createstructure_inputCheck.hpp"
#include "libraries/bin/createstructure_download.hpp"
#include "libraries/bin/createstructure_chooseTemplate.hpp"
#include "libraries/bin/createstructure_elaborate.hpp"
#include "libraries/bin/createstructure_changes.hpp"
#include "libraries/bin/createstructure_upload.hpp"
*/

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

	Priority priority = Priority(inputs);

	while (true)
	{
		// Take the workload
		json workloadData = Workload::getWorkload(inputs);

		// Elaborate the job
		switch (workloadData["type"].get<int>())
		{
		case 0:
// Run priority
#ifdef DEBUG
			cout << "Running priority: " << workloadData.dump(4) << endl;
#endif // DEBUG
			priority.execute(workloadData["priority_instruction"].get<string>(), workloadData["priority_ID"].get<int>());
			break;
		case 1:
// Create a repo
#ifdef DEBUG
			cout << "Creating a repo: " << workloadData.dump(4) << endl;
#endif // DEBUG
			Repo::all(workloadData["workload"]);
			return 0;
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
	}

	/*
		elaborateAll(
			path,
			getChanges(
				inputs,
				path + "/.createstructure/change.json"));

#ifdef DEBUG
		cout << getEmoji("✓") << "\t"
			 << "getted changes and elaborated all" << endl;
#endif // DEBUG





		upload(string("https:\u002F\u002F") +
				   inputs["username"].get<string>() +
				   string(":") +
				   inputs["token"].get<string>() +
				   string("@github.com/") +
				   (inputs["answers"]["isOrg"].get<bool>() ? inputs["answers"]["org"].get<string>() : inputs["username"].get<string>()) +
				   string("/") +
				   o["name"].get<string>(),

			   string("/media/createstructure/") +
				   inputs["username"].get<string>() +
				   string("???") +
				   inputs["answers"]["name"].get<string>());





		// Set work as finished
		json finishJson;
		finishJson["server_id"] = inputs["server_id"].get<string>();
		finishJson["server_code"] = inputs["server_code"].get<string>();
		finishJson["work_id"] = inputs["work_id"].get<string>();

		string endLink("https:\u002F\u002Fwww.castellanidavide.it/other/rest/product/finished_work.php");
#ifdef DEBUG
		cout << getEmoji("✓") << "\t" << textRequest(endLink, "", finishJson, "POST") << endl;
#endif // DEBUG
	}
	else
	{
		cout << "Given uncorrect data " << getEmoji("sad") << endl;
	}
	*/

	return 0;
}
