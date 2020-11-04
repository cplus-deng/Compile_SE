#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#define FileNameSize 50
#define MaxSize 100+10
#define VtSize 8
char buf[MaxSize];
int m_index=0;
enum symbol{
	Vn=0,
	plus,
	multi,
	i,
	left,
	right,
	s,
	error
};
typedef enum symbol sym;

sym reduce_i(sym topOfStack);
sym reduce_exp(sym a1,sym a2,sym a3);

sym stack[MaxSize];
int Top=0;
bool isEmpty(){
	return Top==0;
}
sym top(){
	if(stack[Top-1]!=Vn){
		return stack[Top-1];
	}
	else {
		return stack[Top-2];
	}
}
void push(sym a){
	stack[Top++]=a;
if(a!=Vn){
	char p='#';
	switch(a){
		case plus:{
			p='+';
			break;
		}
		case multi:{
			p='*';
			break;
		}
		case i:{
			p='i';
			break;
		}
		case left:{
			p='(';
			break;
		}
		case right:{
			p=')';
			break;
		}
		default:{
			break;
		}
	}
	if(p!='#'){
		printf("I%c\n",p);
	}
}
}
sym pop(){
	return stack[--Top];
}
int pri_matrix[VtSize][VtSize];

void init_matrix(){
	pri_matrix[plus][plus]=1;pri_matrix[plus][multi]=-1;pri_matrix[plus][i]=-1;pri_matrix[plus][left]=-1;pri_matrix[plus][right]=1;pri_matrix[plus][s]=1;
	pri_matrix[multi][plus]=1;pri_matrix[multi][multi]=1;pri_matrix[multi][i]=-1;pri_matrix[multi][left]=-1;pri_matrix[multi][right]=1;pri_matrix[multi][s]=1;
	pri_matrix[i][plus]=1;pri_matrix[i][multi]=1;pri_matrix[i][i]=-2;pri_matrix[i][left]=-2;pri_matrix[i][right]=1;pri_matrix[i][s]=1;
	pri_matrix[left][plus]=-1;pri_matrix[left][multi]=-1;pri_matrix[left][i]=-1;pri_matrix[left][left]=-1;pri_matrix[left][right]=0;pri_matrix[left][s]=1;
	pri_matrix[right][plus]=1;pri_matrix[right][multi]=1;pri_matrix[right][i]=-2;pri_matrix[right][left]=-2;pri_matrix[right][right]=1;pri_matrix[right][s]=1;
	pri_matrix[s][plus]=-1;pri_matrix[s][multi]=-1;pri_matrix[s][i]=-1;pri_matrix[s][left]=-1;pri_matrix[s][right]=-1;pri_matrix[s][s]=-1;
}
int pri(sym inStack,sym outStack){
	return pri_matrix[inStack][outStack];
}
sym trans(char ch){
	if(ch=='i'){
		return i;
	}
	else if(ch=='+'){
		return plus;
	}
	else if(ch=='*'){
		return multi;
	}
	else if(ch=='('){
		return left;
	}
	else if(ch==')'){
		return right;
	}
	else if(ch=='#'){
		return s;
	}
	else{
		return error;
	}
}
sym transAll(char ch){
	if(ch=='E'||ch=='F'||ch=='T'||ch=='N'){
		return Vn;
	}
	else if(ch=='#'){
		return s;
	}
	else{
		return trans(ch);
	}
}
int reduce();
int main(int argc,char *argv[]) {
	FILE* in;
	char File[FileNameSize];
	if(argc>1){
		strcpy(File,argv[1]);
	}
	else{
		//strcpy(File,"test.txt");
		perror("No Input!");
		return -1;
	}
	in=fopen(File,"r");
	if(in==NULL) {
		perror("Open Fail!");
		return -1;
	}
	if(fgets(buf,MaxSize,in)==NULL) return -1;
	//______________________________________Unix___________________
	buf[strlen(buf)-2]='#';
	//______________________________________Windows__________________
	//buf[strlen(buf)-1]='#';
	//buf[strlen(buf)]='\n';
	//__________________________________________________________________
	push(s);
	//printf("%s\n",buf);
	//printf("%d\n",strlen(buf));
	init_matrix();
	while(buf[m_index]!='\n'&&buf[m_index]!='\r'){
		char ch=buf[m_index];
		//为非终结符 
		if(transAll(ch)==Vn){
			push(Vn);
			m_index++;
			continue;
		}
		else if(transAll(ch)==error){
			//printf("Why E? :%c\n",ch);
			//if(ch=='\n'||ch=='\r'){
			//	printf("回车\n");
			//}
			printf("E\n");
			return -1;
		}
		//为终结符 
		else{
			int priority=pri(top(),trans(ch));
			//如果栈顶优先级低于ch,则入栈 
			if(priority==-1||priority==0){
				push(trans(ch));
			}
			//如果栈顶优先级高，则归约 
			else if(priority==1){
				if(reduce()==0){
					printf("RE\n");
					return -1;
				}
				printf("R\n");
				continue;
			}
			else{
				printf("E\n");
				return -1;
			}
		}
		m_index++;
	}
}

int reduce(){
	//i->N
	if(Top-1>0 && stack[Top-1]==i){
		stack[Top-1]=Vn;
	}
	//N*N->N
	else if(Top-3>0 && stack[Top-1]==Vn && stack[Top-2]==multi && stack[Top-3]==Vn){
		stack[Top-3]=Vn;
		Top-=2;
	}
	//N+N->N
	else if(Top-3>0 && stack[Top-1]==Vn && stack[Top-2]==plus && stack[Top-3]==Vn){
		stack[Top-3]=Vn;
		Top-=2;
	}
	//(N)->N
	else if(Top-3>0 && stack[Top-1]==right && stack[Top-2]==Vn && stack[Top-3]==left){
		stack[Top-3]=Vn;
		Top-=2;
	}
	else{
		return 0;
	}
	return 1;
}
