#include "fileOperations.h"
#include "binaryOperations.h"
#include "validations.h"
#include "encodingMethods.h"
#include "utilitiesMain.h"
#include <iostream>
#include <cctype>

using namespace std;

int main() {

    // Mostrar menú de métodos de codificación disponibles
    cout << "Metodos para codificar: " << endl;
    cout << "1. Codificacion por cantidad de bits.\n2. Codificacion por rotacion de 1 bit hacia la derecha." << endl;
    int mCod = intValidation(1, 2);

    // Solicitar tamaño de bloque para la codificación
    cout << "Ingrese la cantidad de bits para cada bloque: " << endl;
    int sCod = intValidation(3, 8);

    // Variables para almacenar la contraseña del administrador
    int tamRealPassw = 0;
    char *arrayRealPassw = nullptr;
    string binRealPassw = "";

    try {
        // Leer contraseña del archivo y convertir a binario (ignorando espacios)
        readFileChar("password.txt", arrayRealPassw, tamRealPassw);
        for (int i = 0; i < tamRealPassw; i++) {
            if (isspace(arrayRealPassw[i])) continue;
            binRealPassw += binaryConversion(arrayRealPassw[i]);
        }
        delete[] arrayRealPassw;  // Liberar memoria del arreglo leído
    } catch (const char* msg) {
        cerr << "Error al leer el archivo: " << msg << endl;
        return 1;
    }

    // Codificar la contraseña del administrador según el método elegido
    string codRealPassw = "";
    if (mCod == 1) {
        firstMethodCodification(binRealPassw, codRealPassw, sCod);
    } else {
        secondMethodCodification(binRealPassw, codRealPassw, sCod);
    }
    writeTextFile(codRealPassw, "sudo.txt"); // Guardar contraseña codificada

    // Cargar datos binarios de los usuarios desde archivo
    int tamFileBin = 0;
    char *arrayCharBin = nullptr;
    string binaryData = "";
    leerDatosArchivo(binaryData, tamFileBin, arrayCharBin);

    // Decodificar usuarios desde los datos binarios
    int numUsers = 0;
    string **users = nullptr;
    loadUsersFromBin(binaryData, users, numUsers, mCod, sCod);

    bool modifiedData = false; // Indica si hubo cambios en los datos
    cout << "\n**********BIENVENIDO**********\n";

    bool exit = false;
    while (!exit) {
        // Menú principal del sistema
        cout << "\nMENU PRINCIPAL:\n1. Menu administrador.\n2. Menu usuario.\n3. Salir.\n";
        int opc = intValidation(1, 3);

        if (opc == 1) {
            menuAdministrador(mCod, sCod, codRealPassw, users, numUsers, modifiedData);
        } else if (opc == 2) {
            menuUsuario(users, numUsers, modifiedData);
        } else {
            exit = true;

            // Guardar siempre si hay usuarios, no solo si mofiedData es true
            if (numUsers > 0) {
                const string separador = "00101111"; // Separador binario entre usuarios
                string totalCodif = "";

                for (int j = 0; j < numUsers; j++) {
                    // Codificar cada campo del usuario (cédula, clave, dinero)
                    for (int i = 0; i < 3; i++) {
                        string codifData = users[j][i];
                        string binData = "";

                        // Aplicar codificación según el método elegido
                        if (mCod == 1) {
                            firstMethodCodification(codifData, binData, sCod);
                        } else {
                            secondMethodCodification(codifData, binData, sCod);
                        }

                        totalCodif += binData;
                    }

                    // Añadir separador entre usuarios (excepto después del último)
                    if (j < numUsers - 1) {
                        totalCodif += separador;
                    }
                }

                // Guardar datos binarios codificados en el archivo
                try {
                    writeBinaryFile(totalCodif, "users.bin", false);
                    cout << "Datos guardados correctamente." << endl;
                } catch (const char* msg) {
                    cerr << "Error al guardar: " << msg << endl;
                }
            }

            cout << "Ha salido del programa" << endl;

            // Liberar memoria usada por la matriz de usuarios
            for (int i = 0; i < numUsers; i++) {
                delete[] users[i];
            }
            delete[] users;
        }
    }
    return 0;
}
