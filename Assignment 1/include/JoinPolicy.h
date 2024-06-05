#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;

class Simulation;


class JoinPolicy {
    public:
    virtual ~JoinPolicy();
    virtual string  get()=0;
    virtual int join(Simulation &s, int partyID, vector<int> offers) =0;
};

class MandatesJoinPolicy : public JoinPolicy {
    public:
    ~MandatesJoinPolicy() override;
    string get()override;
    int join(Simulation &s, int partyID, vector<int> offers) override;
};

class LastOfferJoinPolicy : public JoinPolicy {
    public:
    ~LastOfferJoinPolicy()override;
    string get()override;
    int join(Simulation &s, int partyID, vector<int> offers) override;
};