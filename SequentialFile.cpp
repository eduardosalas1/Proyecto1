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
        int auxf_size = size(auxiliarfile);

        if (auxf_size < 10) {
            Alumno temp;
            int record_size = sizeof(Alumno);
            int the_pos = Search(stol(alumno.Dni), true);

            readRecord(filename,temp,the_pos*record_size);

            // Record is at the end of file
            if (temp.next == -1) {
                temp.next = (the_pos + 1) * record_size;
                writeRecord(filename, temp, the_pos*record_size, false);
                writeRecord(filename,alumno, 0, true);
            }
            else {
                // Record in the main file
                if (temp.next > 0) {
                    alumno.next = temp.next;
                    temp.next = -1 * (auxf_size+1) * record_size;
                    writeRecord(filename, temp, the_pos*record_size, false);
                    writeRecord(auxiliarfile, alumno, 0, true);
                }
                    // Record in auxliar file
                else {
                    int pos_it = -1*(temp.next+record_size);
                    readRecord(auxiliarfile, temp, -1*(temp.next+record_size));
                    while (temp.next < 0 && stol(temp.Dni) < stol(alumno.Dni)) {
                        pos_it = -1*(temp.next+record_size);
                        readRecord(auxiliarfile, temp, pos_it);
                    }
                    alumno.next = temp.next;
                    temp.next = -1 * (auxf_size+1) * record_size;
                    writeRecord(auxiliarfile, temp, pos_it, false);
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
        ifstream file;
        if(!less_equal && size(auxiliarfile) > 0) {
            int i = -1;
            Alumno Re;
            file.open(auxiliarfile,ios::in|ios::binary);
            while (file >> Re) {
                if (stol(Re.Dni) == key) return i;
                i--;
            }
            file.close();
        }
        file.open(filename,ios::in|ios::binary);
        if (!file)
            return -1;

        int register_size = sizeof(Alumno);
        int left = 0, right = size(filename) - 1, middle;

        while (left <= right) {
            Alumno re;
            middle = (int)floor((left + right) / 2);
            file.seekg(middle*register_size);
            file >> re;
            while (re.next == 0) {
                middle--;
                file.seekg(middle*register_size);
                file >> re;
            }
            if (stol(re.Dni) < key)
                left = middle + 1;
            else if (stol(re.Dni) > key)
                right = middle - 1;
            else
                return middle;
        }
        file.close();

        if (less_equal) {
            if (right < 0)
                return 0;
            else if (left > (size(filename) - 1))
                return size(filename) - 1;
            else
                return (left < right)
                       ? left : right;
        }
        return -1;
    }

    bool deletion(int pos) {
        parartodo.lock();
        if (pos < 0 || pos > size(filename)-1)
            return false;

        int rcd_size = (int)sizeof(Alumno);
        int d_pos = pos * rcd_size;
        Alumno temp1, temp2;

        readRecord(filename, temp1, d_pos);
        if (temp1.next == 0)
            return false;
        if (pos == 0 || temp1.next == -1) {
            temp1.next = 0;
            writeRecord(filename,temp1, 0, false);
            return true;
        }

        // Find previous record
        int posit;
        string fi_name;
        int prev_rcd = Search(stol(temp1.Dni)-1,true);

        readRecord(filename, temp2, prev_rcd*rcd_size);
        if (temp2.next > 0) {
            fi_name = filename;
            posit = prev_rcd*rcd_size;
        } else {
            posit = -1*(temp2.next+rcd_size);
            readRecord(auxiliarfile, temp2, -1*(temp2.next+rcd_size));
            while (temp2.next < 0) {
                posit = -1*(temp2.next+rcd_size);
                readRecord(auxiliarfile, temp2, posit);
            }
            fi_name = auxiliarfile;
        }

        // Update pointer records
        if (temp2.next == d_pos) {
            temp2.next = temp1.next;
            temp1.next = 0;
            writeRecord(filename, temp1, d_pos, false);
            writeRecord(fi_name, temp2, posit, false);
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

    void readRecord(const string& file, Alumno & temp, int pos) {
        ifstream nfile;
        nfile.open(file, ios::in| ios::binary);
        nfile.seekg(pos);
        nfile >> temp;
        nfile.close();
    }

    void writeRecord(const string &mfile, Alumno &record, int pos, bool app) {
        parartodotres.lock();
        fstream archivo;
        archivo.open(mfile, ios::in| ios::out| ios::binary);
        if (app)
            archivo.seekp(0, ios::end);
        else archivo.seekp(pos);
        archivo << record;
        archivo.close();
        parartodotres.unlock();
    }

    void joinFiles(Alumno& record) {
        int rcd_size = (int)sizeof(Alumno), ntx = 0;
        Alumno temp1;
        readRecord(filename, temp1, 0);
        // rename(file_name.c_str(), bu_name.c_str());
        fstream file(filename, ios::out| ios::binary);
        file.close();

        int pos_it = temp1.next;
        while (pos_it != -1) {
            if (pos_it == 0) {
                temp1.next = (++ntx)*rcd_size;
                writeRecord(filename, temp1, 0, true);
                if (pos_it < 0)
                    readRecord(auxiliarfile,temp1,-1*(pos_it+rcd_size));
                else
                    readRecord(bu_name, temp1, pos_it);
            }

            readRecord(filename,temp1,(size(filename)-1)*rcd_size);
            temp1.next = -1;
            writeRecord(filename, temp1, (size(filename)-1)*rcd_size, false);
            file.open(auxiliarfile, ios::out| ios::binary);
            file.close();
        }
    }
};
