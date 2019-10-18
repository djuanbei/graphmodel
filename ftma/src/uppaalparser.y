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


  
 %}

%union{
  int int_value;
  TYPE_T type_value;
  vector<string> * str_vec_pointer;
  COMP_OPERATOR com_op;
  
 }

%token <int_value> CONSTANT

%token<int_value> IDENTIFIER



%token<int_value> TEMPLATE


%type<int_value> const_expression


%type<type_value> type_specifier

%type<str_vec_pointer> identifier_list

%type<str_vec_pointer> ta_list

%type<com_op> compare_relation

%token STRING_LITERAL    URGENT BROADCAST META
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF  
%token  INT     CONST  VOID  CLOCK CHAN BOOL
%token TRUE_Y FALSE_Y

%token STRUCT   ELLIPSIS

%token COMMENT CASE DEFAULT IF ELSE SWITCH WHILE DO FOR  CONTINUE BREAK RETURN  SYSTEM

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


identifier_list
: IDENTIFIER
{
  $$=new vector<string> ( );
  $$->push_back( symbol_table[$1]);
}
| identifier_list ',' IDENTIFIER
{
  $$=$1;
  $$->push_back(symbol_table[$3]);
}
;
ta_list
:IDENTIFIER
{
  $$=new vector<string> ( );
  $$->push_back( symbol_table[$1]);
}
|
TEMPLATE
{
  $$=new vector<string> ( );
  $$->push_back( symbol_table[$1]);
}
| ta_list ',' IDENTIFIER
{
  $$=$1;
  $$->push_back(symbol_table[$3]);
}
| ta_list ',' TEMPLATE
{
  $$=$1;
  $$->push_back(symbol_table[$3]);
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

atomic_constraint:

IDENTIFIER compare_relation  const_expression 
{
  model_parser->parseConstraint( current_data, symbol_table[$1], $2, $3 );
}
|
IDENTIFIER compare_relation  IDENTIFIER
{
  model_parser->parseConstraint( current_data, symbol_table[ $1], $2,symbol_table[ $3] );
}
|
IDENTIFIER '-' IDENTIFIER  compare_relation  const_expression
{
  model_parser->parseConstraint( current_data, symbol_table[ $1], symbol_table[ $3], $4, $5 );
    
  /* if(getType( symbol_table[$1])==CLOCK_T &&getType( symbol_table[$3])==CLOCK_T  ){ */
  /*   int clock_id1=current_data->getId( CLOCK_STR, symbol_table[$1])+1; */
  /*   int clock_id2=current_data->getId( CLOCK_STR, symbol_table[$3])+1; */
  /*   if( $4==EQ){ */
      
  /*     INT_TAS_t::CS_t *cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, GE,  $5  ); //x-y< rhs */

  /*     current_data->addPointer( CLOCK_CS,CLOCK_CS, cs); */
      
  /*     cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, LE,  $5  ); //x-y< rhs */

  /*     current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);                      */
  /*   }else{ */
  /*     INT_TAS_t::CS_t *cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, $4,  $5  ); //x-y< rhs */
  /*     current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);                      */
  /*   } */

  /* } */
  /* else if(getType( symbol_table[$1])==INT_T &&getType( symbol_table[$3])==INT_T  ){ */
  /*   int counter_id1=system_data->getId( INT_STR, symbol_table[$1]); */
  /*   int counter_id2=system_data->getId( INT_STR, symbol_table[$3]); */
  /*   DiaCounterConstraint *cs=InstanceFactory::getInstance().createDiaCounterConstraint( counter_id1, counter_id2, $4, $5); */
  /*   current_data->addPointer( INT_CS,INT_CS, cs); */
  /* } */
  

}
|
IDENTIFIER
{
  model_parser->parseConstraint( current_data, symbol_table[ $1] );
}
|
'!' IDENTIFIER
{
  model_parser->parseConstraint( current_data, symbol_table[ $2], false );
  
}
;

assign_statement:
single_assign_statement
| assign_statement ',' single_assign_statement
;
single_assign_statement:
IDENTIFIER '=' const_expression
{
  model_parser->parseAssign( current_data, symbol_table[$1], $3 );
  
}
|
IDENTIFIER '=' IDENTIFIER
{
  model_parser->parseAssign( current_data, symbol_table[$1], symbol_table[$3] );
}
|
IDENTIFIER '=' TEMPLATE '(' identifier_list ')'
{
  
  TaDec * ta=new TaDec( );
  ta->name=symbol_table[ $1];
  
  for(auto name: ( *( $5) ) ){
    
    TYPE_T type=getType(name );
    int id=-1;
    if(INT_T==type ){
      id= system_data->getId(INT_STR,  name );
    }else if(BOOL_T==type ){
      id= system_data->getId( BOOL_STR, name);
    }else if( CHAN_T==type){
      id=system_data->getId( CHAN_STR, name);
    }
    assert( id>-1);
    ta->param_list.push_back( id);
  }
  
  system_data->addPointer(TEMPLATE_STR,symbol_table[ $1],  ta);
  
  delete $5;
}


const_expression:
CONSTANT
{
  $$=$1;
}
/* | */
/* IDENTIFIER */
/* { */
/*   $$=0; */
/*   if(current_data->hasValue(INT_STR, symbol_table[$1]) ){ */
/*     $$=current_data->getValue(INT_STR,symbol_table[$1]); */
/*   }else if ( system_data->hasValue(INT_STR, symbol_table[$1]) ){ */
/*     $$=system_data->getValue(INT_STR,   symbol_table[$1]); */
/*   } */
/* } */
|

TRUE_Y{
  $$=1;
}
|
FALSE_Y{
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
  }
}

| type_specifier IDENTIFIER '[' const_expression  ']' ';'
{
  string name=symbol_table[ $2];
  switch( $1){
    case INT_T:{
      for( int i=0; i< $4; i++){
        system_data->setValue( INT_STR, current_data->getVarFullName(arrayToVar(name, i)) );
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
        system_data->setValue( BOOL_STR, current_data->getVarFullName(arrayToVar(name, i )));
      }
      break;
    }
     
    case CHAN_T:{
      for( int i=0; i< $4; i++){
        system_data->setValue( CHAN_STR, current_data->getVarFullName(arrayToVar(name, i )), ONE2ONE_CH);
      }
      break;
    }
    case URGENT_CHAN_T:{
      for( int i=0; i< $4; i++){
        system_data->setValue( CHAN_STR, current_data->getVarFullName(arrayToVar(name, i )), URGENT_CH);
      }
      break;
    }
    case BROADCAST_CHAN_T: {
      for( int i=0; i< $4; i++){
        system_data->setValue( CHAN_STR, current_data->getVarFullName(arrayToVar(name, i )), BROADCAST_CH);
      }
      break;
    }
  }
}

|  type_specifier IDENTIFIER '=' const_expression ';'
{
  switch( $1){
    case INT_T:
      system_data->setValue(INT_STR, current_data->getVarFullName(symbol_table[$2]), $4);
      break;
    case CLOCK_T:
      current_data->setValue(CLOCK_STR, symbol_table[$2], $4);
      break;
      
    case BOOL_T:
      system_data->setValue(BOOL_STR, current_data->getVarFullName(symbol_table[$2]), $4);
      break;
  }
  
}
| CONST type_specifier IDENTIFIER '=' const_expression ';'

{
  switch( $2){
    case INT_T:
      system_data->setValue(INT_STR, current_data->getVarFullName(symbol_table[$3]), $5);
      break;
    case CLOCK_T:
      current_data->setValue(CLOCK_STR, symbol_table[$3], $5);
      break;

    case BOOL_T:
      system_data->addValue(BOOL_STR, current_data->getVarFullName(symbol_table[$3]), $5);
      break;
  }
  
}


|  TYPEDEF INT '[' const_expression ',' const_expression ']' IDENTIFIER ';'
{

  vector<int> temp;
  for( int i=$4; i<=$6; i++  ){
    temp.push_back( i);
  }
  system_data->addIntArray(current_data->getVarFullName(symbol_table[$8]),temp);
}


system_declaration
: SYSTEM ta_list ';'
{
  SystemDec * sys=new SystemDec( );
  for( size_t i=0; i< $2->size( ); i++){
    string name=(*$2)[ i];
    if(system_data->hasValue(TEMPLATE_STR, name  )){
      
      TaDec* temp=new TaDec( );
      temp->no_parameter=true;
      temp->name= name;
      sys->ta_list.push_back(temp );
    }else{
      void* sys_dec=system_data->getPointer(TEMPLATE_STR, name );
      sys->ta_list.push_back((TaDec*)sys_dec );
      
    }
  }
  
  system_data->clearPoints(TEMPLATE_STR );
  
  system_data->addPointer(SYSTEM_STR, SYSTEM_STR,sys );
  delete $2;
}

%%

#include "uppaalscan.h"
   
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


