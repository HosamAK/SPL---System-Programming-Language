#include "../include/StompProtocol.h"
#include "../include/event.h"


StompProtocol::StompProtocol():id(0), receipt(0), username(""), passcode(""), isLogged(false), idMap(), SummaryEvents() /* , SummaryEvents() */{}

void StompProtocol::insertHashm(string des){//update destination-id hash map
    idMap.insert({des, id});
    id++;
}

string StompProtocol::createFrame(string input){//create frame
	vector<string> arguments;
	StringtoArguments(input, ' ', std::ref(arguments));
	
	return helpFunction(arguments[0], arguments);
}

int StompProtocol::getIDM(string s){//return id for destination
    auto  it = idMap.find(s);
    return it->second;
}

void StompProtocol::deleteID(string s){//update destination-id hash map
    idMap.erase(s);
}

string StompProtocol::helpFunction(string command, vector<string> arguments){
	string Frame = "EMPTY";//trash value
	if(command == "login"){//CONNECT
		//user already connected
		cout<<"The client is already logged in, log out before trying again"<<endl;


	}else if(command == "join"){//SUBSCRIBE
		insertHashm(arguments[1]);//update hash map
		int id_ = getIDM(arguments[1]);
		string id = to_string(id_);
		string strec = to_string(receipt);
		receipt++;
		cout<<"Joined channel " + arguments[1]<<endl;
		Frame = "SUBSCRIBE\ndestination:/" + arguments[1] + "\nid:" + id + "\nreceipt:" + strec + "\n";
		return Frame;


	}else if(command == "exit"){//UNSUBSCRIBE
		string id = to_string(getIDM(arguments[1]));
		string strec = to_string(receipt);
		receipt++;
		deleteID(arguments[1]);
		cout<<"Exited channel " + arguments[1]<<endl;
		Frame = "UNSUBSCRIBE\nid:" + id + "\nreceipt:" + strec + "\n";
		return Frame;
		

	}else if(command == "logout"){//DISCONNECT
		isLogged = false;
		string strec = to_string(receipt);
		receipt++;
		Frame = "DISCONNECT\nreceipt:" + strec + "\n";

	}else if(command == "report"){//REPORT
		vector<string> temp;
		names_and_events nne = parseEventsFile(arguments[1]);
		string teama = nne.team_a_name;
		string teamb = nne.team_b_name;

		for(Event e : nne.events){
			Frame = "SEND\ndestination:/" + teama + "_" + teamb + "\n\nuser: " + username + "\nteam a: " + teama + "\nteam b: " + teamb + "\nevent name: ";

			string game_updates = HashmaptoString(e.get_game_updates());
			string team_a_upd = HashmaptoString(e.get_team_a_updates());
			string team_b_upd = HashmaptoString(e.get_team_b_updates());

			string subFrame = "\ngeneral game updates:\n" + game_updates + "team a updates:\n" + team_a_upd + "team b updates:\n" + team_b_upd + "description:\n" + e.get_discription();

			Frame += e.get_name() + "\ntime: " + to_string(e.get_time()) + subFrame + "\n";
			temp.push_back(Frame);
			int size = temp.size();
			
			Frame = "";
			for(int i=0; i<size-1; i++){
				Frame += temp.at(i) + "\n";
			}
			Frame += temp.at(size - 1);
		}

	}else if(command == "summary"){//SUMMARY
		string content = "";
		vector<string> teams;
		vector<string> events;
		int counter = 5;
		int newcounter =3;
		bool active = false;
		bool halftime = false;
		StringtoArguments(arguments[1], '_', std::ref(teams));
		content += teams[0] +" vs " + teams[1] +"\nGame stats:\nGeneral stats:\n";
		for(string rep : SummaryEvents[arguments[2]][arguments[1]]){
			StringtoArguments(rep, '\n', std::ref(events));
			vector<string> name;
			vector<string> time;
			StringtoArguments(events[3], ':', std::ref(name));
			StringtoArguments(events[4], ':', std::ref(time));
			




			int i=7;
			map<string, string> general_game_stats;
			while(events[i] == "    "){
				vector<string> ger_game_stats_split;
				StringtoArguments(events[i], ':', std::ref(ger_game_stats_split));
				general_game_stats[ger_game_stats_split[0].substr(4)] = ger_game_stats_split[1];
				i++;
			}
			for(auto it = general_game_stats.begin(); it != general_game_stats.end(); it++){
				content += it->first + ":" + it->second + "\n";
			}
			i++;

			content += team[0] + " stats:\n";
			map<string, string> teama_stats;
			while(events[i] == "    "){
				vector<string> teama_st;
				StringtoArguments(events[i], ':', std::ref(teama_st));
				teama_stats[teama_st[0].substr(4)] = teama_st[1];
				i++;
			}
			for(auto it = teama_stats.begin(); it != teama_stats.end(); it++){
				content += it->first + ":" + it->second + "\n";
			}
			i++;

			content += teams[1] + " stats:\n";
			map<string, string> teamb_stats;
			while(events[i] == "    "){
				vector<string> teamb_st;
				StringtoArguments(events[i], ':', std::ref(teamb_st));
				teamb_stats[teamb_st[0].substr(4)] = teamb_st[1];
				i++;
			}
			for(auto it = teamb_stats.begin(); it != teamb_stats.end(); it++){
				content += it->first + ":" + it->second + "\n";
			}
			i++;

			string NameandTime = time[1].substr(1) + " - " + name[1].substr(1) + ":\n\n";
			content += NameandTime;
			string descr = "";
			for(int j = i; j<events.size(); j++){
				descr += events[j] +" ";
			}

			content += descr + "\n\n\n";
		}

		ofstream summary;
		summary.open(arguments[3] + ".txt");
		summary << content;
		summary.close();
	}
	return Frame;
}

void StompProtocol::StringtoArguments(string input, char split, vector<string> &command){
	stringstream ss(input);
	string s;
	while(std::getline(ss, s, split)){
		command.push_back(s);
	}
}

void StompProtocol::updateisLogged(){
	isLogged = true;
}

string StompProtocol::HashmaptoString(map<string, string> ConvertMap){
	map<string, string>::iterator it;
	string converted = "";
	
	for(it=ConvertMap.begin(); it != ConvertMap.end(); ++it){
		converted += "    " + it->first + ": " + it->second + "\n";
	}
	return converted;
}

void StompProtocol::insertReport(string report){
	vector<string> splitted;
	StringtoArguments(report, '\n', std::ref(splitted));
	vector<string> us;
	StringtoArguments(splitted[3], ' ', std::ref(us));
	string user = us[1];
	vector<string> tp;
	StringtoArguments(splitted[1], '/', std::ref(tp));
	string topic = tp[1];
	SummaryEvents[user][topic].push_back(report);
}

string StompProtocol::VectoStr(vector<string> v){
	string result = "";
	for(int i=0; i<v.size(); i++)
		result += v[i] + "\n";
	return result;
}