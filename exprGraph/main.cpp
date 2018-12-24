#include "defs.h"
#include <math.h>
#include <algorithm>

using namespace std;

shared_ptr<BoolExp> buildExpr(shared_ptr<BoolExp> current);

int main()
{
    ofstream fout("OUTPUT_DOTFILE_PROOF.gv"); //the file to output dot notation to
    string input = ""; //for user input
    shared_ptr<BoolExp> current = nullptr; //a specific proposition
    shared_ptr<BoolExp> conclusion = nullptr; //another specific proposition this time a conclusion
    ProofNode proof; //the root node
    int premisecount = 0; //the count of premises/propositions in the proof
    int conclusionindex = 0; //what index the users conclusion is at
    //bool counterarg = false; //true if the program finds a counter argument

    //build the user premises
    //cout << "Building argument enter q to end:" << endl;
    while (input != "q")
    {
        current = buildExpr(current);
        proof.AddPremise(current);
        //proof.PrintGraph(cout);
        //cout << endl;
        //cout << "Building argument enter q to end or c to continue:" << endl;
        cin >> input;
        cin.ignore(256,'\n');
        premisecount++;
        conclusionindex++;
    }

    //display the current proof to the user
    //proof.PrintGraph(cout);

    //build a conclusion and display the current proof
    //cout << "Build your conclusion:" << endl;
    conclusion = buildExpr(current);
    current = shared_ptr<BoolExp>(new NotExp(conclusion));
    proof.AddPremise(current);
    //proof.PrintGraph(cout);
    //cout << endl;

    //cout << "Beginning Full Graph:" << endl;
    proof.Prove();
    proof.BlockOffBranches();
    proof.PrintGraph(cout);
    proof.PrintGraph(fout);

    fout.close();
    return EXIT_SUCCESS;
}


//build a wff
shared_ptr<BoolExp> buildExpr(shared_ptr<BoolExp> current)
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
            return current;
        }

        //if they want a negation operator then we need to recurse to
        //define its operands
        if (input == "n")
        {
            //cout << "Continuing to interior of negation: " << endl;
            shared_ptr<BoolExp> temp;
            temp = buildExpr(current);
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
            temp1 = buildExpr(current);
            //cout << "Building right side of And: " << endl;
            temp2 = buildExpr(current);
            current = shared_ptr<BoolExp>(new AndExp(temp1,temp2));
            return current;
        }

        if (input == "o")
        {
            shared_ptr<BoolExp> temp1;
            shared_ptr<BoolExp> temp2;
            //cout << "Building left side of Or: " << endl;
            temp1 = buildExpr(current);
            //cout << "Building right side of Or: " << endl;
            temp2 = buildExpr(current);
            current = shared_ptr<BoolExp>(new OrExp(temp1,temp2));
            return current;
        }

        if (input == "c")
        {
            shared_ptr<BoolExp> temp1;
            shared_ptr<BoolExp> temp2;
            //cout << "Building left side of Conditional: " << endl;
            temp1 = buildExpr(current);
            //cout << "Building right side of Conditional: " << endl;
            temp2 = buildExpr(current);
            current = shared_ptr<BoolExp>(new CondExp(temp1,temp2));
            return current;
        }
    }

    return nullptr;
}

