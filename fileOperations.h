#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H
#include <string>

int readFileChar(const std::string &file, char *&arregloChar, int &tamFile);
int readFileString(const std::string &file, std::string *&arregloString, int &tamFile);
int writeBinaryFile(std::string &strCodification, const std::string &outputFileName, bool append);
int writeTextFile(std::string &strDecodification, const std::string &outputFileName);

#endif // FILEOPERATIONS_H
