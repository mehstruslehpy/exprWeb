#ifndef _VAREXP_H_
#define _VAREXP_H_
#include "boolexp.h"
#include "context.h"
#include <string.h>	//cstring functions
using namespace std;

/**VarExp are just names they can be thought of as terminals in the language
*of propositional formulas
*/
class VarExp : public BoolExp
{
    /**Context is used to map names to values*/
    friend Context;
    /**the name of the VarExp*/
    string _name;
public:
    /**VarExps can be constructed via passing the intended name*/
    VarExp (string);
    virtual ~VarExp ( );

    /** Recursively retrieves and returns a string representation of the VarExp
    *   @return returns the name of this VarExp as a string
    */
    virtual string Name() const;

    /**Lookups the name of this VarExp in context and returns its value
    *	@param con the context for name lookup to be used on
    *   @return returns the evaluation of the BoolExp as a boolean
    */
    virtual bool Evaluate(Context& con);
    /** Left in for possible future usage see BoolExp explanation
    *   @param name the name to use for replacement
    *   @param exp the expression to use for replacement
    *   @return returns the new BoolExp
    */
    virtual shared_ptr<BoolExp> Replace(string, BoolExp&);
    /** Create a copy of this VarExp
    *   @return returns the copy of this VarExp
    */
    virtual shared_ptr<BoolExp> Copy() const;
    /**returns a BoolReturn containing two of the same VarExp's
    *	@return returns a BoolReturn with operands as copies of this VarExp
    */
    virtual BoolReturn Infer();
};
#endif
