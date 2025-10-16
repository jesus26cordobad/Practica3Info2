#include "encodingMethods.h"
#include <string>

void firstMethodCodification(const std::string dataBinary, std::string &codification, int n){
    /**
     * @brief Codifica una cadena binaria aplicando un método de inversión condicional por bloques.
     *
     * Divide la cadena binaria en bloques de tamaño `n` y aplica distintas transformaciones
     * según la cantidad de unos y ceros en el bloque anterior. En el primer bloque, todos
     * los bits son invertidos. En los siguientes bloques:
     * - Si el bloque anterior tiene más ceros que unos, se invierten los bits en posiciones impares.
     * - Si el bloque anterior tiene más unos que ceros, se invierten los bits en posiciones múltiplos de 3 (comenzando en la posición 2).
     * - Si el bloque anterior tiene igual cantidad de unos y ceros, se invierten todos los bits.
     *
     * @param dataBinary Cadena binaria original a codificar.
     * @param codification Referencia donde se almacenará la cadena codificada.
     * @param n Tamaño de cada bloque para aplicar las reglas de codificación.
     */

    codification = dataBinary; // Copia de la cadena original para no modificarla directamente
    int cycles = (dataBinary.size() + n - 1) / n; // Número de bloques de tamaño n
    int cont1 = 0, cont0 = 0;

    for (int i = 0; i < cycles; i++){
        int beg = i * n; // Índice de inicio del bloque actual

        if (i == 0){ // Para el primer bloque, se invierten todos los bits
            for (int j = 0; j < n; j++){

                if (codification[beg + j] == '1'){
                    codification[beg + j] = '0';
                    cont1 ++;
                }
                else{
                    codification[beg + j] = '1';
                    cont0 ++;
                }
            }
        }
        else{
            //Se usan los contadores del ciclo anterior
            if (cont0 > cont1 ){ // Si hay más ceros, se invierten los bits en posiciones impares
                for (int j = 0 ; j < (n/2); j ++){ // j < n/2 se usa para iterar solo las veces necesarias

                    if (codification[beg + (2*j+1)] == '1'){
                        codification[beg + (2*j+1)] = '0';
                    }
                    else{
                        codification[beg + (2*j+1)] = '1';
                    }
                }
            }
            else if(cont1 > cont0){ // Si hay más unos, se invierten los bits en posiciones múltiplos de 3
                for (int j = 0; j < (n/3); j ++){

                    if (codification[beg + (3*j+2)] == '1'){
                        codification[beg + (3*j+2)] = '0';
                    }
                    else{
                        codification[beg + (3*j+2)] = '1';
                    }
                }
            }
            else{ // Si hay igual cantidad de unos y ceros, se invierten todos los bits
                for (int j = 0; j < n; j ++){

                    if (codification[beg + j] == '1'){
                        codification[beg + j] = '0';
                    }
                    else{
                        codification[beg + j] = '1';
                    }
                }
            }
            // Se reinician los contadores y se cuentan los unos y ceros del bloque actual
            cont1 = 0; cont0 = 0;
            for (int j = 0; j < n; j++){
                if(dataBinary[beg + j] == '1') cont1 ++;
                else cont0 ++;
            }
        }
    }
}

void secondMethodCodification(std::string dataBinary, std::string &codification, int n){
    /**
     * @brief Codifica una cadena binaria aplicando una rotación a la derecha por bloques.
     *
     * Divide la cadena binaria en bloques de tamaño `n` y rota cada bloque una posición hacia la derecha.
     * Solo los bloques de tamaño exacto `n` son procesados; los bloques incompletos al final se ignoran.
     *
     * @param dataBinary Cadena binaria original a codificar.
     * @param codification Referencia donde se almacenará la cadena codificada.
     * @param n Tamaño de cada bloque sobre el cual se aplicará la rotación.
     */

    for (size_t i = 0; i< dataBinary.size(); i += n){ // Iterar sobre la cadena de datos binarios en bloques de tamaño n

        std::string block = dataBinary.substr(i, n);  // Extrae el bloque de n bits

        if (block.size() == static_cast<size_t>(n)){ // Solo procesar bloques de tamaño exacto
            // Rotar los bits del bloque hacia la derecha
            // 'block.substr(n - 1)' toma el último bit del bloque, y 'block.substr(0, n - 1)' toma los primeros (n - 1) bits
            // Al concatenarlos, se consigue el bloque rotado
            std::string rotatedBlock = block.substr(n - 1) + block.substr(0, n - 1);
            codification += rotatedBlock; // Concatenar el bloque rotado a la codificación final
        }
        else{
            codification += block; // Bloques incompletos sin modificar
        }
    }
    // PADDING: Rellenar con ceros hasta que sea múltiplo de 8
    int remainder = codification.size() % 8;
    if (remainder != 0) {
        int padding = 8 - remainder;
        codification.append(padding, '0');
    }
}

void firstMethodDecodification(const std::string dataBinary, std::string &decodified, int n){
    /**
     * @brief Decodifica una cadena binaria previamente codificada con el primer método.
     *
     * Esta función revierte la transformación aplicada por `firstMethodCodification`, dividiendo
     * la cadena binaria en bloques de tamaño `n` y aplicando las reglas inversas:
     * - En el primer bloque se invierte cada bit.
     * - En los bloques siguientes, la transformación depende de la cantidad de unos y ceros
     *   en el bloque anterior (ya decodificado):
     *   - Si hay más ceros: se invierten bits en posiciones impares.
     *   - Si hay más unos: se invierten bits en posiciones múltiplos de 3 (comenzando en la posición 2).
     *   - Si hay la misma cantidad: se invierten todos los bits.
     *
     * @param dataBinary Cadena binaria codificada que se desea decodificar.
     * @param decodified Referencia donde se almacenará la cadena binaria decodificada.
     * @param n Tamaño de los bloques usados durante la codificación (debe coincidir con el valor usado en la codificación).
     */

    decodified = dataBinary; // Copia de la cadena original para modificarla sin afectar la original
    int cycles = (dataBinary.size() + n - 1) / n;  // Número de bloques de tamaño n
    int cont1 = 0, cont0 = 0;

    for (int i = 0; i < cycles; i++){
        int beg = i * n; // Índice de inicio del bloque actual

        if (i == 0){ // Para el primer bloque, se invierte cada bit
            for (int j = 0; j < n; j++){

                if (decodified[beg + j] == '1'){
                    decodified[beg + j] = '0';
                }
                else{
                    decodified[beg + j] = '1';
                }
                // Se cuentan los 0s y 1s en el bloque decodificado
                if (decodified[beg + j] == '1') cont1 ++;
                else cont0++;
            }
        }
        else{
            //Se usan los contadores del bloque anterior
            if (cont0 > cont1 ){ // Si hay más ceros, se invierten los bits en posiciones impares
                for (int j = 0 ; j < (n/2); j ++){ // j < n/2 se usa para iterar solo las veces necesarias

                    if (decodified[beg + (2*j+1)] == '1'){
                        decodified[beg + (2*j+1)] = '0';
                    }
                    else{
                        decodified[beg + (2*j+1)] = '1';
                    }
                }
            }
            else if(cont1 > cont0){ // Si hay más unos, se invierten los bits en posiciones múltiplos de 3
                for (int j = 0; j < (n/3); j ++){

                    if (decodified[beg + (3*j+2)] == '1'){
                        decodified[beg + (3*j+2)] = '0';
                    }
                    else{
                        decodified[beg + (3*j+2)] = '1';
                    }
                }
            }
            else{ // Si hay igual cantidad de unos y ceros, se invierten todos los bits
                for (int j = 0; j < n; j ++){

                    if (decodified[beg + j] == '1'){
                        decodified[beg + j] = '0';
                    }
                    else{
                        decodified[beg + j] = '1';
                    }
                }
            }
            // Recuento de 1s y 0s del bloque actual decodificado para la siguiente iteracion
            cont1 = 0; cont0 = 0;

            for (int j = 0; j < n && beg + j < dataBinary.length(); j++){
                if(dataBinary[beg + j] == '1') cont1 ++;
                else cont0 ++;
            }
        }
    }
}



void secondMethodDecodification(std::string dataBinary, std::string &decodified, int n){
    /**
     * @brief Decodifica una cadena binaria previamente codificada con el segundo método.
     *
     * Esta función revierte la rotación a la derecha aplicada por `secondMethodCodification`.
     * Divide la cadena binaria en bloques de tamaño `n` y rota cada bloque una posición hacia la izquierda.
     * Solo los bloques completos (de tamaño exacto `n`) son considerados.
     *
     * @param dataBinary Cadena binaria codificada que se desea decodificar.
     * @param decodified Referencia donde se almacenará la cadena binaria decodificada.
     * @param n Tamaño de los bloques utilizados durante la codificación (debe coincidir con el valor usado en la codificación).
     */

    for (size_t i = 0; i< dataBinary.size(); i += n){ //Se itera sobre dataBinary, de 'n' en 'n' caracteres

        std::string block = dataBinary.substr(i, n);  // Extrae un bloque de n bits

        if (block.size() == static_cast<size_t>(n)){ // Si el bloque tiene el tamaño exacto de 'n' bits
            // Rotar los bits del bloque hacia la izquieda
            // 'block.substr(1, n - 1)' toma los ultimos tres bits, y 'block[0]' toma el primer bit
            // Al concatenarlos, se consigue el bloque rotado
            std::string rotatedBlock = block.substr(1, n-1) + block[0];
            decodified += rotatedBlock;
        }
        else{
            decodified += block;
        }
    }
}
