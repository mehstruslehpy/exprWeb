#include <iostream>
#include <fstream>

using namespace std;

/**read a formula from the user into a string
*	@param	the input stream to read from (probably not necessary)
*	@param	the string to read into
*	@return currently returns 0 always but may return something more useful in the future
*/
int readFormula(istream& instrm,string& str);
/**recursively parse a formula and emit the correct sequence of characters to a file
*	@param fout the file stream to emit to
*	@param the string to parse
*	@param the start position in the string to parse from (possibly unnecessary)
*	@param the ending position in the string to parse from (possibly unnecessary)
*	@return currently returns 1 always but may return something more useful in the future
*/
int parseFormula(ofstream& fout,const string& str, int start, int stop);
/**ask the user if the previously entered formula was the last
*	@return true if the user choose so returns false if not
*/
bool isLastFormula();
/**given a string and the index of an open paren return the index of it's matching paren
*	@param str the string being analyzed
*	@param start the opening paren to be matched
*	@return returns the index of the required closing paren or -1 if it cannot find a closing paren
*/
int matchEndParens(const string& str,int start);
/**emit the heredoc preamble code to a file
*	@param fout the ofstream to output to
*/
void hereDocPreamble(ofstream& fout);
/**remove space characters from the input string
*	@param str the string having whitespace removed
*/
void removeWhiteSpace(string& str);

/**check if the given string is a single propostional variable
*	@param str the string to be analyzed
*	@return returns 1 if the input string is a variable and -1 if not
*/
int isVariable(const string& str);
/**check if the given string is a conditional
*	@param str the string to be analyzed
*	@return returns 1 if the input string is a conditional and -1 if not
*/
int isConditional(const string&);
/**check if the given string is an and
*	@param str the string to be analyzed
*	@return returns 1 if the input string is an and returns -1 if not
*/
int isAnd(const string&);
/**check if the given string is an or
*	@param str the string to be analyzed
*	@return returns 1 if the input string is an or returns -1 if not
*/
int isOr(const string&);
/**check if the given string is a not
*	@param str the string to be analyzed
*	@return returns 1 if the input string is a not and -1 if not
*/
int isNot(const string&);

/**emit the correct character sequence for a two character operator
*	@param fout the stream to output to
*	@param str the string being analyzed
*	@param start the starting point in the string to work on
*	@param oploc the location of the first character of the operator in the expression
*	@param op the operator that will be emitted
*/
void twoplaceemit(ofstream& fout,const string& str,int start,int oploc,const string& op);
/**emit the correct character sequence for a one character operator
*	@param fout the stream to output to
*	@param str the string being analyzed
*	@param start the starting point in the string to work on
*	@param oploc the location of the operator in the expression
*	@param op the operator that will be emitted
*/
void oneplaceemit(ofstream& fout,const string& str,int start,int oploc,const string& op);

int main()
{
    bool last = false;
    ofstream outfile;
    string input = "";
    int i = 0;

    //open the output file
    outfile.open("output_heredoc.txt");

    //emit generic preamble code to here doc
    hereDocPreamble(outfile);

    //read parse and emit every premise
    //ask the user if they want to go to the conclusion each
    //iteration
    while (!last)
    {
        cout << "Please Enter a formula:" << endl;
        readFormula(cin,input);
        removeWhiteSpace(input);
        parseFormula(outfile,input,0,input.size());
        last = isLastFormula();
        //the menu uses different ending characters for premises and conclusions
        if (last)
        {
            cout << "FILE:\tq #conclusion follows" << endl;
            outfile << "\tq #conclusion follows" << endl << endl;
        }
        else
        {
            cout << "FILE:\tc #proposition " << i << endl;
            outfile << "\tc #proposition " << i << endl << endl;
        }
        i++;
    }

    //read in the conclusion parse and emit it to the file
    cout << "Please Enter a conclusion:" << endl;
    readFormula(cin,input);
    removeWhiteSpace(input);
    parseFormula(outfile,input,0,input.size());

    //generic ending code for the file
    cout << "FILE:ENDOFMESSAGE" << endl;
    outfile << "ENDOFMESSAGE" << endl;

    //we are done close the file and exit
    outfile.close();
    return 0;
}

//returns the position of amatching end paren given a string and
//the position of a starting paren
int matchEndParens(const string& str,int start)
{
    int nesting = 0;
    int ending = -1;
    for (unsigned int i = start; i < str.size(); i++)
    {
        if ( str[i]=='(')
        {
            nesting++;
        }
        else if ( str[i]==')')
        {
            nesting--;
        }
        if (nesting==0)
        {
            ending = i;
            break;
        }
    }
    return ending;
}

//get a formula from the user
int readFormula(istream& instrm,string& str)
{
    getline(instrm,str,'\n');
    return 0;
}

//takes a formula from the user outputs correct code to the file for the outermost
//expression type and then recurses on any subexpressions
int parseFormula(ofstream& fout,const string& str, int start, int stop)
{
    int oploc = -1;

    if (isVariable(str)!=-1)
    {
        cout << "FILE:\tu" << endl;
        fout << "\tu" << endl;
        cout << "FILE:\tv" << endl;
        fout << "\tv" << endl;
        cout << "FILE:\t" << str[0] << endl;
        fout << "\t" << str[0] << endl;
        return 1;
    }
    else if ((oploc = isNot(str))!=-1)
    {
        string sub = str.substr(1,str.size()-1);
        cout << "FILE:\tu" << endl;
        fout << "\tu" << endl;
        cout << "FILE:\tn" << endl;
        fout << "\tn" << endl;
        parseFormula(fout,sub,0,0);
        return 1;
    }
    else if ((oploc = isConditional(str))!=-1)
    {
        twoplaceemit(fout,str,start,oploc,"c");
        return 1;
    }
    else if ((oploc = isOr(str))!=-1)
    {
        oneplaceemit(fout,str,start,oploc,"o");
        return 1;
    }
    else if ((oploc = isAnd(str))!=-1)
    {
        oneplaceemit(fout,str,start,oploc,"a");
        return 1;
    }
    return 1;
}

//ask the user if this is the last formula returns true or false
bool isLastFormula()
{
    bool ret = false;
    string str = "";
    do
    {
        cout << "Premise or conclusion?(c/p)" << endl;
        getline(cin,str,'\n');
    } while(str[0]!='c'&&str[0]!='p'&&str[0]!='C'&&str[0]!='P');

    if (str=="c"||str=="C")ret=true;
    else ret = false;

    return ret;
}

//every heredoc starts the same way
void hereDocPreamble(ofstream& fout)
{
    cout << "FILE: #!/bin/sh" << endl;
    fout << "#!/bin/sh" << endl;
    cout << "FILE: ../main > proveroutput.txt <<-ENDOFMESSAGE" << endl;
    fout << "../main > proveroutput.txt <<-ENDOFMESSAGE" << endl;
    fout << "\tp #this program is always prove mode" << endl;
}
//identify if the formula is a variable return 1 if true -1 if false
int isVariable(const string& str)
{
    if (isalpha(str[0]) && isupper(str[0])) return 1;
    else return -1;
}
//identify if the formula is a conditional return the position of
//the '-' part of the conditional operator if true return -1 if false
int isConditional(const string& str)
{
    int retval = -1;
    int nesting = 0;
    int start = 0;
    for (unsigned int i = start; i < str.size()&&str[0]=='('; i++)
    {
        if(i==str.size())
        {
            i=-1;
            break;
        }
        if ( str[i]=='(') nesting++;
        if ( str[i]==')'&&i!=str.size()) nesting--;
        if (i<str.size() && str[i]== '-'&& nesting==1) retval = i;
    }
    return retval;
}
//identify if the formula is an and return the position of the & operator if true
//return -1 if false
int isAnd(const string& str)
{
    int retval = -1;
    int nesting = 0;
    int start = 0;
    for (unsigned int i = start; i < str.size()&&str[0]=='('; i++)
    {
        if(i==str.size())
        {
            i=-1;
            break;
        }
        if ( str[i]=='(') nesting++;
        if ( str[i]==')'&&i!=str.size()) nesting--;
        if (i<str.size() && str[i]== '&'&&nesting==1) retval = i;
    }
    return retval;
}
//identify if the formula is an or return the position of v operator if true
//return -1 if false
int isOr(const string& str)
{
    int retval = -1;
    int nesting = 0;
    int start = 0;
    for (unsigned int i = start; i < str.size()&&str[0]=='('; i++)
    {
        if(i==str.size())
        {
            i=-1;
            break;
        }
        if ( str[i]=='(') nesting++;
        if ( str[i]==')'&&i!=str.size()) nesting--;
        if (i<str.size() && str[i]== 'v'&& nesting==1) retval = i;
    }
    return retval;

}
//identify if the formula is a not expression return 1 if true return -1 if false
int isNot(const string& str)
{
    if (str[0] == '~') return 1;
    else return -1;
}
//remove any spaces from a given string
void removeWhiteSpace(string& str)
{
    for(unsigned int i = 0; i < str.size(); i++)
    {
        if(str[i]==' ')str.erase(i,1);
    }
}
//emit and recurse to parse any children formulas this version is for two character operators
void twoplaceemit(ofstream& fout,const string& str,int start,int oploc, const string& op)
{
    cout << "FILE:\tb" << endl;
    fout << "\tb" << endl;
    cout << "FILE:\t" << op << endl;
    fout << "\t" << op << endl;
    string sub;
    int lstart = 1;
    int lend = matchEndParens(str,lstart);

    if (lstart==lend)
    {
        sub = str.substr(lstart,oploc-lstart);
    }
    else
    {
        sub = str.substr(lstart,lend-lstart+1);
    }
    parseFormula(fout,sub,0,0);

    int rstart = oploc+2;
    int rend = matchEndParens(str,rstart);

    if (rstart==rend)
    {
        sub = str.substr(oploc+2,str.size()-oploc-3);
    }
    else
    {
        sub = str.substr(rstart,rend-rstart+1);
    }
    parseFormula(fout,sub,0,0);
}
//emit and recurse to parse any children formulas this version is for one character operators
void oneplaceemit(ofstream& fout,const string& str,int start,int oploc, const string& op)
{
    cout << "FILE:\tb" << endl;
    fout << "\tb" << endl;
    cout << "FILE:\t" << op << endl;
    fout << "\t" << op << endl;
    string sub;
    int lstart = 1;
    int lend = matchEndParens(str,lstart);

    if (lstart==lend)
    {
        sub = str.substr(lstart,oploc-1);
    }
    else
    {
        sub = str.substr(lstart,lend-lstart+1);
    }
    parseFormula(fout,sub,0,0);

    int rstart = oploc+1;
    int rend = matchEndParens(str,rstart);

    if (rstart==rend)
    {
        sub = str.substr(oploc+1,str.size()-oploc-2);
    }
    else
    {
        sub = str.substr(rstart,rend-rstart+1);
    }
    parseFormula(fout,sub,0,0);
}
