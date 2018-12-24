#include "prover.h"

//construct the prover with sane defaults
Prover::Prover()
    :_premcount(0), _highestasm(1)
{}

//need to fill this in to clean up properly
Prover::~Prover()
{}

//return the index of the negation of the input string in _premi[i]
int Prover::MatchNegation(string input)
{
    int ret = 0;
    string negate = "~"+input;

    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (negate == _premi[i]->Name()) {
            ret = i;
        }
    }
    return ret;
}

//return the index of the negation of the input string in _premi[i]
//only if it is not contradicted/blocked
int Prover::MatchNegationUB(string input)
{
    int ret = 0;
    string negate = "~"+input;

    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (negate == _premi[i]->Name() && _assum[i] != -1) {
            ret = i;
        }
    }
    return ret;
}

//find the index of the input string
int Prover::MatchString(string input)
{
    int ret = 0;

    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (input == _premi[i]->Name()) {
            ret = i;
        }
    }
    return ret;
}

//find the index of the input string
//only if it is not contradicted/blocked
int Prover::MatchStringUB(string input)
{
    int ret = 0;

    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (input == _premi[i]->Name() && _assum[i] != -1) {
            ret = i;
        }
    }
    return ret;
}

//setup functions
//add a premise/proposition/inference to the proof
void Prover::AddPremise(shared_ptr<BoolExp> exp, PropType p, string reason)
{
    _blocked.push_back(0);	//every premise begins unblocked and uncontradicted
    _assum.push_back(_highestasm);	//record the highest assumption at the time of creation
    BoolReturn inf = exp->Infer(); //we need to check what this expression contains

    //all VAR_EXP and NOT_EXP containing VAR_EXP's are starred by default
    //starred expressions have nothing useful that can be inferred
    //stars are printed next to each line number
    if (p == VAR_EXP || (p == NOT_EXP && inf.op1->Type() == VAR_EXP))
    {
        _starred.push_back(1);
    }
    else
    {
        _starred.push_back(0);	//everything else is unstarred
    }

    ++_premcount;	//increment the count of premises/propositions
    _premi.push_back(exp);	//add the premise/proposition
    _reasons.push_back(reason);	//log the reason/rule justifying it
    _ptypes.push_back(p);	//log the type
}

//apply an inference rule if possible to the formula at index i
//there are special rules for different kinds of NOT_EXP's hence why the NOT_EXP
//branch has a further nested switch statement
void Prover::Infer(int i)
{
    //do not infer anything from blocked/contradicted expressions
    if (_blocked[i]) return;

    switch(_ptypes[i])
    {
    case OR_EXP:
    {
        OrInference(i);
        break;
    }
    case VAR_EXP:
        break;
    case AND_EXP:
    {
        AndInference(i);
        break;
    }
    case NOT_EXP:
    {
        //we need to switch on the type contained in the not expression
        BoolReturn inf = _premi[i]->Infer();

        switch(inf.op1->Type())
        {
        case NOT_EXP:
        {
            NotNotInference(i);
            break;
        }
        case COND_EXP:
        {
            NotCondInference(i);
            break;
        }
        case OR_EXP:
        {
            NotOrInference(i);
            break;
        }
        case AND_EXP:
        {
            NotAndInference(i);
            break;
        }
        default:
        {
            break;
        }
        }
        break;
    }
    case COND_EXP:
    {
        CondInference(i);
        break;
    }
    default:
    {
        break;
    }
    }
}

//if possible make an assumption
bool Prover::MakeAssumption()
{
    bool ret = false;

    //find only one assumption if possible
    for (int i = 0; i < _premcount && !ret; i++)
    {
        if (NotAndAsm(i)||OrAsm(i)||CondAsm(i))
        {
            ret = true;
        }
    }

    return ret;
}

//search for any contradictions
//notify the user if one is found
//block off any contradictions at the proper assumption levels
bool Prover::FindContradiction()
{
    //use match negation, check each formula for its own negation
    bool ret = false;
    int contraa = -1;
    int contrab = -1;

    //check for a contradiction and notify the user
    for (unsigned int i = 0; i < _premi.size(); i++)
    {
        if (MatchNegationUB(_premi[i]->Name())&&_assum[i]!=-1)
        {
            cout << "Negation of line " << i << " found!" << endl;
            cout << "line " << i << " contradicts line " << MatchNegationUB(_premi[i]->Name()) << endl;
            ret = true;
            //whenever a contradiction is found a whole set of assumptions is invalidated
            _highestasm--;
            contraa = i;
            contrab = MatchNegationUB(_premi[i]->Name());
            break;
        }
    }

    //add the new premise by RAA and block off all lines at the previous assumption level
    if (ret == true)
    {
        int k = 0;

        //find the premise to negate
        while(_assum[k]!=_highestasm+1) k++;
        shared_ptr<BoolExp> tmp(new NotExp(_premi[k]->Copy()));

        //add it to the proof if possible
        if(_highestasm) AddPremise(tmp,tmp->Type(), "by contradiction "+to_string(contraa)+", "+to_string(contrab));
        //else delete tmp;

        //block off the relevant propositions
        for (unsigned int i = 0; i < _premi.size(); i++)
        {
            if (_assum[i] ==_highestasm+1||_blocked[i])
            {
                _blocked[i]++;
                _assum[i] = -1;
            }
        }
    }
    return ret;
}

//pretty print the list of premises and reasons this will mind the current starring
//and level of blocking
void Prover::PrintPremises() const
{
    cout << "PREMISES: " << endl;
    int j = 0;

    for (auto i = _premi.begin(); i != _premi.end(); ++i, ++j)
    {
        if (_starred[j])
        {
            cout << "* ";
        }
        else
        {
            cout << "  ";
        }

        cout << j << ") ";

        if(_blocked[j] > 0)
        {
            for (int k = _blocked[j]; k != 0; k--)
            {
                cout << "[";
            }
        }

        cout << " " << _premi[j]->Name();
        cout << "\t{ " + _reasons[j] + " }" << endl;
    }
}

//some useful methods
string Prover::PremiseName(int i) const
{
    return _premi[i]->Name();
}
bool Prover::Evaluate(int i, Context& context) const
{
    return _premi[i]->Evaluate(context);
}
int Prover::PremiseCount() const
{
    return _premcount;
}
int Prover::HighestAssumption() const
{
    return _highestasm;
}

//inference rules
void Prover::OrInference(int i)
{
    //RULE: (AvB), ~A |- B
    BoolReturn inf = _premi[i]->Infer();

    //search for the negation of the first operand
    if (MatchNegationUB(inf.op1->Name()))
    {
        //if not already in the proof add the second operand
        if (!MatchStringUB(inf.op2->Name()))
        {
            _starred[i] = 1;
            AddPremise(inf.op2->Copy(),inf.op2->Type(),
                       "(AvB), ~A |- B on "+to_string(i)+", "+to_string(MatchNegationUB(inf.op1->Name())));
        }
    }
    //again but with the second operand
    if (MatchNegationUB(inf.op2->Name()))
    {
        if (!MatchStringUB(inf.op1->Name()))
        {
            _starred[i] = 1;
            AddPremise(inf.op1->Copy(),inf.op1->Type(),
                       "(AvB), ~A |- B on " + to_string(i) + ", "+to_string(MatchNegationUB(inf.op2->Name())));
        }
    }

}
void Prover::AndInference(int i)
{
    //RULE: (A&B) |- A, B
    BoolReturn inf = _premi[i]->Infer();

    //add the first operand if it is not already present in the proof
    if (!MatchStringUB(inf.op1->Name()))
    {
        _starred[i] = 1;
        AddPremise(inf.op1->Copy(),inf.op1->Type(),
                   "(A&B) |- A, B on "+to_string(i));
    }
    if (!MatchStringUB(inf.op2->Name()))
    {
        _starred[i] = 1;
        AddPremise(inf.op2->Copy(),inf.op2->Type(),
                   "(A&B) |- A, B on "+to_string(i));
    }

}
void Prover::CondInference(int i)
{
    //RULE: (A->B), A |- B
    BoolReturn inf = _premi[i]->Infer();

    //check if the first operand already exists in the proof
    if (MatchStringUB(inf.op1->Name()))
    {
        //check if the second operand already exists in the proof
        //if not add it
        if (!MatchStringUB(inf.op2->Name()))
        {
            _starred[i] = 1;
            AddPremise(inf.op2->Copy(),inf.op2->Type(),
                       "(A->B), A |- B on " +to_string(i)+", "+to_string(MatchStringUB(inf.op1->Name())));
        }
    }

    //RULE: (A->B), ~B |- ~A
    //check if the negation of the secon operand already exists in the proof
    if (MatchNegationUB(inf.op2->Name()))
    {
        //check if the negation of the first operand already exists in the proof
        //if not add it
        if (!MatchNegationUB(inf.op1->Name()))
        {
            shared_ptr<BoolExp> tmp(new NotExp(inf.op1->Copy()));
            _starred[i] = 1;
            AddPremise(tmp,tmp->Type(),
                       "(A->B), ~B |- ~A on "+to_string(i)+", "+to_string(MatchNegationUB(inf.op2->Name())));
        }
    }

}

void Prover::NotNotInference(int i)
{
    //RULE: ~~A |- A
    BoolReturn inf = _premi[i]->Infer();
    inf = inf.op1->Infer();

    //if the unnegated proposition does not already exist in the proof add it
    if (!MatchStringUB(inf.op1->Name()))
    {
        _starred[i] = 1;
        AddPremise(inf.op1->Copy(),inf.op1->Type(),
                   "~~A |- A on " + to_string(i));
    }
}
void Prover::NotOrInference(int i)
{
    //~(AvB) |- ~A, ~B
    BoolReturn inf = _premi[i]->Infer();
    inf = inf.op1->Infer();

    //check if both negated operands already exist in the proof
    //if they do not then add them
    if (!MatchNegationUB(inf.op1->Name()))
    {
        _starred[i] = 1;
        shared_ptr<BoolExp> tmp(new NotExp(inf.op1->Copy()));
        AddPremise(tmp,tmp->Type(),
                   "~(AvB) |- ~A, ~B on " +to_string(i));
    }

    if (!MatchNegationUB(inf.op2->Name()))
    {
        _starred[i] = 1;
        shared_ptr<BoolExp> tmp(new NotExp(inf.op2->Copy()));
        AddPremise(tmp,tmp->Type(),
                   "~(AvB) |- ~A, ~B on " +to_string(i));
    }

}
void Prover::NotAndInference(int i)
{
    //~(A&B), A |- ~B
    BoolReturn inf = _premi[i]->Infer();
    inf = inf.op1->Infer();

    //check if the first operand exists in the proof
    if (MatchStringUB(inf.op1->Name()))
    {
        //check if the negation of the second operand exists in the proof
        //if not add it to the proof
        if (!MatchNegationUB(inf.op2->Name()))
        {
            shared_ptr<BoolExp> tmp(new NotExp(inf.op2->Copy()));
            _starred[i] = 1;
            AddPremise(tmp,tmp->Type(),
                       "~(A&B), A |- ~B on "+to_string(i)+", "+to_string(MatchStringUB(inf.op1->Name())));
        }
    }
    //same idea but operand 2
    if (MatchStringUB(inf.op2->Name()))
    {
        if (!MatchNegationUB(inf.op1->Name()))
        {
            shared_ptr<BoolExp> tmp(new NotExp(inf.op1->Copy()));
            _starred[i] = 1;
            AddPremise(tmp,tmp->Type(),
                       "~(A&B), A |- ~B on " + to_string(i) +", "+to_string(MatchStringUB(inf.op2->Name())));

        }
    }
}
void Prover::NotCondInference(int i)
{
    //~(A->B) |- A, ~B
    BoolReturn inf = _premi[i]->Infer();
    inf = inf.op1->Infer();

    //check to see if the propositions already exist before adding them
    if (!MatchStringUB(inf.op1->Name())||!MatchNegationUB(inf.op2->Name()))
    {
        shared_ptr<BoolExp> tmp(new NotExp(inf.op2->Copy()));
        _starred[i] = 1;
        AddPremise(inf.op1->Copy(),inf.op1->Type(),
                   "~(A->B) |- A, ~B on "+to_string(i));
        AddPremise(tmp,tmp->Type(),
                   "~(A->B) |- A, ~B on "+to_string(i));
    }

}

//methods for making assumptions
bool Prover::NotAndAsm(int i)
{
    bool ret = false;
    BoolReturn inf = _premi[0]->Infer();
    inf = _premi[i]->Infer();

    if(_premi[i]->Type() == NOT_EXP && _premi[i]->Infer().op1->Type() == AND_EXP && //if a nand expression is found
            (!MatchStringUB( _premi[i]->Infer().op1->Infer().op1->Name()  )			//and the lhs&rhs are not already present
             &&!MatchStringUB( _premi[i]->Infer().op1->Infer().op2->Name()  )))
    {
        if (!MatchStringUB(inf.op1->Infer().op1->Name())			//if the lhs is not present
                &&!MatchNegationUB(inf.op1->Infer().op2->Name())	//and the negation of the lhs and rhs
                &&!MatchNegationUB(inf.op1->Infer().op1->Name()))	//are not already present
        {
            ++_highestasm;											//increment the highest assumption count
            AddPremise(inf.op1->Infer().op1->Copy(),				//add the lhs as an assumption
                       inf.op1->Infer().op1->Type(),
                       "negated and assumption to break "+ to_string(i));
            ret = true;
        }

        if (!MatchStringUB(inf.op1->Infer().op2->Name())			//if the rhs is not present
                &&!MatchNegationUB(inf.op1->Infer().op1->Name())	//and the negation of the rhs and lhs
                &&!MatchNegationUB(inf.op1->Infer().op2->Name()) )	//are not already present
        {
            ++_highestasm;											//increment the assumption count
            AddPremise(inf.op1->Infer().op2->Copy(),				//add the rhs as an assumption
                       inf.op1->Infer().op2->Type(),
                       "negated and assumption to break "+ to_string(i));
            ret = true;
        }

    }
    return ret;
}
bool Prover::OrAsm(int i)
{
    bool ret = false;
    BoolReturn inf = _premi[0]->Infer();
    inf = _premi[i]->Infer();

    //if _premi[i] is an OR_EXP and the negation of either operands are not already present
    if(_premi[i]->Type() == OR_EXP &&
            (!MatchNegationUB(inf.op1->Name())&&!MatchNegationUB(inf.op2->Name())))
    {
        if (!MatchStringUB(inf.op1->Name()))	//add the lhs if it is not already present
        {
            ++_highestasm;
            AddPremise(inf.op1->Copy(),inf.op1->Type(), "or assumption to break " + to_string(i));
            ret = true;
        }

        if (!MatchStringUB(inf.op2->Name()))	//add the rhs if it is not already present
        {
            ++_highestasm;
            AddPremise(inf.op2->Copy(),inf.op2->Type(), "or assumption to break " + to_string(i));
            ret = true;
        }
    }

    return ret;
}
bool Prover::CondAsm(int i)
{
    bool ret = false;
    BoolReturn inf = _premi[0]->Infer();
    inf = _premi[i]->Infer();

    //if _premi[i] is a conditional and
    //either the lhs is not present or the negation of the rhs is not present
    if(_premi[i]->Type() == COND_EXP
            &&!MatchStringUB(inf.op1->Name())
            &&!MatchNegationUB(inf.op2->Name()))
    {
        if (!MatchStringUB(inf.op1->Name()))
        {
            ++_highestasm;
            AddPremise(inf.op1->Copy(),inf.op1->Type(), "conditional assumption to break " + to_string(i));
            ret = true;
        }
    }
    return ret;
}

void Prover::AssignByFormula(Context& context)
{
    vector<string> assigned;
    vector<string> unassigned;
    bool trf = false;

    //for every formula in the proof
    for (int i = 0; i < _premcount; i++ )
    {
        //get a variable letter from the current formula
        Traverse(unassigned,_premi[i]);

        //assign to every unassigned variable that was found
        for (unsigned int i = 0; i < unassigned.size(); i++)
        {
            if (find(assigned.begin(),assigned.end(),unassigned[i]) == assigned.end())
            {
                cout << unassigned[i] << " = " << endl;
                cin >> trf;
                cin.ignore(256,'\n');
                context.AssignByStr(unassigned[i],trf);
                assigned.push_back(unassigned[i]);
            }
        }

        //clear the unassigned vector in prep for the next line
        unassigned.clear();
    }
}
void Prover::Traverse(vector<string>& vec, shared_ptr<BoolExp> boolexp)
{
    if (boolexp->Type() != VAR_EXP)
    {
        //cout << "debug boop"<<endl;
        if (boolexp->Infer().op1->Type() == VAR_EXP)
        {
            vec.push_back(boolexp->Infer().op1->Name());
        }
        else
        {
            Traverse(vec,boolexp->Infer().op1);
        }

        if (boolexp->Infer().op2->Type() == VAR_EXP)
        {
            vec.push_back(boolexp->Infer().op2->Name());
        }
        else
        {
            Traverse(vec,boolexp->Infer().op2);
        }
    }
}
