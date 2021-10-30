#include<stdio.h>
#include <cstring>
#include <fstream>  // Función de biblioteca de flujo de archivos
#include <iostream>
#include<ctype.h>
#include<stdio.h>
#include<windows.h>
//definir longitudes - tamaños de la tabla de simbolos
#define L_NOMBRE 100
#define N 5

using namespace std;
//estructuras
//tokens
struct tok
{
	public:
		char token[100]; //token.
		int IDlex; //codigo de token.
		tok(const char *, int);
};
//nodos
struct nodoA
{
	public:
		int IDSint;
		char token[100];
		struct nodoA *hijo;
		struct nodoA *hermano;
};
//-Datos
struct registro
{
	int ID;
	char nombre[L_NOMBRE];
	char hambito[100];
	char contenido[100];
	char Tipo[20];
	char info[1000];
};
//-Nodos
struct nodoT
{
	registro reg;
	nodoT *ant, *sig;
};
//---Alias
typedef struct nodoT nodoT;
typedef struct registro registro;
typedef struct nodoA *ABB;//nombre para un apuntador de un nodo

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
    "|","%","~","[","]","{",
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

//Funciones de analisis sintactico
void inserta( ABB,tok );

//Funciones de latabla de simbolos
void agregar();
void buscar();
void borrar();
int hash(char *key);
void acomodar(registro r, int llave);
void mostar(registro r, int llave);//Temporal
nodoT *buscar(char *nombre,int llave);
void eliminar(nodoT *q, int llave);

nodoT *tabla[]={NULL,NULL,NULL,NULL,NULL}; //Listas enlazadas

int main(int argc, char **argv)
{
    
	
	return 0;
}
tok::tok(const char *_Token="", int _IDLex=0)
{
	strcpy(this->token,_Token);
	this->IDlex=_IDLex;
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
void inserta( ABB &arbol,tok token_a)
{
	int i;
	if(arbol==NULL)
	{
		arbol = new(struct nodoA);
		arbol->IDSint=token_a.IDlex;
		strcpy(arbol->token,token_a.token);
		arbol->hijo=NULL;
		arbol->hermano=NULL;
	}
	else
	{
		
	}
}
ABB Analisis_Sintactico(const char *Codigo_Fuente)
{
	ABB arbol=NULL;
	ABB subarbol=NULL;
	int pos=0;
	const int tama = strlen(Codigo_Fuente);
	tok Token_Sintactico = Analisis_Lexico(Codigo_Fuente,pos,tama);
	tok temp,am("Hambito",500),b("Estrucura",510),c("Exprecion",520),d("Declaracion",530),e("Condicion",540);
	if(Token_Sintactico.IDlex==16)
	{
		
	}
}
// - Agregar registros
void agregar()
{
	registro r;
	cout<<"Nombre: ";
	cin>>r.nombre;
	cout<<"Informacion: ";
	cin>>r.info;
	int llave = hash(r.nombre);
	//ver casillas y registros en el que se guarda el dato
	cout<<"Llave: "<<llave<<":"<<r.nombre;
	acomodar(r,llave);
}
	
// - Buscar registros
void buscar()
{
	//buscar lista correxpondiente del nombre
	registro r;
	char nombre[L_NOMBRE];
	cout<<"Nombre a buscar: ";
	cin>>nombre;
	int llave = hash(nombre);
	//ver casillas y registros en el que se guardo el dato
	cout<<"Llave: "<<llave<<":"<<nombre;
	nodoT *resultado = buscar(nombre,llave);
	if(resultado!=NULL)
	{
		mostar(resultado->reg, llave);
	}
	else
	{
		cout<<"\n no se encuentra";
	}
}
// - Eliminar registros
void borrar()
{
	registro r;
	char nombre[L_NOMBRE];
	cout<<"Nombre a buscar: ";
	cin>>nombre;
	int llave = hash(nombre);
	//ver casillas y registros en el que se guardo el dato
	cout<<"Llave: "<<llave<<":"<<nombre;
	nodoT *resultado = buscar(nombre,llave);
	if(resultado!=NULL)
	{
		//mostrar que existe
		mostar(resultado->reg, llave);
		eliminar(resultado, llave);
	}
	else
	{
		cout<<"\nno se encuentra";
	}
}
// - Otras
int hash(char *key)
{
	//funcion crear en cual de las lista se guardara
	int longitud=strlen(key)-1;//longitud de la cadena
	int suma = 0;
	for(int i=0; i<longitud; i++)
	{
		suma+=key[i];//suma de los ascii
	}
	return suma%N; //valores desde 0 a 4 para acomodarlo en la tabla
	//llave
}
void acomodar(registro r, int llave)
{
	//acomodar en la tabla
	nodoT *nuevo = new(nodoT);
	nuevo->reg = r;
	nuevo->ant = NULL;
	nuevo->sig = tabla[llave];
	if(tabla[llave])
	tabla[llave]->ant=nuevo;
	tabla[llave]=nuevo; //acomodar datos en la casilla
}
nodoT *buscar(char *nombre,int llave)
{
	//ver si existe el registro.
	nodoT *p = tabla[llave];
	while(p)
	{
		if(strcmp(p->reg.nombre,nombre)==0)
		{
			return p;
		}
		p=p->sig;
	}
}
void mostar(registro r, int llave)
{
	//mostrar registros.
	cout<<"\nID: "<<r.ID;
	cout<<"\nnombre: "<<r.nombre;
	cout<<"\nhambito: "<<r.hambito;
	cout<<"\ncontenido: "<<r.contenido;
	cout<<"\ntipo: "<<r.Tipo;
	cout<<"\ninformacion: "<<r.info<<endl;
	char hambito[100];
	char contenido[100];
	char Tipo[20];
	char info[1000];
}
void eliminar(nodoT *q, int llave)
{
	if(q->sig)
	{
		q->sig->ant=q->ant;
	}
	if(q->ant)
	{
		q->ant->sig=q->sig;
	}
	else
	{
		tabla[llave]=q->sig;
	}
	free(q);
}
