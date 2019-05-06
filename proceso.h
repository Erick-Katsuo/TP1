#ifndef _PROCESO_
#define _PROCESO_

#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;
const int ERROR = -1;

const int MAX_NOMBRE = 50;
const int MAX_NOTAS = 20;
const float MISMO_NUMERO = 0.0;
const float NOTAS_TOTALES = 12;
const float FRECUENCIA_BASE = 440.0;

const float CONSTANTE_PROPORCIONALIDAD = pow(2,(1.0/12.0));

const float NUMERO_NOTA_BASE = 4;

const int REINICIAR = -1;

const float SIN_MULTIPLICAR = 1;
const float NOTA_CICLO_MINIMO = 3;
const float NOTA_CICLO_MAXIMO = 5;
const float NOTA_CICLO_NULO = 4;

const float CORRECTOR_LUGAR_NOTA = 9;

const float VOLUMEN = 128;
const float PI = 3.14159265;

const float PASAJE_SEGUNDOS = pow(10,-3);
const float CORRIMIENTO_HORIZONTAL = 0;
const float CORRIMIENTO_VERTICAL = 127;

const float PASAJE_BYTE = 8;
const float ESPACIO_BASE_ARCHIVO_WAV = 36;

const string CHUNK_ID = "RIFF";
const string FORMATO = "WAVE";
const string TERCER_DATO = "fmt";
const string ESPACIO_CHUNK_FORMATO = "16";
const float FORMATO_AUDIO = 1;
const float CANTIDAD_CANALES = 1;
const string ENCABEZAMIENTO_CHUNK = "data";
const string ESPACIO_SECCION_DATOS = "44";




struct dato_archivo{
    string nota;
    float numero_nota;
    float duracion;
    float distancia_nota_base;
    float frecuencia_nota;
};


/*Pre:-.
 *Post: Quedan asignados los valores iniciales para comenzar el programa.
*/
void preguntar_parametros_iniciales(char nombre_archivo[], char nombre_archivo_musica[], float &opcion_bits, float &cantidad_muestras_segundo );

/*Pre: El archivo de "archivo_notas" debe existir y contener informacion.
 *Post: Se desarrollaran los calculos y asignaciones necesarias para crear el archivo ".wav".
*/
void comenzar_creacion_archivo(ifstream &archivo_notas, float cantidad_muestras_segundo, float opcion_bits, ofstream &archivo_wav);

#endif /*_PROCESO_*/
