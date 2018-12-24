#include "defs.h"
#include <math.h>
#include <algorithm>

using namespace std;

/**builds a well formed formula based on user input via an interactive menu
*	@param current the current BoolExp being created
*	@param the context mapping variables to values
*	@return returns a shared_ptr<BoolExp> referring to the created formula
*/
shared_ptr<BoolExp> buildExpr(shared_ptr<BoolExp> current,Context& context);
/**attempt to bruteforce a counterargument by testing every possible permutation
*	@param myvec a vector of bools containing one element for every formula
*	@param proof the proof
*	@param context the Context structure mapping values to variables for this proof
*	@param premises the count of premises
*	@param conclusions the index of the conclusion in the proof
*	@return returns true if a counterarg is found otherwise false
*/
bool testEveryPermutation(vector<bool>& myvec,Prover& proof,Context& context,int premises,int conclusion);
/**the program is running in proof mode the user will input arguments to be proved or disproved
*	@return indiscriminately returns 0 but may return something more useful in the future
*/
int prover();
/**the program is running in calculator mode the user will input formulas and values to be calculated
*	@return indiscriminately returns 0 but may return something more useful in the future
*/
int calculator();
/**initiates either calculator or prover mode based on user input*/
int main()
{
    string choice = "";
    //cout << "Calculate or Prove?(c/p):" << endl;
    cin >> choice;
    cin.ignore(256,'\n');

    if (choice == "c") {
        calculator();
    }

    if (choice == "p") {
        prover();
    }

    return 0;
}

int prover()
{
    string input = ""; //for user input
    Context context; //the Context class maps variables to boolean values
    shared_ptr<BoolExp> current = nullptr; //a specific proposition
    shared_ptr<BoolExp> conclusion = nullptr; //another specific proposition this time a conclusion
    Prover proof; //the actual proof class
    int premisecount = 0; //the count of premises/propositions in the proof
    int conclusionindex = 0; //what index the users conclusion is at
    bool counterarg = false; //true if the program finds a counter argument

    //build the user premises
    //cout << "Building argument enter q to end:" << endl;
    while (input != "q")
    {
        current = buildExpr(current,context);
        proof.AddPremise(current, current->Type(), "given");
        //proof.PrintPremises();
        //cout << endl;
        //cout << "Building argument enter q to end or c to continue:" << endl;
        cin >> input;
        cin.ignore(256,'\n');
        premisecount++;
        conclusionindex++;
    }

    //display the current proof to the user
    //proof.PrintPremises();

    //build a conclusion and display the current proof
    //cout << "Build your conclusion" << endl;
    conclusion = buildExpr(current,context);
    current = shared_ptr<BoolExp>(new NotExp(conclusion));
    proof.AddPremise(current, current->Type(), "~ of the conclusion");
    //proof.PrintPremises();
    //cout << endl;

    //begin trying to prove or disprove the argument
    input = "";
    //cout << "Beginning inference step enter q to quit:" << endl;
    for (int i = 0; input != "q"; i++)
    {
        //cout << "Iteration: " << i << endl;
        proof.FindContradiction();
        //cout << "Continue inferences or quit?(c/q)" << endl;
        cin >> input;
        cin.ignore(256,'\n');

        //every 50 iterations but not the very first one the proof is
        //allowed to try and make an assumption
        if (!(i%50)&&i!=0) {
            proof.MakeAssumption();
        }

        //i%proof.PremiseCount() ensures we never index into an invalid line
        proof.Infer(i%proof.PremiseCount());
        //print status info to the user
        //cout << "Assumptions: " << proof.HighestAssumption() << endl;
        //cout << "Premises: "<< proof.PremiseCount() << endl;

        //check if a contradiction exists
        proof.FindContradiction();

        //every 100 iterations but not the first the program is allowed
        //to try and find a counterargument
        if(!(i%100)&&i!=0)
        {
            vector<bool> truthvec(context.VarCount(),false);
            counterarg = testEveryPermutation(truthvec,
                                              proof,
                                              context,
                                              premisecount,
                                              conclusionindex);
        }

        //if no assumptions are left then the starting premises have
        //been contradicted and the proof is completed the argument is
        //valid by RAA
        //if a counterargument is found the argument is invalid and the
        //the proof is complete
        if (proof.HighestAssumption()==0||counterarg) {
            break;
        }
    }

    //the output differs based on validity
    if(!counterarg)
    {
        proof.PrintPremises();
        cout << "Therefore " << conclusion->Name() << " is a valid conclusion." << endl;
        cout << "Program Ending" << endl;
    }
    else
    {
        cout << "The conclusion is invalid, because the above counterargument can make all"
             << " the premises true \nand the negation of the conclusion true too, which makes"
             << " the intended conclusion false."
             << endl;
    }

    return 0;
}

int calculator()
{
    //similar variables to the prover
    string input = "";
    Context context;
    shared_ptr<BoolExp> current = nullptr;
    Prover proof;

    //read all the formulas the user wants to evaluate
    cout << "Building formulas to evaluate enter q to end:" << endl;
    while (input != "q")
    {
        current = buildExpr(current,context);
        proof.AddPremise(current, current->Type(), "given");
        proof.PrintPremises();
        cout << endl;
        cout << "Building formulas to evaluate enter q to end or c to continue:" << endl;
        cin >> input;
        cin.ignore(256,'\n');
    }

    //print them so the user knows what they are evaluating
    proof.PrintPremises();

    //Assign values to variables based on user input
    cout << "Beginning variable assignment step:" << endl;
    //context.AssignValues();
    proof.AssignByFormula(context);
    cout << endl;
    cout << "Dumping context:" << endl;
    context.DumpContext();
    //evaluate each formula based on the chosen values and print
    cout << "Beginning calculation step enter q to quit:" << endl;
    for (int i = 0; i < proof.PremiseCount(); i++)
    {
        cout << proof.PremiseName(i) << " = " << proof.Evaluate(i,context) << endl;
    }

    cout << "Program Ending" << endl;
    return 0;
}

//build a wff
shared_ptr<BoolExp> buildExpr(shared_ptr<BoolExp> current, Context& context)
{
    string input = ""; //for user input

    //the only operators supported are unary or binary so ask which one the user wants
    do
    {
        //cout << "Unary or Binary?(u/b): " << endl;
        cin >> input;
        cin.ignore(256,'\n');
    }
    while (input != "u" && input != "b");

    //if they want a unary operator it can be either a variable or
    //it can be a negation operator
    if(input == "u")
    {
        do
        {
            //cout << "Variable or Negation?(v/n)" << endl;
            cin >> input;
            cin.ignore(256,'\n');
        }
        while (input != "v" && input != "n");

        //if they want a variable we need to know its name
        if (input == "v")
        {
            //cout << "Enter a variable name:" << endl;
            cin >> input;
            cin.ignore(256,'\n');
            current = shared_ptr<BoolExp>(new VarExp(input.c_str()));
            context.DefaultAssign(input.c_str());
            return current;
        }

        //if they want a negation operator then we need to recurse to
        //define its operands
        if (input == "n")
        {
            //cout << "Continuing to interior of negation: " << endl;
            shared_ptr<BoolExp> temp;
            temp = buildExpr(current,context);
            current = shared_ptr<BoolExp>(new NotExp(temp));
            return current;
        }
    }

    //binary operators can be and/or/conditionals each one requires recursing
    //to fill in the individual operands
    if (input == "b")
    {
        do
        {
            //cout << "And, Or, or Conditional?(a/o/c)" << endl;
            cin >> input;
            cin.ignore(256,'\n');
        }
        while (input != "a" && input != "o" && input != "c");

        if (input == "a")
        {
            shared_ptr<BoolExp> temp1;
            shared_ptr<BoolExp> temp2;
            //cout << "Building left side of And: " << endl;
            temp1 = buildExpr(current,context);
            //cout << "Building right side of And: " << endl;
            temp2 = buildExpr(current,context);
            current = shared_ptr<BoolExp>(new AndExp(temp1,temp2));
            return current;
        }

        if (input == "o")
        {
            shared_ptr<BoolExp> temp1;
            shared_ptr<BoolExp> temp2;
            //cout << "Building left side of Or: " << endl;
            temp1 = buildExpr(current,context);
            //cout << "Building right side of Or: " << endl;
            temp2 = buildExpr(current,context);
            current = shared_ptr<BoolExp>(new OrExp(temp1,temp2));
            return current;
        }

        if (input == "c")
        {
            shared_ptr<BoolExp> temp1;
            shared_ptr<BoolExp> temp2;
            //cout << "Building left side of Conditional: " << endl;
            temp1 = buildExpr(current,context);
            //cout << "Building right side of Conditional: " << endl;
            temp2 = buildExpr(current,context);
            current = shared_ptr<BoolExp>(new CondExp(temp1,temp2));
            return current;
        }
    }

    return nullptr;
}

//test every proposition in prover with every permutation of 1's and 0's in myvec if every proposition
//ends up true for a given permutation then we have found a counterargument
bool testEveryPermutation(vector<bool>& myvec,Prover& proof, Context& context,int premises,int conclusion)
{
    //cout << "Checking if the argument is invalid" << endl;
    bool premchk = true; //to switch on the return value
    bool concchk = true; //same
    bool counterarg = false; //the return value is false by default
    sort(myvec.begin(),myvec.end());

    //for every item in the vector start with a vector of 0's
    //fill in one more 1 than the last iteration and check every
    //permutation with the new count of 1's
    for (unsigned i=0; i<=myvec.size(); i++)
    {
        //zero the vector
        for(unsigned k=0; k<myvec.size(); k++) {
            myvec[k]=0;
        }

        //add the correct amount of 1's to the zeroized vector
        for(unsigned k=0; k<i; k++) {
            myvec[k]=1;
        }

        //sort the vector
        sort(myvec.begin(),myvec.end());

        //check each permutation
        do
        {
            context.AssignCombination(myvec);
            premchk = true;

            for(int i=0; i < premises; i++)
            {
                if (!proof.Evaluate(i,context)) {
                    premchk = false;
                }
            }

            concchk=proof.Evaluate(conclusion,context);

            //print if we find a counterargument
            if (premchk&&concchk)
            {
                cout << endl;
                cout << "Counter-argument:" << endl;
                context.DumpContext();
                cout << endl;
                cout << "Evaluation:" << endl;

                for(int i=0; i<premises+1; i++)
                {
                    cout << proof.PremiseName(i) << "="<<proof.Evaluate(i,context)<<endl;
                }

                counterarg = true;
                cout << endl;
            }
        }
        while(next_permutation(myvec.begin(),myvec.end()));
    }

    return counterarg;
}

