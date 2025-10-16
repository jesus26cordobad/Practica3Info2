#include "fileOperations.h"
#include "encodingMethods.h"
#include "binaryOperations.h"
#include "validations.h"
#include <iostream>
#include <cctype>
#include <bitset>

using namespace std;


void menuAdministrador(int mCod, int sCod, string codRealPassw, string **&users, int &numUsers, bool &modifiedData) {
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
            for (size_t j = 0; j + 8 <= dineroStr.size(); j += 8) {
                string byte = dineroStr.substr(j, 8);
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
    int estadoLectura = readFileChar("users.bin", arrayCharBin, tamFileBin);
    if (estadoLectura == 0) {
        for (int i = 0; i < tamFileBin; i++) {
            binaryData += arrayCharBin[i];
        }
    } else {
        cerr << "Aviso: El archivo 'users.bin' no se leyo (codigo " << estadoLectura << "), se continuara con la ejecucion." << endl;
    }
}
