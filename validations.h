#ifndef VALIDATIONS_H
#define VALIDATIONS_H
#include <string>

bool rangeValidation(int numero, int intIni, int intFin);
int intValidation(int limInf,int limSup);
std::string strValidation(size_t limSup, std::string param);
std::string strRangeValidation(size_t limInf, size_t limSup, std::string param);

#endif // VALIDATIONS_H
