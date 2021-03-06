#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#include "parser.hpp"

//----------------------------

Expr::Expr(string s) { sym = s; kind = ATOM; }

Expr::Expr(vector<string> tokens, int start)
{
  unsigned int i=start,n=tokens.size();
  kind = LIST; // assume there are multiple tokens, change to ATOM later
  while (i<n)
  {
    if (tokens[i]==")") { end = i-1; return; }
    else if (tokens[i]=="(") 
    {
      Expr* part=new Expr(tokens, i+1); 
      sub.push_back(part);
      i = part->end+1; 
      if (i>=n || tokens[i]!=")") throw SyntaxError("didn't find closing paren: "+part->toString());
    }
    else sub.push_back(new Expr(tokens[i])); 
    i++; 
  }
}

// deep copy

Expr::Expr(Expr* e)
{
  kind = e->kind;
  if (e->kind==ATOM) sym = e->sym;
  else
  {
    for (unsigned int i=0 ; i<e->sub.size() ; i++)
      sub.push_back(new Expr(e->sub[i])); // deep copy
  }
}

// make a new Expr from a list (make copies of items)

Expr::Expr(vector<Expr*> L)
{
  kind = LIST;
  for (unsigned int i=0 ; i<L.size() ; i++)
    sub.push_back(new Expr(L[i])); // deep copy
}

string Expr::toString()
{
  if (kind==ATOM) return sym;
  else 
  {
    string s("(");
    unsigned int n=sub.size();
    for (unsigned int i=0 ; i<n; i++) {
      s += sub[i]->toString()+(i<n-1 ? " ": "");
    }
    s += ")";
    return s;
  }
}

vector<string> tokenize(string s)
{
  vector<string> tokens;
  unsigned int i=0,n=s.size();
  while (i<n)
  {
    if (s[i]=='(') { tokens.push_back("("); i++; }
    else if (s[i]==')') { tokens.push_back(")"); i++; }
    else if (s[i]==' ') i++;
    else {
      int j=i;
      while (j<n && s[j]!=' ' && s[j]!='(' && s[j]!=')') j++;
      tokens.push_back(string(s.substr(i,j-i)));
      i = j; }
  }
  return tokens;
}

//  takes a sentence as a string, constructs an Expr object for it, and returns a pointer to it
//  expression should have only 1 element at the top level, like "a" or "(a b)"
Expr* parse(string s)
{
  vector<string> tokens=tokenize(s);
  Expr* res=new Expr(tokens,0);
  if (res->sub.size()!=1) throw SyntaxError("expression should have only 1 element at the top level: "+s);
  return res->sub[0];
}

// overload the operator==

bool Eq(Expr* a, Expr* b)
{
  if (a->kind==ATOM) {
    if (b->kind!=ATOM || a->sym!=b->sym) return false; }
  else // a->kind==LIST
  {
    if (b->kind!=LIST || a->sub.size()!=b->sub.size()) return false;
    for (unsigned int i=0 ; i<a->sub.size() ; i++)
      if (!Eq(a->sub[i],b->sub[i])) return false;
  }
  return true;
}

//-------------------------------------

// auxiliary function for reading in a whole KB file and returning a vector or expression objects
vector<Expr*>load_kb(string fname)
{
  vector<Expr*> KB;
  ifstream input;
  input.open(fname);
  string line;
  while (getline(input,line))
  {
    if (line.size()==0 || line[0]=='#') continue;
    KB.push_back(parse(line));
  }
  return KB;
}

void show_kb(vector<Expr*>& KB, set<string>& KBclauses)
{
  for (unsigned int i=0 ; i<KB.size() ; i++)
  {
    cout << i << ". ";
    cout << KB[i]->toString() << endl;
    KBclauses.insert(KB[i]->toString());
  }
}

Expr* negate_query(Expr* query) {
  vector<string> parts = tokenize(query->toString());
  string negQuery = "(or ";

  // if query starts with a "not", can just replace this with an "or" (double negation)
  if (parts.size() > 2 && parts[2] != "not")
  {
    negQuery += parts[2] + ")";  
  }
  else
  {
    negQuery += "(not " + parts[0] + "))";
  }

  return parse(negQuery);
}

//----------------------------

SyntaxError::SyntaxError(string s) : runtime_error("Syntax Error: "+s) {} 

// for trying to apply an ROI to an expression for which it doesn???t apply (for example,
// trying to apply double-negation elimination to ???(not P)???)
RuleApplicationError::RuleApplicationError(string s) : runtime_error("Rule Application Error: "+s) {} 


