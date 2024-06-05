#pragma once

#include <vector>
#include "Graph.h"


class SelectionPolicy;
class Graph;

class Agent
{
public:
    Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy);
    Agent(const Agent &other);
    int getPartyId() const;
    int getId() const;
    void step(Simulation &s);
    ~Agent();
    Agent &operator=(const Agent &other);
    Agent &operator=(Agent &&other);
    SelectionPolicy* getSelectionPolicy()const;
    void setPolicy(SelectionPolicy *selectionPolicy);
    void setId(int id);
    void setPartyId(int id);

private:
    int mAgentId;
    int mPartyId;
    SelectionPolicy *mSelectionPolicy;
};
