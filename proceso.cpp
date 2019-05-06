#include "proceso.h"
#include <sstream>

/*Pre: Se deben proporcionar los datos correctos por parametro.
 *Post: Se asigna el valor de desfase a la variable "ultima_frecuencia".
*/
void calcular_desfase(double &ultima_frecuencia, double frecuencia_calculada, dato_archivo datos_extraidos, float muestras_totales){
    ultima_frecuencia = asin( (frecuencia_calculada - CORRIMIENTO_VERTICAL ) / VOLUMEN) - datos_extraidos.frecuencia_nota * ((muestras_totales-1)/muestras_totales)  * 180.0 / PI;
}

/*Pre:-.
 *Post: graba dentro del archivo wav los valores de las frecuencias de la nota actual.
*/
void calcular_imagen_grafica(dato_archivo datos_extraidos, float muestras_totales, ofstream &archivo_wav, double &ultima_frecuencia){
    double frecuencia_calculada=0;
    char frecuencia_final;
    for(int i=0; i<(int)muestras_totales ; i++){
        frecuencia_calculada = (VOLUMEN * (sin( (datos_extraidos.frecuencia_nota*(2*PI)) * (i/muestras_totales) + ultima_frecuencia))) + CORRIMIENTO_VERTICAL  ;
        frecuencia_final = (char) frecuencia_calculada;
        archivo_wav.write((char*)&frecuencia_final, sizeof(char));
    }
    calcular_desfase(ultima_frecuencia, frecuencia_calculada, datos_extraidos , muestras_totales);
}

/*Pre:Datos correctamente proporcionados por parametro.
 *Post: Calcula las muestras correspondiente a la nota actual.
*/
void calcular_muestras_totales_nota(dato_archivo datos_extraidos, float cantidad_muestras_segundo, float &muestras_totales){
    muestras_totales = cantidad_muestras_segundo * datos_extraidos.duracion * PASAJE_SEGUNDOS;
}

/*Pre:-.
 *Post: Asigna la frecuencia de la nota pedida.
*/
void calcular_frecuencia_nota(dato_archivo &datos_extraidos){
    datos_extraidos.frecuencia_nota = FRECUENCIA_BASE * pow(CONSTANTE_PROPORCIONALIDAD,datos_extraidos.distancia_nota_base);
}


/*Pre:-.
 *Post: Dependiendo del caso, calculara correctamente la cantidad de vueltas y la distancia
 *      final de la nota requerida.
*/
void calcular_distancia_final(dato_archivo &datos_extraidos, float cantidad_notas){
    float multiplicador=SIN_MULTIPLICAR;
    if(datos_extraidos.numero_nota<NOTA_CICLO_MINIMO){
        multiplicador = NOTA_CICLO_NULO - datos_extraidos.numero_nota;
    }else if(datos_extraidos.numero_nota>NOTA_CICLO_MAXIMO){
        multiplicador = datos_extraidos.numero_nota - NOTA_CICLO_NULO;
    }
    if(datos_extraidos.numero_nota >= NUMERO_NOTA_BASE ){
        datos_extraidos.distancia_nota_base = (multiplicador)*NOTAS_TOTALES + datos_extraidos.distancia_nota_base;
    }else if(datos_extraidos.numero_nota < NUMERO_NOTA_BASE ){
        datos_extraidos.distancia_nota_base = - (multiplicador)*NOTAS_TOTALES + datos_extraidos.distancia_nota_base;
    }
}

/*Pre:-.
 *Post: Queda asignada la distancia entre la nota base (A4) y la nota leida.
*/
void ver_distancia_nota_base(dato_archivo &datos_extraidos){
    float cantidad_notas = MISMO_NUMERO;
    float valor_nota = 0;
    string notas[MAX_NOTAS]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
    for(int i=0 ; i< (int)NOTAS_TOTALES ; i++){
        if(datos_extraidos.nota == notas[i]){
            valor_nota = (float)i -CORRECTOR_LUGAR_NOTA ;
            datos_extraidos.distancia_nota_base = valor_nota;
            calcular_distancia_final(datos_extraidos, cantidad_notas);
            valor_nota = 0;
            i=((int)NOTAS_TOTALES);
        }
    }
}

/*Pre:-.
 *Post: quedaran correctamente definidos los datos para el struct "datos_extraidos".
*/
void separar_datos(dato_archivo &datos_extraidos){
    string limitador = "#";
    size_t posicion = 1;
    string muestra;
    if(datos_extraidos.nota[1]=='#'){
        posicion += datos_extraidos.nota.find(limitador);
    }
    muestra = datos_extraidos.nota.substr(0, posicion);
    if(datos_extraidos.nota[1]=='#'){
        datos_extraidos.nota.erase(0,  limitador.length() +1);
    }else{
        datos_extraidos.nota.erase(0,  limitador.length() );
    }
    stringstream convertir(datos_extraidos.nota);
    convertir>>datos_extraidos.numero_nota;
    datos_extraidos.nota=muestra;
}

void crear_dato_notas_wav(dato_archivo &datos_extraidos, ifstream &archivo_notas, ofstream &archivo_wav, float cantidad_muestras_segundo, float muestras_totales){
    double ultima_frecuencia=0;
    while(archivo_notas>>datos_extraidos.nota>>datos_extraidos.duracion){
        separar_datos(datos_extraidos);
        ver_distancia_nota_base(datos_extraidos);
        calcular_frecuencia_nota(datos_extraidos);
        calcular_muestras_totales_nota(datos_extraidos, cantidad_muestras_segundo, muestras_totales);
        calcular_imagen_grafica(datos_extraidos, muestras_totales, archivo_wav, ultima_frecuencia);
    }
}

/*Pre: .
 *Post: Calcula el valor correspondiente para la variable "alinear_bloque".
*/
void calcular_alinear_bloque(float cantidad_muestras_segundo, float &alinear_bloque){
    alinear_bloque = (cantidad_muestras_segundo * CANTIDAD_CANALES ) / PASAJE_BYTE;
}

/*Pre: .
 *Post: Calcula el valor correspondiente para la variable "tasa_bytes".
*/
void calcular_tasa_bytes(float cantidad_muestras_segundo, float opcion_bits, float &tasa_bytes){
    tasa_bytes = (cantidad_muestras_segundo * CANTIDAD_CANALES * opcion_bits ) / PASAJE_BYTE;
}


/*Pre: .
 *Post: Queda asignada la informacion inicial del archivo wav.
*/
void crear_encabezado_wav(int espacio_final_wav, ofstream &archivo_wav, float cantidad_muestras_segundo, float opcion_bits){
    float tasa_bytes=0;
    float alinear_bloque=0;
    archivo_wav.write((char*)&CHUNK_ID, sizeof(CHUNK_ID));
    archivo_wav.write((char*)&espacio_final_wav, sizeof(espacio_final_wav));
    archivo_wav.write((char*)&FORMATO, sizeof(FORMATO));
    archivo_wav.write((char*)&TERCER_DATO, sizeof(TERCER_DATO));
    archivo_wav.write((char*)&ESPACIO_CHUNK_FORMATO, sizeof(ESPACIO_CHUNK_FORMATO));
    archivo_wav.write((char*)&FORMATO_AUDIO, sizeof(FORMATO_AUDIO));
    archivo_wav.write((char*)&CANTIDAD_CANALES, sizeof(CANTIDAD_CANALES));
    archivo_wav.write((char*)&cantidad_muestras_segundo, sizeof(cantidad_muestras_segundo));
    calcular_tasa_bytes(cantidad_muestras_segundo, opcion_bits, tasa_bytes);
    archivo_wav.write((char*)&tasa_bytes, sizeof(tasa_bytes));
    calcular_alinear_bloque(cantidad_muestras_segundo, alinear_bloque);
    archivo_wav.write((char*)&alinear_bloque, sizeof(alinear_bloque));
    archivo_wav.write((char*)&cantidad_muestras_segundo, sizeof(cantidad_muestras_segundo));
    archivo_wav.write((char*)&ENCABEZAMIENTO_CHUNK, sizeof(ENCABEZAMIENTO_CHUNK));
    archivo_wav.write((char*)&ESPACIO_SECCION_DATOS, sizeof(ESPACIO_SECCION_DATOS));
}

/*Pre: Los datos obtenidos deben ser correctos para poder calcular el tamaño exacto del archivo a crear.
 *Post: Queda asignado el espacio final del archivo wav en la variable "espacio_final_wav".
*/
void calcular_espacio_archivo_wav(float duracion, float opcion_bits, float cantidad_muestras_segundo, int &espacio_final_wav){
    espacio_final_wav = ((duracion * opcion_bits * cantidad_muestras_segundo * PASAJE_SEGUNDOS) / PASAJE_BYTE) + ESPACIO_BASE_ARCHIVO_WAV;
}

/*Pre: El archivo de "archivo_notas" debe existir y contener informacion.
 *Post: Se desarrollaran los calculos y asignaciones necesarias para crear el archivo ".wav".
*/
void comenzar_creacion_archivo(ifstream &archivo_notas, float cantidad_muestras_segundo, float opcion_bits, ofstream &archivo_wav){
    dato_archivo datos_extraidos;
    int espacio_final_wav=0;
    float muestras_totales=0;
    int duracion=0;
    archivo_notas>>duracion;
    calcular_espacio_archivo_wav(duracion, opcion_bits, cantidad_muestras_segundo, espacio_final_wav);
    crear_encabezado_wav(espacio_final_wav, archivo_wav, cantidad_muestras_segundo, opcion_bits);
    crear_dato_notas_wav(datos_extraidos, archivo_notas, archivo_wav, cantidad_muestras_segundo, muestras_totales);
}

/*Pre:-.
 *Post: Quedan asignados los valores iniciales para comenzar el programa.
*/
void preguntar_parametros_iniciales(char nombre_archivo[], char nombre_archivo_musica[], float &opcion_bits, float &cantidad_muestras_segundo ){
    cout<<" Nombre del archivo(.txt): ";
    cin>>nombre_archivo;
    cout<<endl<<" Nombre del archivo a crear(.wav): ";
    cin>>nombre_archivo_musica;
    cout<<endl<<" 8 - 16 - 32\n Ingrese el tipo de bit a usar: ";
    cin>>opcion_bits;
    cout<<endl<<" Cantidad de muestras por segundo: ";
    cin>>cantidad_muestras_segundo;
}
