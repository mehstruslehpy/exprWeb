#include "boolexp.h"
BoolReturn::BoolReturn(shared_ptr<BoolExp> o1, shared_ptr<BoolExp> o2)
    :op1(o1), op2(o2)
{}

BoolExp::BoolExp (PropType p)
    : _type(p)
{}
BoolExp::~BoolExp ( )
{}
PropType BoolExp::Type() const
{
    return _type;
}
