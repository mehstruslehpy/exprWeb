#ifndef _ANDEXP_H_
#define _ANDEXP_H_
#include "boolexp.h"
#include <string.h>	//cstring functions

using namespace std;

/**AndExp's are derived from BoolExp's they implement the logical proposition (A&B)*/
class AndExp : public BoolExp
{
    /** The left operand of an AndExp */
    shared_ptr<BoolExp> _operand1;
    /** The right operand of an AndExp */
    shared_ptr<BoolExp> _operand2;
public:
    /**	Constructs the specified AndExp
    *	@param op1 becomes the left hand operand
    *	@param op2 becomes the right hand operand
    */
    AndExp ( shared_ptr<BoolExp> op1, shared_ptr<BoolExp> op2);
    /**The AndExp destructor*/
    virtual ~AndExp ( );

    /** Recursively retrieves and returns a string representation of the AndExp
    *	@return returns the name of this AndExp as a string
    */
    virtual string Name() const;

    /** Recursively evaluates the AndExp via (op1.Evaluate() && op2.Evaluate())
    *	@param con is the Context class which maps variables to values
    *	@return returns the evaluation of the BoolExp as a boolean
    */
    virtual bool Evaluate(Context& con);
    /** Left in for possible future usage see BoolExp explanation
    *	@param name the name to use for replacement
    *	@param exp the expression to use for replacement
    *	@return returns the new BoolExp
    */
    virtual shared_ptr<BoolExp> Replace(string name, BoolExp& exp);
    /** Create a copy of this AndExp
    *	@return returns the copy of this AndExp
    */
    virtual shared_ptr<BoolExp> Copy() const;
    /** Make an inference from this AndExp
    *	@return returns a BoolReturn with both operands set to the operands of this AndExp
    */
    virtual BoolReturn Infer();
};

#endif
