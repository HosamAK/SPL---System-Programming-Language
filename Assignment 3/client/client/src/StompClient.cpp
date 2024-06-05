#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <thread>
#include "../include/ConnectionHandler.h"
#include "../include/StompProtocol.h"

short getPort(string arg);
string getHost(string arg);
int Send(string Frame, ConnectionHandler &connectionHandler);
int Get(string Frame, StompProtocol sp, ConnectionHandler &connectionHandler, bool &reconnect);
void StringtoArguments(string input, char split, vector<string> &command);

using namespace std;


int main(int argc, char *argv[]) {
	// TODO: implement the STOMP client
	while(true){
		StompProtocol sp;
		const short bufsize = 1024;
		char buf[bufsize];

		bool reconnect = false;
    	std::cin.getline(buf, bufsize);//input command
		std::string line(buf);//convert input command to string

		vector<string> arguments;
		StringtoArguments(line, ' ', std::ref(arguments));//convert the command to arguments by space
	
		string host = getHost(arguments[1]);
		short port = getPort(arguments[1]);
		ConnectionHandler connectionHandler(host, port);//connect user to server

    	if (!connectionHandler.connect()) {//check error for connection
        	std::cerr << "Could not connect to server" << std::endl;
        	return 1;
		}

		string Frame = sp.createFrame(line);//create connect frame
		cout<<Frame<<endl;
		if (!connectionHandler.sendLine(Frame)) {//send frame
        	std::cout << "Disconnected. Exiting...\n" << std::endl;
			return 1;
    	}

		sp.updateisLogged();//update user to connected

		while(!reconnect){
        	std::cin.getline(buf, bufsize);//input command
			std::string line(buf);//convect input command to string

			Frame = sp.createFrame(line);//create frame
			cout<<Frame<<endl;
			thread sendThread(Send, Frame, std::ref(connectionHandler));
			thread getThread(Get,Frame, sp, std::ref(connectionHandler), std::ref(reconnect));
			sendThread.join();
			getThread.join();
		}
	}
	return 0;
}

short getPort(string arg){//get port from input command
	vector<string> splitarg;
	StringtoArguments(arg, ':', std::ref(splitarg));
	short port = stoi(splitarg[1]);
	return port;
}

string getHost(string arg){//get host from input command
	vector<string> splitarg;
	StringtoArguments(arg, ':', std::ref(splitarg));
	return splitarg[0];
}

void StringtoArguments(string input, char split, vector<string> &command){//convert string to arguments (vector of string)
	stringstream ss(input);
	string s;
	while(std::getline(ss, s, split)){
		command.push_back(s);
	}
}

int Send(string Frame, ConnectionHandler &connectionHandler){
	while(true){
		if(Frame != "EMPTY"){
			if (!connectionHandler.sendLine(Frame)) {//check frame sent
        		std::cout << "Disconnected. Exiting...\n" << std::endl;
        		break;
    		}
		}
	} return 0;
}

int Get(string Frame, StompProtocol sp, ConnectionHandler &connectionHandler, bool &reconnect){
	while(true){
		if(Frame.substr(0, 10) == "DISCONNECT"){//check user disconnected
			break;
		}

		string answer;
        if (!connectionHandler.getLine(answer)) {//check connection
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }


		if(answer == "5"){
			reconnect = true;
		}

		vector<string> rep;
		StringtoArguments(answer, '\n', std::ref(rep));

        answer.resize(answer.length()-1);
        if(answer == "bye") {
            std::cout << "Disconnected. Exiting...\n" << std::endl;
            break;
        }

		else if(rep[0] == "	Connected")
			cout<< "Login Successful"<<endl;

		else if(rep[0] == "MESSAGE"){
			string report;
			for(int i=8; i<rep.size()-1; i++){
				report += rep[i] + "\n";
			}

			report += rep[rep.size()-1];
			sp.insertReport(report);
		}

		//check if we got an error
		else if(rep[0] == "ERROR"){
			for(int i=0;i<rep.size(); i++)
				cout<<rep[i]<<endl;
		}else{
			for(int i=0; i<rep.size(); i++){
				cout<<rep[i]<<endl;
			}
		}
	}
	return 0;
}