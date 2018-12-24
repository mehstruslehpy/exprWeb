#include "notexp.h"
NotExp::NotExp ( shared_ptr<BoolExp> op)
    : BoolExp(NOT_EXP)
{
    _operand = op;
}

NotExp::~NotExp ( )
{}

bool NotExp::Evaluate(Context& con)
{
    return !_operand->Evaluate(con);
}

shared_ptr<BoolExp> NotExp::Replace(string name, BoolExp& exp)
{
    return shared_ptr<BoolExp>(new NotExp(_operand->Replace(name,exp)));
}

shared_ptr<BoolExp> NotExp::Copy() const
{
    return shared_ptr<BoolExp>(new NotExp(_operand->Copy()));
}

string NotExp::Name() const
{
    return "~" + _operand->Name();
}
BoolReturn NotExp::Infer()
{
    //return BoolReturn(_operand, nullptr);
    return BoolReturn(_operand, _operand);
}

