#include <bits/stdc++.h>
#include <string>
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

		if (isFull())
			return -1;
		for(int i = 0; i < listaAlumnos.size(); i++)
			if (listaAlumnos.at(i)->getDni() == key->getDni())
				return 0;
		listaAlumnos.push_back(key);
		return 1;
	};
	
	int search(string dni){
		
		for(int i = 0; i < listaAlumnos.size(); i++)
		if (listaAlumnos.at(i)->getDni() == dni)
			return 1;
		return 0;

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
	for(int i = 0; i < listaAlumnos.size(); i++)
		if (listaAlumnos.at(i)->getDni() == key){
			listaAlumnos.erase(listaAlumnos.begin() + i);
			return;
		}
	cout << "No existe tal Key" << endl;
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

	int hash(int n){
		return n&( (1 << globaldepth) - 1);
	}

	void split(int bucket_num){

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
	}

	void reinsert(int bucket_num){
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

	}
	void del(string dni){
		int bucket_num = hash(stoi(dni));
		buckets[bucket_num]->del(dni);
	}

	void search(string dni){

		int bucket_num = hash(stoi(dni));
		int banderita = buckets[bucket_num]->search(dni);
		if (banderita == 1){
			cout << dni << " existe " << bucket_num <<  endl;
		}
		else{
			cout << dni << " no existe " << endl;
		}
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

/*
void Directory :: del(string key){
	int bucket_num = hash(stoi(key));
	buckets[bucket_num]->del(key);
}*/

/*
int Directory :: hash (int n){
	return n&( (1 << globaldepth) - 1);
}*/

/*
void Directory :: Doubledirectory(){
	for(int i = 0; i < (1 << globaldepth); i++)
		buckets.push_back(buckets.at(i));
	this->globaldepth++;
	//cout << "line 94 " << this->globaldepth << endl;;
}*/

/*
void Directory :: reinsert(int bucket_num){
	vector <Alumno*> temp;
	buckets[bucket_num]->copy(temp);
	for(int i = 0; i < temp.size(); i++){
		Alumno* key = temp.at(i);
		int bucket_num  = hash(stoi(key->getDni()));
		//cout << "new bucket_num" << bucket_num << endl;
		int banderita = buckets[bucket_num]->insert(key);
		if (banderita == -1){
			split(bucket_num);
			buckets[bucket_num]->insert(key);
		}
	}
}*/

/*
void Directory :: split(int bucket_num){
	int localdepth = buckets[bucket_num]->getdepth();
	//cout << globaldepth << " " << localdepth << endl;
	if (globaldepth == localdepth)
		Doubledirectory();
	int mirrorindex = bucket_num ^ (1 << localdepth);
	buckets[bucket_num]->increasedepth();
	localdepth++;
	//cout << buckets[bucket_num]->getdepth() << endl;
	buckets[mirrorindex] = new Bucket(localdepth, bucketsize);
	int num = 1 << localdepth;
	for(int i = mirrorindex + num; i < (1 << globaldepth); i += num)
		buckets[i] = buckets[mirrorindex];
	for(int i = mirrorindex - num; i >=0 ; i -= num)
		buckets[i] = buckets[mirrorindex];
	reinsert(bucket_num);
}*/

/*
void Directory :: insert(Alumno* key){
	int bucket_num = hash(stoi(key->getDni()));
	int banderita = buckets[bucket_num]->insert(key);
	if (banderita == -1){
        cout << "splitting bucketnum " << bucket_num << endl;
		split(bucket_num);
		insert(key);
	}
	else if (banderita == 0){
		cout << "Key already exists" << endl;
	}
	else{
		cout << key << " inserted in bucket having number " << bucket_num << endl;
	}
}*/

/*
void Directory :: search (string dni){
	int bucket_num = hash(stoi(dni));
	int banderita = buckets[bucket_num]->search(dni);
	if (banderita == 1){
		cout << dni << " exists in bucket number " << bucket_num <<  endl;
	}
	else{
		cout << dni << " doesnot exist " << endl;
	}
}*/
/*
void Directory :: display(){
    set <Bucket*> s;
	for(int i = 0; i < (1 << globaldepth); i++){
        if (s.find(buckets[i]) != s.end())
            continue;
		cout << i << ": ";
		buckets[i]->display();
        s.insert(buckets[i]);
	}
}*/

/*
void Directory :: leerAlumnos(string file) {
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
}*/

int main(){
	int globaldepth, bucket_size;
	cin >> globaldepth >> bucket_size;
	Directory d(globaldepth, bucket_size);
	d.leerAlumnos("Alumnos.txt");
	int opcion;
	cin >> opcion;
	while(opcion != -1){
		if (opcion == 3){
			string key ;
			cin>>key;
			d.search(key);
		}
		else if (opcion == 4){
			string key;
			cin>>key;
			d.del(key);
		}
		else if (opcion == 5){
            d.display();
		}
		else if(opcion == 6){
			string Dni; cin >> Dni;
			string Nombre; cin >> Nombre;
			string Apellidos; cin >> Apellidos;
			string Carrera; cin >> Carrera;
			float Mensualidad; cin >> Mensualidad;
			Alumno* alumno = new Alumno(Dni, Nombre, Apellidos, Carrera, Mensualidad);
			d.insert(alumno);
		}
		cin >> opcion;
	}

}

