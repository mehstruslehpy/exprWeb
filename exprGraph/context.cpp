#include "context.h"
#include "varexp.h"

bool Context::Lookup(string str) const
{
    auto i = _map.find(string(str));

    if (i == _map.end())
    {
        cout << "Query for unregistered variable" << endl;
        abort();
    }

    return i->second;

}

void Context::Assign(shared_ptr<VarExp> var, bool trf)	//trf like true false
{
    _map.insert(std::pair<string,bool>(string(var->_name),trf));
}
void Context::DefaultAssign(string str)
{
    _map.insert(std::pair<string,bool>(str,true)); //default is true
}
void Context::AssignValues()
{
    bool trf = false;

    for (std::map<string,bool>::iterator it=_map.begin(); it!=_map.end(); ++it)
    {
        cout << it->first << " = :" <<endl;
        cin >> trf;
        cin.ignore(256,'\n');
        it->second = trf;
        cout << "You chose: " << it->first << " = " << trf << endl;
    }
}
void Context::AssignCombination(vector<bool>& combination)
{
    int i = 0;

    for (std::map<string,bool>::iterator it=_map.begin(); it!=_map.end(); ++it,++i)
    {
        it->second = combination[i];
    }
}
int Context::VarCount()
{
    int i = 0;

    for (std::map<string,bool>::iterator it=_map.begin(); it!=_map.end(); ++it)
    {
        ++i;
    }

    return i;
}

void Context::DumpContext()
{
    for (std::map<string,bool>::iterator it=_map.begin(); it!=_map.end(); ++it)
    {
        cout << it->first << "=" << it->second << endl;
    }
}
void Context::AssignByStr(string str, bool trf)
{
    for (std::map<string,bool>::iterator it=_map.begin(); it!=_map.end(); ++it)
    {
        if (str == it->first)
        {
            it->second=trf;
            cout << "You chose: " << it->first << " = " << trf << endl;
        }
    }
}
