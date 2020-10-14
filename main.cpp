#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <tuple>
#include <string>
#include <fstream>
#include <future>
#include <chrono>
#include <ctime>
#include "SequentialFile.cpp"
using namespace std;

int main(){

    /*srand(time(nullptr));

    char nums[10] = {'0','1','2','3','4','5','6','7','8','9'};

    //Dni
    vector<string> vDni;
    for(int i = 0; i < 300; i++){//es 300
        string temp = "";
        temp = temp +  nums[rand()%10] +  nums[rand()%10] +  nums[rand()%10] +  nums[rand()%10] +  nums[rand()%10] +  nums[rand()%10] +  nums[rand()%10] +  nums[rand()%10];
        vDni.push_back(temp);
    }

    //Nombre
    string nombre[30] = {"Alvaro", "Eduardo", "Guillermo", "Wilder", "Miguel", "Gonzalo", "Fernanda", "Maria", "Fiorela", "Carla", "Carlos", "Heider", "Juan", "Jorge", "Valeria", "Chiara", "Ximena", "Ivana", "Cesar", "Paolo", "Paola", "Mario", "Xander", "Johnny", "Kevin", "Jimena", "Andrea", "Marcelo", "Grabiela", "Silvia"};
    vector<string> vNombre;
    for(int i = 0; i < 300; i++){
        string temp2 = "";
        temp2 += nombre[rand()%30];
        vNombre.push_back(temp2);
    }

    //Apellidos
    vector<string> vApellido;
    string apellido[30] = {"Salas", "Palacios", "Casas", "Sanchez", "Albarracin", "Aguirre", "Knel", "Ukzeyi", "Benavides", "Santillana", "Casillas", "Bernal", "Oyague", "Marquez", "Medina", "Campodonico", "Campos", "Sins", "Castillo", "Parker", "Lee", "Yelnats", "Guerrero", "Gaz", "Perez", "Berg", "Kayser", "Bin", "Laden", "Diaz"};
    for(int i = 0; i < 300; i++){
        string temp3 = "";
        temp3 += apellido[rand()%30] + " " + apellido[rand()%30];
        vApellido.push_back(temp3);
    }

    //Carrera
    vector<string> vCarrera;
    string carrera[9] = {"CS", "Bioingenieria", "Mecatronica", "Industrial", "Energia", "DS", "Mecanica", "Quimica", "Electronica"};
    for(int i = 0 ; i < 300 ; i++){
        string temp4 = "";
        temp4 += carrera[rand()%9];
        vCarrera.push_back(temp4);
    }


    //Mensualidad
    vector<float> vMensualidad;
    float mensualidad[8] = {1050.5, 1310.5, 2000, 2020.2, 3708.2, 3500.8, 4400.9, 4999.9};
    for(int i = 0; i < 300; i++){
        float temp5 = mensualidad[rand()%8];
        vMensualidad.push_back(temp5);
    }


    ofstream file;
    file.open("Alumnos.dat",ios::out);
    if(file.is_open()){
        file<<"Dni|Nombre|Apellido|Carrera|Mensualidad"<<endl;

        for(auto i = 0 ; i < vDni.size() ; i++){
            file<<vDni.at(i)<<'|'<<vNombre.at(i)<<'|'<<vApellido.at(i)<<'|'<<vCarrera.at(i)<<'|'<<vMensualidad.at(i)<<endl;
        }

    }



    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
    string filename = "Alumnos.dat";

    Alumno alumno1("99999999", "Eduardo", "Salas Palacios", "CS", 2000.45);
    Alumno alumno2("69696969","Andrea","Aguirre","CS",3000);

    SequentialFile sf(filename);

    sf.insertAll();
    
    sf.add(alumno1);

    auto start = chrono::steady_clock::now();
    sf.Search(57301492,true);
	auto end = chrono::steady_clock::now();

    cout<<chrono::duration_cast<chrono::microseconds>(end - start).count() << " microsec"<<endl;


    //thread t0(&SequentialFile::insertAll, &sf);
    //t0.join();

    

    

    
   // sf.add(alumno1);

    
   // sf.Search(99999999,true);

    

  

   

/*
    thread taux(&SequentialFile::add, &sf, alumno1);
    taux.join();
    //sf.add(alumno1);
    auto auxaux = async(&SequentialFile::Search, &sf, 99999999, true);
    auto auxaux2 = auxaux.get();
    cout << auxaux2 << endl;
    //cout << sf.Search(99999999, true)<<endl;
    auto ayuda = async(&SequentialFile::deletion, &sf, 30);
    auto ayuda2 = ayuda.get();
    cout << ayuda2 << endl;
    //cout<<sf.deletion(30)<<endl;
    thread t3(&SequentialFile::add, &sf, alumno2);
    t3.join();
    //sf.add(alumno2);
    auto aux1 = async(&SequentialFile::Search, &sf, 99999999, true);
    auto respuesta1 = aux1.get();
    cout << respuesta1 << endl;
    //cout << sf.Search(99999999, true)<<endl;
    auto aux2 = async(&SequentialFile::Search, &sf, 69696969, true);
    auto respuesta2 = aux2.get();
    cout << respuesta2 << endl;
    //cout<<sf.Search(69696969,true)<<endl;
    auto ayuda3 = async(&SequentialFile::deletion, &sf, 207);
    auto ayuda4 = ayuda3.get();
    cout << ayuda4 << endl;
    //cout<<sf.deletion(207)<<endl;

    ifstream file("Alumnos.dat");
    int pos = 207*sizeof(Alumno);
    Alumno temp;
    file.seekg(pos);

    file>>temp;

    cout<<temp.next;
*/

    return 0;

}