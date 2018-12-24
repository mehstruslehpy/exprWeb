#include "prooftree.h"
int ProofNode::_instances = 0;
ProofNode::ProofNode()
    : _left(nullptr),_right(nullptr)
{
    _instances++;
    _nodeID = to_string(_instances);
}

void ProofNode::Infer(ProofNode& node)
{
    //exit the infer function if nothing else can be inferred

    //loop for every expression in the current node
    for(auto const& value: node._premises)
    {
        //make an inference for each expression
        BoolReturn inference = value->Infer();

        switch (value->Type())
        {
        case OR_EXP:
        {
            PassToLeavesSplit(inference);
            break;
        }
        case AND_EXP:
        {
            //AND_EXP's take only one path, the "center path" can just be the left branch
            if(!_left)_left.reset(new ProofNode);
            _left->PassToLeaves(inference.op1->Copy());
            _left->PassToLeaves(inference.op2->Copy());
            if(_right)_right->PassToLeaves(inference.op1->Copy());
            if(_right)_right->PassToLeaves(inference.op2->Copy());
            break;
        }
        case COND_EXP:
        {
            //The material conditional is just (~A v B)
            shared_ptr<BoolExp> tmpnot(new NotExp(inference.op1->Copy()));
            BoolReturn tmpinfer(tmpnot,inference.op2->Copy());
            PassToLeavesSplit(tmpinfer);
            break;
        }
        case NOT_EXP:
        {
            //NOT_EXP are special and we have special rules for handling them
            NotInfer(node,value);
            break;
        }
        case VAR_EXP: //do nothing for VAR_EXP's
        {
            break;
        }
        default:
        {
            cout << "void ProofNode::Infer(shared_ptr<ProofNode> node): default inference case" << endl;
            exit(EXIT_FAILURE);
        }
        }
    }
}
void ProofNode::NotInfer(ProofNode& node,shared_ptr<BoolExp> value)
{
    //the expression inside the not
    BoolReturn innerexp = value->Infer();
    switch (innerexp.op1->Type())
    {
    case OR_EXP:
    {
        shared_ptr<BoolExp> tmp1(new NotExp(innerexp.op1->Infer().op1->Copy()));
        shared_ptr<BoolExp> tmp2(new NotExp(innerexp.op2->Infer().op2->Copy()));
        if(!_left)_left.reset(new ProofNode);
        _left->PassToLeaves(tmp1);
        _left->PassToLeaves(tmp2);
        if(_right)_right->PassToLeaves(tmp1);
        if(_right)_right->PassToLeaves(tmp2);
        break;
    }
    case AND_EXP:
    {
        shared_ptr<BoolExp> tmp1(new NotExp(innerexp.op1->Infer().op1->Copy()));
        shared_ptr<BoolExp> tmp2(new NotExp(innerexp.op2->Infer().op2->Copy()));
        BoolReturn tmpinfer(tmp1,tmp2);
        PassToLeavesSplit(tmpinfer);
        break;
    }
    case COND_EXP:
    {
        shared_ptr<BoolExp> tmp1(innerexp.op1->Infer().op1->Copy());
        shared_ptr<BoolExp> tmp2(new NotExp(innerexp.op1->Infer().op2->Copy()));
        if(!_left)_left.reset(new ProofNode);
        _left->PassToLeaves(tmp1);
        _left->PassToLeaves(tmp2);
        if(_right)_right->PassToLeaves(tmp1);
        if(_right)_right->PassToLeaves(tmp2);
        break;
    }
    case NOT_EXP: //nested not expressions become un-negated
    {
        if(!_left)_left.reset(new ProofNode);
        _left->PassToLeaves(innerexp.op1->Infer().op1->Copy());
        if(_right)_right->PassToLeaves(innerexp.op1->Infer().op1->Copy());
        break;
    }
    case VAR_EXP: //variables inside a not expression do nothing
    {
        break;
    }
    default:
    {
        cout << "ERROR: void ProofNode::NotInfer(ProofNode& node,shared_ptr<BoolExp> value): default inference case" << endl;
        exit(EXIT_FAILURE);
    }
    }
}
void ProofNode::PassToLeaves(shared_ptr<BoolExp> expr)
{
    if (_left || _right)
    {
        if(_left)_left->PassToLeaves(expr);
        if(_right)_right->PassToLeaves(expr);
    }
    else
    {
        AddPremise(expr);
    }
}
void ProofNode::PassToLeavesSplit(BoolReturn& inference)
{
    if (_left)_left->PassToLeavesSplit(inference);
    if (_right)_right->PassToLeavesSplit(inference);
    if((!_left)&&(!_right))
    {
        _left.reset(new ProofNode);
        _right.reset(new ProofNode);
        _left->AddPremise(inference.op1->Copy());
        _right->AddPremise(inference.op2->Copy());
    }
}
void ProofNode::AddPremise(shared_ptr<BoolExp> exp)
{
    _premises.push_back(exp);
}
void ProofNode::Prove()
{
    //make any inferences if possible
    Infer(*this);
    //recurse down both branches
    if (_left) _left->Prove();
    if (_right) _right->Prove();
    //print the finished graph
}
bool ProofNode::IsCompleted()
{
    return false;
}
void ProofNode::PrintGraph(ostream& fout)
{
    //graph notation preamble
    fout << "digraph proof {" << endl;
    PrintNodes(fout);
    fout << endl;
    PrintLabels(fout);
    fout << "INFO_NODE [label=\"The conclusion is the negation of the last premise in the top node.\nXXXXX is used to denote a branch which contains a contradiction\" shape=box]" << endl;
    fout << "}" << endl;
    fout << endl;
}
void ProofNode::PrintNodes(ostream& fout)
{
    //print child connections if possible
    if (_left)
    {
        fout << "\t"+NodeID()+" -> "+_left->NodeID()+" ;" << endl;
    }

    if (_right)
    {
        fout << "\t"+NodeID()+" -> "+_right->NodeID()+" ;" << endl;
    }

    //recurse to lower nodes
    if (_left) this->_left->PrintNodes(fout);
    if (_right) this->_right->PrintNodes(fout);
}
void ProofNode::PrintLabels(ostream& fout)
{
    fout << NodeID() << " [label=" << NodeName() << "];" << endl;
    if (_left) this->_left->PrintLabels(fout);
    if (_right) this->_right->PrintLabels(fout);
}
string ProofNode::NodeID()
{
    return _nodeID;
}
string ProofNode::NodeName()
{
    string accum = "\"";
    if (_left==nullptr&&_right==nullptr&&_premises.size()==0) return (accum="XXXXX");
    for(auto const& value: _premises)
    {
        accum += value->Name();
        accum +="\\n";
    }
    accum +="\"";
    return accum;
}

void ProofNode::BlockOffBranches()
{
    //cout << "DEBUG:BlockOffBranches()" << endl;
    vector<string> formulasSoFar;
    //bool contra = false;
    //for every premise in the current node
    for(auto const& value: _premises)
    {
        formulasSoFar.push_back(value->Name());
        //cout << "DEBUG:" << value->Name() << endl;
    }

    //if we've reached a leaf begin checking formulas
    if (_left!=nullptr)
    {
        //cout << "DEBUG:recurse left" << endl;
        _left->BlockOffBranches(formulasSoFar);
    }
    if (_right!=nullptr)
    {
        //cout << "DEBUG:recurse right" << endl;
        _right->BlockOffBranches(formulasSoFar);
    }
    if (_left==nullptr&&_right==nullptr)
    {
        CheckForBlock(formulasSoFar);
    }
}
void ProofNode::BlockOffBranches(vector<string> formulasSoFar)
{
    //cout << "DEBUG:BlockOffBranches(string)" << endl;
    //bool contra = false;
    //for every premise in the current node gather it up
    for(auto const& value: _premises)
    {
        formulasSoFar.push_back(value->Name());
    }

    //if we've reached a leaf begin checking formulas
    if (_left!=nullptr)
    {
        //cout << "DEBUG:recurse left" << endl;
        _left->BlockOffBranches(formulasSoFar);
    }
    if (_right!=nullptr)
    {
        //cout << "DEBUG:recurse right" << endl;
        _right->BlockOffBranches(formulasSoFar);
    }
    if (_left==nullptr&&_right==nullptr)
    {
        CheckForBlock(formulasSoFar);
    }
}
void ProofNode::CheckForBlock(vector<string> forms)
{
    //compare every string
    for(auto const& str1: forms)
    {
        //to every other string
        for(auto const& str2: forms)
        {
            //if one is the negation of the other add a blocking node
            if (IsNegation(str1,str2))
            {
                _left.reset(new ProofNode);
                //cout << "DEBUG:CONTRADICTION!" << endl;
            }
        }
    }
}
bool ProofNode::IsNegation(string str1, string str2)
{
    return (str2=="~"+str1||str2=="~("+str1+")");
}
