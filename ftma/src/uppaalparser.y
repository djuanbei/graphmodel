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

  
  extern void yyerror(const char *); 
  extern void yyerror(const string&);
  extern int yylex();
  extern int yyparse();
  extern FILE* yyin;
  TYPE_T getType(string & name );
  int getParameterId(string &name );
  
 %}

%union{
  int intVal;
  vector<string> * str_vec_pointer;
  COMP_OPERATOR com_op;
  
 }

%token <intVal> CONSTANT

%token<intVal> IDENTIFIER

%token<intVal> PARAM


%token<intVal> TEMPLATE


%type<intVal> const_expression


%type<intVal> type_specifier

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
  $$=1;
}
| CLOCK
{
  $$=2;
}
| BOOL
{
  $$=3;
}
| CHAN
{
  $$=4;
}
| URGENT CHAN
{
  $$=5;
}
| BROADCAST CHAN
{
  $$=6;
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
  void *cs;
  const  TYPE_T type=getType( symbol_table[$1]);
  if( type==CLOCK_T){
    int clock_id=current_data->getId( CLOCK_STR, symbol_table[$1])+1; //CLOCK ID START FROM 1
    if( EQ==$2){
      cs=new      INT_TAS_t::CS_t(clock_id, 0,  GE, $3 ); //x<= c
      current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);

      cs=new      INT_TAS_t::CS_t(clock_id, 0,  LE, $3 ); //x>= c
      current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);
      
    }else{
      cs=new      INT_TAS_t::CS_t(clock_id, 0,  $2, $3 ); //x< c
      current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);
    }

  }
  else if(type==COUNTER_T ){

    // TODO: parameter counter and global counter, local counter mapping
    
    //    int template_parameter_num= current_data->getPointNum(PARAMETER_STR );

    int counter_id=system_data->getId( INT_STR, symbol_table[$1]);

    if(system_data->hasValue(INT_STR,symbol_table[$1] )){
      
      current_data->addValue(USING_INT, symbol_table[$1], counter_id);
    }
      
    cs = InstanceFactory::getInstance().createDiaFreeCounterConstraint(counter_id, $2, $3);
    
    current_data->addPointer( INT_CS,INT_CS, cs);
  }
}
|
IDENTIFIER compare_relation  PARAM
{
  const  TYPE_T type=getType(   symbol_table[$1]);
  assert( COUNTER_T==type);
  int counter_id=system_data->getId( INT_STR, symbol_table[$1]);
  
  if(system_data->hasValue(INT_STR,symbol_table[$1] )){
    current_data->addValue(USING_INT,symbol_table[$1],counter_id );
  }
  
  int param_id=getParameterId(symbol_table[$3] );
    
  DiaFreeCounterPConstraint *cs=InstanceFactory::getInstance().createDiaFreeCounterPConstraint(counter_id, $2, param_id );
  current_data->addPointer( INT_CS,INT_CS, cs);
}
|
IDENTIFIER '-' IDENTIFIER  compare_relation  const_expression
{
  if(getType( symbol_table[$1])==CLOCK_T &&getType( symbol_table[$3])==CLOCK_T  ){
    int clock_id1=current_data->getId( CLOCK_STR, symbol_table[$1])+1;
    int clock_id2=current_data->getId( CLOCK_STR, symbol_table[$3])+1;
    if( $4==EQ){
      
      INT_TAS_t::CS_t *cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, GE,  $5  ); //x-y< rhs

      current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);
      
      cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, LE,  $5  ); //x-y< rhs

      current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);                     
    }else{
      INT_TAS_t::CS_t *cs=new      INT_TAS_t::CS_t(clock_id1, clock_id2, $4,  $5  ); //x-y< rhs
      current_data->addPointer( CLOCK_CS,CLOCK_CS, cs);                     
    }

  }
  else if(getType( symbol_table[$1])==COUNTER_T &&getType( symbol_table[$3])==COUNTER_T  ){
    int counter_id1=system_data->getId( INT_STR, symbol_table[$1]);
    int counter_id2=system_data->getId( INT_STR, symbol_table[$3]);
    
    if(system_data->hasValue(INT_STR,symbol_table[$1] )){
      current_data->addValue(USING_INT,symbol_table[$1],counter_id1 );
    }
    if(system_data->hasValue(INT_STR,symbol_table[$3] )){
      current_data->addValue(USING_INT,symbol_table[$3],counter_id2 );
    }
    
    DiaCounterConstraint *cs=InstanceFactory::getInstance().createDiaCounterConstraint( counter_id1, counter_id2, $4, $5);
    current_data->addPointer( INT_CS,INT_CS, cs);
  }
  

}
|
PARAM
{
  if(PARAMETER_T==getType(symbol_table[ $1] ) ){
    
  }
  
}
|
'!' PARAM
{
  
  
  
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
    int clock_id=current_data->getId( CLOCK_STR, symbol_table[$1] )+1;
    pair<int, int> *pp=new pair<int,int>(clock_id, $3 );
    current_data->addPointer( RESET_STR,RESET_STR, pp);
    
  }else if(getType( symbol_table[$1] )==COUNTER_T ){
    int counter_id=system_data->getId( INT_STR, symbol_table[$1]);
    if(system_data->hasValue(INT_STR,symbol_table[$1] )){
      current_data->addValue(USING_INT,symbol_table[$1],counter_id );
    }
    
    SimpleCounterAction *cs=InstanceFactory::getInstance( ).createSimpleCounterAction( counter_id, $3);
    current_data->addPointer( INT_UPDATE,INT_UPDATE, cs);
  }
  
}
|
IDENTIFIER '=' PARAM
{
  assert(getType(symbol_table[$1] )==COUNTER_T );
  int counter_id=system_data->getId( INT_STR, symbol_table[$1]);

  if(system_data->hasValue(INT_STR,symbol_table[$1] )){
    current_data->addValue(USING_INT,symbol_table[$1], counter_id );
  }
    
  int parameter_id=getParameterId(symbol_table[$3] );


  SimpleCounterPAction *cs  =InstanceFactory::getInstance( ).createSimpleCounterPAction( counter_id,  parameter_id);
  current_data->addPointer( INT_UPDATE,INT_UPDATE, cs);
  
}
|
IDENTIFIER '=' TEMPLATE '(' identifier_list ')'
{
  
  TaDec * ta=new TaDec( );
  ta->name=symbol_table[ $1];
  
  for(auto name: ( *( $5) ) ){
    
    TYPE_T type=getType(name );
    int id=-1;
    if(COUNTER_T==type ){
      id= system_data->getId(INT_STR,  name );
    }else if(BOOL_T==type ){
      id= system_data->getId( BOOL_STR, name);
    }else if( CHANNEL_T==type){
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
|
IDENTIFIER
{
  $$=0;
  if(current_data->hasValue(INT_STR, symbol_table[$1]) ){
    $$=current_data->getValue(INT_STR,symbol_table[$1]);
  }else if ( system_data->hasValue(INT_STR, symbol_table[$1]) ){
    $$=system_data->getValue(INT_STR,   symbol_table[$1]);
  }
}
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
    case 1:
      for( auto v: *$2){
        current_data->setValue(INT_DEC_STR, v);
      }
      delete $2;
      break ;
    case 2:
      for( auto v: *$2){
        current_data->setValue(CLOCK_DEC_STR, v);
      }
      delete $2;
      break ;

    case 3:
      for( auto v: *$2){
        current_data->setValue(BOOL_DEC_STR, v);
      }
      delete $2;
      break ;
      
    case 4:
      for( auto v: *$2){
        current_data->setValue(CHAN_DEC_STR, v, ONE2ONE_CH);
      }
      delete $2;
      break ;

    case 5:

      for( auto v: *$2){
        current_data->setValue(CHAN_DEC_STR, v, URGENT_CH);
      }
      delete $2;
      break ;
    case 6:
      for( auto v: *$2){
        current_data->setValue(CHAN_DEC_STR, v, BROADCAST_CH);
      }
      delete $2;
      break ;
      
  }
}

| type_specifier IDENTIFIER '[' const_expression  ']' ';'
{
  
  string name=symbol_table[ $2];
  
 switch( $1){
   case 1:
     for( int i=0; i< $4; i++){
       current_data->setValue( INT_DEC_STR, arrayToVar(name, i) );
     }
     break;
   case 2:
     for( int i=0; i< $4; i++){
       current_data->setValue( CLOCK_DEC_STR, arrayToVar(name, i ));
     }
     break;

   case 3:
     for( int i=0; i< $4; i++){
       current_data->setValue( BOOL_DEC_STR, arrayToVar(name, i ));
     }
     break;
     
   case 4:
     for( int i=0; i< $4; i++){
       current_data->setValue( CHAN_DEC_STR, arrayToVar(name, i ), ONE2ONE_CH);
     }
     break;

   case 5:
     for( int i=0; i< $4; i++){
       current_data->setValue( CHAN_DEC_STR, arrayToVar(name, i ), URGENT_CH);
     }
     break;
   case 6:
     for( int i=0; i< $4; i++){
       current_data->setValue( CHAN_DEC_STR, arrayToVar(name, i ), BROADCAST_CH);
     }
     break;
 }
}


|  type_specifier IDENTIFIER '=' const_expression ';'
{
  switch( $1){
    case 1:
      current_data->setValue(INT_DEC_STR, symbol_table[$2], $4);
      break;
    case 2:
      current_data->setValue(CLOCK_DEC_STR, symbol_table[$2], $4);
      break;
      
    case 3:
      current_data->setValue(BOOL_DEC_STR, symbol_table[$2], $4);
      break;
  }
  
}
| CONST type_specifier IDENTIFIER '=' const_expression ';'

{
  switch( $2){
    case 1:
      current_data->setValue(INT_DEC_STR, symbol_table[$3], $5);
      break;
    case 2:
      current_data->setValue(CLOCK_DEC_STR, symbol_table[$3], $5);
      break;

    case 3:
      current_data->addValue(BOOL_DEC_STR, symbol_table[$3], $5);
      break;
  }
  
}


|  TYPEDEF INT '[' const_expression ',' const_expression ']' IDENTIFIER ';'
{

  vector<int> temp;
  for( int i=$4; i<=$6; i++  ){
    temp.push_back( i);
  }
  current_data->addIntArray(symbol_table[ $8],temp);
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
      vector<void*> sys_dec_vec=system_data->getPoints(TEMPLATE_STR, name );
      sys->ta_list.push_back((TaDec*)sys_dec_vec[ 0] );
      
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

TYPE_T getType(string & name ){
  
  if(current_data->hasPointer( PARAMETER_STR, name) ){
    return PARAMETER_T;
  }

  
  if(current_data->hasValue( CLOCK_STR, name) ){
    return CLOCK_T;
  }

  if(current_data->hasValue( INT_STR, name)){
    return COUNTER_T;
  }
  if(current_data->hasValue( BOOL_STR, name) ){
    return BOOL_T;
  }
  if( current_data->hasValue( CHAN_STR, name)){
    return CHANNEL_T;
  }

  if( system_data->hasValue(CLOCK_STR, name )){
    return CLOCK_T;
  }
  
  
  if( system_data->hasValue( INT_STR, name)){
    return COUNTER_T;
  }

  if(system_data->hasValue( BOOL_STR, name) ){
    return BOOL_T;
  }

  if( system_data->hasValue( CHAN_STR, name)){
    return CHANNEL_T;
  }
  
  if(system_data->hasValue( TEMPLATE_STR, name) ){
    return TEMPLATE_T;
  }
  
  return NO_T;
}


int getParameterId(  string &name){
  int param_id=current_data->getPointerId( PARAMETER_STR, name);
  return  param_id;
}


namespace graphsat{
  void parseProblem( const string &str,  UppaalTemplateData *pd,   UppaalTemplateData* d){
    system_data=pd;
    current_data=d;
    yyin=tmpfile();
    fputs( str.c_str(), yyin);
    rewind(yyin);
    yyparse();
    fclose(yyin);
  
  }
} 


