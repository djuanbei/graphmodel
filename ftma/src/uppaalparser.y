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
  #include "model/ta.hpp"
  
  using  std::string;
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


  const UppaalData* uplayerData;
  UppaalData* data;

  
  extern void yyerror(const char *); 
  extern void yyerror(const string&);
  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;
  TYPE_T getType(string & name );
  
 %}

%union{
  int intVal;
  vector<string> * str_vec_pointer;
  COMP_OPERATOR com_op;
  
 }

%token <intVal> CONSTANT

%token<intVal> IDENTIFIER

%token<intVal> PARAM



%type<intVal> const_expression
%type<intVal> type_specifier

%type<str_vec_pointer> identifier_list

%type<com_op> compare_relation

%token STRING_LITERAL    URGENT BROADCAST META
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF  
%token  INT     CONST  VOID  CLOCK CHAN
%token STRUCT   ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR  CONTINUE BREAK RETURN

%nonassoc IFX
%nonassoc ELSE

%start translation_unit


%%

type_specifier
: INT
{
  $$=1;
}
| CLOCK
{
  $$=2;
}
| CHAN
{
  $$=3;
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
;

external_declaration
: variable_declaration
| constraint_statement
| assign_statement
;
constraint_statement:
atomic_constraint
|
constraint_statement AND_OP atomic_constraint

;

atomic_constraint:

IDENTIFIER compare_relation  const_expression 
{
  void *cs;
  const  TYPE_T type=getType( symbol_table[$1]);
  if( type==CLOCK_T){
    int clock_id=data->getId( CLOCK_STR, symbol_table[$1])+1; //CLOCK ID START FROM 1
    cs=new      INT_TAS_t::CS_t(clock_id, 0,  $2, $3 ); //x< c
    data->addPointer( CLOCK_CS, cs);
  }
  else if(type==COUNTER_T ){
    int counter_id=uplayerData->getId( COUNTER_STR, symbol_table[$1]);
    cs = CounterConstraintFactory::getInstance().createDiaFreeCounterConstraint( counter_id, $2, $3);
    data->addPointer( COUNTER_CS, cs);
  }

}
|
IDENTIFIER compare_relation  PARAM
{
  const  TYPE_T type=getType(   symbol_table[$1]);
  assert( COUNTER_T==type);
  int counter_id=uplayerData->getId( COUNTER_STR, symbol_table[$1]);
  int param_id=data->getId( PARAMETER_STR, symbol_table[$3]);
  DiaFreeCounterPConstraint *cs=CounterConstraintFactory::getInstance().createDiaFreeCounterPConstraint(counter_id, $2, param_id );
  data->addPointer( COUNTER_CS, cs);
}
|
IDENTIFIER '-' IDENTIFIER  compare_relation  const_expression
{
  if(getType( symbol_table[$1])==CLOCK_T &&getType( symbol_table[$3])==CLOCK_T  ){
    int clock_id1=data->getId( CLOCK_STR, symbol_table[$1])+1;
    int clock_id2=data->getId( CLOCK_STR, symbol_table[$3])+1;
    INT_TAS_t::CS_t *cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, $4,  $5  ); //x-y< rhs
    data->addPointer( CLOCK_CS, cs);                     
  }
  else if(getType( symbol_table[$1])==COUNTER_T &&getType( symbol_table[$3])==COUNTER_T  ){
    int counter_id1=uplayerData->getId( COUNTER_STR, symbol_table[$1]);
    int counter_id2=uplayerData->getId( COUNTER_STR, symbol_table[$3]);
    DiaCounterConstraint *cs=CounterConstraintFactory::getInstance().createDiaCounterConstraint( counter_id1, counter_id2, $4, $5);
    data->addPointer( COUNTER_CS, cs);
  }


}

;

assign_statement:
single_assign_statement
| assign_statement ',' single_assign_statement
;
single_assign_statement:
IDENTIFIER '=' const_expression
{
  if( getType( symbol_table[$1] )==CLOCK_T){
    int clock_id=data->getId( CLOCK_STR, symbol_table[$1] )+1;
    pair<int, int> *pp=new pair<int,int>(clock_id, $3 );
    data->addPointer( RESET_STR, pp);
    
  }else if(getType( symbol_table[$1] )==COUNTER_T ){
    int counter_id=uplayerData->getId( COUNTER_STR, symbol_table[$1]);
    SimpleCounterAction *cs=CounterActionFactory::getInstance( ).createSimpleCounterAction( counter_id, $3);
    data->addPointer( COUNTER_UPDATE, cs);
  }
  
}
|
IDENTIFIER '=' PARAM
{
  assert(getType(symbol_table[$1] )==COUNTER_T );
  int counter_id=uplayerData->getId( COUNTER_STR, symbol_table[$1]);

  int parameter_id=data->getId( PARAMETER_STR, symbol_table[$3]);

  SimpleCounterPAction *cs  =CounterActionFactory::getInstance( ).createSimpleCounterPAction( counter_id,  parameter_id);
  data->addPointer( COUNTER_UPDATE, cs);
  
}
const_expression:
CONSTANT
{
  $$=$1;
}
|
IDENTIFIER
{
  $$=0;
  if(data->hasValue(COUNTER_STR, symbol_table[$1]) ){
    $$=data->getValue(COUNTER_STR,symbol_table[$1]);
  }else if ( uplayerData->hasValue(COUNTER_STR, symbol_table[$1]) ){
    $$=uplayerData->getValue(COUNTER_STR,   symbol_table[$1]);
  }

}
;

variable_declaration
: type_specifier identifier_list ';'
{
  switch( $1){
    case 1:
      for( auto v: *$2){
        data->addValue(COUNTER_STR, v);
      }
      delete $2;
      break ;
    case 2:
      for( auto v: *$2){
        data->addValue(CLOCK_STR, v);
      }
      delete $2;
      break ;
      
    case 3:
      for( auto v: *$2){
        data->addValue(CHANNEL_STR, v, ONE2ONE);
      }
      delete $2;
      break ;
  }
}


| BROADCAST CHAN identifier_list ';'
{
  for( auto v: *$3){
    data->addValue(CHANNEL_STR, v, ONE2ALL);  
  }
  delete $3;
}

| BROADCAST CHAN IDENTIFIER '[' const_expression  ']' ';'
{
  for( int i=0; i< $5; i++ ){
    data->addValue(CHANNEL_STR, symbol_table[$3], ONE2ALL);  
  }
}

|  type_specifier IDENTIFIER '=' const_expression ';'
{
  switch( $1){
    case 1:
      data->addValue(COUNTER_STR, symbol_table[$2], $4);
      break;
    case 2:
      data->addValue(CLOCK_STR, symbol_table[$2], $4);
 
      break;

    case 3:
      data->addValue(CHANNEL_STR, symbol_table[$2], $4);

      break;
  }
  
}
| CONST type_specifier IDENTIFIER '=' const_expression ';'

{
  switch( $2){
    case 1:
      data->addValue(COUNTER_STR, symbol_table[$3], $5);

      break;
    case 2:
      data->addValue(CLOCK_STR, symbol_table[$3], $5);

      break;

    case 3:
      data->addValue(CHANNEL_STR, symbol_table[$3], $5);

      break;
  }
  
}



|  TYPEDEF INT '[' const_expression ',' const_expression ']' IDENTIFIER ';'
{

  vector<int> temp;
  for( int i=$4; i<=$6; i++  ){
    temp.push_back( i);
  }
  data->addIntArray(symbol_table[ $8],temp);
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

TYPE_T getType(string & name ){
  if(data->hasValue( CLOCK_STR, name) ){
    return CLOCK_T;
  }
  if(data->hasValue( PARAMETER_STR, name) ){
    return PARAMETER_T;
  }
  if( uplayerData->hasValue( COUNTER_STR, name)){
    return COUNTER_T;
  }
  return NO_T;
}
namespace graphsat{
  void parseProblem( const string &str, const UppaalData *pd,   UppaalData* d){
    uplayerData=pd;
    data=d;
    yyin=tmpfile();
    fputs( str.c_str(), yyin);
    rewind(yyin);
    yyparse();
    fclose(yyin);
  
  }
} 


