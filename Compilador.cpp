#include<stdio.h>
#include <cstring>
#include <fstream>  // Función de biblioteca de flujo de archivos
#include <iostream>
//Variables
using namespace std;
struct tok
{
	public:
		char token[100]=""; //token.
		int IDlex; //codigo de token.
};
char reserveWord[32][20]={ // Codificación de palabra reservada 1-32
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};
char singleOperator[25][10]={  // Codificación de operador de un solo byte 33-57
    "+","-","*","/","<",">",
    ";","(",")","^",",","#",
    "|","%","~","[]","]","{}",
    "}"," ",":","!","=","&",
    "."
};
static char mutilOperator[17][10]={  // Codificación de operador de doble byte 58-74
    "++","--","==","<=",">=","<<",
    ">>","&&","||","::","!=","\\",
	"\?","\"","\'","\n","\t"
};

struct tok Analisis_Lexico();
void Analisis_Sintactico();
void Analisis_Semantico();
void Generacion_de_codigo();
void Optimosacon();
void Generacion_de_codigo_objeto();
void Errores();
void Tabla_de_simbolos();

//Funciones de analisis lexico
int isLetter(char);
int isDigit(char);
int isOperater(char);
int isReserve(char []);
int isMultiO(char []);

int main(int argc, char **argv)
{
	
	return 0;
}
// Determinar si es una letra
int isLetter(char ch){
    if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')){return 1;}
    return 0;
}
// Determina si es un número
int isDigit(char ch){
    if((ch>='0'&&ch<='9')){return 300;}
    return 0;
}
// Determinar si es un operador y devolver el valor relativo del código
int isOperater(char ch){
    int i;
    for(i=0;i<sizeof(singleOperator)/10;i++){
        if(ch==singleOperator[i][0]){
            return i+33;
        }
    }
    return -1;
}
// Determinar si es una palabra reservada y devolver el valor relativo del código
int isReserve(char token[]){
    int i;
    for(i=0;i<sizeof(reserveWord)/20;i++){
        if(strcmp(token,reserveWord[i])==0){
            return i+1;
        }
    }
    return 100;
}
int isMultiO(char token[]){
    int i;
    for(i=0;i<sizeof(mutilOperator)/10;i++){
        if(strcmp(token,mutilOperator[i])==0){
            return i+64;
        }
    }
    return -1;
}
struct tok  Analisis_Lexico(const char *pal,int &n,int tam)
{
	tok a;
	strcpy(a.token,"");
	int i,c=0,IDL;
	for(i=n;i<=tam;i++,c++)
	{
		if( (c != 0 && IDL == 200) )
		{
			IDL=isMultiO(a.token);
			a.token[c]=pal[i];
			n++;
			break;
		}
		if(c==0 && pal[i] == '\\' && pal[i+1] == '\n' )
		{
			n+=2;
			continue;
		}
		IDL=isLetter(pal[i]);
		if(IDL==1)
		{
			a.token[c]=pal[i];
		}
		else
		{
			IDL=isDigit(pal[i]);
			if(IDL==300)
			{
				a.token[c]=pal[i];
				if(a.token[c+1]=='.')
				{
					a.token[c+1]=pal[i+1];
					i++;
					c++;
				}
			}
			else
			{
				IDL=isOperater(pal[i]);
				if(IDL>0)
				{
					
					if(c==0)
					{
						if(IDL == 52)
						{
							n++;
							break;
						}
						else if(IDL==33 && pal[i] == pal[i+1] )
						{
							IDL=200;
							n++;
							continue;
						}
						else if(IDL==34 && pal[i] == pal[i+1] )
						{
							IDL=200;
							n++;
							continue;
						}
						else if(IDL==37 && (pal[i] == pal[i+1] || pal[i+1] == '=') )
						{
							IDL=200;
							n++;
							continue;
						}
						else if(IDL==38 && (pal[i] == pal[i+1] || pal[i+1] == '=') )
						{
							IDL=200;
							n++;
							continue;
						}
						else if(IDL == 45 && pal[i] == pal[i+1] )
						{
							IDL=200;
							n++;
							continue;
						}
						else if(IDL == 53 && pal[i] == pal[i+1] )
						{
							IDL=200;
							n++;
							continue;
						}
						else if(IDL == 54 && pal[i] == '=' )
						{
							IDL=200;
							n++;
							continue;
						}
						else if(IDL == 55 && pal[i] == pal[i+1]  )
						{
							IDL=200;
							n++;
							continue;
						}
						else if(IDL == 56 && pal[i] == pal[i+1]  )
						{
							IDL=200;
							n++;
							continue;
						}
						a.token[c]=pal[i];
						n++;
						a.token[c+1]='\0';
						break;
					}
					else
					{
						if(IDL == 52)
						{
							n++;
							a.token[c+1]='\0';
						}
						IDL=isReserve(a.token);
						n++;
						a.token[c+1]='\0';
						break;
					}
				}
			}
		}
		n++;
	}
	a.IDlex=IDL;
	return a;
}

