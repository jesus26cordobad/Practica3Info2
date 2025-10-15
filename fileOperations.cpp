#include "fileOperations.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int readFileChar(const string &file, char *&arregloChar, int &tamFile){
    /**
     * @brief Lee un archivo en modo binario y almacena su contenido carácter por carácter.
     *
     * Abre un archivo en modo binario, obtiene su tamaño, reserva memoria dinámica y
     * copia el contenido del archivo en un arreglo de caracteres.
     *
     * @param file Ruta al archivo binario.
     * @param arregloChar Referencia a un puntero donde se almacenará el contenido leído.
     * @param tamFile Referencia donde se almacenará el tamaño del archivo en bytes.
     * @return int Código de estado: 0 = éxito, 1 = error al abrir, 2 = tamaño inválido,
     *              3 = error de memoria, 4 = error de lectura.
     */


    ifstream archivo(file, ios::binary | ios::ate); //se abre el archivo en modo binario
    if (!archivo) {
        arregloChar = nullptr;
        tamFile = 0;
        return 1; //Error al abrir el archivo
    }

    tamFile = archivo.tellg(); // Obtener el tamaño del archivo en bytes

    if (tamFile <= 0) { //Excepcion por tamaño de archivo invalido
        archivo.close();
        arregloChar = nullptr;
        tamFile = 0;
        return 2; // Tamaño inválido
    }

    archivo.seekg(0, ios::beg); // Regresar al inicio del archivo para comenzar a leer

    try{
        // Intentar reservar memoria dinámica para almacenar el contenido del archivo
        arregloChar = new char[tamFile];
    }catch (bad_alloc &e){  // Si ocurre un error de asignación de memoria
        archivo.close();
        arregloChar = nullptr;
        tamFile = 0;
        return 3; // Error de memoria
    }
    archivo.read(arregloChar, tamFile); // Leer el archivo en el arregloChar

    if (!archivo) { // Si ocurrió un error de lectura, liberar memoria y retornar código de error
        delete[] arregloChar;
        arregloChar = nullptr;
        tamFile = 0;
        archivo.close();
        return 4; // Error al leer el archivo completamente
    }

    archivo.close();
    return 0;
}

int readFileString(const string &file, string *&arregloString, int &tamFile){

    /**
     * @brief Lee un archivo de texto línea por línea, ignorando las líneas vacías.
     *
     * Abre un archivo de texto, cuenta y almacena las líneas no vacías en un arreglo dinámico.
     *
     * @param file Ruta al archivo de texto.
     * @param arregloString Referencia a un puntero donde se almacenarán las líneas del archivo.
     * @param tamFile Referencia donde se almacenará la cantidad de líneas no vacías.
     * @return int 0 si se lee correctamente, 1 si ocurre un error al abrir el archivo.
     */

    ifstream archivo(file, ios::in); // Abrir el archivo en modo lectura
    if (!archivo){
        cerr << "No se puede abrir el archivo. " << endl;
        return 1; // Error al abrir el archivo
    }
    tamFile = 0; // Inicializar el contador de líneas no vacías
    string linea; // Variable para almacenar cada linea del archivo

    while (getline(archivo, linea)) { // Leer cada línea del archivo
        if (!linea.empty()) { // Si la línea no está vacía
            tamFile++;
        }
    }

    archivo.clear();  // Limpiar cualquier flag (como EOF)
    archivo.seekg(0, ios::beg);  // Volver al inicio del archivo para leer las líneas

    arregloString = new string[tamFile]; // Reservar memoria para las líneas no vacías

    int i = 0;
    while (getline(archivo, linea)) {
        if (!linea.empty()) { // Solo almacenar las líneas no vacías
            arregloString[i++] = linea;
        }
    }

    archivo.close();
    return 0;
}


int writeBinaryFile(string &strCodification, const string &outputFileName, bool append = true){
    /**
     * @brief Escribe una cadena de bits ('0' y '1') en un archivo como datos binarios reales.
     *
     * Convierte cada grupo de 8 bits a un byte y lo escribe en un archivo binario.
     *
     * @param strCodification Cadena de bits (debe tener longitud múltiplo de 8).
     * @param outputFileName Ruta del archivo binario de salida.
     * @param append Si es true, se agrega al archivo; si es false, se sobrescribe.
     * @return int 0 si se escribió correctamente; lanza excepciones en caso de error.
     */


    if (strCodification.empty() || strCodification.size() % 8 != 0){ // Verifica si el tamaño de la cadena binaria es múltiplo de 8
        throw "Cadena vacía o no es múltiplo de 8 bits.";
    }

    ofstream outputFile; // Crea el archivo de salida
    if (append){ // Si append es verdadero, abir el archivo en modo "agregar", de lo contrario en modo "sobrescribir"
        outputFile.open(outputFileName, ios::binary | ios::app);
    } else {
        outputFile.open(outputFileName, ios::binary | ios::out | ios::trunc);
    }

    if (!outputFile) {
        throw "Error al abrir el archivo para escritura.";
    }

    try{
        // Iterar a través de la cadena binaria en bloques de 8 bits (1 byte)
        for (size_t i = 0; i < strCodification.size(); i += 8) { //Se procesa 1 byte (8 bits) a la vez

            unsigned char byte = 0;
            for (int j = 0; j < 8; ++j) {
                // Convertir cada carácter '0' o '1' en un valor binario y asignarlo al byte
                // (strCodification[i + j] - '0') convierte un carácter '0' o '1' en un valor binario (0 o 1)
                // << (7 - j) asegura que el bit se coloca en la posición correcta (de izquierda a derecha)
                byte |= (strCodification[i + j] - '0') << (7 - j);
            }

            outputFile.write(reinterpret_cast<const char*>(&byte), sizeof(byte)); // Escribir el byte en el archivo
            if (!outputFile) {
                throw "Error al escribir los datos en el archivo.";
            }
        }
    } catch (...) {
        outputFile.close();
        throw; //
    }

    try {
        outputFile.close();
        if (!outputFile) {
            throw "Error al cerrar el archivo.";
        }
    } catch (...) {
        throw "Error desconocido al intentar cerrar el archivo.";
    }

    return 0;
}

int writeTextFile(string &strCodification, const string &outputFileName){
    /**
     * @brief Convierte una cadena de bits a texto ASCII y lo guarda en un archivo de texto.
     *
     * Cada grupo de 8 bits se interpreta como un carácter ASCII. El resultado se escribe en el archivo.
     *
     * @param strCodification Cadena binaria codificada (debe tener longitud múltiplo de 8).
     * @param outputFileName Ruta del archivo de texto de salida.
     * @return int 0 si se escribió correctamente; lanza excepciones si hay errores.
     */


    if (strCodification.empty() || strCodification.size() % 8 != 0) { //Verificación de la cadena binaria
        throw "La cadena binaria es inválida o no es múltiplo de 8 bits.";
    }

    ofstream outputFile(outputFileName, ios::out | ios::app); // Abrir archivo en modo de escritura
    if (!outputFile) {
        throw "Error al abrir el archivo para escritura.";
    }

    string codification; // Variable para almacenar el texto final convertido desde la cadena binaria

    try {
        // Iterar sobre la cadena binaria en bloques de 8 bits
        for (size_t i = 0; i < strCodification.size(); i += 8) {
            unsigned char byte = 0;
            for (int j = 0; j < 8; ++j) {
                // Convertir cada bit a un byte
                byte |= (strCodification[i + j] - '0') << (7 - j);
            }
            codification += static_cast<char>(byte); // Agregar el byte como carácter ASCII a la cadena
        }

        outputFile.write(codification.c_str(), codification.size()); // Escribir la cadena de texto en el archivo
        if (!outputFile){
            throw "Error al escribir los datos en el archivo.";
        }

        outputFile.close(); //Cierre del archivo
    } catch(...){
        outputFile.close(); // Intentar cerrar el archivo si ocurre una excepción
        throw; // Re-lanzar la excepción
    }
    return 0;
}
