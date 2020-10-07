#include<string.h> 
#include<ctype.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#define TokenSize 20
#define FileNameSize 20

#define READ if(!read()) break;

char 	token[TokenSize];
int 	counter;
char 	ch;
int 	num;
FILE	*in;
enum 	sym{
	Ident, 	//标识符  0 
	Begin,	//1 保留字 
	End,	//2 
	For,	//3 
	If,		//4 
	Then,	//5 
	Else, 	//6 保留字 
	Int, 	//无符号整数 
	Colon,
	Plus,
	Star,
	Comma,
	LParenthesis,
	RParenthesis,
	Assign, //赋值符号 := (kao 
	Unknown
};
typedef enum sym SYMBOL;
SYMBOL symbol;
bool isSpace()
{
	if(ch-' '==0)
	{
		return true;
	}
	return false;
}

bool isTab()
{
	if(ch-'\t'==0)
	{
		return true;
	}
	return false;
}

bool isNewline()
{
	if(ch-'\n'==0)
	{
		return true;
	}
	return false;
}
bool isEnter()
{
	if(ch-'\r'==0)
	{
		return true;
	}
	return false;
}

int clearToken()
{
	memset(token,'\0',TokenSize);
	return 1;
}

bool isLetter() 
{
	if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))
	{
		return true;
	}
	return false;
}

bool isDigit()
{
	if(ch>='0'&&ch<='9')
	{
		return true;
	}
	return false;
}

bool isColon()
{
	if(ch==':')
	{
		return true;
	}
	return false;
}

bool isComma()
{
	if(ch==',')
	{
		return true;
	}
	return false;
}

bool isSemi()
{
	if(ch==';')
	{
		return true;
	}
	return false;
}

bool isEqu()
{
	if(ch=='=')
	{
		return true;
	}
	return false;
}

bool isPlus()
{
	if(ch=='+')
	{
		return true;
	}
	return false;
}

bool isMinus()
{
	if(ch=='-')
	{
		return true;
	}
	return false;
}

bool isDivi()
{
	if(ch=='/')
	{
		return true;
	}
	return false;
}

bool isStar()
{
	if(ch=='*') 
	{
		return true;
	}
	return false;
}

bool isLpar()
{
	if(ch=='(')
	{
		return true;
	}
	return false;
}

bool isRpar()
{
	if(ch==')')
	{
		return true;
	}
	return false;
}

int catToken()
{
	token[counter++]=ch;
	token[counter]='\0';
	return counter;
}

void retract()
{
	fseek(in,-1,SEEK_CUR);
}
bool read()
{
	ch=fgetc(in);
	if(feof(in))
		return false;
	//printf("%c ",ch);
	return true;
}
int reserve(char* t,int c)
{
	if(strcmp(t,"BEGIN")==0) return 1;
	else if(strcmp(t,"END")==0) return 2;
	else if(strcmp(t,"FOR")==0) return 3;
	else if(strcmp(t,"IF")==0)  return 4;
	else if(strcmp(t,"THEN")==0) return 5;
	else if(strcmp(t,"ELSE")==0) return 6;
	else return 0;
}
int SourceCodeLength()
{
	fseek(in,0,SEEK_SET);
	char c=getc(in);
	int length=0;
	while(!feof(in))
	{
		length++;
		c=getc(in);
	}
	fseek(in,0,SEEK_SET);
	return length;
}
void printSymbol(SYMBOL s,char* t)
{
	// 0标识符 7无符号整数
	if(s==Ident) 
	{
		printf("Ident(%s)\n",t);
	}
	else if(s==Int)
	{
		char* p=t;
		while(*p=='0') p++;
		if(strlen(p)==0){
			printf("Int(0)\n");
		}
		else{
			printf("Int(%s)\n",p);
		}
		
	}
	else if(s==Unknown)
	{
		printf("Unknown\n");
	}
	else if(s==Colon)
	{
		printf("Colon\n");
	}
	else if(s==Plus)
	{
		printf("Plus\n");
	}
	else if(s==Star)
	{
		printf("Star\n");
	}
	else if(s==Comma)
	{
		printf("Comma\n");
	}
	else if(s==LParenthesis)
	{
		printf("LParenthesis\n");
	}
	else if(s==RParenthesis)
	{
		printf("RParenthesis\n");
	}
	else if(s==Assign) 
	{
		printf("Assign\n");
	}
	else{
		int l=strlen(t);
		int i;
		printf("%c",t[0]);
		for(i=1;i<l;i++){
			printf("%c",tolower(t[i]));
		}
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	char filename[FileNameSize];
	if(argc>1)
	{
		strcpy(filename,argv[1]);
	}
	else
	{
		//printf("No input.\n");
		//return -1;
		strcpy(filename,"test.txt");
	}
	
	in=fopen(filename,"r");
	if(in==NULL)
	{
		perror("open fail.");
		return -1;
	}
	//read();
	while(1)
	{
		clearToken();counter=0; 
		//getchar();
		//READ
		if(!read()) break;
		while(isSpace() || isNewline() || isTab() || isEnter())
		{
			//READ
			if(!read()) break;
		}
		if(feof(in)) 
		{
			break;
		}
		if(isLetter())
		{
			while(isLetter() || isDigit())
			{
				catToken();
				//READ
				if(!read()) break;
			}
			if(feof(in)) 
			{
				break;
			}
			retract();
			int resultValue=reserve(token,counter);
			if(resultValue==0) symbol=Ident; //标识符 
			else symbol=resultValue; 		 //保留字 
		}
		else if(isDigit())
		{
			while(isDigit())
			{
				catToken();
				//READ
				if(!read()) break;
			}
			if(feof(in)) 
			{
				break;
			}
			retract();
			symbol=Int;
		}
		else if(isColon())
		{
			catToken();
			//READ
			if(!read()) break;
			if(isEqu()) {
				symbol=Assign;
				catToken();
			}
			else {
				retract();symbol=Colon;
			}
				
		}
		else if(isPlus()) 	{
			symbol=Plus;
			catToken();
		}
		//else if(isMinus()) 	symbol=Minus;
		else if(isStar()) {
			symbol=Star;
			catToken();
		}
		else if(isLpar()){
			symbol=LParenthesis;
			catToken();
		} 	
		else if(isRpar()) {
			symbol=RParenthesis;
			catToken();
		} 	
		else if(isComma()) {
			symbol=Comma;
			catToken();
		}	
		else{
			symbol=Unknown;
			printf("Unknown\n");
			exit(0);
		} 
		
		printSymbol(symbol,token);
		
	}
	if(strlen(token)>0)
	{
		printSymbol(symbol,token);
	}
	fclose(in);return 0;
}

