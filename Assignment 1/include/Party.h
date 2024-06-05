#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

class JoinPolicy;
class Simulation;

enum State
{
    Waiting,
    CollectingOffers,
    Joined
};

class Party
{
public:
    Party(int id, string name, int mandates, JoinPolicy *jp); 

    State getState() const;
    void setState(State state);
    int getMandates() const;
    void step(Simulation &s);
    const string &getName() const;
    ~Party();
    Party(const Party &other);
    JoinPolicy* getJoinPolicy()const;
    Party &operator=(const Party &other);
    int getId() const;
    Party &operator=(Party &&other);
    bool hasOffer(int cId) const;
    int getCoId() const;
    void setCoId(int co);
    void updateCooldown();
    void startCooldown();
    void addOffer(int agentId, int coalitionId);

private:
    int mId;
    string mName;
    int mMandates;
    JoinPolicy *mJoinPolicy;
    State mState;
    int cooldown;
    int cId;//coalition Id
    vector<int> mcIds;//all the coalitions that sent an offer
    vector<int> offers;//agentId offers
};
