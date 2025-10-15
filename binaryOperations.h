#ifndef BINARYOPERATIONS_H
#define BINARYOPERATIONS_H
#include <string>

std::string binaryConversion(char character);
void loadUsersFromBin(const std::string &binaryData, std::string **&users, int &numUsers, int metodoCod, int cantidadBits);
std::string textToBinary(const std::string &input);
std::string binaryToText(const std::string &binary);

#endif // BINARYOPERATIONS_H
