#include "Registro.h"

void printFiles(const string&);

int main() {
    string filename = "data.dat";
    vector<Registro> vec;
    Registro r;
    strcpy(r.carrera,"CS");
    r.ciclo = 5;
    for (int i = 124; i >= 121; --i){
        strcpy(r.codigo,to_string(i*10).c_str());
        vec.push_back(r);
    }

    cout << "================\nInsertar registros\n================\n";
    cout << "\nCodigos antes de ser agregados al archivo\n\t";
    for (auto x : vec) cout << x.codigo << " ";
    cout << endl;

    OrderFileStructure o(filename);
    o.insertAll(vec);

    ifstream File;
    File.open(filename,ios::in| ios::binary);
    Registro record;
    cout << "Registros despues de ser agregados al archivo\n";
    while (File >> record) record.detalle();
    File.close();
    cout << endl ;

    // Busqueda binaria     ¬---puede cambiar el valor de las keys
    cout << "================\nBusqueda binaria\n================\n\n";
    int key1 = stoi(vec[1].codigo), key2 = 0000;
    cout << "Codigo " << key1 << " ubicado en la posicion ";
    cout << o.binarySearch(key1, false) << endl;
    cout << "Codigo " << key2 << " ubicado en la posicion ";
    cout << o.binarySearch(key2, false) << endl;
    cout << endl;

    cout << "================\nAgreagar registros\n================\n\n";
    cout << "Agregar 6 registros adicionales\n";
    Registro re;
    strcpy(re.carrera,"CS");
    re.ciclo = 3;
    for (int i = 1221; i < 1227; ++i){
        strcpy(re.codigo,to_string(i).c_str());
        o.add(re);
    }
    printFiles(filename);

    cout << "================\nEliminar registro\n================\n\n";
    cout << "Eliminar registro de la posicion 1\n";
    cout << "Eliminacion" << (o.deleter(1) ? "exitosa" : "sin exito");
    printFiles(filename);

    cout << "================\nPrueba adicional\n================\n\n";
    cout << "Unir todos los registros en uno solo file luego \nde pasar los 10 registros en el archivo auxiliar\n";
    cout << "Se insertan 5 registros mas\n";
    re.ciclo = 6;
    for (int i = 1231; i < 1236; ++i){
        strcpy(re.codigo,to_string(i).c_str());
        o.add(re);
    }
    printFiles(filename);
    cout << "Notese que se creo un archivo backup\n\n";

    cout << "================\nFin de pruebas\n================\n\n";
    return  0;
}


void printFiles(const string& filename){
    Registro re;
    fstream File;
    cout << endl << " Archivo principal\n";
    File.open(filename, ios::in| ios::binary);
    while (File >> re) re.detalle();
    File.close();
    cout << endl << " Archivo auxiliar\n";
    File.open("auxiliar.dat", ios::in| ios::binary);
    while (File >> re) re.detalle();
    File.close();
    cout << endl;
}