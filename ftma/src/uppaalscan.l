D            [0-9]
L            [a-zA-Z_]
H            [a-fA-F0-9]
E            [-]?{D}+


%{
#include <stdio.h>

void count();
void comment();
void commentLine();
int check_type();

%}

%%
"/*"            { comment();  return (COMMENT); }
"//"              { commentLine(); return (COMMENT); }



"break"            { count(); return(BREAK); }
"case"            { count(); return(CASE); }
"const"            { count(); return(CONST); }
"urgent"           { count(); return(URGENT); }
"broadcast"        { count(); return(BROADCAST); }
"meta"             { count(); return (META);}
"continue"        { count(); return(CONTINUE); }
"default"        { count(); return(DEFAULT); }
"do"            { count(); return(DO); }
"else"            { count(); return(ELSE); }
"for"            { count(); return(FOR); }
"if"            { count(); return(IF); }
"int"            { count(); return(INT); }
"clock"            { count(); return(CLOCK); }
"chan"            { count(); return(CHAN); }
"bool"           { count();  return(BOOL);}
"return"        { count(); return(RETURN); }
"struct"        { count(); return(STRUCT); }
"switch"        { count(); return(SWITCH); }
"typedef"        { count(); return(TYPEDEF); }
"void"            { count(); return(VOID);   }
"while"           { count(); return(WHILE);  }
"system"          { count(); return(SYSTEM); }
"true"         { count(); return(TRUE_YY); }
"false"       { count(); return(FALSE_YY); }
"argument"    {count(); return (ARGUMENT); }

{L}({L}|{D})*        { count(); return(check_type()); }



{E}       { count();
  yylval.int_value=atoi(yytext);
  return(CONSTANT);
}


\"(\\.|[^\\"])*\"    { count(); return(STRING_LITERAL); }

"..."            { count(); return(ELLIPSIS); }
">>="            { count(); return(RIGHT_ASSIGN); }
"<<="            { count(); return(LEFT_ASSIGN); }
"+="            { count(); return(ADD_ASSIGN); }
"-="            { count(); return(SUB_ASSIGN); }
"*="            { count(); return(MUL_ASSIGN); }
"/="            { count(); return(DIV_ASSIGN); }
"%="            { count(); return(MOD_ASSIGN); }
"&="            { count(); return(AND_ASSIGN); }
"^="            { count(); return(XOR_ASSIGN); }
"|="            { count(); return(OR_ASSIGN); }
">>"            { count(); return(RIGHT_OP); }
"<<"            { count(); return(LEFT_OP); }
"++"            { count(); return(INC_OP); }
"--"            { count(); return(DEC_OP); }
"->"            { count(); return(PTR_OP); }
"&&"            { count(); return(AND_OP); }
"||"            { count(); return(OR_OP); }
"<="            { count(); return(LE_OP); }
">="            { count(); return(GE_OP); }
"=="            { count(); return(EQ_OP); }
"!="            { count(); return(NE_OP); }
";"            { count(); return(';'); }
("{"|"<%")        { count(); return('{'); }
("}"|"%>")        { count(); return('}'); }
","            { count(); return(','); }
":"            { count(); return(':'); }
"="            { count(); return('='); }
"("            { count(); return('('); }
")"            { count(); return(')'); }
("["|"<:")        { count(); return('['); }
("]"|":>")        { count(); return(']'); }
"."            { count(); return('.'); }
"&"            { count(); return('&'); }
"!"            { count(); return('!'); }
"~"            { count(); return('~'); }
"-"            { count(); return('-'); }
"+"            { count(); return('+'); }
"*"            { count(); return('*'); }
"/"            { count(); return('/'); }
"%"            { count(); return('%'); }
"<"            { count(); return('<'); }
">"            { count(); return('>'); }
"^"            { count(); return('^'); }
"|"            { count(); return('|'); }
"?"            { count(); return('?'); }

[ \t\v\n\f]        { count(); }
.            { /* ignore bad characters */ }

%%

/*
 when input return 0, then terminal
 */
int yywrap()
{
    return(1);
}


void comment()
{
    char c, c1;
    
    loop:
    while ((c = yyinput()) != '*' && c != 0)
    putchar(c);
    
    if ((c1 = yyinput()) != '/' && c != 0)
    {
        unput(c1);
        goto loop;
    }
    
    if (c != 0)
    putchar(c1);
}

void commentLine()
{
    char c;
    while ((c = yyinput()) != '\n' && c != 0);
 }
        

int column = 0;

void count()
{
    int i;
    
    for (i = 0; yytext[i] != '\0'; i++)
    if (yytext[i] == '\n')
    column = 0;
    else if (yytext[i] == '\t')
    column += 8 - (column % 8);
    else
    column++;
    
    //ECHO;
}

int findSym(const string& name ){
   vector<string>::iterator it=find(symbol_table.begin( ), symbol_table.end( ), name );
   if( it==symbol_table.end( )){
     return -1;
   }
   return it-symbol_table.begin( );

}

/**
@return PARAM if sym is one of template parameter;
 TEMPLATE if sym is one name of template;
 IDENTIFIER otherwise.
*/

int check_type()
{
    /*
     * pseudo code --- this is what it should check
     *
     *    if (yytext == type_name)
     *        return(TYPE_NAME);
     *
     *    return(IDENTIFIER);
     */
    
    /*
     *    it actually will only return IDENTIFIER
     */
   string  sym=string( yytext);
   // if(current_data->hasValue(INT_STR, sym) ){
   //   yylval.int_value=current_data->getValue(INT_STR, sym);
   //   return CONSTANT;
   // }else if ( system_data->hasValue(INT_STR, sym) ){
   //   yylval.int_value=system_data->getValue(INT_STR,   sym);
   //   return CONSTANT;
   // }
   
   Token *token=new Token();
   token->id=findSym( sym);
   token->xml_name=sym;
   token->code_name=sym;

   yylval.token_value= token;

   
   if(find(symbol_table.begin( ), symbol_table.end( ), sym ) ==symbol_table.end( ) ){
      symbol_table.push_back(sym);
   }
   
  TYPE_T type=current_data->getType(sym);
  
switch(type){
case 
}
  if(NO_T==type){
    return IDENTIFIER;
  }
   if(current_data->hasPointer(PARAMETER_T,sym)){
      int parameter_id=model_parser->getParameterId(current_data, sym);
      ParaElement* p = (ParaElement *) current_data->getPointer( PARAMETER_T, sym );
      if(p->is_ref){
          return REF_PARAMETER_YY;
      }
      return PARAMETER_YY;
   }
   if(system_data->hasValue(TEMPLATE_T, sym ) ){

     return TEMPLATE_YY;
   }
   /**
    * The clock variable only declare in template section and can not declare as global variable
    *
    */
   
   if ( current_data->hasValue( CLOCK_T, sym ) ) {

     return CLOCK_YY;
   }
   string code_name = current_data->getVarFullName( sym );
   token->code_name=code_name;
  
   if ( system_data->hasValue( INT_T, code_name ) ) {
     return INT_YY;
   }
   
   if ( system_data->hasValue( BOOL_T, code_name ) ) {
     return BOOL_YY;
   }
   
   if ( system_data->hasValue( CHAN_T, code_name ) ) {
     return CHAN_YY;
   }
  
   token->code_name=sym;
   if ( system_data->hasValue( INT_T, sym ) ) {
     return INT_YY;
   }
   
   if ( system_data->hasValue( BOOL_T, sym ) ) {
     return BOOL_YY;
   }
   
   if ( system_data->hasValue( CHAN_T, sym ) ) {
     return CHAN_YY;
   }
   return(IDENTIFIER);
   
}



