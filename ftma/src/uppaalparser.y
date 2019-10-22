%{
  #include<iostream>
  #include<string>
  #include <stdio.h>
  #include<map>
  #include<vector>
  #include <utility>
  #include "channel.h"
  #include "util/dbmutil.hpp"
  #include "io/uppaaldata.h"
  //  #include "io/uppaalmodelparser.h"
  #include "model/ta.hpp"
  
  using std::string;
  using std::map;
  using std::vector;
  using std::make_pair;
      
  using namespace graphsat;
  
  int yydebug=1;   
  using std::cerr;
  using std::endl;
  using std::cout;

  
  vector<string> symbol_table;
  
  extern  int lineNum;


  UppaalData* current_data;
  //  UppaalParser* model_parser;
  
  extern void yyerror(const char *); 
  extern void yyerror(const string&);
  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;

  struct Token{
    int id;
    string  symbol;
  };
  
 %}

%union{
  int int_value;
  Token* token_value;
  TYPE_T type_value;
  vector<string> * str_vec_pointer;
  vector<ArgumentItem>* item_vec_pointer;
  vector<ParaElement>* para_element_pointer;
  COMP_OPERATOR com_op;
  
 }

%token <int_value> CONSTANT

%token<token_value> IDENTIFIER


%token<token_value> TEMPLATE_YY

%token<token_value> PARAMETER_YY

%token<token_value> REF_PARAMETER_YY

%token<token_value>  CLOCK_YY

%token<token_value>  INT_YY

%token<token_value>  BOOL_YY

%token<token_value>  CHAN_YY


%token STRING_LITERAL    URGENT BROADCAST META
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF  
%token  INT     CONST  VOID  CLOCK CHAN BOOL
%token TRUE_YY FALSE_YY

%token STRUCT   ELLIPSIS

%token COMMENT CASE DEFAULT IF ELSE SWITCH WHILE DO FOR  CONTINUE BREAK RETURN  SYSTEM ARGUMENT


%type<int_value> const_expression


%type<type_value> type_specifier
%type<type_value> base_type_specifier


%type<token_value>  clock_yy
%type<token_value>  int_yy
%type<token_value>  bool_yy
%type<token_value>  chan_yy
%type<int_value> clock_identifier
%type<int_value> counter_identifier
%type<int_value> parameter_identifier
%type<int_value> chan_identifier
%type<str_vec_pointer> identifier_list

%type<para_element_pointer> formal_argument_list
%type<item_vec_pointer> real_argument_list
%type<str_vec_pointer> timed_automata_list
%type<com_op> compare_relation


%nonassoc IFX
%nonassoc ELSE

%start translation_unit


%%
base_type_specifier
: INT
{
  $$=INT_T;
}
| CLOCK
{
  $$=CLOCK_T;
}
| BOOL
{
  $$=BOOL_T;
}
| CHAN
{
  $$=CHAN_T;
}
| URGENT CHAN
{
  $$=URGENT_CHAN_T;
}
| BROADCAST CHAN
{
  $$=BROADCAST_CHAN_T;
}
;


type_specifier:
base_type_specifier
{
  $$=$1;
}
|
CONST base_type_specifier
{
   $$=(TYPE_T)(((int)$2)+2);
}
|
base_type_specifier '&'
{
   $$=(TYPE_T)(((int)$1)+1);
}
|
CONST base_type_specifier '&'
{
  $$=(TYPE_T)(((int)$2)+3);
}
;


clock_yy:
CLOCK_YY
{
  $$=$1;
}
|
clock_yy '[' const_expression ']'
{
   $$=$1;
   $$->symbol=arrayToVar($$->symbol, $3);
}
;

int_yy:
INT_YY
{
  $$=$1;
}
|
int_yy '[' const_expression ']'
{
  $$=$1;
  $$->symbol=arrayToVar($$->symbol, $3);
}
;

bool_yy:
BOOL_YY
{
  $$=$1;
}
|
bool_yy '[' const_expression ']'
{
  $$=$1;
  $$->symbol=arrayToVar($$->symbol, $3);
};

chan_yy:
CHAN_YY
{
  $$=$1;
}
|
chan_yy '[' const_expression ']'
{
  $$=$1;
  $$->symbol=arrayToVar($$->symbol, $3);
};


identifier_list:
IDENTIFIER
{
  $$=new vector<string> ( );
  $$->push_back( $1->symbol);
  delete $1;
}
| identifier_list ',' IDENTIFIER
{
  $$=$1;
  $$->push_back($3->symbol);
  delete $3;
}
;


formal_argument_list:
type_specifier IDENTIFIER
{
  $$=new vector<ParaElement> ( );
  ParaElement elem;
  elem.is_ref=isRefType($1);
  elem.name=$2->symbol;
  elem.type_name=current_data->getTypeName($1);
  elem.type=$1;

  $$->push_back(elem);
  delete $2;
}
|
formal_argument_list ',' type_specifier IDENTIFIER
{
  $$=$1;
  ParaElement elem;
  elem.is_ref=isRefType($3);
  elem.name=$4->symbol;
  elem.type_name=current_data->getTypeName($3);
  elem.type=$3;
  $$->push_back(elem);
  delete $4;
}

real_argument_list
: counter_identifier
{
  $$=new vector<ArgumentItem> ( );
  ArgumentItem item;
  item.is_constant=false;
  item.id=$1;
  $$->push_back( item);
}
|
chan_identifier
{
  $$=new vector<ArgumentItem> ( );
  ArgumentItem item;
  item.is_constant=false;
  item.id=$1;
  $$->push_back( item);
}
| const_expression
{
  $$=new vector<ArgumentItem> ( );
  ArgumentItem item;
  item.is_constant=true;
  item.id=$1;
  $$->push_back( item);
}
| real_argument_list ',' counter_identifier
{
  $$=$1;
  ArgumentItem item;
  item.is_constant=false;
  item.id=$3;
  $$->push_back(item);
}

| real_argument_list ',' chan_identifier
{
  $$=$1;
  ArgumentItem item;
  item.is_constant=false;
  item.id=$3;
  $$->push_back(item);
}

| real_argument_list ',' const_expression
{
  $$=$1;
  ArgumentItem item;
  item.is_constant=true;
  item.id=$3;
  $$->push_back(item);
}

;


timed_automata_list
:IDENTIFIER
{
  $$=new vector<string> ( );
  $$->push_back($1->symbol);
  delete $1;
}
|
TEMPLATE_YY
{
  $$=new vector<string> ( );
  $$->push_back($1->symbol);
  delete $1;
}
| timed_automata_list ',' IDENTIFIER
{
  $$=$1;
  $$->push_back($3->symbol);
  delete $3;
}
| timed_automata_list ',' TEMPLATE_YY
{
  $$=$1;
  $$->push_back($3->symbol);
  delete $3;
}
;


compare_relation:

EQ_OP{
  $$=EQ;
}
|
LE_OP{
  $$=LE;
}
|
GE_OP{
  $$=GE;
}
|
'<' {
  $$=LT;
}
|
'>'{
  $$=GT;
}
|
NE_OP{
  $$=NE;
}

translation_unit
: external_declaration
| translation_unit external_declaration
| COMMENT
| translation_unit COMMENT
;

external_declaration
: variable_declaration
| constraint_statement
| assign_statement
| communicate_statement
| template_declaration
| system_declaration
;
constraint_statement:
atomic_constraint
|
constraint_statement AND_OP atomic_constraint



clock_identifier:
clock_yy{
  $$=current_data->getClockId($1->symbol);
  delete $1;
};

counter_identifier:
int_yy {
  $$=current_data->getGlobalId( INT_T, $1->symbol);
  delete $1;
}
|
bool_yy{
  $$=current_data->getGlobalId(  BOOL_T, $1->symbol);
  delete $1;
}
|
REF_PARAMETER_YY{
  $$=current_data->getParameterId( $1->symbol);
  delete $1;
}
;

parameter_identifier:
PARAMETER_YY{
  $$=current_data->getParameterId( $1->symbol);
  delete $1;
}
;

chan_identifier:
chan_yy{
  $$=current_data->getGlobalId(   CHAN_T, $1->symbol );
  delete $1;
};


atomic_constraint:
clock_identifier compare_relation  const_expression
{
  current_data->addClockConstraint( $1, GLOBAL_CLOCK_ID, $2, $3);
}
|
const_expression compare_relation clock_identifier
{
  COMP_OPERATOR nop=negation( $2);
  current_data->addClockConstraint(  $3 , GLOBAL_CLOCK_ID, nop, $1);
}
|
clock_identifier compare_relation parameter_identifier
{
  current_data->addClockConstraint(  $1, GLOBAL_CLOCK_ID, $2, 0, $3 ); 
}
|
parameter_identifier compare_relation clock_identifier
{
  COMP_OPERATOR nop=negation( $2);
  current_data->addClockConstraint(  $3, GLOBAL_CLOCK_ID, nop, 0, $1 ); 
}
|
parameter_identifier compare_relation const_expression
{
  void*  cs=createParameterConstraint( $1, DUMMY_ID, $2, $3 );
  current_data->addPointer( INT_CS_T,  cs );
}
|
const_expression compare_relation parameter_identifier
{
  COMP_OPERATOR nop=negation( $2);
  void*  cs=  createParameterConstraint($3, DUMMY_ID, nop, $1 );
  current_data->addPointer( INT_CS_T, cs );
}
|
parameter_identifier compare_relation parameter_identifier
{
  void* cs = createParameterConstraint( $1, $3, $2, 0 );
  current_data->addPointer( INT_CS_T,  cs );
}
|
parameter_identifier '-'  parameter_identifier compare_relation const_expression
{
  void* cs = createParameterConstraint( $1, $3, $4, $5 );
  current_data->addPointer( INT_CS_T,  cs );
}
|
counter_identifier compare_relation const_expression
{
  void *cs = createCounterConstraint( $1, DUMMY_ID, $2, $3 );
  current_data->addPointer( INT_CS_T,  cs );
}
|
const_expression compare_relation counter_identifier
{
  COMP_OPERATOR nop=negation( $2);
  void *cs = createCounterConstraint( $3, DUMMY_ID, nop, $1 );
  current_data->addPointer( INT_CS_T,  cs );
}
|
counter_identifier compare_relation parameter_identifier
{
  void* cs = createCounterParameterConstraint( $1, $3, $2, 0 );
  current_data->addPointer( INT_CS_T, cs );
}
|
parameter_identifier  compare_relation  counter_identifier{
  COMP_OPERATOR nop=negation( $2);
  void* cs = createCounterParameterConstraint( $3, $1, nop, 0 );
  current_data->addPointer(INT_CS_T , cs );
}
|
clock_identifier '-' clock_identifier compare_relation const_expression
{
  current_data->addClockConstraint( $1, $3, $4, $5 );
}
|
clock_identifier '-' clock_identifier compare_relation parameter_identifier
{
   current_data->addClockConstraint( $1, $3, $4, 0, $5 );
}
|
counter_identifier '-' counter_identifier compare_relation const_expression
{
  void * cs = createCounterConstraint( $1, $3, $4, $5 );
  current_data->addPointer( INT_CS_T, cs );
}
|
counter_identifier '-' counter_identifier compare_relation parameter_identifier
{
  void * cs = createCounterConstraint( $1, $3, $4, 0, $5 );
  current_data->addPointer( INT_CS_T,  cs );
}
|
bool_yy{
  int id=current_data->getGlobalId(  BOOL_T, $1->symbol );
  void *cs = createCounterConstraint( id, DUMMY_ID, NE, 0 );
  current_data->addPointer( INT_CS_T,  cs );
  delete $1;
}
|
'!'
bool_yy{
  int id=current_data->getGlobalId( BOOL_T, $2->symbol) ;
  void *cs = createCounterConstraint( id, DUMMY_ID, EQ, 0 );
  current_data->addPointer( INT_CS_T, cs );  
  delete $2;
}
|
PARAMETER_YY
{
  cout<<1<<endl;
}
|
REF_PARAMETER_YY
{
  cout<<1<<endl;
}

|
'!' PARAMETER_YY
{
  cout<<1<<endl;
}
|
'!' REF_PARAMETER_YY
{
  cout<<1<<endl;
}
;

assign_statement:
single_assign_statement
| assign_statement ',' single_assign_statement
;

single_assign_statement:
clock_identifier '=' const_expression 
{
  pair<int, int> *reset=new pair<int, int>($1, $3 );
  current_data->addPointer(RESET_T, reset );
}
|
counter_identifier '=' const_expression
{
  CounterAction   *action=new CounterAction(ASSIGNMENT_ACTION, RHS_CONSTANT_T,  $1, $3);
  current_data->addPointer( INT_UPDATE_T, action);
}
|
counter_identifier '=' counter_identifier
{
  CounterAction *action =new CounterAction( ASSIGNMENT_ACTION, RHS_COUNTER_T, $1, $3 );
  current_data->addPointer( INT_UPDATE_T, action);
}
|
counter_identifier '='  parameter_identifier
{
  CounterAction *action =new CounterAction( ASSIGNMENT_ACTION, RHS_PARAMETER_T , $1, $3 );
  current_data->addPointer( INT_UPDATE_T, action);
}
|
counter_identifier ADD_ASSIGN const_expression
{
  CounterAction   *action=new CounterAction(SELF_INC_ACTION, RHS_CONSTANT_T,  $1, $3);
  current_data->addPointer( INT_UPDATE_T, action);
}
|
counter_identifier ADD_ASSIGN counter_identifier
{
  CounterAction *action =new CounterAction( SELF_INC_ACTION, RHS_COUNTER_T, $1, $3 );
  current_data->addPointer( INT_UPDATE_T, action);
}
|
counter_identifier ADD_ASSIGN  parameter_identifier
{
  CounterAction *action =new CounterAction( SELF_INC_ACTION, RHS_PARAMETER_T , $1, $3 );
  current_data->addPointer( INT_UPDATE_T, action);
}

|
counter_identifier SUB_ASSIGN const_expression
{
  CounterAction   *action=new CounterAction(SELF_DEC_ACTION, RHS_CONSTANT_T,  $1, $3);
  current_data->addPointer( INT_UPDATE_T, action);
}
|
counter_identifier SUB_ASSIGN counter_identifier
{
  CounterAction *action =new CounterAction( SELF_DEC_ACTION, RHS_COUNTER_T, $1, $3 );
  current_data->addPointer( INT_UPDATE_T, action);
}
|
counter_identifier SUB_ASSIGN  parameter_identifier
{
  CounterAction *action =new CounterAction( SELF_DEC_ACTION, RHS_PARAMETER_T , $1, $3 );
  current_data->addPointer( INT_UPDATE_T, action);
}

|
IDENTIFIER '=' TEMPLATE_YY '(' real_argument_list ')' ';'
{

  TaDec * ta=new TaDec( );
  ta->name= $1->symbol;
  ta->tmt_name=$3->symbol;
  ta->param_list.insert(ta->param_list.end( ), $5->begin( ), $5->end( ) );
  current_data->addPointer(TEMPLATE_T, $1->symbol,  ta);
  
  delete $1;
  delete $3;
  delete $5;
};

communicate_statement:

chan_identifier '!'
{
  
  
}
|
chan_identifier '?'
{
  
}
;

const_expression:
CONSTANT
{
  $$=$1;
}
|
TRUE_YY{
  $$=1;
}
|
FALSE_YY{
  $$=0;
}
;

variable_declaration
: type_specifier identifier_list ';'
{
  TYPE_T type=base_type($1 );
  switch( type){
    case INT_T:{
      for( auto v: *$2){
        current_data->setValue(INT_T, v); //All the variables  in system_data
      }
      break ;
    }
    case CLOCK_T:{
      for( auto v: *$2){
        current_data->setValue(CLOCK_T, v);  //clock variable only can declare in template
      }
      break ;
    }
    case BOOL_T: {
      for( auto v: *$2){
        current_data->setValue(BOOL_T, v);
      }
      break ;
    }
    case CHAN_T: {
      for( auto v: *$2){
        current_data->setValue(CHAN_T, v, ONE2ONE_CH);
      }
      break ;
    }

    case URGENT_CHAN_T:{
      for( auto v: *$2){
       current_data->setValue(CHAN_T, v, URGENT_CH);
      }
      break ;
    }
    case BROADCAST_CHAN_T:{
      for( auto v: *$2){
        current_data->setValue(CHAN_T, v, BROADCAST_CH);
      }
      break ;
    }
    default:{
      assert( false);
    }

  }
  delete $2;
}
| type_specifier IDENTIFIER '[' const_expression  ']' ';'
{
  string name= $2->symbol;
  TYPE_T type=base_type($1 );
  
  switch( type){
    case INT_T:{
      current_data->setValue( INT_T,  name);
      for( int i=0; i< $4; i++){
        current_data->setValue( INT_T,  arrayToVar(name, i));
      }
      break;
    }
    case CLOCK_T: {
      current_data->setValue( CLOCK_T, name );
      for( int i=0; i< $4; i++){
        current_data->setValue( CLOCK_T, arrayToVar(name, i ));
      }
      break;
    }

    case BOOL_T:{
      current_data->setValue( BOOL_T, name);
      for( int i=0; i< $4; i++){
        current_data->setValue( BOOL_T, arrayToVar(name, i));
      }
      break;
    }
     
    case CHAN_T:{
      current_data->setValue( CHAN_T, name, ONE2ONE_CH);
      for( int i=0; i< $4; i++){
        current_data->setValue( CHAN_T, arrayToVar(name, i ), ONE2ONE_CH);
      }
      break;
    }
    case URGENT_CHAN_T:{
      current_data->setValue( CHAN_T, name, URGENT_CH);
      for( int i=0; i< $4; i++){
        current_data->setValue( CHAN_T, arrayToVar(name, i ), URGENT_CH);
      }
      break;
    }
    case BROADCAST_CHAN_T: {
      current_data->setValue( CHAN_T,name, BROADCAST_CH);
      for( int i=0; i< $4; i++){
        current_data->setValue( CHAN_T, arrayToVar(name, i ), BROADCAST_CH);
      }
      break;
    }
    default:
      assert( false);
  }
}

|  type_specifier IDENTIFIER '=' const_expression ';'
{
  TYPE_T type=base_type($1 );
  
  switch(type){
    case INT_T:
      current_data->setValue(INT_T, $2->symbol, $4);
      break;
      
    case CLOCK_T:
      current_data->setValue(CLOCK_T,  $2->symbol,  $4);
      break;
      
    case BOOL_T:
      current_data->setValue(BOOL_T,  $2->symbol,  $4);
      break;
    default:
      assert( false);
  }
  
}

|  TYPEDEF INT '[' const_expression ',' const_expression ']' IDENTIFIER ';'
{

  vector<int> temp;
  for( int i=$4; i<=$6; i++  ){
    temp.push_back( i);
  }
  current_data->addIntArray($8->symbol,temp);
}

template_declaration:
ARGUMENT  formal_argument_list
{
  for(auto e: *($2)){
    current_data->addPointer( PARAMETER_T, e.name, new ParaElement(e));
  }
  delete $2;
}
;

system_declaration
: SYSTEM timed_automata_list ';'
{
  SystemDec * sys=new SystemDec( );
  for( size_t i=0; i< $2->size( ); i++){
    string name=(*$2)[ i];
    if(current_data->hasValue(TEMPLATE_T, name  )){
      
      TaDec* temp=new TaDec( );
      temp->has_parameter=false;
      temp->name= name;
      temp->tmt_name=name;
      sys->timed_automata_list.push_back(temp );
    }else{
      void* ta_dec=current_data->getPointer(TEMPLATE_T, name );
      sys->timed_automata_list.push_back((TaDec*)ta_dec );
    }
  }
  
  current_data->clearPoints(TEMPLATE_T );
  
  current_data->addPointer(SYSTEM_T,sys );
  delete $2;
}

%%

#include "io/uppaalscan.h"
   
/* extern int column; */

void yyerror(const char *s)
{
  printf("\n jjj error  %s\n",s);
  fflush(stdout);
  assert( false);
}
void yyerror(const string &s){


  cout<<"error in: "<<endl;
  fflush(stdout);
  assert( false);
  
}


namespace graphsat{
  void parseProblem( const string &str,   UppaalData* d){
    
    current_data=d;
    yyin=tmpfile();
    fputs( str.c_str(), yyin);
    rewind(yyin);
    yyparse();
    fclose(yyin);
  
  }
} 


