#include <bitset>
#include "encodingMethods.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string binaryConversion(char character){
    /**
     * @brief Convierte un carácter a su representación binaria de 8 bits.
     *
     * Esta función toma un carácter ASCII y lo convierte a una cadena que representa
     * su valor binario en formato de 8 bits.
     *
     * @param character El carácter a convertir.
     * @return QString Una cadena de 8 caracteres ('0' o '1') representando el valor binario del carácter.
     */

    bitset<8> bits(character); // convertir caracter a su representación binaria de 8 bits
    return bits.to_string(); // convertir el bitset a una cadena de string
}

void loadUsersFromBin(const std::string &binaryData, std::string **&users, int &numUsers, int metodoCod, int cantidadBits) {
    /**
     * @brief Carga los datos de usuarios desde una cadena binaria en memoria.
     *
     * Esta función toma una cadena de datos binarios codificados, la convierte a texto
     * interpretando los campos de cédula, clave y dinero para cada usuario. Utiliza un
     * separador específico entre bloques de usuario y un método de decodificación determinado.
     *
     * @param binaryData Cadena de datos binarios a decodificar.
     * @param users Referencia a un puntero doble donde se almacenarán los datos de los usuarios.
     *              Cada usuario tiene un arreglo de 3 strings de binarios: cédula, clave y dinero.
     * @param numUsers Referencia al número de usuarios cargados.
     * @param metodoCod Método de codificación usado: 1 para el primero, cualquier otro para el segundo.
     * @param cantidadBits Número de bits utilizado en la codificación.
     */
    const std::string separador = "00101111"; // Separador entre bloques de usuario

    if (binaryData.empty()) { //Si no hay datos binarios, no hace nada
        numUsers = 0;
        users = nullptr;
        ofstream outFile("users.bin", ios::binary); // Crea el archivo si no existe
        if (!outFile) {
            cerr << "Error al crear el archivo 'users.bin'" << endl;
            return;
        }
        outFile.close();
        return;
    }

    // Convertir los datos binarios a una cadena de bits ('0' y '1')
    string bitStream = "";
    for (unsigned char c : binaryData) {
        for (int i = 7; i >= 0; --i) {
            bitStream += ((c >> i) & 1) ? '1' : '0';
        }
    }

    // Dividir la cadena de bits en bloques, usando el separador
    vector<string> bloquesUsuarios;
    size_t start = 0, end;
    while ((end = bitStream.find(separador, start)) != string::npos) {
        bloquesUsuarios.push_back(bitStream.substr(start, end - start));
        start = end + separador.length();
    }

    // Agregar el último bloque si no tiene el separador al final
    if (start < bitStream.size()) {
        bloquesUsuarios.push_back(bitStream.substr(start));
    }

    numUsers = bloquesUsuarios.size(); // Número de usuarios encontrados
    users = new string*[numUsers];

    for (int idx = 0; idx < numUsers; ++idx) {
        users[idx] = new string[3]; // Crear espacio para cédula, clave y dinero
        string bloque = bloquesUsuarios[idx];

        // Verifica que el bloque tenga la longitud mínima esperada (cedula + clave = 80 + 32 = 112 bits)
        if (bloque.length() < 112) {
            cerr << "Bloque " << idx << " muy corto. Se ignora.\n";
            continue;
        }

        // Dividir el bloque en partes: cédula (80 bits), clave (32 bits) y dinero (resto)
        string bitsCedula = bloque.substr(0, 80);
        string bitsClave = bloque.substr(80, 32);
        string bitsDinero = bloque.substr(112); // Lo que queda

        string decodedCedula, decodedClave, decodedDinero;

        if (metodoCod == 1) {
            firstMethodDecodification(bitsCedula, decodedCedula, cantidadBits);
            firstMethodDecodification(bitsClave, decodedClave, cantidadBits);
            firstMethodDecodification(bitsDinero, decodedDinero, cantidadBits);
        } else {
            secondMethodDecodification(bitsCedula, decodedCedula, cantidadBits);
            secondMethodDecodification(bitsClave, decodedClave, cantidadBits);
            secondMethodDecodification(bitsDinero, decodedDinero, cantidadBits);
        }

        // Guardar los datos decodificados en el arreglo de usuarios
        users[idx][0] = decodedCedula;
        users[idx][1] = decodedClave;
        users[idx][2] = decodedDinero;
    }
}
