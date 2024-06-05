#include "Party.h"
#include "Simulation.h"
#include "JoinPolicy.h"

Party::Party(int id, string name, int mandates, JoinPolicy *jp) : mId(id), mName(name), mMandates(mandates), mJoinPolicy(jp), mState(Waiting), cooldown(-1), cId(-1), mcIds(), offers()
{
    // You can change the implementation of the constructor, but not the signature!
}

//destructor
Party::~Party(){
    if(mJoinPolicy)
        delete mJoinPolicy;
}

//copy constructor
Party::Party(const Party &other): mId(other.mId), mName(other.mName), mMandates(other.mMandates),mJoinPolicy(other.mJoinPolicy), mState(other.mState), cooldown(other.cooldown), cId(other.cId), mcIds(other.mcIds), offers(other.offers) {

    if(mJoinPolicy->get() == "L"){
        mJoinPolicy = new LastOfferJoinPolicy;
    }else{
        mJoinPolicy = new MandatesJoinPolicy;
    }
   // mJoinPolicy(other.mJoinPolicy->clone());
}

//copy assignment operator
Party &Party::operator=(const Party &other) {
    if (this != &other){
        if(mJoinPolicy) delete mJoinPolicy;
        mState = other.getState();
        mName = other.getName();
        mMandates = other.getMandates();
        mId = other.getId();
        mJoinPolicy  = other.getJoinPolicy();
    }
    return *this;
}



//move assignment operator
Party &Party::operator=(Party &&other) {
 
    mState = other.getState();
    mName = other.getName();
    mMandates = other.getMandates();
    mId = other.getId();

    if (mJoinPolicy) delete mJoinPolicy; 
    mJoinPolicy = other.getJoinPolicy();
    other.mJoinPolicy=nullptr;
    return *this;
}

State Party::getState() const
{
    return mState;
}

void Party::setState(State state)
{
    mState = state;
}

int Party::getMandates() const
{
    return mMandates;
}

const string & Party::getName() const
{
    return mName;
}

JoinPolicy* Party::getJoinPolicy()const{
    return mJoinPolicy;
}


int Party::getId() const
{
    return mId;
}

//check if we have an offer from coalition with id == cId
bool Party::hasOffer(int cId) const{
    for(int i : mcIds){
        if(i == cId)
            return true;
    }
    return false;

}
void Party::addOffer(int agentId, int coalitionId){
    startCooldown();
    offers.push_back(agentId);
    mcIds.push_back(coalitionId);
}

int Party::getCoId() const{
    return cId;
}


void Party::setCoId(int co){
    cId = co;
}

void Party::step(Simulation &s)
{   
    //still collecting
    if(mState == CollectingOffers)
        updateCooldown();
    //cooldown ended
    if(cooldown == 0){
        //the agent to copy and join
        int agentId = mJoinPolicy->join(s, mId, offers);
        //making a new agent with same policy and this new party
        Agent ag = Agent(s.getAgents()[agentId]);
        ag.setId(s.getAgents().size());
        ag.setPartyId(mId);
        s.addAgent(ag);
        //setting same coalition
        setCoId(s.getParty(s.getAgents()[agentId].getPartyId()).getCoId());
        setState(Joined);
    }
}

void Party::updateCooldown(){
    if( cooldown > 0)
        cooldown--;
}

void Party::startCooldown(){
    if(mState == Waiting){
        cooldown = 3;
        mState = CollectingOffers;
    }
}


