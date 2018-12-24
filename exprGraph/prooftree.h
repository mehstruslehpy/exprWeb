#ifndef _PROVER_H
#define _PROVER_H
#include "defs.h"
#include <vector>
#include <string>
#include <algorithm>
class ProofNode
{
    static int _instances; //counts proof nodes declaration
    string _nodeID; //a number for each instance of a proof node
    vector<shared_ptr<BoolExp>> _premises; //the vector of formulas at this node
    //vector<bool> _checked; //tracks whether or not a specific formula has been inferred from or not
    shared_ptr<ProofNode> _left; //the left branch by default the main branch if no right branch exists
    shared_ptr<ProofNode> _right;
    //bool _blocked; //true if this node is on a branch containing a contradiction
    //bool _root; //true if the current node is the root/top of a tree

    void Infer(ProofNode&);
    void NotInfer(ProofNode&,shared_ptr<BoolExp>);
    void Prove(ProofNode&);
    void PrintNodes(ostream&);
    void PrintLabels(ostream&);
    string NodeName();
    string NodeID();
    void PassToLeaves(shared_ptr<BoolExp>);
    void PassToLeavesSplit(BoolReturn&);

    //blocking functions
    void BlockOffBranches(vector<string>);
    void CheckForBlock(vector<string>);
    bool IsNegation(string,string);
public:
    //rule of five functions
    ProofNode();
    ProofNode(const ProofNode&) = default; //Copy constructor
    ProofNode(ProofNode&&) = default; //Move constructor
    ProofNode& operator=(const ProofNode&) & = default; //Copy assignment operator
    ProofNode& operator=(ProofNode&&) & = default; //Move assignment operator
    virtual ~ProofNode() { } //Destructor
    //end rule of five functions

    void BlockOffBranches();
    void AddPremise(shared_ptr<BoolExp>);
    void Prove();
    bool IsCompleted();
    void PrintGraph(ostream&);
};

#endif
