#include "validations.h"
#include <iostream>
#include <string>

using namespace std;

bool rangeValidation(int numero, int intIni, int intFin){
    /**
     * @brief Verifica si un número entero se encuentra dentro de un rango.
     *
     * Comprueba si el valor de `numero` está en el intervalo cerrado [intIni, intFin].
     *
     * @param numero Número a validar.
     * @param intIni Límite inferior del rango.
     * @param intFin Límite superior del rango.
     * @return true si el número está dentro del rango, false en caso contrario.
     */

    return (numero >= intIni && numero <= intFin); // Retorna verdadero si el número está dentro del rango, de lo contrario falso
}

int intValidation(int limInf,int limSup){

    /**
     * @brief Solicita al usuario un número entero dentro de un rango específico.
     *
     * Pide repetidamente al usuario que ingrese un número hasta que proporcione
     * una entrada válida y dentro del rango [limInf, limSup].
     *
     * @param limInf Límite inferior permitido.
     * @param limSup Límite superior permitido.
     * @return int Número válido ingresado por el usuario.
     */

    int num = 0;

    while (true){
        cout << "Digite la opcion (" << limInf << " - " << limSup << "): ";
        cin >> num;

        // Verificar si la entrada es válida (no contiene caracteres no numéricos)
        if (cin.fail()) {
            cin.clear(); // Limpiar el error de entrada
            cin.ignore(1000, '\n'); //Ignorar caracteres erroneos
            cout << "Entrada no valida. Por favor ingrese un numero." << endl;
        } else if (!rangeValidation(num, limInf, limSup)) {
            cout << "Saldo insuficiente." << endl;
        } else{
            return num;
        }
    }
}

std::string strValidation(size_t limSup, std::string param){
    /**
     * @brief Solicita una cadena con una longitud exacta.
     *
     * Pide al usuario ingresar una cadena cuyo número de caracteres sea igual a `limSup`.
     *
     * @param limSup Longitud exacta requerida.
     * @param param Nombre del campo (para mensajes en consola).
     * @return std::string Cadena ingresada válida.
     */

    bool valid = false; std::string valor = "";

    while (!valid){
        cout << "Ingrese " << param << ": " << endl;
        cin >> valor;

        if (valor.length() == limSup){
            valid = true; // Si la longitud es correcta, la cadena es válida
        } else{
            cout << "La " << param << " debe contener " << limSup << " caracteres." << endl;
        }
    }
    return valor;
}

std::string strRangeValidation(size_t limInf, size_t limSup, std::string param){
    /**
     * @brief Solicita una cadena cuya longitud esté dentro de un rango permitido.
     *
     * Pide al usuario ingresar una cadena con longitud entre `limInf` y `limSup` (inclusive).
     *
     * @param limInf Longitud mínima permitida.
     * @param limSup Longitud máxima permitida.
     * @param param Nombre del campo (para mensajes en consola).
     * @return std::string Cadena ingresada válida.
     */

    bool valid = false; std::string valor = "";

    while (!valid){
        cout << "Ingrese " << param << ": " << endl;
        cin >> valor;

        if (valor.length() >= limInf && valor.length() <= limSup){
            valid = true; // Si la longitud está dentro del rango, la cadena es válida
        } else{
            cout << "La " << param << " debe contener entre " << limInf << " y " << limSup << " caracteres." << endl;
        }
    }
    return valor;
}
