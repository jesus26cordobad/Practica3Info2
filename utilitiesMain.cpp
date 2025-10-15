#include "fileOperations.h"
#include "encodingMethods.h"
#include "binaryOperations.h"
#include "validations.h"
#include <iostream>
#include <cctype>
#include <bitset>

using namespace std;


void menuAdministrador(int mCod, int sCod, string codRealPassw, string **&users, int &numUsers, bool &modifiedData) {
    /**
     * @brief Menú de administrador que permite registrar un nuevo usuario en el sistema.
     *
     * Verifica la contraseña del administrador aplicando una codificación binaria
     * y, si es válida, solicita los datos del nuevo usuario, los convierte a binario
     * y los almacena en el arreglo dinámico de usuarios.
     *
     * @param mCod Método de codificación a usar (1 o 2).
     * @param sCod Semilla o clave adicional para el método de codificación.
     * @param codRealPassw Contraseña codificada válida del administrador.
     * @param users Referencia doble al arreglo dinámico de usuarios.
     * @param numUsers Referencia al número actual de usuarios (será incrementado).
     * @param modifiedData Referencia a la bandera que indica si hubo cambios en los datos.
     */
    string passwInp, binPasswInp = "", codPasswInp = "";
    cout << "Ingrese la contrasena de administrador: " << endl;
    cin >> passwInp;

    // Convertir contraseña a binario
    for (size_t i = 0; i < passwInp.length(); i++) {
        if (isspace(passwInp[i])) continue;
        binPasswInp += binaryConversion(passwInp[i]);
    }

    // Codificar contraseña binaria usando el método correspondiente
    if (mCod == 1) {
        firstMethodCodification(binPasswInp, codPasswInp, sCod);
    } else {
        secondMethodCodification(binPasswInp, codPasswInp, sCod);
    }

    // Validar contraseña y registrar nuevo usuario si es correcta
    if (codPasswInp == codRealPassw) {

        cout << "\n--- Registro de usuarios ---\n" << endl;
        string identification = strValidation(10, "identificacion del nuevo usuario");
        string key = strValidation(4, "clave del nuevo usuario");
        string money = strRangeValidation(4, 10, "saldo del nuevo usuario");

        // Convertir los datos a binario
        string binIdentification = "", binKey = "", binMoney = "";
        for (char c : identification) binIdentification += binaryConversion(c);
        for (char c : key) binKey += binaryConversion(c);
        for (char c : money) binMoney += binaryConversion(c);

        // Crear nuevo arreglo con espacio para el nuevo usuario
        string **nuevo = new string*[numUsers + 1];
        for (int i = 0; i < numUsers; ++i) {
            nuevo[i] = new string[3];
            for (int j = 0; j < 3; ++j) {
                nuevo[i][j] = users[i][j];
            }
        }

        // Asignar datos binarios del nuevo usuario
        nuevo[numUsers] = new string[3];
        nuevo[numUsers][0] = binIdentification;
        nuevo[numUsers][1] = binKey;
        nuevo[numUsers][2] = binMoney;

        // Liberar memoria del arreglo anterior
        for (int i = 0; i < numUsers; ++i) {
            delete[] users[i];
        }
        delete[] users;

        // Actualizar puntero y cantidad de usuarios
        users = nuevo;
        numUsers++;
        modifiedData = true;

        cout << "Usuario registrado exitosamente." << endl;
    } else {
        cout << "Contrasena incorrecta." <<endl;
    }
}

void menuUsuario(string **users, int numUsers, bool &modifiedData) {
    /**
     * @brief Menú de usuario que permite consultar o retirar dinero tras autenticación.
     *
     * Verifica credenciales del usuario ingresado y, si son válidas, le permite
     * consultar su saldo o realizar un retiro. Se descuenta un valor fijo por el uso.
     *
     * @param users Arreglo dinámico de usuarios registrados en el sistema.
     * @param numUsers Número total de usuarios registrados.
     * @param modifiedData Referencia a la bandera que indica si hubo cambios en los datos.
     */

    string cedulaIng = strValidation(10, "cedula");
    string claveIng = strValidation(4, "clave");

    // Convertir datos ingresados a binario
    string binCedulaIng = "", binClaveIng = "";
    for (char c : cedulaIng) binCedulaIng += binaryConversion(c);
    for (char c : claveIng) binClaveIng += binaryConversion(c);

    bool found = false;

    // Buscar coincidencia en los usuarios registrados
    for (int i = 0; i < numUsers; i++) {
        string id = users[i][0];
        string clave = users[i][1];
        string dineroStr = users[i][2];

        if (binCedulaIng == id && binClaveIng == clave) {
            found = true;

            // Convertir saldo binario a string
            string actMoneyStr = "";
            for (size_t i = 0; i + 8 <= dineroStr.size(); i += 8) {
                string byte = dineroStr.substr(i, 8);
                char c = static_cast<char>(bitset<8>(byte).to_ulong());
                actMoneyStr += c;
            }

            // Convertir saldo a entero
            int actMoneyInt = 0;
            try {
                actMoneyInt = stoi(actMoneyStr);
            } catch (const invalid_argument& e) {
                cout << "Error al convertir dinero a entero: " << e.what() << endl;
                continue;
            }

            // Descontar costo por uso del cajero
            actMoneyInt -= 1000;
            if (actMoneyInt < 0) actMoneyInt = 0;

            cout << "\n--- Menu usuarios ---\n1. Consultar dinero.\n2. Retirar dinero\n";
            int opcUser = intValidation(1, 2);

            if (opcUser == 1) {
                cout << "Saldo actual: " << actMoneyInt << endl;
                cout << "\nSe ha descontado 1000 COP por el uso del cajero.\n";

            } else {
                int retiro = intValidation(0, actMoneyInt);
                actMoneyInt -= retiro;
                cout << "Retiro exitoso.\nNuevo saldo: " << actMoneyInt << endl;
                cout << "\nSe ha descontado 1000 COP por el uso del cajero.\n";
            }

            // Actualizar saldo en formato binario
            string newMoneyStr = to_string(actMoneyInt);
            string newMoneyBin = "";
            for (char c : newMoneyStr) {
                newMoneyBin += binaryConversion(c);
            }

            users[i][2] = newMoneyBin;
            modifiedData = true;
            break;
        }
    }

    if (!found) {
        cout << "Usuario o clave incorrecta." << endl;
    }
}

void leerDatosArchivo(string &binaryData, int &tamFileBin, char *&arrayCharBin) {
    /**
     * @brief Lee el archivo binario de usuarios y convierte su contenido a una cadena binaria.
     *
     * Intenta leer el archivo "users.bin" en formato binario. Si se lee exitosamente,
     * su contenido se almacena en un string binario y en un arreglo de caracteres.
     *
     * @param binaryData Referencia al string donde se almacenará el contenido binario leído.
     * @param tamFileBin Referencia al entero que guarda el tamaño del archivo leído.
     * @param arrayCharBin Referencia al puntero donde se almacenará el arreglo leído desde el archivo.
     */
    int estadoLectura = readFileChar("users.bin", arrayCharBin, tamFileBin);
    if (estadoLectura == 0) {
        for (int i = 0; i < tamFileBin; i++) {
            binaryData += arrayCharBin[i];
        }
    } else {
        cerr << "Aviso: El archivo 'users.bin' no se leyo (codigo " << estadoLectura << "), se continuara con la ejecucion." << endl;
    }
}
