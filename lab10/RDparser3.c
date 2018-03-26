#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//#include "parsecheck.c"
#include "token.c"

int ptr=0,alert=0;

TOKEN *token;
void program();
void declarations();
void statement_list();
void data_type();
void identifier_list();
void A();
void statement();
void assign_stat();
void decision_stat();
void looping_stat();
void expn();
void eprime();
void simple_expn();
void seprime();
void term();
void tprime();
void factor();
void relop();
void addop();
void mulop();

void getToken(){
	if(alert==0)
		token = getNextToken();
	else
		alert=0;
}

void program(){
	printf("Entered program function\n");
	getToken();
	printf("Program-> %s %s\n",token->type,token->value);
	if(strcmp(token->value,"main")==0){
		getToken();
		printf("Program-> %s %s\n",token->type,token->value);
		if(strcmp(token->value,"(")==0){
			getToken();
			printf("Program-> %s %s\n",token->type,token->value);
			if(strcmp(token->value,")")==0){
				getToken();
				printf("Program-> %s %s\n",token->type,token->value);
				if(strcmp(token->value,"{")==0){
					declarations();
					statement_list();
					getToken();
					printf("Program-> %s %s\n",token->type,token->value);
					if(strcmp(token->value,"}")==0){
						return;
					}
					else
					{
						printf("Error in program function } expected\n");
						exit(0);
					}
				}
				else
				{
					printf("Error in program function { expected\n");
					exit(0);
				}
			}
			else
			{
				printf("Error in program function ) expected\n");
				exit(0);
			}
		}
		else
		{
			printf("Error in program function ( expected\n");
			exit(0);
		}
	}
	else
	{
		printf("Error in program function main expected\n");
		exit(0);
	}
}

void declarations(){
	printf("Entered declarations\n");
	getToken();
	if(strcmp(token->value,"int")==0 || strcmp(token->value,"char")==0 ){
		printf("Declarations -> %s %s\n",token->type,token->value);
		alert=1;
		data_type();
		identifier_list();
		getToken();
		printf("Declarations -> %s %s\n",token->type,token->value);
		if(strcmp(token->value,";")==0){
			declarations();
			return;
		}
		else{

			printf("Error in declarations function ; expected\n");
			exit(0);
		}
	}
	else{
		alert=1;
		return;
	}
}

void data_type(){
	printf("Entered data_type function\n");
	getToken();
	printf("Data-Type -> %s %s\n",token->type,token->value);	
	if(strcmp(token->value,"int")==0 || strcmp(token->value,"char")==0){
		printf("Successful data_type returned\n");
		return;
	}
	else{
		printf("Error in data_type function int or char expected\n");
		exit(0);
	}
}

void identifier_list(){
	printf("Entered identifier_list function\n");
	getToken();
	printf("Identfier-List -> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"id")==0){
		A();
		return;
	}
	else{
		printf("Error in identifier_list function identifier expected\n");
		exit(0);
	}
}

void A(){
	printf("Entered A function\n");
	getToken();
	printf("Identfier-List -> %s %s\n",token->type,token->value);
	if(strcmp(token->value,",")==0){
		identifier_list();
		return;
	}
	else if(strcmp(token->value,"[")==0){
		getToken();
		printf("Identfier-List -> %s %s\n",token->type,token->value);
		if(strcmp(token->type,"number")==0){
			getToken();
			printf("Identfier-List -> %s %s\n",token->type,token->value);
			if(strcmp(token->value,"]")==0){
				getToken();
				if(strcmp(token->value,",")==0){
					identifier_list();
					return;
				}
				else{
					alert=1;
					return;
				}

			}
			else{
				printf("Error in function identifier_list ] expected\n");
				exit(0);
			}
		}
		else{
			printf("Error in function identifier_list size of the array should be a number\n");
			exit(0);
		}
	}
	else if(strcmp(token->value,";")==0){
		alert=1;
		return;
	}
	else{
		printf("Error in A function , or [ expected\n");
		exit(0);
	}
}

void statement_list(){
	printf("Entered statement_list function\n");
	getToken();
	alert=1;
	if(strcmp(token->type,"id")==0 || strcmp(token->type,"kw")==0){
		statement();
		statement_list();
	}
	else{
		return;
	}
}

void statement(){
	printf("Entered statement function\n");
	getToken();
	alert=1;
	printf("Statement -> %s %s\n",token->type,token->value);
	if(strcmp(token->value,"if")==0){
		decision_stat();
		return;
	}
	else if(strcmp(token->value,"while")==0 || strcmp(token->value,"for")==0){
		looping_stat();
		return;
	}
	else if(strcmp(token->type,"id")==0){
		assign_stat();
		getToken();
		printf("Statement -> %s %s\n",token->type,token->value);
		if(strcmp(token->value,";")==0){
			return;
		}
		else{
			printf("Error in statement function ; expected\n");
			exit(0);
		}
	}
	else{
		printf("Error in statement function if or while or for or identifier expected\n");
		exit(0);
	}
	
}

void assign_stat(){
	printf("Entered assign_stat function\n");
	getToken();
	printf("Assign-stat -> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"id")==0){
		getToken();
		printf("Assign-stat -> %s %s\n",token->type,token->value);
		if(strcmp(token->value,"=")==0){
			expn();
			return;
		}
		else{
			printf("Error in assign_stat function = expected\n");
			exit(0);
		}
	}
	else{
		printf("Error in assign_stat function existing identifier expected\n");
		exit(0);
	}

}

void decision_stat(){
	printf("Entered decision_stat function\n");
	getToken();
	printf("decision_stat -> %s %s\n",token->type,token->value);
	if(strcmp(token->value,"if")==0){
		getToken();
		printf("decision_stat -> %s %s\n",token->type,token->value);
		if(strcmp(token->value,"(")==0){
			expn();
			getToken();
			printf("decision_stat -> %s %s\n",token->type,token->value);
			if(strcmp(token->value,")")==0){
				getToken();
				printf("decision_stat -> %s %s\n",token->type,token->value);
				if(strcmp(token->value,"{")==0){
					statement_list();
					getToken();
					printf("decision_stat -> %s %s\n",token->type,token->value);
					if(strcmp(token->value,"}")==0){
						return;
					}
					else{
						printf("Error in decision_stat function '}' expected\n");
						exit(0);
					}
				}
				else{
					printf("Error in decision_stat function '{' expected\n");
					exit(0);
				}
			}
			else{
				printf("Error in decision_stat function ')' expected\n");
				exit(0);
			}
		}
		else{
			printf("Error in decision_stat function '(' expected\n");
			exit(0);
		}

	}
	else{
		printf("Error in decision_stat function 'if' keyword expected\n");
		exit(0);
	}

}

void looping_stat(){
	printf("Entered looping_stat function\n");
	getToken();
	printf("looping_stat -> %s %s\n",token->type,token->value);
	if(strcmp(token->value,"while")==0){
		getToken();
		printf("looping_stat -> %s %s\n",token->type,token->value);
		if(strcmp(token->value,"(")==0){
			expn();
			getToken();
			printf("looping_stat -> %s %s\n",token->type,token->value);
			if(strcmp(token->value,")")==0){
				getToken();
				printf("looping_stat -> %s %s\n",token->type,token->value);
				if(strcmp(token->value,"{")==0){
					statement_list();
					getToken();
					printf("looping_stat -> %s %s\n",token->type,token->value);
					if(strcmp(token->value,"}")==0){
						return;
					}
					else{
						printf("Error in looping_stat function '}' expected\n");
						exit(0);
					}
				}
				else{
					printf("Error in looping_stat function '{' expected\n");
					exit(0);
				}
			}
			else{
				printf("Error in looping_stat function ')' expected\n");
				exit(0);
			}
		}
		else{
			printf("Error in looping_stat function '(' expected\n");
			exit(0);
		}

	}
	else if(strcmp(token->value,"for")==0){
		getToken();
		printf("looping_stat -> %s %s\n",token->type,token->value);
		if(strcmp(token->value,"(")==0){
			assign_stat();
			getToken();
			printf("looping_stat -> %s %s\n",token->type,token->value);
			if(strcmp(token->value,";")==0){
				expn();
				getToken();
				printf("looping_stat -> %s %s\n",token->type,token->value);
				if(strcmp(token->value,";")==0){
					assign_stat();
					getToken();
					printf("looping_stat -> %s %s\n",token->type,token->value);
					if(strcmp(token->value,")")==0){
						getToken();
						printf("looping_stat -> %s %s\n",token->type,token->value);
						if(strcmp(token->value,"{")==0){
							statement_list();
							getToken();
							printf("looping_stat -> %s %s\n",token->type,token->value);
							if(strcmp(token->value,"}")==0){
								return;
							}
							else{
								printf("Error in looping_stat function '}' expected\n");
								exit(0);
							}
						}
						else{
							printf("Error in looping_stat function '{' expected\n");
							exit(0);
						}
					}
					else{
						printf("Error in looping_stat function ')' expected\n");
						exit(0);
					}
				}
				else{
					printf("Error in looping_stat function ';' expected\n");
					exit(0);
				}
			}
			else{
				printf("Error in looping_stat function ';' expected\n");
				exit(0);
			}
		}
		else{
			printf("Error in looping_stat function '(' expected\n");
			exit(0);
		}
	}
	else{
		printf("Error in looping_stat function , for or while keyword expected\n");
		exit(0);
	}
}

void expn(){
	printf("Entered expn function\n");
	getToken();
	if(strcmp(token->type,"id")==0 || strcmp(token->type,"number")==0){
		alert=1;
		simple_expn();
		eprime();
		return;
	}
	else{
		printf("error in function expn identifier or number expected\n");
		exit(0);
	}
}

void eprime(){
	printf("Entered eprime function\n");
	getToken();
	alert=1;
	if(strcmp(token->type,"relop")==0){
		relop();
		simple_expn();
	}
	else{
		return;
	}
	
}

void simple_expn(){
	printf("Entered simple_expn function\n");
	getToken();
	if(strcmp(token->type,"id")==0 || strcmp(token->type,"number")==0){
		alert=1;
		term();
		seprime();
		return;
	}
	else{
		printf("error in function simple_expn identifier or number expected\n");
		exit(0);
	}
}

void seprime(){
	printf("Entered seprime function\n");
	getToken();
	alert=1;
	//printf("Seprime -> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"addop")==0){		
		addop();
		term();
		seprime();
		return;
	}
	else{
		return;
	}
}

void term(){
	printf("Entered term function\n");
	getToken();
	//printf("Term -> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"id")==0 || strcmp(token->type,"number")==0){
		alert=1;
		factor();
		tprime();
		return;
	}
	else{
		printf("error in function term identifier or number expected\n");
		exit(0);
	}
}

void tprime(){
	printf("Entered tprime function\n");
	getToken();
	alert=1;
	//printf("Tprime -> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"mulop")==0){		
		mulop();
		factor();
		tprime();
		return;
	}
	else{
		return;
	}
}

void factor(){
	printf("Entered factor function\n");
	getToken();
	printf("Factor-> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"id")==0 || strcmp(token->type,"number")==0){
		return;
	}
	else{
		printf("error in function factor identifier or number expected\n");
		exit(0);
	}
}

void relop(){
	printf("Entered relop function\n");
	getToken();
	printf("Relop-> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"relop")==0){
		return;
	}
	else{
		printf("error in function factor relop expected\n");
		exit(0);
	}
}

void addop(){
	printf("Entered addop function\n");
	getToken();
	printf("Addop-> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"addop")==0){
		return;
	}
	else{
		printf("error in function factor addop expected\n");
		exit(0);
	}
}

void mulop(){
	printf("Entered mulop function\n");
	getToken();
	printf("Mulop-> %s %s\n",token->type,token->value);
	if(strcmp(token->type,"mulop")==0){
		return;
	}
	else{
		printf("error in function factor mulop expected\n");
		exit(0);
	}
}

int main(){
	openfiles("input.c");
	program();
	getToken();
	if(strcmp(token->type,"EOT")==0)
		printf("Parsing Successfully completed\n");
	else
		printf("Can't parse\n");
	return 0;
}