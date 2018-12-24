#ifndef _BOOLEXP_H_
#define _BOOLEXP_H_

#include <iostream>
#include <fstream>
#include <memory>
#include "context.h"

//need to fwd declare this for BoolReturn
/** A BoolExp is an abstract base class for the other expression types */
class BoolExp;

/** PropType is used so that an expression can announce it's own type and so that functions
*	can do different things for differrent kinds of expressions
*/
enum PropType
{
    /**The type for an OR expression*/
    OR_EXP,
    /**The type for a Variable expression*/
    VAR_EXP,
    /**The type for a NOT expression*/
    NOT_EXP,
    /**The type for an AND expression*/
    AND_EXP,
    /**The type for an Conditional expression*/
    COND_EXP
};

/** BoolReturn is a convenience struct for returning pointers to the members of a BoolExp this feature
*is heavily used in the prover class
*/
struct BoolReturn
{
    /** The BoolReturn constructor
    *	initializes the two shared_ptr's to point to the requested members
    *	of a chosen BoolExp
    */
    BoolReturn(shared_ptr<BoolExp>,shared_ptr<BoolExp>);
    /** The BoolReturn destructor */
    ~BoolReturn() {}
    /** op1 is used for the left hand member of a BoolExp or the only member of a unary BoolExp*/
    shared_ptr<BoolExp> op1;
    /** op2 is used for the right hand member of a BoolExp or the only member of a unary BoolExp*/
    shared_ptr<BoolExp> op2;
};

using namespace std;

/** A BoolExp is an abstract base class for derived boolean logic expression classes*/
class BoolExp
{
    /** All BoolExp's have a member referring to their type */
    PropType _type;
public:
    /** Derived classes supply the corresponding proposition types to BoolExp's via this constructor*/
    BoolExp (PropType);

    /** The destructor for a BoolExp*/
    virtual ~BoolExp ( );

    /** Returns the name of the formula by calling this method recursively on nested boolexp's
    *	every BoolExp supports this so any arbitrary tree of BoolExp's can be printed.
    *	@return returns the name of this BoolExp as a string
    */
    virtual string Name() const = 0;

    /** BoolExp's are meant to be evaluated in a recursive manner like how they are printed
    *	but implementation of this feature is determined by the derived classes themselves
    *	@param con is a class which maps variable names to values
    *	@return returns the evaluation of the BoolExp as a boolean
    */
    virtual bool Evaluate(Context& con) = 0;

    /** All BoolExp's support a replace operation which is implemented by derived classes
    *	note that I did not find this method very useful but it is left in due to possibly
    *	being useful in the future
    *	@param str receives the name of bol
    *	@param bol receives a BoolExp
    *	@return returns the new BoolExp
    */
    virtual shared_ptr<BoolExp> Replace(string str, BoolExp& bol) = 0;

    /** Create a copy of this AndExp
    *	@return returns the copy of this BoolExp
    */
    virtual shared_ptr<BoolExp> Copy() const = 0;

    /** Every BoolExp includes information pertaining to its type, every BoolExp's type is determined by its outermost type
    *	@return returns the type of the outermost formula
    */
    virtual PropType Type() const;
    /** Infer allows BoolExp's to be modified via inference rules. Generally a BoolExp
    *	Just returns its children but having this as a virtual method allows a BoolExp
    *	to self modify upon inference if needed
    *	@return returns a properly configured BoolReturn
    */
    virtual BoolReturn Infer() = 0;
};

#endif
