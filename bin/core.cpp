// Dependencies
#include <bits/stdc++.h>
#include "libraries/bin/json.hpp"
#include "libraries/bin/createstructure.hpp"

// using ...
using namespace std;
using json = nlohmann::json;

// Definitions
#define DEBUG

// Declared functions
int main(int argc, char *argv[]);

// Code
int main(int argc, char *argv[]) {
	/* Main: the start point of the code
	 *
	 * inputs:
	 * 	- argc: the number of command-line arguments
	 *	- argv: an array containing all command-line arguments
	 *
	 * output:
	 *	- a run code: if it works in the correct way it will return 0
	 */
	// Function viariable(s)
	json inputs;
	string path;

	// Assert the correct number of inputs
	assert(argc ==  2); // name_of_program data(json)

	// Get the given input data
	inputs = json::parse(string(argv[1]));

#ifdef DEBUG
	cout << inputs.dump() << endl;
#endif // DEBUG

	if (inputCheck(inputs)) {

#ifdef DEBUG
		cout << getEmoji("✓") << "\t" << "inputs checked" << endl;
#endif // DEBUG

		path = string("/media/createstructure/") +
			inputs["username"].get<string>() +
			string("???") +
			inputs["answers"]["name"].get<string>();

#ifdef DEBUG
		cout << getEmoji("✓") << "\t" << "create path variable" << endl;
#endif // DEBUG

		download(
				chooseTemplate(
					inputs["answers"]["template"].get<string>(),
					inputs["token"].get<string>(),
					inputs["username"].get<string>()
				),
				path.c_str()
			);

#ifdef DEBUG
		cout << getEmoji("✓") << "\t" << "choosed & downloaded template" << endl;
#endif // DEBUG

		elaborateAll(
					path,
					getChanges(
						inputs,
						path + "/.createstructure/change.json"
					)
				);

#ifdef DEBUG
		cout << getEmoji("✓") << "\t" << "getted changes and elaborated all" << endl;
#endif // DEBUG

		json o;
		o["name"] = (
						inputs["answers"]["prefix"].get<string>() 
						== "" 
					) ?
						inputs["answers"]["name"].get<string>() :
						inputs["answers"]["prefix"].get<string>() +
							"-" +
							inputs["answers"]["name"].get<string>();
		o["description"] = inputs["answers"]["descr"].get<string>();
		o["private"] = inputs["answers"]["private"].get<bool>();

		if (
				inputs["answers"]["isOrg"].get<bool>() &&
				inputs["answers"]["team"].get<string>() != "")
			{
				string teamLink = string("https:\u002F\u002Fapi.github.com/orgs/") +
										inputs["answers"]["org"].get<string>() +
										"/teams";

				json teams = jsonRequest(teamLink, inputs["token"].get<string>(), nullptr, "");
				
				long long int teamId = -1;
				for (auto& [key, value] : teams.items()) {
					if (value["name"].get<string>() == inputs["answers"]["team"].get<string>())
						teamId = value["id"].get<long long int>();
				}
				
				if (teamId == -1)
				{
					json teamInfo;
					teamInfo["name"] = inputs["answers"]["team"].get<string>();
					teamInfo["description"] = "Team made automatically by createstructure.";

					json team = jsonRequest(teamLink, inputs["token"].get<string>(), teamInfo, "POST");
					teamId = team["id"].get<long long int>();
				}

				o["team_id"] = teamId;
			}

		string link = string("https:\u002F\u002Fapi.github.com/") +
				(inputs["answers"]["isOrg"].get<bool>() ?
	                                (string("orgs/") + inputs["answers"]["org"].get<string>()) :
	                                "user") +
				string("/repos");
		string token = inputs["token"].get<string>();
		request(link, token, o, "POST");

		upload(string("https:\u002F\u002F") +
                        inputs["username"].get<string>() +
                        string(":") +
                        inputs["token"].get<string>() +
                        string("@github.com/") +
                        (inputs["answers"]["isOrg"].get<bool>() ?
				inputs["answers"]["org"].get<string>() :
				inputs["username"].get<string>()) +
                        string("/") +
                        o["name"].get<string>(),

			string("/media/createstructure/") +
			inputs["username"].get<string>() +
			string("???") +
			inputs["answers"]["name"].get<string>()
			);

#ifdef DEBUG
		cout << getEmoji("✓") << "\t" << "uploaded repo" << endl;
#endif // DEBUG

		// Set work as finished
		json finishJson;
                finishJson["server_id"] = inputs["server_id"].get<string>();
                finishJson["server_code"] = inputs["server_code"].get<string>();
                finishJson["work_id"] = inputs["work_id"].get<string>();

                string endLink("https:\u002F\u002Fwww.castellanidavide.it/other/rest/product/finished_work.php");
#ifdef DEBUG
                cout << getEmoji("✓") << "\t" << textRequest(endLink, "", finishJson, "POST") << endl;
#endif // DEBUG
	} else {
		cout << "Given uncorrect data " << getEmoji("sad") << endl;
	}

	return 0;
}

