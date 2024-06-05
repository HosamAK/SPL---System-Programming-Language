#include "SelectionPolicy.h"
#include "Simulation.h"
#include <string>

using std::string;

SelectionPolicy::~SelectionPolicy() = default;
EdgeWeightSelectionPolicy::~EdgeWeightSelectionPolicy() = default;
MandatesSelectionPolicy::~MandatesSelectionPolicy() = default;

//returns id of the party to send offer to
int MandatesSelectionPolicy::select(Simulation &sim, int agentId){
    Graph g = sim.getGraph();
    //party id for the agent
    int prId = sim.getAgents()[agentId].getPartyId();
    int max = -1;
    for(int i = 0; i < g.getNumVertices(); i++){
        //check if there is an edge
        if(g.getEdgeWeight(prId, i) > 0 && g.getParty(i).getState() != Joined){
            //check if it doesnt already have offer from same coalition
            if(!(sim.getParty(i).hasOffer(sim.getParty(prId).getCoId()))){
                if(max == -1){
                    max = i;
                }
                else if(g.getMandates(max) < g.getMandates(i)){
                    max = i;
                }
            }
        }
    }
    return max;
}
string MandatesSelectionPolicy::get(){
    return "M";
}
string EdgeWeightSelectionPolicy::get(){
    return "E";
}
//returns id of the party to send offer to
int EdgeWeightSelectionPolicy::select(Simulation &sim, int agentId){
    Graph g = sim.getGraph();
    //party id for the agent
    int prId = sim.getAgents()[agentId].getPartyId();
    int max = prId;
    for(int i = 0; i < g.getNumVertices(); i++){
        //check if there is an edge
        if(g.getEdgeWeight(prId, i) > 0 && g.getParty(i).getState() != Joined){
            //check if it doesnt already have offer from same coalition
            if(!sim.getParty(i).hasOffer(sim.getParty(prId).getCoId())){
                if(g.getEdgeWeight(max, prId) < g.getEdgeWeight(i, prId)){
                    max = i;
                }
            }
        }
    }
    //not found
    if(max == prId)
        return -1;

    return max;
}