#include "fileOperations.h"
#include "encodingMethods.h"
#include "binaryOperations.h"

#include <iostream>
#include <cctype>

using namespace std;

int main()
{
    cout << "CODIFICACION" << endl;
    string openFileCod = "";
    //cout << "Ingrese el nombre del archivo fuente: " << endl;
    //cin >> openFileCod;
    openFileCod = "archivo.txt";

    int sCod = 0, tamCod = 0;

    char *arrayCharCod = nullptr;
    readFileChar(openFileCod, arrayCharCod, tamCod); //Se lee el archivo por caracter
    string binaryCod = "";

    for (int i = 0; i < tamCod; i++){
        if (isspace(arrayCharCod[i])) {
            continue; // Ignorar todos los caracteres de espacio en blanco
        }
        binaryCod += binaryConversion(arrayCharCod[i]); // Se concatena cada bin(char[i]) en un string
    }

    delete[] arrayCharCod; // Se libera la memoria del arreglo char, y se trabaja con el string de binarios
    arrayCharCod = nullptr;

    char mCod;
    cout << "Digite el metodo que desea para codificar: " << endl;
    cout << "1. Primer metodo: codificacion por cantidad de bits.\n2. Segundo metodo: codificacion por rotacion de 1 bit hacia la derecha." << endl;
    cin >> mCod;
    cout << "Ingrese la cantidad de bits para cada bloque: " << endl;
    cin >> sCod;
    //seed = 4;

    //FALTA VALIDAR ENTRADAS

    //m = '2';

    if (mCod == '1'){
        cout << "El archivo en binario seria: " << endl;
        cout << binaryCod << endl;

        string codifBinary = "";
        firstMethodCodification(binaryCod,codifBinary, sCod);
        cout << "El archivo codificado con el primer metodo seria: " << endl;
        cout << codifBinary << endl;

        string outputFile = "archivoCodificado.bin";
        writeBinaryFile (codifBinary, outputFile);
    }
    else{
        cout << "El archivo en binario seria: " << endl;
        cout << binaryCod << endl;

        string codifBinary = "";
        secondMethodCodification(binaryCod, codifBinary, sCod);
        cout << "El archivo codificado con el segundo metodo seria: " << endl;
        cout << codifBinary << endl;

        string outputFile = "archivoCodificado.bin";
        writeBinaryFile (codifBinary, outputFile);
    }

    cout << "DECODIFICACION" << endl;

    string openFileDec = "";
    //cout << "Ingrese el nombre del archivo fuente: " << endl;
    //cin >> openFileDec;
    openFileDec = "archivoCodificado.bin";

    int sDec = 0, tamDec = 0;

    char *arrayCharDec = nullptr;
    readFileChar(openFileDec, arrayCharDec, tamDec); //Se lee el archivo por caracter
    string binaryDec = "";

    for (int i = 0; i < tamDec; i++){
        if (isspace(arrayCharDec[i])) {
            continue; // Ignorar todos los caracteres de espacio en blanco
        }
        binaryDec += binaryConversion(arrayCharDec[i]); // Se concatena cada bin(char[i]) en un string
    }

    delete[] arrayCharDec; // Se libera la memoria del arreglo char, y se trabaja con el string de binarios
    arrayCharDec = nullptr;

    char mDec;
    cout << "Digite el metodo que desea para decodificar: " << endl;
    cout << "1. Primer metodo: decodificacion por cantidad de bits.\n2. Segundo metodo: decodificacion por rotacion de 1 bit hacia la derecha." << endl;
    cin >> mDec;
    cout << "Ingrese la cantidad de bits para cada bloque: " << endl;
    cin >> sDec;
    //seed = 4;

    //FALTA VALIDAR ENTRADAS

    //m = '2';

    if (mDec == '1'){
        cout << "El archivo en binario seria: " << endl;
        cout << binaryDec << endl;

        string decodText = "";
        firstMethodDecodification(binaryDec,decodText, sDec);
        cout << "El archivo decodificado con el primer metodo seria: " << endl;
        cout << decodText << endl;

        string outputFile = "archivoDecodificado.txt";
        writeTextFile (decodText, outputFile);
    }
    else{
        cout << "El archivo en binario seria: " << endl;
        cout << binaryDec << endl;

        string decodText = "";
        secondMethodDecodification(binaryDec, decodText, sDec);
        cout << "El archivo decodificado con el segundo metodo seria: " << endl;
        cout << decodText << endl;

        string outputFile = "archivoDecodificado.txt";
        writeTextFile (decodText, outputFile);
    }
    return 0;
}
