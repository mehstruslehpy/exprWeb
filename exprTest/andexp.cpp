#include "andexp.h"
AndExp::AndExp ( shared_ptr<BoolExp> op1, shared_ptr<BoolExp> op2)
    : BoolExp(AND_EXP)
{
    _operand1 = op1;
    _operand2 = op2;
}

AndExp::~AndExp ( )
{}

bool AndExp::Evaluate(Context& con)
{
    bool val1 = _operand1->Evaluate(con);
    bool val2 = _operand2->Evaluate(con);

    return val1 && val2;
}

shared_ptr<BoolExp> AndExp::Replace(string name, BoolExp& exp)
{
    return shared_ptr<BoolExp>(new AndExp(_operand1->Replace(name,exp),
                                          _operand2->Replace(name,exp)));
}

shared_ptr<BoolExp> AndExp::Copy() const
{
    return shared_ptr<BoolExp>(new AndExp(_operand1->Copy(),
                                          _operand2->Copy()));
}

string AndExp::Name() const
{
    string ret = "(" + _operand1->Name() + " & " + _operand2->Name() + ")";
    return ret;
}
BoolReturn AndExp::Infer()
{
    return BoolReturn(_operand1,_operand2);
}
