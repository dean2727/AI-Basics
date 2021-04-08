#include <vector>
#include <string>
#include <stdexcept>
#include <set>
#include <unordered_map>

#ifndef PARSER_HPP
#define PARSER_HPP

using namespace std;

//----------------------------------

enum EXPR_TYPE { ATOM , LIST };

typedef unordered_map<string,bool> MODEL;

class Expr 
{
public:
  EXPR_TYPE kind;
  string sym;  // if kind is ATOM, this is defined
  vector<Expr*> sub;  // if kind is LIST, this is defined
  int end;

  Expr(string s);
  Expr(vector<string> tokens, int start);
  Expr(Expr* e); // deep copy
  Expr(vector<Expr*> L); // create new COMPLEX expression from list
  string toString();
};

//----------------------------------

vector<string> tokenize(string s);

Expr* parse(string s);

void add_symbols_from_rule(set<string>& unique, vector<string> sentence);

// check for equality (overload the == operator?)

bool Eq(Expr* a, Expr* b);

vector<Expr*>load_kb(string fname);

void show_kb(vector<Expr*>& KB);

set<string> get_unique_symbols(vector<Expr*>& KB);

int evaluate_clause(Expr* clause, MODEL* model);

bool is_unit_clause(Expr* clause, MODEL* model);

pair<string, bool> get_unit_clause(Expr* clause, MODEL* model);

//----------------------------------

// for throwing exceptions...

class SyntaxError : public runtime_error
{
public:
  SyntaxError(string s);
};

class RuleApplicationError : public runtime_error
{
public:
  RuleApplicationError(string s);
};

#endif
