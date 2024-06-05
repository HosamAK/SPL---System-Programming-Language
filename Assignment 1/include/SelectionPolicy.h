#pragma once
#include <string>
class Simulation;

using std::string;

class SelectionPolicy {
    public:
    virtual ~SelectionPolicy();
    virtual string get() = 0;
    virtual int select(Simulation &, int agentId) = 0;
 };

class MandatesSelectionPolicy: public SelectionPolicy{
    public:
    ~MandatesSelectionPolicy() override;
    string get() override;
    int select(Simulation &, int agentId) override;
};

class EdgeWeightSelectionPolicy: public SelectionPolicy{
    public:
    ~EdgeWeightSelectionPolicy() override;
    string get() override;
    int select(Simulation &, int agentId) override;
 };