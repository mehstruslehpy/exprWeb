#ifndef _NOTEXP_H_
#define _NOTEXP_H_
#include "boolexp.h"
#include <string.h>

using namespace std;

/**NotExp's are derived from BoolExp's they implement the logical proposition ~A
*note that a NotExp can contain any other kind of BoolExp ex: ~(A&(B->(CvD))) is a
*valid NotExp
*/
class NotExp : public BoolExp
{
    /** NotExp's only contain one operand*/
    shared_ptr<BoolExp> _operand;
public:
    /** Constructs the specified NotExp
    *   @param op becomes the operand for this NotExp
    */
    NotExp ( shared_ptr<BoolExp> op);
    /**The NotExp destructor*/
    virtual ~NotExp ( );

    /** Recursively retrieves and returns a string representation of the NotExp
    *   @return returns the name of this NotExp as a string
    */
    virtual string Name() const;

    /** Recursively evaluates the NotExp via !op1.Evaluate()
    *   @param con is the Context class which maps variables to values
    *   @return returns the evaluation of the BoolExp as a boolean
    */
    virtual bool Evaluate(Context& con);
    /** Left in for possible future usage see BoolExp explanation
    *   @param name the name to use for replacement
    *   @param exp the expression to use for replacement
    *   @return returns the new BoolExp
    */
    virtual shared_ptr<BoolExp> Replace(string, BoolExp&);
    /** Create a copy of this NotExp
    *   @return returns the copy of this NotExp
    */
    virtual shared_ptr<BoolExp> Copy() const;
    /** Make an inference from this NotExp
    *   @return returns a BoolReturn with both operands set to the operands of this NotExp
    */
    virtual BoolReturn Infer();
};

#endif
