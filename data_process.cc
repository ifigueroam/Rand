/* =================================================================
 *
 * Archivo: 			data_process.c
 * Descripción:
 * 								Programa de ordenamiento y procesamiento de datos
 * 								Dado un achivo de datos, se obtienen los valores
 * 								identificadores y pesos de la lectura del archivo, 
 * 								luego se procede a ordenarlos por alguno de los
 * 								tres metodos de ordenamiento: Quick Sort, Merge
 * 								Sort o Heap Sort, dado por un parametro de entrada
 * 								donde se calcula el tiempo usado en el cálculo 
 * 								del algoritmo a través de la función predefinida
 * 								CLOCK_MONOTONIC de la librería time.h de C/C++.
 *
 *	Autor: 				Iván Figueroa Monsalve
 *	mail:					ivfigue@gmail.com
 *	Asignatura:		Laboratorio de Complejidad de Algoritmos 
 *  Profesor:			Oscar Rojas
 *  mail: 				oscar.rojas.d@usach.cl
 *
 *  Versión: 			1.0
 *  Compilador: 	gcc
 *  Ejecución: 		g++ data_process.cc -o dtp -pthread && ./dtp n
 *  							donde n es el método de ordenamiento elegido
 *  							el cual puede ser: merge, quick o heap sort
 *  ================================================================
 */

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <sys/time.h>
#include <sched.h>
#include <pthread.h>
#include <map>
#include <algorithm>
#include <utility>
#include <string>
#include <math.h>
using namespace std;
struct timespec START_TIME_A0, END_TIME_A0, LATENCIA_A0; 
struct timespec START_TIME_A1, END_TIME_A1, LATENCIA_A1; 

typedef pair<int,int> PAIR;
vector<PAIR> PAIRS;

/* Calcular tiempo 
 *
 */
void diff_time_Ns( timespec *t_fin, timespec *t_ini, timespec *delta ){
  if( ( (*t_fin).tv_nsec - (*t_ini).tv_nsec ) < 0 ){
    if( (*t_fin).tv_sec == (*t_ini).tv_sec ){
      (*delta).tv_sec  = 0;
      (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
    }
    else{
      (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec - 1;
      (*delta).tv_nsec = 1000000000 + (*t_fin).tv_nsec - (*t_ini).tv_nsec;
    }
  }
  else{
    if( (*t_fin).tv_sec == (*t_ini).tv_sec ){
      (*delta).tv_sec  = 0;
      (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
    }
    else{
      (*delta).tv_sec  = (*t_fin).tv_sec - (*t_ini).tv_sec;
      (*delta).tv_nsec = (*t_fin).tv_nsec - (*t_ini).tv_nsec;
    }
  }
}

/* Asignar Cores de CPU
 *
 *
 */
void AsignarCPUThread(int cpuId){
	int s, j;
	cpu_set_t cpuset;
	pthread_t thread;
	int speid=cpuId;
	thread = pthread_self();
	CPU_ZERO(&cpuset);
	CPU_SET(speid, &cpuset);
	s = pthread_setaffinity_np(thread,sizeof(cpu_set_t), &cpuset);
}

/* Crear Tokens
 *
 */
void Tokenize(const string& str, vector<string>& tokens, const string& delimiters = " "){
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  string::size_type pos     = str.find_first_of(delimiters, lastPos);
  while (string::npos != pos || string::npos != lastPos)
  {
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    lastPos = str.find_first_not_of(delimiters, pos);
    pos = str.find_first_of(delimiters, lastPos);
  }
}

/*Funcion de Comparacion
 *
 */
bool comp(const PAIR&C1, const PAIR&C2){
  return C1.second > C2.second;
}

/* Funcion Quick_Transcriptor
 *
 */
void Quick_Transcriptor(){
  vector<string> TKN;
  vector<PAIR>::iterator IT;
  vector<PAIR> TEMP;
  vector<PAIR> RANK;
  string linea;
  int id_doc, proc, numdoc, porcendoc, countRankList;
	long int tiempoComputoNs;
  char datos_wand[100];
  char transcript_wand[100];
  char ranklist_wand[100];
  sprintf(datos_wand,"datos_wand/lista_UB_256_Nodo_0.dat");
  ifstream WandParsing (datos_wand);
  if(WandParsing.is_open() && WandParsing.good()){
    cout<<"\n Leyendo archivo de datos: \n "<<datos_wand<<" ..."<<endl;
    while( getline( WandParsing,linea ) && !WandParsing.eof() )
    {
      TKN.clear();
      TEMP.clear();
      Tokenize( linea, TKN, " ");
      numdoc = atoi(TKN[2].c_str());
      porcendoc = trunc(numdoc*0.1);
      numdoc = numdoc*2;
      countRankList = 0;
      for(int i = 1; i < numdoc; i = i+2 ){
        id_doc = atoi(TKN[i+2].c_str());
        proc   = atoi(TKN[i+3].c_str());
        PAIRS.push_back(make_pair(id_doc,proc));
        TEMP.push_back(make_pair(id_doc,proc));
      }
      //mandar a ordenar
		  clock_gettime(CLOCK_MONOTONIC, &START_TIME_A1);
      sort(TEMP.begin(),TEMP.end(),comp);
		  clock_gettime(CLOCK_MONOTONIC, &END_TIME_A1);
      //extraer el 10% de TEMP y guardar en archivo
      for(IT = TEMP.begin(); IT != TEMP.end() && countRankList < porcendoc; ++IT){
        RANK.push_back(make_pair(IT->first,IT->second));
        countRankList++;
      }
    }
    WandParsing.close();
  }
  sprintf(transcript_wand,"datos_wand/lista_UB_256_Nodo_0xT.dat");
  ofstream WandTranscript (transcript_wand);
  if(WandTranscript.is_open() && WandTranscript.good()){
    cout<<"\n Escribiendo datos de transcripcion A_0 a archivo: \n "<<transcript_wand<<endl;
		clock_gettime(CLOCK_MONOTONIC, &START_TIME_A0);
    sort(PAIRS.begin(),PAIRS.end(),comp);
		clock_gettime(CLOCK_MONOTONIC, &END_TIME_A0);
    for(IT = PAIRS.begin(); IT != PAIRS.end(); ++IT)
      WandTranscript<<IT->first<<" "<<IT->second<<endl;
    WandTranscript.close();
  }      
  sprintf(ranklist_wand,"datos_wand/lista_UB_256_Nodo_0xR.dat");
  ofstream WandRankList (ranklist_wand);
  if(WandRankList.is_open() && WandRankList.good()){
    cout<<"\n Escribiendo datos ranking por lista A_1 a archivo: \n "<<ranklist_wand<<endl;
    for(IT = RANK.begin(); IT != RANK.end(); ++IT)
      WandRankList<<IT->first<<" "<<IT->second<<endl;
    WandRankList.close();
  }
	diff_time_Ns(&END_TIME_A0,&START_TIME_A0,&LATENCIA_A0); 
	tiempoComputoNs=LATENCIA_A0.tv_sec*1000000000+LATENCIA_A0.tv_nsec;
	cout <<endl<< "tiempo ordenamiento quicksort A0 en Nano Segundos: "<<tiempoComputoNs;
	diff_time_Ns(&END_TIME_A1,&START_TIME_A1,&LATENCIA_A1); 
	tiempoComputoNs=LATENCIA_A1.tv_sec*1000000000+LATENCIA_A1.tv_nsec;
	cout <<endl<< "tiempo ordenamiento quicksort A1 en Nano Segundos: "<<tiempoComputoNs;
}

/* Funcion Merge_Transcriptor
 *
 */
void Merge_Transcriptor(){
  vector<string> TKN;
  vector<PAIR>::iterator IT;
  vector<PAIR> TEMP_A;
  vector<PAIR> TEMP_B;
  vector<PAIR> TEMP_C;
  vector<PAIR> RANK;
  string linea;
  int id_doc, proc, numdoc, meddoc, porcendoc, countRankList;
	long int tiempoComputoNs;
  char datos_wand[100];
  char transcript_wand[100];
  char ranklist_wand[100];
  sprintf(datos_wand,"datos_wand/lista_UB_256_Nodo_0.dat");
  ifstream WandParsing (datos_wand);
  if(WandParsing.is_open() && WandParsing.good()){
    cout<<"\n Leyendo archivo de datos: \n "<<datos_wand<<" ..."<<endl;
    while( getline( WandParsing,linea ) && !WandParsing.eof() )
    {
      TKN.clear();
      TEMP_A.clear();
      TEMP_B.clear();
      Tokenize( linea, TKN, " ");
      numdoc = atoi(TKN[2].c_str());
      porcendoc = trunc(numdoc*0.1);
      meddoc = numdoc/2;
      numdoc = numdoc*2;
      countRankList = 0;
      for(int i = 1; i < numdoc; i = i+2 ){
        id_doc = atoi(TKN[i+2].c_str());
        proc   = atoi(TKN[i+3].c_str());
        PAIRS.push_back(make_pair(id_doc,proc));
        if(id_doc%porcendoc == 0)
          TEMP_A.push_back(make_pair(id_doc,proc));
        else
          TEMP_B.push_back(make_pair(id_doc,proc));
      }
      //mandar a ordenar
      sort(TEMP_A.begin(), TEMP_A.end()+meddoc);
      sort(TEMP_B.begin(), TEMP_B.end()+meddoc);
      merge(TEMP_A.begin(),TEMP_A.end()+meddoc,TEMP_B.begin(),TEMP_B.end()+meddoc,TEMP_C.begin());
      //extraer el 10% de TEMP y guardar en archivo
      for(IT = TEMP_C.begin(); IT != TEMP_C.end() && countRankList < porcendoc; ++IT){
        RANK.push_back(make_pair(IT->first,IT->second));
        countRankList++;
      }
    }
    WandParsing.close();
  }
  sprintf(transcript_wand,"datos_wand/lista_UB_256_Nodo_0xT.dat");
  ofstream WandTranscript (transcript_wand);
  if(WandTranscript.is_open() && WandTranscript.good()){
    cout<<"\n Escribiendo datos de transcripcion A_0 a archivo: \n "<<transcript_wand<<endl;
    for(IT = PAIRS.begin(); IT != PAIRS.end(); ++IT)
      WandTranscript<<IT->first<<" "<<IT->second<<endl;
    WandTranscript.close();
  }      
  sprintf(ranklist_wand,"datos_wand/lista_UB_256_Nodo_0xR.dat");
  ofstream WandRankList (ranklist_wand);
  if(WandRankList.is_open() && WandRankList.good()){
    cout<<"\n Escribiendo datos ranking por lista A_1 a archivo: \n "<<ranklist_wand<<endl;
    for(IT = RANK.begin(); IT != RANK.end(); ++IT)
      WandRankList<<IT->first<<" "<<IT->second<<endl;
    WandRankList.close();
  }
}

/* Funcion Heap_Transcriptor
 *
 */
void Heap_Transcriptor(){
  vector<string> TKN;
  vector<PAIR>::iterator IT;
  vector<PAIR> TEMP;
  vector<PAIR> RANK;
  string linea;
  int id_doc, proc, numdoc, porcendoc, countRankList;
	long int tiempoComputoNs;
  char datos_wand[100];
  char transcript_wand[100];
  char ranklist_wand[100];
  sprintf(datos_wand,"datos_wand/lista_UB_256_Nodo_0.dat");
  ifstream WandParsing (datos_wand);
  if(WandParsing.is_open() && WandParsing.good()){
    cout<<"\n Leyendo archivo de datos: \n "<<datos_wand<<" ..."<<endl;
    while( getline( WandParsing,linea ) && !WandParsing.eof() )
    {
      TKN.clear();
      TEMP.clear();
      Tokenize( linea, TKN, " ");
      numdoc = atoi(TKN[2].c_str());
      porcendoc = trunc(numdoc*0.1);
      numdoc = numdoc*2;
      countRankList = 0;
      make_heap(TEMP.begin(),TEMP.end());
      for(int i = 1; i < numdoc; i = i+2 ){
        id_doc = atoi(TKN[i+2].c_str());
        proc   = atoi(TKN[i+3].c_str());
        PAIRS.push_back(make_pair(id_doc,proc));
        TEMP.push_back(make_pair(id_doc,proc));
        push_heap(TEMP.begin(),TEMP.end());
      }
      //mandar a ordenar
		  clock_gettime(CLOCK_MONOTONIC, &START_TIME_A1);
      sort_heap(TEMP.begin(),TEMP.end());
		  clock_gettime(CLOCK_MONOTONIC, &END_TIME_A1);
      reverse(TEMP.begin(),TEMP.end());
      //extraer el 10% de TEMP y guardar en archivo
      for(IT = TEMP.begin(); IT != TEMP.end() && countRankList < porcendoc; ++IT){
        RANK.push_back(make_pair(IT->first,IT->second));
        countRankList++;
      }
    }
    WandParsing.close();
  }
  sprintf(transcript_wand,"datos_wand/lista_UB_256_Nodo_0xT.dat");
  ofstream WandTranscript (transcript_wand);
  if(WandTranscript.is_open() && WandTranscript.good()){
    cout<<"\n Escribiendo datos de transcripcion A_0 a archivo: \n "<<transcript_wand<<endl;
    make_heap(PAIRS.begin(),PAIRS.end());
    for(IT = PAIRS.begin(); IT != PAIRS.end(); ++IT)
      WandTranscript<<IT->first<<" "<<IT->second<<endl;
    push_heap(PAIRS.begin(),PAIRS.end());
		clock_gettime(CLOCK_MONOTONIC, &START_TIME_A0);
    sort_heap(PAIRS.begin(),PAIRS.end());
		clock_gettime(CLOCK_MONOTONIC, &END_TIME_A0);
    reverse(PAIRS.begin(),PAIRS.end());
    WandTranscript.close();
  }      
  sprintf(ranklist_wand,"datos_wand/lista_UB_256_Nodo_0xR.dat");
  ofstream WandRankList (ranklist_wand);
  if(WandRankList.is_open() && WandRankList.good()){
    cout<<"\n Escribiendo datos ranking por lista A_1 a archivo: \n "<<ranklist_wand<<endl;
    for(IT = RANK.begin(); IT != RANK.end(); ++IT)
      WandRankList<<IT->first<<" "<<IT->second<<endl;
    WandRankList.close();
  }
	diff_time_Ns(&END_TIME_A0,&START_TIME_A0,&LATENCIA_A0); 
	tiempoComputoNs=LATENCIA_A0.tv_sec*1000000000+LATENCIA_A0.tv_nsec;
	cout <<endl<< "tiempo ordenamiento heap sort A0 en Nano Segundos: "<<tiempoComputoNs;
	diff_time_Ns(&END_TIME_A1,&START_TIME_A1,&LATENCIA_A1); 
	tiempoComputoNs=LATENCIA_A1.tv_sec*1000000000+LATENCIA_A1.tv_nsec;
	cout <<endl<< "tiempo ordenamiento heap sort A1 en Nano Segundos: "<<tiempoComputoNs;
}

/* Funcion Principal
 *
 */
int main(int argc, char **argv){
	
	if(argc != 2){
		cout<<"\n Este programa debe fue llamado con "<<argv[0];
		cout<<"\n Debe ingresar solo 1 argumento";
    cout<<"\n *-------------------------------------*";
		cout<<"\n | Argumento |  metodo de ordenamiento |";
    cout<<"\n |-------------------------------------|";
		cout<<"\n |   1       |  Quick Sort             |";
		cout<<"\n |   2       |  Merge Sort             |";
		cout<<"\n |   3       |  Heap  Sort             |";
    cout<<"\n *-------------------------------------*";
    cout<<endl;
		return EXIT_FAILURE;
	}
	else{
    /*Definicion de variables del programa */
		int param, res, id_doc, peso, n;		

		long pruebaInt;

    /*Definicion de vectores */
    vector<PAIR>::iterator IT;

		param = atoi(argv[1]);
		AsignarCPUThread(0);
      switch(param){
			  case 1:{
              cout<<"\n Ordenar por Quick Sort \n";
		          AsignarCPUThread(0);
              Quick_Transcriptor();
              /*for(IT = PAIRS.begin(); IT != PAIRS.end(); ++IT)
                cout<<IT->first<<" "<<IT->second<<endl;*/
              cout<<endl;
						  break;
		  	}
        case 2:{
              cout<<"\n Ordenar por Merge Sort \n";
		          AsignarCPUThread(1);
              Merge_Transcriptor();
              cout<<endl;
              break;
        }
        case 3:{
              cout<<"\n Ordenar por Heap Sort \n";
		          AsignarCPUThread(2);
              Heap_Transcriptor();
              cout<<endl;
              break;
        }
        default: {
              cout<<"\n Ordenar por Heap Sort \n";
		          AsignarCPUThread(3);
              Heap_Transcriptor();
              cout<<endl;
              break;
        }
      }
		return EXIT_SUCCESS;
	}
}


