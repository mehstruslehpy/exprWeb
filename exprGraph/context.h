#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <iostream>
#include <map>
#include <stdlib.h>
#include <vector>
#include <memory>
using namespace std;

class VarExp; //forward decl

/**The Context class is mainly used for mapping variables to values*/
class Context
{
    /**The map the context class wraps*/
    map<string, bool> _map;
public:
    /**lookup and return the boolean value of the input string in the map
    *	@param str is the variable being looked up
    *	@return returns the value of the selected string
    */
    bool Lookup(string str) const;
    /**assign the value true to the variable referred to by str
    *	@param str is the variable being assigned to
    */
    void DefaultAssign(string str);
    /** insert the VarExp var with the bool value of trf into the map
    *	@param var the variable being inserted
    *	@param trf the value that var will receive
    */
    void Assign(shared_ptr<VarExp> var, bool trf);
    /**	Assign a value to every variable in the map via user input */
    void AssignValues();
    /** Assign a value to the selected string if it exists in the map
    *	@param str the string to be looked up and assigned to
    *	@param trf the boolean value to assign to str
    */
    void AssignByStr(string str, bool trf);
    /** assign to each variable a value from combination linearly this is useful
    *	bruteforcing counterarguments
    *	@param combination the vector of boolean values to map to variables
    */
    void AssignCombination(vector<bool>& combination);
    /**return a count of all the variables in the map
    *	@return returns a count of all the variables in the map
    */
    int VarCount();
    /**print what each variable is mapped to in the map*/
    void DumpContext();
};
#endif
