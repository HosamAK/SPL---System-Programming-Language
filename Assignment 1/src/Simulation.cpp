#include "../include/Simulation.h"


Simulation::Simulation(Graph graph, vector<Agent> agents) : mGraph(graph), mAgents(agents), coNum(0)
{
    // You can change the implementation of the constructor, but not the signature!
    //setting coalition id for the parties with a coalition at the beginning
    for(Agent ag: mAgents){
        if(mGraph.getParty(ag.getPartyId()).getState() == Joined && mGraph.getParty(ag.getPartyId()).getCoId() == -1){
            mGraph.getParty(ag.getPartyId()).setCoId(ag.getId());
        }
    }

    //gets the num of coalitions
    for(Agent ag: mAgents){
        if(mGraph.getParty(ag.getPartyId()).getCoId() > coNum){
            coNum = mGraph.getParty(ag.getPartyId()).getCoId();
        }
    }
}

void Simulation::step()
{
    // TODO: implement this method
    for(int i = 0; i < mGraph.getNumVertices(); i++){
        getParty(i).step(*this);
    }
    for(Agent ag: mAgents){
        ag.step(*this);
    }
}

bool Simulation::shouldTerminate() const
{
    // TODO implement this method

    // no more parties that cant sen offer 
    //a collition has at least 61 manddates
    vector<vector<int>> v = this->getPartiesByCoalitions();

    //count mandates
    for(vector<int> vec : v){
        int mandets = 0;
        for(int i : vec){
            Party p = mGraph.getParty(i);
            mandets += p.getMandates();
        }
        if(mandets >= 61)
            return true;
        
    }

    //find at least 1 party that didnt join
    for(int i = 0; i < mGraph.getNumVertices(); i++){
        if(mGraph.getParty(i).getState() != Joined)
            return false;
    }

    return true; 
}

const Graph &Simulation::getGraph() const
{
    return mGraph;
}

const vector<Agent> &Simulation::getAgents() const
{
    return mAgents;
}

vector<Agent> &Simulation::getAgents()
{
    return mAgents;
}

const Party &Simulation::getParty(int partyId) const
{
    return mGraph.getParty(partyId);
}

Party &Simulation::getParty(int partyId)
{
    return mGraph.getParty(partyId);
}

void Simulation::addAgent(Agent ag){
    mAgents.push_back(ag);
}

/// This method returns a "coalition" vector, where each element is a vector of party IDs in the coalition.
/// At the simulation initialization - the result will be [[agent0.partyId], [agent1.partyId], ...]
const vector<vector<int>> Simulation::getPartiesByCoalitions() const
{
    // TODO: you MUST implement this method for getting proper output, read the documentation above.
    
    vector<vector<int>> v(coNum+1);

    for(int i = 0 ; i < mGraph.getNumVertices() ; i++){
        Party p = mGraph.getParty(i);
        if(p.getCoId() != -1)
            v[p.getCoId()].push_back(p.getId());
    }
    return v;
}
