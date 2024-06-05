#include "JoinPolicy.h"
#include "Simulation.h"
using std::string;

JoinPolicy::~JoinPolicy() = default;
MandatesJoinPolicy::~MandatesJoinPolicy() = default;
LastOfferJoinPolicy::~LastOfferJoinPolicy() = default;

//returns the id with the agent to join
int MandatesJoinPolicy::join(Simulation &s, int partyID, vector<int> offers){
    int max = offers[0];
    for(int i : offers){
        if(s.getParty(s.getAgents()[i].getPartyId()).getMandates() > s.getParty(s.getAgents()[max].getPartyId()).getMandates())
            max = i;

    }
    return max;
}
string LastOfferJoinPolicy::get(){
      return "L";
}
//returns the id with the agent to join
int LastOfferJoinPolicy::join(Simulation &s, int partyID, vector<int> offers){
    return offers[offers.size()-1];
}
string MandatesJoinPolicy::get(){
      return "M";
}