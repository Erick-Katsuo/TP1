#include "proceso.h"

int main(){
    char nombre_archivo[MAX_NOMBRE];
    char nombre_archivo_musica[MAX_NOMBRE];
    float opcion_bits = 0;
    float cantidad_muestras_segundo = 0;
    preguntar_parametros_iniciales(nombre_archivo, nombre_archivo_musica, opcion_bits, cantidad_muestras_segundo);
    ifstream archivo_notas(nombre_archivo);
    if(!archivo_notas){
        cout<<endl<<" Error: archivo no encontrado"<<endl;
        return ERROR;
    }
    ofstream archivo_wav(nombre_archivo_musica, ios::binary);
    comenzar_creacion_archivo(archivo_notas, cantidad_muestras_segundo, opcion_bits, archivo_wav);
    archivo_notas.close();
    archivo_wav.close();
    return 0;
}
