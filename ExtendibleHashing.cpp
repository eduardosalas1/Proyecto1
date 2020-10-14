#include <bits/stdc++.h>
#include <string>
#include <chrono>
#include <ctime>
using namespace std;

class Alumno {
private:
	string Dni;
    string Nombre;
    string Apellidos;
    string Carrera;
    float Mensualidad;

public:
    Alumno(string Dni, string nombre, string apellidos, string carrera, float mensualidad) : Dni{std::move(Dni)}, Nombre{std::move(nombre)}, Apellidos{std::move(apellidos)}, Carrera{std::move(carrera)}, Mensualidad{mensualidad} {}

    void mostrarAlumno() {
		cout << "Dni        : " << Dni << endl;
        cout << "Nombre     : " << Nombre << endl;
        cout << "Apellidos  : " << Apellidos << endl;
        cout << "Carrera    : " << Carrera << endl;
        cout << "Mensualidad: " << Mensualidad << endl;
    }

    string getDni(){
        return Dni;
    }

    string getNombre(){
        return Nombre;
    }

    string getApellidos(){
        return Apellidos;
    }

    string getCarrera(){
        return Carrera;
    }

    float getMensualidad(){
        return Mensualidad;
    }
};



class Bucket{
	vector <Alumno*> listaAlumnos;
	int localdepth;
	int size;
	mutex parartodo, parartododos, parartodotres;

	public:
	Bucket(int depth, int size){
		this->size = size;
		this->localdepth = depth;
	}
	
	bool isFull(){
		if (listaAlumnos.size() == size)
		return true;
	else
		return false;
	};

	int insert(Alumno* key){
		parartodo.lock();
		if (isFull())
			return -1;
		for(int i = 0; i < listaAlumnos.size(); i++)
			if (listaAlumnos.at(i)->getDni() == key->getDni())
				return 0;
		listaAlumnos.push_back(key);
		return 1;
		parartodo.unlock();
	};
	
	int search(string dni){
		parartododos.lock();
		for(int i = 0; i < listaAlumnos.size(); i++)
		if (listaAlumnos.at(i)->getDni() == dni)
			return 1;
		return 0;
		parartododos.unlock();

	};
	
	void copy(vector <Alumno*> &temp){
		for(int i = 0; i < listaAlumnos.size(); i++)
        	temp.push_back(listaAlumnos.at(i));
    	listaAlumnos.clear();

	};

	int getdepth(){
		return this->localdepth;
	
	};

	void display(){
		cout << localdepth << "   ";
    	for(int i = 0; i < listaAlumnos.size(); i++)
        	listaAlumnos.at(i)->mostrarAlumno();cout << " ";
    	cout << endl;
	};

	void del(string key){
	parartodotres.lock();
	for(int i = 0; i < listaAlumnos.size(); i++)
		if (listaAlumnos.at(i)->getDni() == key){
			listaAlumnos.erase(listaAlumnos.begin() + i);
			return;
		}
	cout << "No existe tal Key" << endl;
	parartodotres.unlock();
}

	void increasedepth(){
    	this->localdepth++;
	}
};


/*void Bucket :: increasedepth(){
    this->localdepth++;

}*/

/*bool Bucket :: isFull(){
	if (listaAlumnos.size() == size)
		return true;
	else
		return false;
}*/

/*int Bucket :: insert(Alumno* key){
	if (isFull())
		return -1;
	for(int i = 0; i < listaAlumnos.size(); i++)
		if (listaAlumnos.at(i)->getDni() == key->getDni())
			return 0;
	listaAlumnos.push_back(key);
	return 1;
}*/

/*int Bucket :: search(string dni){
	for(int i = 0; i < listaAlumnos.size(); i++)
		if (listaAlumnos.at(i)->getDni() == dni)
			return 1;
	return 0;
}*/


class Directory{
	int globaldepth;
	int bucketsize;
	vector <Bucket *> buckets;
	mutex parartodocuatro, parartodocinco, parartodoseis,parartodosiete,
	parartodoocho;

	int hash(int n){
		return n&( (1 << globaldepth) - 1);
	}

	void split(int bucket_num){
		parartodocuatro.lock();
		int localdepth = buckets[bucket_num]->getdepth();
		if (globaldepth == localdepth)
			Doubledirectory();
		int mirrorindex = bucket_num ^ (1 << localdepth);
		buckets[bucket_num]->increasedepth();
		localdepth++;
		buckets[mirrorindex] = new Bucket(localdepth, bucketsize);
		int num = 1 << localdepth;
		for(int i = mirrorindex + num; i < (1 << globaldepth); i += num)
			buckets[i] = buckets[mirrorindex];
		for(int i = mirrorindex - num; i >=0 ; i -= num)
			buckets[i] = buckets[mirrorindex];
		reinsert(bucket_num);
		parartodocuatro.unlock();

	}

	void reinsert(int bucket_num){
		parartodocinco.lock();
		vector <Alumno*> temp;
		buckets[bucket_num]->copy(temp);
		for(int i = 0; i < temp.size(); i++){
			Alumno* key = temp.at(i);
			int bucket_num  = hash(stoi(key->getDni()));
			int banderita = buckets[bucket_num]->insert(key);
			if (banderita == -1){
				split(bucket_num);
				buckets[bucket_num]->insert(key);
			}
		}
		parartodocinco.unlock();
	}

	void Doubledirectory(){
		for(int i = 0; i < (1 << globaldepth); i++)
			buckets.push_back(buckets.at(i));
		this->globaldepth++;
	}
	
	public:

	Directory(int globaldepth, int bucket_size){
		this->globaldepth = globaldepth;
		this->bucketsize = bucket_size;
		for(int i = 0; i <  (1 << globaldepth); i++)
			buckets.push_back(new Bucket(globaldepth, bucket_size));
	}
	
	void leerAlumnos(string file){

		ifstream archivo(file);
    	if (archivo.is_open()) {
    	    string campos[5], fila;
    	    getline(archivo, fila);
    	    while (!archivo.eof()) {
    	        getline(archivo, fila);
    	        istringstream stringStream(fila);
    	        uint contador = 0;
    	        while (getline(stringStream, fila, '|')) {
    	            campos[contador] = fila;
    	            contador++;
    	        }
    	        Alumno* alumno = new Alumno(campos[0], campos[1], campos[2], campos[3], stof(campos[4]));
    	        insert(alumno);
    	        cout << endl;
    	    }
    	}

	}

	void insert(Alumno* key){
		parartodoseis.lock();
		int bucket_num = hash(stoi(key->getDni()));
		int banderita = buckets[bucket_num]->insert(key);
		if (banderita == -1){
    	    cout << "separando bucket... " << bucket_num << endl;
			split(bucket_num);
			insert(key);
		}
		else if (banderita == 0){
			cout << "Ya existe el dni" << endl;
		}
		else{
			cout << key << " insertado " << bucket_num << endl;
		}
		parartodoseis.unlock();


	}
	void del(string dni){
		parartodosiete.lock();
		int bucket_num = hash(stoi(dni));
		buckets[bucket_num]->del(dni);
		parartodosiete.unlock();
	}

	void search(string dni){
		parartodoocho.lock();
		int bucket_num = hash(stoi(dni));
		int banderita = buckets[bucket_num]->search(dni);
		if (banderita == 1){
			cout << dni << " existe " << bucket_num <<  endl;
		}
		else{
			cout << dni << " no existe " << endl;
		}
		parartodoocho.unlock();

	}

	void display(){

		set <Bucket*> s;
		for(int i = 0; i < (1 << globaldepth); i++){
    	    if (s.find(buckets[i]) != s.end())
    	        continue;
			cout << i << ": ";
			buckets[i]->display();
    	    s.insert(buckets[i]);
		}
	}
};

int main(){
	int globaldepth, bucket_size;
	cout << "Ingresa la profundidad global: \n";
	cin >> globaldepth;
	cout << "Ingresa el tamaño del bucket: \n";
	cin >> bucket_size;
	Directory d(globaldepth, bucket_size);
	thread principal(&Directory::leerAlumnos, &d, "Alumnos.txt");
	principal.join();
	//d.leerAlumnos("Alumnos.txt");
	int opcion;
	cout << "Ingrese la opcion: \n-1: salir\n 1: mostrar registros\n 2: insertar registro\n 3: buscar un registro\n 4: eliminar registro\n";
	cin >> opcion;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	while(opcion != -1){
		if (opcion == 3){
			string key ;
			cin>>key;
			auto start = chrono::steady_clock::now();
			thread t0(&Directory::search, &d, key);
			t0.join();
			//d.search(key);
			auto end = chrono::steady_clock::now();
			cout << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microsec";
		}
		else if (opcion == 4){
			string key;
			cin>>key;
			thread t1(&Directory::del, &d, key);
			t1.join();
			//d.del(key);
		}
		else if (opcion == 1){
			thread t3(&Directory::display, &d);
			t3.join();
            //d.display();
		}
		else if(opcion == 2){
			string Dni;
			cout << "Ingrese el dni" ; cin >> Dni;
			string Nombre;
			cout << "Ingrese el nombre"; cin >> Nombre;
			string Apellidos;
			cout << "Ingrese el apellido"; cin >> Apellidos;
			string Carrera; 
			cout << "Ingrese la carrera"; cin >> Carrera;
			float Mensualidad;
			cout << "Ingrese la mensualidad"; cin >> Mensualidad;
			Alumno* alumno = new Alumno(Dni, Nombre, Apellidos, Carrera, Mensualidad);
			auto start = chrono::steady_clock::now();
			thread t2(&Directory::insert, &d, alumno);
			t2.join();
			//d.insert(alumno);
			auto end = chrono::steady_clock::now();
			cout << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microsec";
		}
		cout << "Ingrese la opcion: \n-1: salir\n 1: mostrar registros\n 2: insertar registro\n 3: buscar un registro\n 4: eliminar registro\n";
		cin >> opcion;
	}

	
   

    chrono::duration<double> elapsed_seconds = end-start;
    time_t end_time = chrono::system_clock::to_time_t(end);

    cout<<"finished computation at"<<ctime(&end_time)<<endl;
    cout<<"elapsed time:"<< elapsed_seconds.count() <<"s\n";


	return 0;

}

