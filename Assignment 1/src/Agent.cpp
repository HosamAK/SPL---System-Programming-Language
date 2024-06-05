#include "Agent.h"
#include "SelectionPolicy.h"
#include "Simulation.h"
Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId), mSelectionPolicy(selectionPolicy)
{
    // You can change the implementation of the constructor, but not the signature!
}

//copy cconstructor
Agent::Agent(const Agent &other): mAgentId(other.mAgentId),mPartyId(other.mPartyId),mSelectionPolicy(other.mSelectionPolicy){
    if(mSelectionPolicy->get() == "M"){
        mSelectionPolicy = new MandatesSelectionPolicy;
    }else{
        mSelectionPolicy = new EdgeWeightSelectionPolicy;
    }
    // this -> mAgentId = other.getId();
    // this -> mPartyId = other.getPartyId();
    // this -> mSelectionPolicy  = other.getSelectionPolicy();
}
//copy assignment 
Agent &Agent::operator=(const Agent &other) {
    if (this != &other){
        if(mSelectionPolicy) delete mSelectionPolicy;
        this->setId(other.getId());
        mPartyId = other.getPartyId();
        mSelectionPolicy  = other.getSelectionPolicy();
    }
    return *this;
}
//destrucctor
Agent::~Agent(){
    if(mSelectionPolicy){
        delete mSelectionPolicy;
    }
}

//move assignment 
Agent &Agent::operator=( Agent &&other) {
 
    this->setId(other.getId());
    mPartyId = other.getPartyId();

    if (mSelectionPolicy) delete mSelectionPolicy; 
    mSelectionPolicy = other.getSelectionPolicy();
    other.setPolicy(nullptr);
    return *this;
}

int Agent::getId() const
{
    return mAgentId;
}

void Agent::setId(int id){
    mAgentId = id;
}

int Agent::getPartyId() const
{
    return mPartyId;
}

void Agent::setPartyId(int id)
{
    mPartyId = id;
}


SelectionPolicy* Agent::getSelectionPolicy()const{
    return mSelectionPolicy;
}

void Agent::setPolicy(SelectionPolicy *selectionPolicy){
    mSelectionPolicy =  selectionPolicy;
}


void Agent::step(Simulation &s)
{
    int pId = mSelectionPolicy->select(s, mAgentId);
    //if pId is -1 then no such selection has been found
    if( !(pId == -1)) {
        s.getParty(pId).addOffer(mAgentId, s.getParty(s.getAgents()[mAgentId].getPartyId()).getCoId());
    }
}

