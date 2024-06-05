#pragma once

#include <vector>

#include "Graph.h"
#include "Agent.h"
#include "Party.h"

using std::string;
using std::vector;

class Agent;
class Party;


class Simulation
{
public:
    Simulation(Graph g, vector<Agent> agents);

    void step();
    bool shouldTerminate() const;

    const Graph &getGraph() const;
    const vector<Agent> &getAgents() const;
    vector<Agent> &getAgents();
    const Party &getParty(int partyId) const;
    Party &getParty(int partyId);
    const vector<vector<int>> getPartiesByCoalitions() const;
    void addAgent(Agent ag);

private:
    Graph mGraph;
    vector<Agent> mAgents;
    int coNum;
};
