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
  #include "io/uppaalmodelparser.h"
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


  UppaalTemplateData* system_data;
  UppaalTemplateData* current_data;
  UppaalParser* model_parser;
  
  extern void yyerror(const char *); 
  extern void yyerror(const string&);
  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;

  struct Token{
    int id;
    string  xml_name;
    string  code_name;
  };


  
 %}

%union{
  int int_value;
  Token* token_value;
  TYPE_T type_value;
  vector<string> * str_vec_pointer;
  vector<ArgumentItem>* item_vec_pointer;
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

%token COMMENT CASE DEFAULT IF ELSE SWITCH WHILE DO FOR  CONTINUE BREAK RETURN  SYSTEM


%type<int_value> const_expression
%type<type_value> type_specifier
//%type<token_value> identifier
%type<token_value>  clock_yy
%type<token_value>  int_yy
%type<token_value>  bool_yy
%type<token_value>  chan_yy
%type<int_value> clock_identifier
%type<int_value> counter_identifier
%type<int_value> parameter_identifier
%type<int_value> chan_identifier
%type<str_vec_pointer> identifier_list
%type<item_vec_pointer> argument_list
%type<str_vec_pointer> timed_automata_list
%type<com_op> compare_relation


%nonassoc IFX
%nonassoc ELSE

%start translation_unit


%%

type_specifier
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


/* identifier: */
/* IDENTIFIER */
/* { */
/*   $$=$1; */
/* } */
/* | */
/* identifier '[' const_expression ']' */
/* { */
/*   $$=$1; */
/*   $$->code_name= arrayToVar($$->code_name, $3); */
/* } */
/* ; */

clock_yy:
CLOCK_YY
{
  $$=$1;
}
|
clock_yy '[' const_expression ']'
{
   $$=$1;
   $$->code_name=arrayToVar($$->code_name, $3);
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
  $$->code_name=arrayToVar($$->code_name, $3);
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
  $$->code_name=arrayToVar($$->code_name, $3);
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
  $$->code_name=arrayToVar($$->code_name, $3);
};


identifier_list:
IDENTIFIER
{
  $$=new vector<string> ( );
  $$->push_back( $1->code_name);
  delete $1;
}
| identifier_list ',' IDENTIFIER
{
  $$=$1;
  $$->push_back($3->code_name);
  delete $3;
}
;


argument_list
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
| argument_list ',' counter_identifier
{
  $$=$1;
  ArgumentItem item;
  item.is_constant=false;
  item.id=$3;
  $$->push_back(item);
}

| argument_list ',' chan_identifier
{
  $$=$1;
  ArgumentItem item;
  item.is_constant=false;
  item.id=$3;
  $$->push_back(item);
}

| argument_list ',' const_expression
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
  $$->push_back($1->code_name);
  delete $1;
}
|
TEMPLATE_YY
{
  $$=new vector<string> ( );
  $$->push_back($1->code_name);
  delete $1;
}
| timed_automata_list ',' IDENTIFIER
{
  $$=$1;
  $$->push_back($3->code_name);
  delete $3;
}
| timed_automata_list ',' TEMPLATE_YY
{
  $$=$1;
  $$->push_back($3->code_name);
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
| comment_list
;

external_declaration
: variable_declaration
| constraint_statement
| assign_statement
| communicate_statement 
| system_declaration
;
constraint_statement:
atomic_constraint
|
constraint_statement AND_OP atomic_constraint

;
comment_list:
COMMENT
|
comment_list COMMENT
;

clock_identifier:
clock_yy{
  $$=current_data->getClockId($1->code_name);
  delete $1;
};

counter_identifier:
int_yy {
  $$=model_parser->getGlobalId( current_data, INT_STR, $1->code_name);
  delete $1;
}
|
bool_yy{
  $$=model_parser->getGlobalId( current_data,  BOOL_STR, $1->code_name) ;
  delete $1;
}
|
REF_PARAMETER_YY{
  $$=model_parser->getParameterId(current_data, $1->code_name);
  delete $1;
}
;

parameter_identifier:
PARAMETER_YY{
  $$=model_parser->getParameterId(current_data, $1->code_name);
  delete $1;
}
;

chan_identifier:
chan_yy{
  $$=model_parser->getGlobalId( current_data,  CHAN_STR, $1->code_name );
  delete $1;
};


atomic_constraint:
clock_identifier compare_relation  const_expression
{
  model_parser->addClockConstraint(current_data, $1, GLOBAL_CLOCK_ID, $2, $3);
}
|
const_expression compare_relation clock_identifier
{
  COMP_OPERATOR nop=negation( $2);
  model_parser->addClockConstraint(current_data,  $3 , GLOBAL_CLOCK_ID, nop, $1);
}
|
clock_identifier compare_relation parameter_identifier
{
  model_parser->addClockConstraint( current_data, $1, GLOBAL_CLOCK_ID, $2, 0, $3 ); 
}
|
parameter_identifier compare_relation clock_identifier
{
  COMP_OPERATOR nop=negation( $2);
  model_parser->addClockConstraint( current_data, $3, GLOBAL_CLOCK_ID, nop, 0, $1 ); 
}
|
parameter_identifier compare_relation const_expression
{
  void*  cs=createParameterConstraint( $1, DUMMY_ID, $2, $3 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
const_expression compare_relation parameter_identifier
{
  COMP_OPERATOR nop=negation( $2);
  void*  cs=  createParameterConstraint($3, DUMMY_ID, nop, $1 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
parameter_identifier compare_relation parameter_identifier
{
  void* cs = createParameterConstraint( $1, $3, $2, 0 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
parameter_identifier '-'  parameter_identifier compare_relation const_expression
{
  void* cs = createParameterConstraint( $1, $3, $4, $5 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
counter_identifier compare_relation const_expression
{
  void *cs = createCounterConstraint( $1, DUMMY_ID, $2, $3 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
const_expression compare_relation counter_identifier
{
  COMP_OPERATOR nop=negation( $2);
  void *cs = createCounterConstraint( $3, DUMMY_ID, nop, $1 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
counter_identifier compare_relation parameter_identifier
{
  void* cs = createCounterParameterConstraint( $1, $3, $2, 0 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
parameter_identifier  compare_relation  counter_identifier{
  COMP_OPERATOR nop=negation( $2);
  void* cs = createCounterParameterConstraint( $3, $1, nop, 0 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
clock_identifier '-' clock_identifier compare_relation const_expression
{
  model_parser->addClockConstraint( current_data, $1, $3, $4, $5 );
}
|
clock_identifier '-' clock_identifier compare_relation parameter_identifier
{
  model_parser->addClockConstraint( current_data, $1, $3, $4, 0, $5 );
}
|
counter_identifier '-' counter_identifier compare_relation const_expression
{
  void * cs = createCounterConstraint( $1, $3, $4, $5 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
counter_identifier '-' counter_identifier compare_relation parameter_identifier
{
  void * cs = createCounterConstraint( $1, $3, $4, 0, $5 );
  current_data->addPointer( INT_CS, INT_CS, cs );
}
|
bool_yy{
  int id=model_parser->getGlobalId( current_data, BOOL_STR, $1->code_name );
  void *cs = createCounterConstraint( id, DUMMY_ID, NE, 0 );
  current_data->addPointer( INT_CS, INT_CS, cs );
  delete $1;
}
|
'!'
bool_yy{
  int id=model_parser->getGlobalId( current_data,BOOL_STR, $2->code_name) ;
  void *cs = createCounterConstraint( id, DUMMY_ID, EQ, 0 );
  current_data->addPointer( INT_CS, INT_CS, cs );  
  delete $2;
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
  current_data->addPointer(RESET_STR, RESET_STR, reset );
}
|
counter_identifier '=' const_expression
{
  CounterAction   *action=new CounterAction(ASSIGNMENT_ACTION, RHS_CONSTANT_T,  $1, $3);
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}
|
counter_identifier '=' counter_identifier
{
  CounterAction *action =new CounterAction( ASSIGNMENT_ACTION, RHS_COUNTER_T, $1, $3 );
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}
|
counter_identifier '='  parameter_identifier
{
  CounterAction *action =new CounterAction( ASSIGNMENT_ACTION, RHS_PARAMETER_T , $1, $3 );
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}
|
counter_identifier ADD_ASSIGN const_expression
{
  CounterAction   *action=new CounterAction(SELF_INC_ACTION, RHS_CONSTANT_T,  $1, $3);
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}
|
counter_identifier ADD_ASSIGN counter_identifier
{
  CounterAction *action =new CounterAction( SELF_INC_ACTION, RHS_COUNTER_T, $1, $3 );
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}
|
counter_identifier ADD_ASSIGN  parameter_identifier
{
  CounterAction *action =new CounterAction( SELF_INC_ACTION, RHS_PARAMETER_T , $1, $3 );
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}

|
counter_identifier SUB_ASSIGN const_expression
{
  CounterAction   *action=new CounterAction(SELF_DEC_ACTION, RHS_CONSTANT_T,  $1, $3);
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}
|
counter_identifier SUB_ASSIGN counter_identifier
{
  CounterAction *action =new CounterAction( SELF_DEC_ACTION, RHS_COUNTER_T, $1, $3 );
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}
|
counter_identifier SUB_ASSIGN  parameter_identifier
{
  CounterAction *action =new CounterAction( SELF_DEC_ACTION, RHS_PARAMETER_T , $1, $3 );
  current_data->addPointer( INT_UPDATE,INT_UPDATE, action);
}

|
IDENTIFIER '=' TEMPLATE_YY '(' argument_list ')'
{
  
  TaDec * ta=new TaDec( );
  ta->name= $1->xml_name;
  ta->param_list.insert(ta->param_list.end( ), $5->begin( ), $5->end( ) );
  system_data->addPointer(TEMPLATE_STR, $1->xml_name,  ta);
  
  delete $1;
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
  switch( $1){
    case INT_T:{
      for( auto v: *$2){
        system_data->setValue(INT_STR, current_data->getVarFullName(v)); //All the variables  in system_data
      }
      delete $2;
      break ;
    }
    case CLOCK_T:{
      for( auto v: *$2){
        current_data->setValue(CLOCK_STR, v);  //clock variable only can declare in template
      }
      delete $2;
      break ;
    }

    case BOOL_T: {
      for( auto v: *$2){
        system_data->setValue(BOOL_STR, current_data->getVarFullName(v));
      }
      delete $2;
      break ;
    }
    case CHAN_T: {
      for( auto v: *$2){
        system_data->setValue(CHAN_STR, current_data->getVarFullName(v), ONE2ONE_CH);
      }
      delete $2;
      break ;
    }

    case URGENT_CHAN_T:{
      for( auto v: *$2){
        system_data->setValue(CHAN_STR, current_data->getVarFullName(v), URGENT_CH);
      }
      delete $2;
      break ;
    }
    case BROADCAST_CHAN_T:{
      for( auto v: *$2){
        system_data->setValue(CHAN_STR, current_data->getVarFullName(v), BROADCAST_CH);
      }
      delete $2;
      break ;
    }
    default:
      assert( false);
  }
}

| type_specifier IDENTIFIER '[' const_expression  ']' ';'
{
  string name= $2->code_name;
  switch( $1){
    case INT_T:{
      for( int i=0; i< $4; i++){
        system_data->setValue( INT_STR,  name);
      }
      break;
    }
    case CLOCK_T: {
      for( int i=0; i< $4; i++){
        current_data->setValue( CLOCK_STR, arrayToVar(name, i ));
      }
      break;
    }

    case BOOL_T:{
      for( int i=0; i< $4; i++){
        system_data->setValue( BOOL_STR, name);
      }
      break;
    }
     
    case CHAN_T:{
      for( int i=0; i< $4; i++){
        system_data->setValue( CHAN_STR, arrayToVar(name, i ), ONE2ONE_CH);
      }
      break;
    }
    case URGENT_CHAN_T:{
      for( int i=0; i< $4; i++){
        system_data->setValue( CHAN_STR, arrayToVar(name, i ), URGENT_CH);
      }
      break;
    }
    case BROADCAST_CHAN_T: {
      for( int i=0; i< $4; i++){
        system_data->setValue( CHAN_STR, arrayToVar(name, i ), BROADCAST_CH);
      }
      break;
    }
    default:
      assert( false);
  }
}

|  type_specifier IDENTIFIER '=' const_expression ';'
{
  switch( $1){
    case INT_T:
      system_data->setValue(INT_STR, $2->code_name, $4);
      break;
    case CLOCK_T:
      current_data->setValue(CLOCK_STR,  $2->code_name,  $4);
      break;
      
    case BOOL_T:
      system_data->setValue(BOOL_STR,  $2->code_name,  $4);
      break;
    default:
      assert( false);
  }
  
}
| CONST type_specifier IDENTIFIER '=' const_expression ';'

{
  switch( $2){
    case INT_T:
      system_data->setValue(INT_STR, $3->code_name, $5);
      break;
    case CLOCK_T:
      current_data->setValue(CLOCK_STR,  $3->code_name, $5);
      break;

    case BOOL_T:
      system_data->addValue(BOOL_STR, $3->code_name, $5);
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
  system_data->addIntArray($8->code_name,temp);
}


system_declaration
: SYSTEM timed_automata_list ';'
{
  SystemDec * sys=new SystemDec( );
  for( size_t i=0; i< $2->size( ); i++){
    string name=(*$2)[ i];
    if(system_data->hasValue(TEMPLATE_STR, name  )){
      
      TaDec* temp=new TaDec( );
      temp->no_parameter=true;
      temp->name= name;
      sys->timed_automata_list.push_back(temp );
    }else{
      void* sys_dec=system_data->getPointer(TEMPLATE_STR, name );
      sys->timed_automata_list.push_back((TaDec*)sys_dec );
    }
  }
  
  system_data->clearPoints(TEMPLATE_STR );
  
  system_data->addPointer(SYSTEM_STR, SYSTEM_STR,sys );
  delete $2;
}

%%

#include "io/uppaalscan.h"
   
/* extern int column; */

void yyerror(const char *s)
{
    fflush(stdout);
    assert( false);
    printf("\n jjj error\n");
}
void yyerror(const string &s){

  fflush(stdout);
  assert( false);
  printf("\n kkk error\n");
  
}





namespace graphsat{
  void parseProblem( const string &str, UppaalParser* parser,  UppaalTemplateData *pd,   UppaalTemplateData* d){
    
    model_parser=parser;
    system_data=pd;
    current_data=d;
    yyin=tmpfile();
    fputs( str.c_str(), yyin);
    rewind(yyin);
    yyparse();
    fclose(yyin);
  
  }
} 


