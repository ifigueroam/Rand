/* =================================================================
 *
 * Archivo: 			complejidad_operaciones.c
 * Descripción:
 * 								Programa de complejidad de operaciones
 * 								Obtiene un listado de los costos computacionales
 * 								al ejecutar la expresión matematica dada por:
 * 								res = res + (a + b * c)/d - log10 (b * c) 
 * 											+ pow((a * c), 10);
 * 								donde se ejcutan cuatro operaciones elementales
 * 								(suma, resta, multiplicacion y division) y dos
 * 								operaciones complejas (logaritmo y potencia),
 * 								para ello se calculan los tiempos de acuerdo al
 * 								tipo de dato a calcular (int, unsigned int, 
 * 								unsigned long int, float, double, long double),
 * 								el costo computacional del algoritmo es obtenido
 * 								a través de la función predefinida:
 * 								CLOCK_MONOTONIC de la librería time.h de C/C++.
 *
 *	Autor: 				Iván Figueroa Monsalve
 *	mail:					ivfigue@gmail.com
 *	Asignatura:		Electivo I de Complejidad de Algoritmos
 *  Profesor:			Oscar Rojas
 *  mail: 				oscar.rojas.d@usach.cl
 *
 *  Versión: 			1.0
 *  Compilador: 	gcc
 *  Ejecución: 		g++ cOperaciones.cc -o cOp -pthread && ./cOperaciones n
 *  							donde n es el tipo de dato a consultar
 *  ================================================================
 */

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <sys/time.h>
#include <sched.h>
#include <pthread.h>
#include <cmath>
using namespace std;
struct timespec START_TIME, END_TIME, LATENCIA,
								MUL_STIME, MUL_ETIME, MUL_LATENCY,
								ADD_STIME, ADD_ETIME, ADD_LATENCY,
								SUB_STIME, SUB_ETIME, SUB_LATENCY,
								DIV_STIME, DIV_ETIME, DIV_LATENCY,
								LOG_STIME, LOG_ETIME, LOG_LATENCY,
								POW_STIME, POW_ETIME, POW_LATENCY; 

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

void calculo_unsigned_int(){
		unsigned int	a=1048576, 
									b=1048576, 
									c=1048576, 
									d=1048576,
									res=0,	
									n=1000000,
									aux_res, aux_sub, aux_mul, aux_div, aux_log, aux_sum, aux_pow, Time;
		
		long int	tiempoComputoNs, 
							tAdd = 0, 
							tSub = 0, 
							tMul = 0, 
							tDiv = 0, 
							tLog = 0, 
							tPow = 0, 
							tAll = 0;
		AsignarCPUThread(0);
		for(long i=0;i<n;i++){             
			clock_gettime(CLOCK_MONOTONIC, &START_TIME);
			//res = res + (a + b * c)/d - log10 (b * c) + pow((a * c), 10);
			clock_gettime(CLOCK_MONOTONIC, &MUL_STIME);
			aux_mul = b * c;
			clock_gettime(CLOCK_MONOTONIC, &MUL_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &ADD_STIME);
			aux_sum = a + aux_mul;
			clock_gettime(CLOCK_MONOTONIC, &ADD_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &DIV_STIME);
			aux_div	= aux_sum/d;
			clock_gettime(CLOCK_MONOTONIC, &DIV_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &LOG_STIME);
			aux_log = log10(aux_mul);
			clock_gettime(CLOCK_MONOTONIC, &LOG_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &SUB_STIME);
			aux_sub	= aux_div - aux_log;
			clock_gettime(CLOCK_MONOTONIC, &SUB_ETIME);
			aux_mul = a * c;
			clock_gettime(CLOCK_MONOTONIC, &POW_STIME);
			aux_pow = pow(aux_mul, 10);
			clock_gettime(CLOCK_MONOTONIC, &POW_ETIME);
			aux_res = aux_sub + aux_pow;
			res 		= res + aux_res;
			clock_gettime(CLOCK_MONOTONIC, &END_TIME);
	
			diff_time_Ns(&ADD_ETIME,&ADD_STIME,&ADD_LATENCY); 
			tiempoComputoNs=ADD_LATENCY.tv_sec*1000000000+ADD_LATENCY.tv_nsec;
			tAdd = tAdd + tiempoComputoNs;

			diff_time_Ns(&SUB_ETIME,&SUB_STIME,&SUB_LATENCY); 
			tiempoComputoNs=SUB_LATENCY.tv_sec*1000000000+SUB_LATENCY.tv_nsec;
			tSub = tSub + tiempoComputoNs;

			diff_time_Ns(&MUL_ETIME,&MUL_STIME,&MUL_LATENCY); 
			tiempoComputoNs=MUL_LATENCY.tv_sec*1000000000+MUL_LATENCY.tv_nsec;
			tMul = tMul + tiempoComputoNs;
	
			diff_time_Ns(&DIV_ETIME,&DIV_STIME,&DIV_LATENCY); 
			tiempoComputoNs=DIV_LATENCY.tv_sec*1000000000+DIV_LATENCY.tv_nsec;
			tDiv = tDiv + tiempoComputoNs;
	
			diff_time_Ns(&LOG_ETIME,&LOG_STIME,&LOG_LATENCY); 
			tiempoComputoNs=LOG_LATENCY.tv_sec*1000000000+LOG_LATENCY.tv_nsec;
			tLog = tLog + tiempoComputoNs;
	
			diff_time_Ns(&POW_ETIME,&POW_STIME,&POW_LATENCY); 
			tiempoComputoNs=POW_LATENCY.tv_sec*1000000000+POW_LATENCY.tv_nsec;
			tPow = tPow + tiempoComputoNs;

			diff_time_Ns(&END_TIME,&START_TIME,&LATENCIA); 
			tiempoComputoNs=LATENCIA.tv_sec*1000000000+LATENCIA.tv_nsec;
			tAll = tAll + tiempoComputoNs;
		}
		Time = tAdd/n;
		cout <<endl<< "tiempo de Suma en Nano Segundos:      "<<Time;
		Time = tSub/n;
		cout <<endl<< "tiempo de Resta en Nano Segundos:     "<<Time;
		Time = tMul/n;
		cout <<endl<< "tiempo de Producto en Nano Segundos:  "<<Time;
		Time = tDiv/n;
		cout <<endl<< "tiempo de Division en Nano Segundos:  "<<Time;
		Time = tLog/n;
		cout <<endl<< "tiempo de Logaritmo en Nano Segundos: "<<Time;
		Time = tPow/n;
		cout <<endl<< "tiempo de Potencia en Nano Segundos:  "<<Time;
		Time = tAll/n;
		cout <<endl<< "tiempo de Total en Nano Segundos:     "<<Time<<endl;
}

void calculo_unsigned_long_int(){
		unsigned long int	a=1048576, 
							b=1048576, 
							c=1048576, 
							d=1048576,
							res=0,	
							n=1000000,
							aux_res, aux_sub, aux_mul, aux_div, aux_log, aux_sum, aux_pow, Time;
		
		long int	tiempoComputoNs, 
							tAdd = 0, 
							tSub = 0, 
							tMul = 0, 
							tDiv = 0, 
							tLog = 0, 
							tPow = 0, 
							tAll = 0;
		AsignarCPUThread(0);
		for(long i=0;i<n;i++){             
			clock_gettime(CLOCK_MONOTONIC, &START_TIME);
			//res = res + (a + b * c)/d - log10 (b * c) + pow((a * c), 10);
			clock_gettime(CLOCK_MONOTONIC, &MUL_STIME);
			aux_mul = b * c;
			clock_gettime(CLOCK_MONOTONIC, &MUL_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &ADD_STIME);
			aux_sum = a + aux_mul;
			clock_gettime(CLOCK_MONOTONIC, &ADD_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &DIV_STIME);
			aux_div	= aux_sum/d;
			clock_gettime(CLOCK_MONOTONIC, &DIV_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &LOG_STIME);
			aux_log = log10(aux_mul);
			clock_gettime(CLOCK_MONOTONIC, &LOG_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &SUB_STIME);
			aux_sub	= aux_div - aux_log;
			clock_gettime(CLOCK_MONOTONIC, &SUB_ETIME);
			aux_mul = a * c;
			clock_gettime(CLOCK_MONOTONIC, &POW_STIME);
			aux_pow = pow(aux_mul, 10);
			clock_gettime(CLOCK_MONOTONIC, &POW_ETIME);
			aux_res = aux_sub + aux_pow;
			res 		= res + aux_res;
			clock_gettime(CLOCK_MONOTONIC, &END_TIME);
	
			diff_time_Ns(&ADD_ETIME,&ADD_STIME,&ADD_LATENCY); 
			tiempoComputoNs=ADD_LATENCY.tv_sec*1000000000+ADD_LATENCY.tv_nsec;
			tAdd = tAdd + tiempoComputoNs;

			diff_time_Ns(&SUB_ETIME,&SUB_STIME,&SUB_LATENCY); 
			tiempoComputoNs=SUB_LATENCY.tv_sec*1000000000+SUB_LATENCY.tv_nsec;
			tSub = tSub + tiempoComputoNs;

			diff_time_Ns(&MUL_ETIME,&MUL_STIME,&MUL_LATENCY); 
			tiempoComputoNs=MUL_LATENCY.tv_sec*1000000000+MUL_LATENCY.tv_nsec;
			tMul = tMul + tiempoComputoNs;
	
			diff_time_Ns(&DIV_ETIME,&DIV_STIME,&DIV_LATENCY); 
			tiempoComputoNs=DIV_LATENCY.tv_sec*1000000000+DIV_LATENCY.tv_nsec;
			tDiv = tDiv + tiempoComputoNs;
	
			diff_time_Ns(&LOG_ETIME,&LOG_STIME,&LOG_LATENCY); 
			tiempoComputoNs=LOG_LATENCY.tv_sec*1000000000+LOG_LATENCY.tv_nsec;
			tLog = tLog + tiempoComputoNs;
	
			diff_time_Ns(&POW_ETIME,&POW_STIME,&POW_LATENCY); 
			tiempoComputoNs=POW_LATENCY.tv_sec*1000000000+POW_LATENCY.tv_nsec;
			tPow = tPow + tiempoComputoNs;

			diff_time_Ns(&END_TIME,&START_TIME,&LATENCIA); 
			tiempoComputoNs=LATENCIA.tv_sec*1000000000+LATENCIA.tv_nsec;
			tAll = tAll + tiempoComputoNs;
		}
		Time = tAdd/n;
		cout <<endl<< "tiempo de Suma en Nano Segundos:      "<<Time;
		Time = tSub/n;
		cout <<endl<< "tiempo de Resta en Nano Segundos:     "<<Time;
		Time = tMul/n;
		cout <<endl<< "tiempo de Producto en Nano Segundos:  "<<Time;
		Time = tDiv/n;
		cout <<endl<< "tiempo de Division en Nano Segundos:  "<<Time;
		Time = tLog/n;
		cout <<endl<< "tiempo de Logaritmo en Nano Segundos: "<<Time;
		Time = tPow/n;
		cout <<endl<< "tiempo de Potencia en Nano Segundos:  "<<Time;
		Time = tAll/n;
		cout <<endl<< "tiempo de Total en Nano Segundos:     "<<Time<<endl;
}

void calculo_int(){
		int	a=1048576, 
				b=1048576, 
				c=1048576, 
				d=1048576,
				res=0,	
				n=1000000,
				aux_res, aux_sub, aux_mul, aux_div, aux_log, aux_sum, aux_pow, Time;
		
		long int	tiempoComputoNs, 
							tAdd = 0, 
							tSub = 0, 
							tMul = 0, 
							tDiv = 0, 
							tLog = 0, 
							tPow = 0, 
							tAll = 0;
		AsignarCPUThread(0);
		for(long i=0;i<n;i++){             
			clock_gettime(CLOCK_MONOTONIC, &START_TIME);
			//res = res + (a + b * c)/d - log10 (b * c) + pow((a * c), 10);
			clock_gettime(CLOCK_MONOTONIC, &MUL_STIME);
			aux_mul = b * c;
			clock_gettime(CLOCK_MONOTONIC, &MUL_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &ADD_STIME);
			aux_sum = a + aux_mul;
			clock_gettime(CLOCK_MONOTONIC, &ADD_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &DIV_STIME);
			aux_div	= aux_sum/d;
			clock_gettime(CLOCK_MONOTONIC, &DIV_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &LOG_STIME);
			aux_log = log10(aux_mul);
			clock_gettime(CLOCK_MONOTONIC, &LOG_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &SUB_STIME);
			aux_sub	= aux_div - aux_log;
			clock_gettime(CLOCK_MONOTONIC, &SUB_ETIME);
			aux_mul = a * c;
			clock_gettime(CLOCK_MONOTONIC, &POW_STIME);
			aux_pow = pow(aux_mul, 10);
			clock_gettime(CLOCK_MONOTONIC, &POW_ETIME);
			aux_res = aux_sub + aux_pow;
			res 		= res + aux_res;
			clock_gettime(CLOCK_MONOTONIC, &END_TIME);
	
			diff_time_Ns(&ADD_ETIME,&ADD_STIME,&ADD_LATENCY); 
			tiempoComputoNs=ADD_LATENCY.tv_sec*1000000000+ADD_LATENCY.tv_nsec;
			tAdd = tAdd + tiempoComputoNs;

			diff_time_Ns(&SUB_ETIME,&SUB_STIME,&SUB_LATENCY); 
			tiempoComputoNs=SUB_LATENCY.tv_sec*1000000000+SUB_LATENCY.tv_nsec;
			tSub = tSub + tiempoComputoNs;

			diff_time_Ns(&MUL_ETIME,&MUL_STIME,&MUL_LATENCY); 
			tiempoComputoNs=MUL_LATENCY.tv_sec*1000000000+MUL_LATENCY.tv_nsec;
			tMul = tMul + tiempoComputoNs;
	
			diff_time_Ns(&DIV_ETIME,&DIV_STIME,&DIV_LATENCY); 
			tiempoComputoNs=DIV_LATENCY.tv_sec*1000000000+DIV_LATENCY.tv_nsec;
			tDiv = tDiv + tiempoComputoNs;
	
			diff_time_Ns(&LOG_ETIME,&LOG_STIME,&LOG_LATENCY); 
			tiempoComputoNs=LOG_LATENCY.tv_sec*1000000000+LOG_LATENCY.tv_nsec;
			tLog = tLog + tiempoComputoNs;
	
			diff_time_Ns(&POW_ETIME,&POW_STIME,&POW_LATENCY); 
			tiempoComputoNs=POW_LATENCY.tv_sec*1000000000+POW_LATENCY.tv_nsec;
			tPow = tPow + tiempoComputoNs;

			diff_time_Ns(&END_TIME,&START_TIME,&LATENCIA); 
			tiempoComputoNs=LATENCIA.tv_sec*1000000000+LATENCIA.tv_nsec;
			tAll = tAll + tiempoComputoNs;
		}
		Time = tAdd/n;
		cout <<endl<< "tiempo de Suma en Nano Segundos:      "<<Time;
		Time = tSub/n;
		cout <<endl<< "tiempo de Resta en Nano Segundos:     "<<Time;
		Time = tMul/n;
		cout <<endl<< "tiempo de Producto en Nano Segundos:  "<<Time;
		Time = tDiv/n;
		cout <<endl<< "tiempo de Division en Nano Segundos:  "<<Time;
		Time = tLog/n;
		cout <<endl<< "tiempo de Logaritmo en Nano Segundos: "<<Time;
		Time = tPow/n;
		cout <<endl<< "tiempo de Potencia en Nano Segundos:  "<<Time;
		Time = tAll/n;
		cout <<endl<< "tiempo de Total en Nano Segundos:     "<<Time<<endl;
}

void calculo_float(){
		float	a=1048576, 
					b=1048576, 
					c=1048576, 
					d=1048576,
					res=0,	
					n=1000000,
					aux_res, aux_sub, aux_mul, aux_div, aux_log, aux_sum, aux_pow, Time;
		
		long int	tiempoComputoNs, 
							tAdd = 0, 
							tSub = 0, 
							tMul = 0, 
							tDiv = 0, 
							tLog = 0, 
							tPow = 0, 
							tAll = 0;
		AsignarCPUThread(0);
		for(long i=0;i<n;i++){             
			clock_gettime(CLOCK_MONOTONIC, &START_TIME);
			//res = res + (a + b * c)/d - log10 (b * c) + pow((a * c), 10);
			clock_gettime(CLOCK_MONOTONIC, &MUL_STIME);
			aux_mul = b * c;
			clock_gettime(CLOCK_MONOTONIC, &MUL_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &ADD_STIME);
			aux_sum = a + aux_mul;
			clock_gettime(CLOCK_MONOTONIC, &ADD_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &DIV_STIME);
			aux_div	= aux_sum/d;
			clock_gettime(CLOCK_MONOTONIC, &DIV_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &LOG_STIME);
			aux_log = log10(aux_mul);
			clock_gettime(CLOCK_MONOTONIC, &LOG_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &SUB_STIME);
			aux_sub	= aux_div - aux_log;
			clock_gettime(CLOCK_MONOTONIC, &SUB_ETIME);
			aux_mul = a * c;
			clock_gettime(CLOCK_MONOTONIC, &POW_STIME);
			aux_pow = pow(aux_mul, 10);
			clock_gettime(CLOCK_MONOTONIC, &POW_ETIME);
			aux_res = aux_sub + aux_pow;
			res 		= res + aux_res;
			clock_gettime(CLOCK_MONOTONIC, &END_TIME);
	
			diff_time_Ns(&ADD_ETIME,&ADD_STIME,&ADD_LATENCY); 
			tiempoComputoNs=ADD_LATENCY.tv_sec*1000000000+ADD_LATENCY.tv_nsec;
			tAdd = tAdd + tiempoComputoNs;

			diff_time_Ns(&SUB_ETIME,&SUB_STIME,&SUB_LATENCY); 
			tiempoComputoNs=SUB_LATENCY.tv_sec*1000000000+SUB_LATENCY.tv_nsec;
			tSub = tSub + tiempoComputoNs;

			diff_time_Ns(&MUL_ETIME,&MUL_STIME,&MUL_LATENCY); 
			tiempoComputoNs=MUL_LATENCY.tv_sec*1000000000+MUL_LATENCY.tv_nsec;
			tMul = tMul + tiempoComputoNs;
	
			diff_time_Ns(&DIV_ETIME,&DIV_STIME,&DIV_LATENCY); 
			tiempoComputoNs=DIV_LATENCY.tv_sec*1000000000+DIV_LATENCY.tv_nsec;
			tDiv = tDiv + tiempoComputoNs;
	
			diff_time_Ns(&LOG_ETIME,&LOG_STIME,&LOG_LATENCY); 
			tiempoComputoNs=LOG_LATENCY.tv_sec*1000000000+LOG_LATENCY.tv_nsec;
			tLog = tLog + tiempoComputoNs;
	
			diff_time_Ns(&POW_ETIME,&POW_STIME,&POW_LATENCY); 
			tiempoComputoNs=POW_LATENCY.tv_sec*1000000000+POW_LATENCY.tv_nsec;
			tPow = tPow + tiempoComputoNs;

			diff_time_Ns(&END_TIME,&START_TIME,&LATENCIA); 
			tiempoComputoNs=LATENCIA.tv_sec*1000000000+LATENCIA.tv_nsec;
			tAll = tAll + tiempoComputoNs;
		}
		Time = tAdd/n;
		cout <<endl<< "tiempo de Suma en Nano Segundos:      "<<Time;
		Time = tSub/n;
		cout <<endl<< "tiempo de Resta en Nano Segundos:     "<<Time;
		Time = tMul/n;
		cout <<endl<< "tiempo de Producto en Nano Segundos:  "<<Time;
		Time = tDiv/n;
		cout <<endl<< "tiempo de Division en Nano Segundos:  "<<Time;
		Time = tLog/n;
		cout <<endl<< "tiempo de Logaritmo en Nano Segundos: "<<Time;
		Time = tPow/n;
		cout <<endl<< "tiempo de Potencia en Nano Segundos:  "<<Time;
		Time = tAll/n;
		cout <<endl<< "tiempo de Total en Nano Segundos:     "<<Time<<endl;
}

void calculo_double(){
		double	a=1048576, 
						b=1048576, 
						c=1048576, 
						d=1048576,
						res=0,	
						n=1000000,
						aux_res, aux_sub, aux_mul, aux_div, aux_log, aux_sum, aux_pow, Time;
		
		long int	tiempoComputoNs, 
							tAdd = 0, 
							tSub = 0, 
							tMul = 0, 
							tDiv = 0, 
							tLog = 0, 
							tPow = 0, 
							tAll = 0;
		AsignarCPUThread(0);
		for(long i=0;i<n;i++){             
			clock_gettime(CLOCK_MONOTONIC, &START_TIME);
			//res = res + (a + b * c)/d - log10 (b * c) + pow((a * c), 10);
			clock_gettime(CLOCK_MONOTONIC, &MUL_STIME);
			aux_mul = b * c;
			clock_gettime(CLOCK_MONOTONIC, &MUL_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &ADD_STIME);
			aux_sum = a + aux_mul;
			clock_gettime(CLOCK_MONOTONIC, &ADD_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &DIV_STIME);
			aux_div	= aux_sum/d;
			clock_gettime(CLOCK_MONOTONIC, &DIV_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &LOG_STIME);
			aux_log = log10(aux_mul);
			clock_gettime(CLOCK_MONOTONIC, &LOG_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &SUB_STIME);
			aux_sub	= aux_div - aux_log;
			clock_gettime(CLOCK_MONOTONIC, &SUB_ETIME);
			aux_mul = a * c;
			clock_gettime(CLOCK_MONOTONIC, &POW_STIME);
			aux_pow = pow(aux_mul, 10);
			clock_gettime(CLOCK_MONOTONIC, &POW_ETIME);
			aux_res = aux_sub + aux_pow;
			res 		= res + aux_res;
			clock_gettime(CLOCK_MONOTONIC, &END_TIME);
	
			diff_time_Ns(&ADD_ETIME,&ADD_STIME,&ADD_LATENCY); 
			tiempoComputoNs=ADD_LATENCY.tv_sec*1000000000+ADD_LATENCY.tv_nsec;
			tAdd = tAdd + tiempoComputoNs;

			diff_time_Ns(&SUB_ETIME,&SUB_STIME,&SUB_LATENCY); 
			tiempoComputoNs=SUB_LATENCY.tv_sec*1000000000+SUB_LATENCY.tv_nsec;
			tSub = tSub + tiempoComputoNs;

			diff_time_Ns(&MUL_ETIME,&MUL_STIME,&MUL_LATENCY); 
			tiempoComputoNs=MUL_LATENCY.tv_sec*1000000000+MUL_LATENCY.tv_nsec;
			tMul = tMul + tiempoComputoNs;
	
			diff_time_Ns(&DIV_ETIME,&DIV_STIME,&DIV_LATENCY); 
			tiempoComputoNs=DIV_LATENCY.tv_sec*1000000000+DIV_LATENCY.tv_nsec;
			tDiv = tDiv + tiempoComputoNs;
	
			diff_time_Ns(&LOG_ETIME,&LOG_STIME,&LOG_LATENCY); 
			tiempoComputoNs=LOG_LATENCY.tv_sec*1000000000+LOG_LATENCY.tv_nsec;
			tLog = tLog + tiempoComputoNs;
	
			diff_time_Ns(&POW_ETIME,&POW_STIME,&POW_LATENCY); 
			tiempoComputoNs=POW_LATENCY.tv_sec*1000000000+POW_LATENCY.tv_nsec;
			tPow = tPow + tiempoComputoNs;

			diff_time_Ns(&END_TIME,&START_TIME,&LATENCIA); 
			tiempoComputoNs=LATENCIA.tv_sec*1000000000+LATENCIA.tv_nsec;
			tAll = tAll + tiempoComputoNs;
		}
		Time = tAdd/n;
		cout <<endl<< "tiempo de Suma en Nano Segundos:      "<<Time;
		Time = tSub/n;
		cout <<endl<< "tiempo de Resta en Nano Segundos:     "<<Time;
		Time = tMul/n;
		cout <<endl<< "tiempo de Producto en Nano Segundos:  "<<Time;
		Time = tDiv/n;
		cout <<endl<< "tiempo de Division en Nano Segundos:  "<<Time;
		Time = tLog/n;
		cout <<endl<< "tiempo de Logaritmo en Nano Segundos: "<<Time;
		Time = tPow/n;
		cout <<endl<< "tiempo de Potencia en Nano Segundos:  "<<Time;
		Time = tAll/n;
		cout <<endl<< "tiempo de Total en Nano Segundos:     "<<Time<<endl;
}

void calculo_long_double(){
		long double	a=1048576, 
								b=1048576, 
								c=1048576, 
								d=1048576,
								res=0,	
								n=1000000,
								aux_res, aux_sub, aux_mul, aux_div, aux_log, aux_sum, aux_pow, Time;
		
		long int	tiempoComputoNs, 
							tAdd = 0, 
							tSub = 0, 
							tMul = 0, 
							tDiv = 0, 
							tLog = 0, 
							tPow = 0, 
							tAll = 0;
		AsignarCPUThread(0);
		for(long i=0;i<n;i++){             
			clock_gettime(CLOCK_MONOTONIC, &START_TIME);
			//res = res + (a + b * c)/d - log10 (b * c) + pow((a * c), 10);
			clock_gettime(CLOCK_MONOTONIC, &MUL_STIME);
			aux_mul = b * c;
			clock_gettime(CLOCK_MONOTONIC, &MUL_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &ADD_STIME);
			aux_sum = a + aux_mul;
			clock_gettime(CLOCK_MONOTONIC, &ADD_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &DIV_STIME);
			aux_div	= aux_sum/d;
			clock_gettime(CLOCK_MONOTONIC, &DIV_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &LOG_STIME);
			aux_log = log10(aux_mul);
			clock_gettime(CLOCK_MONOTONIC, &LOG_ETIME);
			clock_gettime(CLOCK_MONOTONIC, &SUB_STIME);
			aux_sub	= aux_div - aux_log;
			clock_gettime(CLOCK_MONOTONIC, &SUB_ETIME);
			aux_mul = a * c;
			clock_gettime(CLOCK_MONOTONIC, &POW_STIME);
			aux_pow = pow(aux_mul, 10);
			clock_gettime(CLOCK_MONOTONIC, &POW_ETIME);
			aux_res = aux_sub + aux_pow;
			res 		= res + aux_res;
			clock_gettime(CLOCK_MONOTONIC, &END_TIME);
	
			diff_time_Ns(&ADD_ETIME,&ADD_STIME,&ADD_LATENCY); 
			tiempoComputoNs=ADD_LATENCY.tv_sec*1000000000+ADD_LATENCY.tv_nsec;
			tAdd = tAdd + tiempoComputoNs;

			diff_time_Ns(&SUB_ETIME,&SUB_STIME,&SUB_LATENCY); 
			tiempoComputoNs=SUB_LATENCY.tv_sec*1000000000+SUB_LATENCY.tv_nsec;
			tSub = tSub + tiempoComputoNs;

			diff_time_Ns(&MUL_ETIME,&MUL_STIME,&MUL_LATENCY); 
			tiempoComputoNs=MUL_LATENCY.tv_sec*1000000000+MUL_LATENCY.tv_nsec;
			tMul = tMul + tiempoComputoNs;
	
			diff_time_Ns(&DIV_ETIME,&DIV_STIME,&DIV_LATENCY); 
			tiempoComputoNs=DIV_LATENCY.tv_sec*1000000000+DIV_LATENCY.tv_nsec;
			tDiv = tDiv + tiempoComputoNs;
	
			diff_time_Ns(&LOG_ETIME,&LOG_STIME,&LOG_LATENCY); 
			tiempoComputoNs=LOG_LATENCY.tv_sec*1000000000+LOG_LATENCY.tv_nsec;
			tLog = tLog + tiempoComputoNs;
	
			diff_time_Ns(&POW_ETIME,&POW_STIME,&POW_LATENCY); 
			tiempoComputoNs=POW_LATENCY.tv_sec*1000000000+POW_LATENCY.tv_nsec;
			tPow = tPow + tiempoComputoNs;

			diff_time_Ns(&END_TIME,&START_TIME,&LATENCIA); 
			tiempoComputoNs=LATENCIA.tv_sec*1000000000+LATENCIA.tv_nsec;
			tAll = tAll + tiempoComputoNs;
		}
		Time = tAdd/n;
		cout <<endl<< "tiempo de Suma en Nano Segundos:      "<<Time;
		Time = tSub/n;
		cout <<endl<< "tiempo de Resta en Nano Segundos:     "<<Time;
		Time = tMul/n;
		cout <<endl<< "tiempo de Producto en Nano Segundos:  "<<Time;
		Time = tDiv/n;
		cout <<endl<< "tiempo de Division en Nano Segundos:  "<<Time;
		Time = tLog/n;
		cout <<endl<< "tiempo de Logaritmo en Nano Segundos: "<<Time;
		Time = tPow/n;
		cout <<endl<< "tiempo de Potencia en Nano Segundos:  "<<Time;
		Time = tAll/n;
		cout <<endl<< "tiempo de Total en Nano Segundos:     "<<Time<<endl;
}

/* Funcion Principal
 *
 */
int main(int argc, char **argv){
	
	if(argc != 2){ //argc == (n+1) argumentos
		cout<<"\n Este programa debe fue llamado con "<<argv[0]<<endl;
		cout<<"\n Debe ingresar unicamente 1 argumentos, donde: ";
		cout<<"\n Argumento  : tipo de dato a evaluar";
		cout<<"\n   puede ser: tipo_dato  valor_parametro";
		cout<<"\n              int               1	";
		cout<<"\n              unsigned int      2	";
		cout<<"\n              unsigned long int 3	";
		cout<<"\n              float             1048576	";
		cout<<"\n              double            5	";
		cout<<"\n              long double       6	";
		cout<<"\n              Todos             *	"<<endl;
		return EXIT_FAILURE;
	}
	else{
		int param;
		param = atoi(argv[1]);
		switch(param){
			case 1:{
						cout<<"Tipo de dato INT: ";
						calculo_int();
						cout<<endl;
						break;
			}
			case 2:{
						cout<<"Tipo de dato UNSIGNED INT: ";
						calculo_unsigned_int();
						cout<<endl;
						break;
			}
			case 3:{
						cout<<"Tipo de dato UNSIGNED LONG INT: ";
						calculo_unsigned_long_int();
						cout<<endl;
						break;
			}
			case 1048576:{
						cout<<"Tipo de dato FLOAT: ";
						calculo_float();
						cout<<endl;
						break;
			}
			case 5:{
						cout<<"Tipo de dato DOUBLE: ";
						calculo_double();
						cout<<endl;
						break;
			}
			case 6:{
						cout<<"Tipo de dato LONG DOUBLE: ";
						calculo_long_double();
						cout<<endl;
						break;
			}
			default: {
						cout<<"Tipo de dato INT: ";
						calculo_int();
						cout<<endl;
						cout<<"Tipo de dato UNSIGNED INT: ";
						calculo_unsigned_int();
						cout<<endl;
						cout<<"Tipo de dato UNSIGNED LONG INT: ";
						calculo_unsigned_long_int();
						cout<<endl;
						cout<<"Tipo de dato FLOAT: ";
						calculo_float();
						cout<<endl;
						cout<<"Tipo de dato DOUBLE: ";
						calculo_double();
						cout<<endl;
						cout<<"Tipo de dato LONG DOUBLE: ";
						calculo_long_double();
						cout<<endl;
						break;
			}
		}
		return EXIT_SUCCESS;
	}
}
