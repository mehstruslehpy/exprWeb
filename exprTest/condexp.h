#ifndef _CONDEXP_H_
#define _CONDEXP_H_
#include "boolexp.h"
#include <string.h>

using namespace std;
/**CondExp's are derived from BoolExp's they implement the logical proposition (A->B)*/
class CondExp : public BoolExp
{
    /** The left operand of an CondExp */
    shared_ptr<BoolExp> _operand1;
    /** The right operand of an CondExp */
    shared_ptr<BoolExp> _operand2;
public:
    /** Constructs the specified CondExp
    *   @param op1 becomes the left hand operand
    *   @param op2 becomes the right hand operand
    */
    CondExp ( shared_ptr<BoolExp>, shared_ptr<BoolExp> );
    /**The CondExp destructor*/
    virtual ~CondExp ( );

    /** Recursively retrieves and returns a string representation of the CondExp
    *	@return returns the name of this CondExp as a string
    */
    virtual string Name() const; 	//return a copy of the name

    /** Recursively evaluates the OrExp via (!op1.Evaluate() || op2.Evaluate())
    *   @param con is the Context class which maps variables to values
    *   @return returns the evaluation of the BoolExp as a boolean
    */
    virtual bool Evaluate(Context&);
    /** Left in for possible future usage see BoolExp explanation
    *   @param name the name to use for replacement
    *   @param exp the expression to use for replacement
    *   @return returns the new BoolExp
    */
    virtual shared_ptr<BoolExp> Replace(string, BoolExp&);
    /** Create a copy of this CondExp
    *   @return returns the copy of this CondExp
    */
    virtual shared_ptr<BoolExp> Copy() const;
    /** Make an inference from this CondExp
    *   @return returns a BoolReturn with both operands set to the operands of this CondExp
    */
    virtual BoolReturn Infer();
};

#endif
