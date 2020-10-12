#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

struct Alumno {

	char Dni [8];
    char Nombre[15] ;
    char Apellidos[15];
    char Carrera [10];
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