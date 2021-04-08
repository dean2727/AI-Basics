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
      unsigned int j=i;
      while (j<n && s[j]!=' ' && s[j]!='(' && s[j]!=')') j++;
      string str = s.substr(i,j-i);
      tokens.push_back(str);
      i = j;
    }
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

bool Eq(Expr* a, Expr* b) {
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
  while (getline(input,line)) {
    if (line.size()==0 || line[0]=='#') continue;
    KB.push_back(parse(line));
  }
  return KB;
}

void show_kb(vector<Expr*>& KB) {
  for (unsigned int i = 0; i < KB.size(); i++) {
    cout << i << ". ";
    cout << KB[i]->toString() << endl;
  }
}

// given a sentence, insert any new symbols from the KB into the set of unique symbols
void add_symbols_from_rule(set<string>& unique, vector<string> sentence) {
  string str;
  for (unsigned int i = 0; i < sentence.size(); i++) {
    str = sentence[i];
    if (str != "(" && str != ")" && str != "not" && str != "or" && str != "and" && str != "implies") {
      unique.insert(str);
    }
  }
}

// returns a set of the unique symbols in the KB
set<string> get_unique_symbols(vector<Expr*>& KB) {
  set<string> uniqueSymbols;

  vector<string> toks;
  for (unsigned int i = 0; i < KB.size(); i++) {
    toks = tokenize(KB[i]->toString());
    add_symbols_from_rule(uniqueSymbols, toks);
  }

  return uniqueSymbols;
}

// evaluates the truth value of a sentence from the CNF version of the KB
// returns -1 if a literal is not assigned a value yet, 0 if false, and 1 if true
int evaluate_clause(Expr* clause, MODEL* model) {
  vector<string> toks = tokenize(clause->toString());

  // initialize with 0, but if we see a true for a literal, change to 1
  // if the literal we see is not in the model (not assigned a truth value), change ret to -1 and break
  int ret = 0;
  string tok;
  for (unsigned int i = 0; i < toks.size(); i++) {
    tok = toks[i];

    // negated literal -> flip truth value of next token (its safe to assume that we are not on last iteration)
    if (tok == "not") {
      if (model->find(toks[i+1]) == model->end()) {
        ret = -1;
        break;
      }
      if (!model->at(toks[i+1])) {
        ret = 1;
      }
    }

    // token should be unique
    else if (tok != "(" && tok != ")" && tok != "or") {
      // if previous token was a "not", skip this literal, we already evaluated it
      if (i != 0 && toks[i-1] == "not") {
        continue;
      }
      if (model->find(tok) == model->end()) {
        ret = -1;
        break;
      }
      if (model->at(tok)) {
        ret = 1;
      }
    }
  }

  return ret;
}

// determines if the clause is a unit clause
bool is_unit_clause(Expr* clause, MODEL* model) {
  int numFalse = 0, numLiterals = 0, numUnassigned = 0;
  string tok;
  vector<string> toks = tokenize(clause->toString());

  // our goal is to see if this clause has n-1 false literals and 1 unassigned literal (symbol)
  for (unsigned int i = 0; i < toks.size(); i++) {
    tok = toks[i];
    
    if (tok == "not") {
      numLiterals++;
      // for false literal, we want the literal to exist in the hash map and be true here
      if (model->count(toks[i+1])) {
        if (model->at(toks[i+1]) == true) numFalse++;          
      }
      else {
        numUnassigned++;
      }
    }
    
    else if (tok != "(" && tok != ")" && tok != "or") {
      // if previous token was a "not", skip this literal, we already evaluated it
      if (i != 0 && toks[i-1] == "not") {
        continue;
      }
      numLiterals++;
      // for false literal, we want the literal to exist and be false here
      if (model->count(tok)) {
        if (model->at(tok) == false) numFalse++;        
      }
      else {
        numUnassigned++;
      }
    }
  }

  if (numFalse == numLiterals - 1 && numUnassigned == 1) {
    return true;
  }
  else {
    return false;
  }
}

// get the unit clause
pair<string, bool> get_unit_clause(Expr* clause, MODEL* model) {
  string lit, tok;
  bool val;
  vector<string> toks = tokenize(clause->toString());

  // we find the non-false literal by finding the first unique token that isnt mapped to false in the model
  for (unsigned int i = 0; i < toks.size(); i++) {
    tok = toks[i];

    if (tok == "not") {
      // we want the literal to be not found or false (double negation = true)
      if (model->find(toks[i+1]) == model->end() || !model->at(toks[i+1])) {
        lit = toks[i+1];
        val = false;
        break;
      }
    }
    
    else if (tok != "(" && tok != ")" && tok != "or") {
      // if previous token was a "not", skip this literal, we already evaluated it
      if (i != 0 && toks[i-1] == "not") {
        continue;
      }
      // we want the literal to be true (or not found in the hash table)
      if (model->find(tok) == model->end() || model->at(tok)) {
        lit = tok;
        val = true;
        break;
      }
    }
  }

  return make_pair(lit, val);
}

//----------------------------

SyntaxError::SyntaxError(string s) : runtime_error("Syntax Error: "+s) {} 

// for trying to apply an ROI to an expression for which it doesn’t apply (for example,
// trying to apply double-negation elimination to “(not P)”)
RuleApplicationError::RuleApplicationError(string s) : runtime_error("Rule Application Error: "+s) {} 


