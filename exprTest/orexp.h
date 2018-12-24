#ifndef _OREXP_H_
#define _OREXP_H_
#include "boolexp.h"
#include <string.h>	//cstring functions

using namespace std;

/**OrExp's are derived from BoolExp's they implement the logical proposition (AvB)*/
class OrExp : public BoolExp
{
    /** The left operand of an OrExp */
    shared_ptr<BoolExp> _operand1;
    /** The right operand of an OrExp */
    shared_ptr<BoolExp> _operand2;
public:
    /** Constructs the specified OrExp
    *   @param op1 becomes the left hand operand
    *   @param op2 becomes the right hand operand
    */
    OrExp ( shared_ptr<BoolExp>, shared_ptr<BoolExp> );

    /**The OrExp destructor*/
    virtual ~OrExp ( );
    /** Recursively retrieves and returns a string representation of the OrExp
    *   @return returns the name of this OrExp as a string
    */
    virtual string Name() const;
    /** Recursively evaluates the OrExp via (op1.Evaluate() || op2.Evaluate())
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
    /** Create a copy of this OrExp
    *   @return returns the copy of this OrExp
    */
    virtual shared_ptr<BoolExp> Copy() const;
    /** Make an inference from this OrExp
    *   @return returns a BoolReturn with both operands set to the operands of this OrExp
    */
    virtual BoolReturn Infer();
};

#endif
