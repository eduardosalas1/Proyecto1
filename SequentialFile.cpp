#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <cmath>

using namespace std;

struct Alumno {

    char Dni [8];
    char Nombre[15] ;
    char Apellidos[25];
    char Carrera [15];
    float Mensualidad;
    int next = -1;

    Alumno(){}

    Alumno(string Dni, string nombre, string apellidos, string carrera, float mensualidad){

        for(auto i = 0 ; i < Dni.size() ; i++)
            this->Dni[i] = Dni[i];

        for(auto i = 0 ; i < nombre.size() ; i++)
            this->Nombre[i] = nombre[i];

        for(auto i = 0 ; i < apellidos.size() ; i++)
            this->Apellidos[i] = apellidos[i];

        for(auto i = 0 ; i < carrera.size() ; i++)
            this->Carrera[i] = carrera[i];

        this->Mensualidad = mensualidad;

    }

    void mostrarAlumno() {
        cout << "Dni        : ";

        for(auto i : Dni)
            cout<<i;

        cout<<endl;

        cout << "Nombre     : " ;

        for(auto i : Nombre)
            cout<<i;

        cout<<endl;

        cout << "Apellidos  : ";

        for(auto i: Apellidos)
            cout<<i;

        cout<<endl;

        cout << "Carrera    : ";

        for(auto i: Carrera)
            cout<<i;

        cout<<endl;

        cout << "Mensualidad: " << Mensualidad << endl;
    }

};

ostream & operator << (ostream & stream, Alumno & alumno) {
    stream.write((char*)&alumno, sizeof(alumno));
    return stream;
}

istream & operator >> (istream & stream, Alumno & alumno) {
    stream.read((char*)&alumno, sizeof(alumno));
    return stream;
}

class SequentialFile{
private:
    string filename;
    string auxiliarfile = "auxiliar.dat";
    string bu_name = "data.dat";
    vector<Alumno> alumnos;
    mutex parartodo, parartododos, parartodotres;

    void clear(){
        ofstream fileof;
        fileof.open(filename,ios::out | ios::trunc);
        fileof.close();
        fileof.open(auxiliarfile,ios::out | ios::trunc);
        fileof.close();
    }

    static bool criterio(Alumno alumnoA, Alumno alumnoB){

        return stol(alumnoA.Dni)<stol(alumnoB.Dni);

    }

public:

    SequentialFile(string file): filename(file){

        ifstream archivo(file);

        if (archivo.is_open()) {

            string campos[5], fila;
            getline(archivo, fila);

            while (!archivo.eof()) {

                getline(archivo, fila);
                istringstream stringStream(fila);
                unsigned int contador = 0;

                while (getline(stringStream, fila, '|')) {
                 
                    campos[contador] = fila;
                    contador++;

                }

                Alumno alumno(campos[0], campos[1], campos[2], campos[3], stof(campos[4]));
                alumnos.push_back(alumno);
                cout << endl;
            
            }

        }

        clear();

    }

    void insertAll(){

        sort(alumnos.begin(),alumnos.end(),criterio);
        int numAlumnos = alumnos.size();
        int alumnoSize = sizeof(Alumno);

        fstream file;
        file.open(filename,ios::out|ios::binary|ios::app);

        if(!file) return;

        for(auto i = 0; i < numAlumnos - 1 ; ++i){
            alumnos.at(i).next = (i+1)*alumnoSize;
            file << alumnos.at(i);
        }

        file << alumnos.at(numAlumnos-1);
        file.close();

    }


    void add(Alumno alumno){

        parartododos.lock();
        int auxiliarSize = size(auxiliarfile);

        if (auxiliarSize < 10) {
         
            Alumno temp;
            int alumnoSize = sizeof(Alumno);
            int posicion = Search(stol(alumno.Dni), true);

            readRecord(filename,temp,posicion*alumnoSize);

            // Alumno al final del File
            if (temp.next == -1) {
                temp.next = (posicion + 1) * alumnoSize;
                writeRecord(filename, temp, posicion*alumnoSize, false);
                writeRecord(filename,alumno, 0, true);
            }
            else {

                // Alumno en el main File
                if (temp.next > 0) {
                    alumno.next = temp.next;
                    temp.next = -1 * (auxiliarSize+1) * alumnoSize;
                    writeRecord(filename, temp, posicion*alumnoSize, false);
                    writeRecord(auxiliarfile, alumno, 0, true);
                }
                    // Alumno en el auxiliar file
                else {

                    int posAuxiliar = -1*(temp.next+alumnoSize);
                    readRecord(auxiliarfile, temp, -1*(temp.next+alumnoSize));
                    while (temp.next < 0 && stol(temp.Dni) < stol(alumno.Dni)) {
                        posAuxiliar = -1*(temp.next+alumnoSize);
                        readRecord(auxiliarfile, temp, posAuxiliar);
                    }
                    alumno.next = temp.next;
                    temp.next = -1 * (auxiliarSize+1) * alumnoSize;
                    writeRecord(auxiliarfile, temp, posAuxiliar, false);
                    writeRecord(auxiliarfile, alumno, 0, true);
                }
            }
        }
        else {
            joinFiles(alumno);
        }
        parartododos.unlock();
    }


    int Search(long key, bool less_equal = false){

        ifstream File;
        if(!less_equal && size(auxiliarfile) > 0) {
            int i = -1;
            Alumno alumno;
            File.open(auxiliarfile,ios::in|ios::binary);
            while (File >> alumno) {
                if (stol(alumno.Dni) == key) return i;
                i--;
            }
            File.close();
        }
        File.open(filename,ios::in|ios::binary);
        if (!File)
            return -1;

        int alumnoSize = sizeof(Alumno);
        int izquierda = 0, derecha = size(filename) - 1, medio;

        while (izquierda <= derecha) {
            Alumno alumno;
            medio = (int)floor((izquierda + derecha) / 2);
            File.seekg(medio*alumnoSize);
            File >> alumno;
            while (alumno.next == 0) {
                medio--;
                File.seekg(medio*alumnoSize);
                File >> alumno;
            }
            if (stol(alumno.Dni) < key)
                izquierda = medio + 1;
            else if (stol(alumno.Dni) > key)
                derecha = medio - 1;
            else
                return medio;
        }
        File.close();

        if (less_equal) {
            if (derecha < 0)
                return 0;
            else if (izquierda > (size(filename) - 1))
                return size(filename) - 1;
            else
                return (izquierda < derecha)
                       ? izquierda : derecha;
        }
        return -1;
    }

    bool deletion(int posicion) {
        parartodo.lock();
        if (posicion < 0 || posicion > size(filename)-1)
            return false;

        int alumnoSize = (int)sizeof(Alumno);
        int deletePosicion = posicion * alumnoSize;
        Alumno temp1, temp2;

        readRecord(filename, temp1, deletePosicion);
        if (temp1.next == 0)
            return false;
        if (posicion == 0 || temp1.next == -1) {
            temp1.next = 0;
            writeRecord(filename,temp1, 0, false);
            return true;
        }

        // Encontrar Registro Previo
        int posicionPrevia;
        string Filename;
        int alumnoPrevio = Search(stol(temp1.Dni)-1,true);

        readRecord(filename, temp2, alumnoPrevio*alumnoSize);
        if (temp2.next > 0) {
            Filename = filename;
            posicionPrevia = alumnoPrevio*alumnoSize;
        } else {
            posicionPrevia = -1*(temp2.next+alumnoSize);
            readRecord(auxiliarfile, temp2, -1*(temp2.next+alumnoSize));
            while (temp2.next < 0) {
                posicionPrevia = -1*(temp2.next+alumnoSize);
                readRecord(auxiliarfile, temp2, posicionPrevia);
            }
            Filename = auxiliarfile;
        }

        // Sobreescribir Alumnos
        if (temp2.next == deletePosicion) {
            temp2.next = temp1.next;
            temp1.next = 0;
            writeRecord(filename, temp1, deletePosicion, false);
            writeRecord(Filename, temp2, posicionPrevia, false);
            return true;
        }

        return false;
        parartodo.unlock();
    }

    int size(const string& Fname){

        int numRecords = 0;
        fstream inFile;
        inFile.open(Fname, ios::in | ios::binary);
        if (inFile.is_open()) {
            inFile.seekg(0, ios::end);
            long bytes = inFile.tellg();
            numRecords = bytes / sizeof(Alumno);
            inFile.close();
        } else cout << "No se pudo abrir el archivo.\n";
        return numRecords;
    }

    void readRecord(const string& file, Alumno & temp, int posicion) {

        ifstream ReadFile;
        ReadFile.open(file, ios::in| ios::binary);
        ReadFile.seekg(posicion);
        ReadFile >> temp;
        ReadFile.close();

    }

    void writeRecord(const string &mfile, Alumno &alumno, int posicion, bool app) {
        parartodotres.lock();
        fstream archivo;
        archivo.open(mfile, ios::in| ios::out| ios::binary);
        if (app)
            archivo.seekp(0, ios::end);
        else archivo.seekp(posicion);
        archivo << alumno;
        archivo.close();
        parartodotres.unlock();
    }

    void joinFiles(Alumno& alumno) {

        int alumnoSize = (int)sizeof(Alumno), ntx = 0;
        Alumno temp1;
        readRecord(filename, temp1, 0);
        // rename(file_name.c_str(), bu_name.c_str());
        fstream file(filename, ios::out| ios::binary);
        file.close();

        int pos_it = temp1.next;
        while (pos_it != -1) {
            if (pos_it == 0) {
                temp1.next = (++ntx)*alumnoSize;
                writeRecord(filename, temp1, 0, true);
                if (pos_it < 0)
                    readRecord(auxiliarfile,temp1,-1*(pos_it+alumnoSize));
                else
                    readRecord(bu_name, temp1, pos_it);
            }

            readRecord(filename,temp1,(size(filename)-1)*alumnoSize);
            temp1.next = -1;
            writeRecord(filename, temp1, (size(filename)-1)*alumnoSize, false);
            file.open(auxiliarfile, ios::out| ios::binary);
            file.close();
        }
    }
};