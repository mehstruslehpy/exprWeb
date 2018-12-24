#include "orexp.h"
OrExp::OrExp ( shared_ptr<BoolExp> op1, shared_ptr<BoolExp> op2)
    : BoolExp(OR_EXP)
{
    _operand1 = op1;
    _operand2 = op2;
}

OrExp::~OrExp ( )
{}

bool OrExp::Evaluate(Context& con)
{
    bool val1 = _operand1->Evaluate(con);
    bool val2 = _operand2->Evaluate(con);

    return val1 || val2;
}

shared_ptr<BoolExp> OrExp::Replace(string name, BoolExp& exp)
{
    return shared_ptr<BoolExp>(new OrExp(_operand1->Replace(name,exp),
                                         _operand2->Replace(name,exp)));
}

shared_ptr<BoolExp> OrExp::Copy() const
{
    return shared_ptr<BoolExp>(new OrExp(_operand1->Copy(),
                                         _operand2->Copy()));
}

string OrExp::Name() const
{
    string ret = "(" + _operand1->Name() + " v " + _operand2->Name() + ")";
    return ret;
}
BoolReturn OrExp::Infer()
{
    return BoolReturn(_operand1,_operand2);
}
