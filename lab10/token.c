#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct token{
	char type[50];
	char value[100];
}TOKEN;

typedef struct node{
	char id[100];
	int num;
	char type[100];
	int size;
	char rtype[100];
	int num_args;
	struct node *next;
	char scope;
	char arguments[100];
}NODE;

int k = 0;
FILE *fp1,*fp2;
char *argsBuf;
char c;
int flag=0,no_of_char_in_args=0,scope_of_variable=1;
int curlyBracket = 0;
NODE **hashTable;
int args=0;
int toggleFunc = 0;
char updateFuncName[50];
char prev_lexeme[50];
char buf[50];
void printSymbolTable(NODE **hashTable){
	int i;
	NODE *cur;
	printf("Name\tType\tSize\tScope\tNo_Args\tReturn Type\t        Arguments\n");
	for(i=0;i<50;i++){
		cur = hashTable[i];
		cur = cur->next;
		while(cur){
			printf("%s\t%s\t%d\t%c\t%d\t%s\t%s\n",cur->id,cur->type,cur->size,cur->scope,cur->num_args,cur->rtype,cur->arguments);
			cur = cur->next;
		}
	}	
}
NODE *createNode(char *str,int n,char *tp,int sz,char *rtp,int scope_of_variable){
	NODE *temp;
	temp = (NODE *)malloc(sizeof(NODE));
	strcpy(temp->id,str);
	temp->num = n;
	temp->next=NULL;
	strcpy(temp->type,tp);
	strcpy(temp->rtype,rtp);
	temp->size = sz;
	temp->num_args=0;
	if(scope_of_variable)
		temp->scope = 'G';
	else
		temp->scope = 'L';
	return temp;
}
int hashFunc(char *str){
	int sum=0,i=0;
	while(str[i]!='\0'){
		sum+=str[i++];
	}
	return sum%50;
}
int iskw(char *buf,int n){
	char kw[][100] = {"if","else","int", "float","char","double","return","for","while","case","switch","break"};
	int num = 12;
	int i=0;
	for(i=0;i<12;i++){
		if(strcmp(kw[i],buf)==0){
			return i+1;
		}
	}
	return 0;
}
int search_insert(NODE **hashTable,char *str,char *prev_lexeme,int scope_of_variable){
	int hashValue = hashFunc(str);
	NODE *cur = hashTable[hashValue];
	NODE *prev = cur;
	int size = 0;
	int tp = 0;
	cur = cur->next;
	while(cur){
		prev = cur;
		if(strcmp(str,cur->id)==0)
			return cur->num;
		cur = cur->next;
	}
	tp = iskw(prev_lexeme,strlen(prev_lexeme));
	if(prev_lexeme && tp){
		if(tp <=4)
			size = 4;
		else if(tp==5)
			size = 1;
		else
			size = 8;
		prev->next = createNode(str,++k,prev_lexeme,size,"no return type",scope_of_variable);
	}
	else{
		prev_lexeme[strlen(prev_lexeme)-1] = '\0';
		tp = iskw(prev_lexeme,strlen(prev_lexeme));
		if(tp)
			prev->next = createNode(str,++k,"func",size,prev_lexeme,1);
		else
			prev->next = createNode(str,++k,"func",size,"unknown return type",1);
	}

	//printf("%d %s %s %d %s %c\n",k,prev->next->id,prev->next->type,prev->next->size,prev->next->rtype,prev->next->scope);
	return k;
}
void update_args(NODE **hashTable,char *str,int num,char *strargs){
	int hashValue = hashFunc(str);
	NODE *cur = hashTable[hashValue];
	NODE *prev = cur;
	int flag = 0;
	cur = cur->next;
	while(cur){
		prev = cur;
		if(strcmp(str,cur->id)==0){
			flag=1;
			break;
		}
		cur = cur->next;
	}
	if(flag){
		cur->num_args = num;
		if(strcmp(strargs,"")==0)
			strcpy(cur->arguments,"NULL");
		else
			strcpy(cur->arguments,strargs);
	}
	//printf("Changed: %d %s %s %d %s %d\n",cur->num,cur->id,cur->type,cur->size,cur->rtype,cur->num_args);
}
int isnum1(char c){
	if(c<='9' && c>='0')
			return 1;
	return 0;
}
int isalpha1(char c){
	if((c>='a' && c<='z')|| (c>='A'&& c<='Z'))
			return 1;
	return 0;
}
NODE **createhashTable(){
	NODE **hashTable = (NODE **)malloc(50*sizeof(NODE *));
	int i=0;
	for(i=0;i<50;i++){
		hashTable[i] = createNode("Dummy",1,"no type",0,"no return type",1);
	}
	return hashTable;
}
void openfiles(char *str){
	fp1 = fopen(str,"r");
	fp2 = fopen("test_output.c","w");
	hashTable = createhashTable(hashTable);
	if(fp1==NULL || fp2==NULL){
		printf("Can't open the file\n");
		exit(0);
	}
}
TOKEN *initializeToken(){
	TOKEN *temp = (TOKEN *)malloc(sizeof(TOKEN));
	return temp;
}
TOKEN *getNextToken(){
	while((c = fgetc(fp1))!=EOF){
		if(c==' '||c=='\n'||c=='\t'){
			fprintf(fp2, "%c",c);
			//continue;
		}
		else if(c=='\"'){
			if(toggleFunc)
				no_of_char_in_args++;
			int i = 1;
			buf[0] = c;
			while((c=fgetc(fp1))!=EOF && c!='\"'){
				buf[i] = c;
				i++;
			}
			buf[i++] = '\"';
			buf[i] = '\0';
			fprintf(fp2,"<literal,%s>",buf);
			strcpy(prev_lexeme,"literal");
			TOKEN *newTok = initializeToken();
			strcpy(newTok->type,"literal");
			strcpy(newTok->value,buf);
			return newTok;
		}
		else if(isnum1(c)){
			if(toggleFunc)
				no_of_char_in_args++;
			int i=0;
			buf[i++] = c;
			while((c=fgetc(fp1))!=EOF && isnum1(c)){
				buf[i] = c;
				i++;
			}
			buf[i] = '\0';
			fseek(fp1,-1,SEEK_CUR);
			fprintf(fp2,"<number,%s>",buf);
			strcpy(prev_lexeme,"number");
			TOKEN *newTok = initializeToken();
			strcpy(newTok->type,"number");
			strcpy(newTok->value,buf);
			return newTok;
		}
		else if(c=='_'|| isalpha1(c)){
			if(toggleFunc)
				no_of_char_in_args++;
			int i=0;
			buf[0] = c;
			i++;
			while((c=fgetc(fp1))!=EOF && (isalpha1(c)||isnum1(c)||c=='_')){
				buf[i] = c;
				i++;
			}
			if(c=='('){
				prev_lexeme[strlen(prev_lexeme)] = 'f';
				prev_lexeme[strlen(prev_lexeme)+1] = '\0';
				toggleFunc = 1;
				flag=1;
				scope_of_variable=0;
				args = 0;
				argsBuf = (char *)malloc(100*sizeof(char));
			}
			fseek(fp1,-1,SEEK_CUR);
			buf[i] = '\0';
			if(iskw(buf,i)){
				fprintf(fp2,"<kw,%s>",buf);
				TOKEN *newTok = initializeToken();
				strcpy(newTok->type,"kw");
				strcpy(newTok->value,buf);
				flag=0;
				strcpy(prev_lexeme,buf);
				return newTok;
			}
			else{
				char buflocal[50];
				int loc = search_insert(hashTable,buf,prev_lexeme,scope_of_variable);
				fprintf(fp2,"<id,%d>",loc);
				TOKEN *newTok = initializeToken();
				strcpy(newTok->type,"id");
				strcpy(newTok->value,buf);
				if(toggleFunc){
					if(flag){
						strcpy(updateFuncName,buf);
						flag=0;
					}
					else{
						sprintf(buflocal,"<id,%d> ",loc);
						strcat(argsBuf,buflocal);
					}
				}
				return newTok;
			}
		}
		else if(c=='<'||c=='='||c=='>'||c=='+'||c=='-'||c=='*'||c=='/'||c=='%'||c=='!'){
			char d = fgetc(fp1);
			char temp[3];
			temp[0] = c;
			temp[1] = d;
			temp[2] = '\0';
			if(c=='<'&& (d=='>'||d=='=')){
				fprintf(fp2,"<relop,%c%c>",c,d);
				strcpy(prev_lexeme,"relop");
				TOKEN *newTok = initializeToken();
				strcpy(newTok->type,"relop");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='>' && d=='='){
				fprintf(fp2,"<relop,%c%c>",c,d);
				strcpy(prev_lexeme,"relop");
				TOKEN *newTok = initializeToken();
				strcpy(newTok->type,"relop");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='='&& d=='='){
				fprintf(fp2,"<relop,%c%c>",c,d);
				strcpy(prev_lexeme,"relop");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"relop");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='+' && d=='+'){
				fprintf(fp2,"<op,%c%c>",c,d);
				strcpy(prev_lexeme,"op");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"op");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='-'&& d=='-'){
				fprintf(fp2,"<op,%c%c>",c,d);
				strcpy(prev_lexeme,"op");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"op");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='!' && d=='='){
				fprintf(fp2,"<relop,%c%c>",c,d);
				strcpy(prev_lexeme,"relop");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"relop");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='+' && d=='='){
				fprintf(fp2,"<op,%c%c>",c,d);
				strcpy(prev_lexeme,"op");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"op");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='<' || c=='>'){
				temp[1] = '\0';
				fprintf(fp2,"<relop,%c>",c);
				fseek(fp1,-1,SEEK_CUR);
				strcpy(prev_lexeme,"relop");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"relop");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='+' || c=='-'){
				temp[1] = '\0';
				fprintf(fp2,"<addop,%c>",c);
				fseek(fp1,-1,SEEK_CUR);
				strcpy(prev_lexeme,"addop");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"addop");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else if(c=='*' || c=='/' || c=='%'){
				temp[1] = '\0';
				fprintf(fp2,"<mulop,%c>",c);
				fseek(fp1,-1,SEEK_CUR);
				strcpy(prev_lexeme,"mulop");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"mulop");
				strcpy(newTok->value,temp);
				return newTok;
			}
			else{
				temp[1] = '\0';
				fprintf(fp2,"<op,%c>",c);
				fseek(fp1,-1,SEEK_CUR);
				strcpy(prev_lexeme,"op");
				TOKEN *newTok = initializeToken();;
				strcpy(newTok->type,"op");
				strcpy(newTok->value,temp);
				return newTok;
			}
		}
		else if(c=='('||c==')'||c=='{'||c=='}'||c=='['||c==']'||c==';'||c==':'||c==','){
			fprintf(fp2,"<symbol,%c>",c);
			char temp[2];
			temp[0] = c;
			temp[1] = '\0';
			TOKEN *newTok = initializeToken();;
			strcpy(newTok->type,"symbol");
			strcpy(newTok->value,temp);
			if(c!=','){
				strcpy(prev_lexeme,""+c);
			}

			if(c=='}'){
				curlyBracket = 0;
				scope_of_variable=1;
			}
			else if(c=='{'){
				scope_of_variable=0;
				curlyBracket=1;
			}
			else if(c==','){
				if(toggleFunc)
					args++;
			}
			else if(c==')'){
				toggleFunc = 0;
				//printf("About to up:%d\n",no_of_char_in_args);
				if(no_of_char_in_args>0)
					args++;
				no_of_char_in_args=0;
				if(!curlyBracket)
					scope_of_variable=1;
				//printf("%s %s\n",updateFuncName,argsBuf);
				update_args(hashTable,updateFuncName,args,argsBuf);
				//free(argsBuf);
			}
			return newTok;
		}
	}
	//printSymbolTable(hashTable);
	fclose(fp1);
	fclose(fp2);
	TOKEN *newTok = initializeToken();
	strcpy(newTok->type,"EOT"); 
	return newTok;

}
