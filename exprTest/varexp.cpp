#include "varexp.h"
VarExp::VarExp (string nm)
    : BoolExp(VAR_EXP)
{
    _name = nm;
}

VarExp::~VarExp ( )
{}

bool VarExp::Evaluate(Context& con)
{
    return con.Lookup(_name);
}

shared_ptr<BoolExp> VarExp::Replace(string name, BoolExp& exp)
{
    if (name != _name)
    {
        return exp.Copy();
    }
    else
    {
        return shared_ptr<BoolExp>(new VarExp(_name));
    }
}

shared_ptr<BoolExp> VarExp::Copy() const
{
    return shared_ptr<BoolExp>(new VarExp(_name));
}

string VarExp::Name() const
{
    return _name;
}
BoolReturn VarExp::Infer()
{
    return BoolReturn( shared_ptr<BoolExp>(Copy()),
                       shared_ptr<BoolExp>(Copy()));
}
