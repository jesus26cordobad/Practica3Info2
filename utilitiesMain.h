#ifndef UTILITIESMAIN_H
#define UTILITIESMAIN_H
#include <string>

void menuAdministrador(int mCod, int sCod, std::string codRealPassw, std::string **&users, int &numUsers, bool &modifiedData);
void menuUsuario(std::string **users, int numUsers, bool &modifiedData);
void leerDatosArchivo(std::string &binaryData, int &tamFileBin, char *&arrayCharBin);

#endif // UTILITIESMAIN_H
