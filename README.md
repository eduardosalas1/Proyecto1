# Proyecto1
 - Eduardo Salas Palacios 201820145
 - Alvaro Aguirre Bernal 201820076
 - Guillermo Sanchez Albarracin 201720046

## Introducción
El objetivo del proyecto es poner en práctica técnicas de organización de archivos implementadas para poder comparar su eficiencia y velocidad.

Los datos usados para este proyecto son una elaboración propia. Diseñamos un algoritmo para poder crear dnis, nombres, apellidos, carreras y mensualidades aleatorias. En total emplearemos 300 datos.

En este trabajo se espera poder implementar correctamente dos técnicas de organización de archivos y experimentar con ellas para poder darnos cuenta de cuál es mejor.

## Descripción

Elegimos dos técnicas: Extendible Hashing y Sequential File.

### Extendible Hashing

La primera usa una clase Alumno con atributos de tipo string y un float. Creamos una clase Bucket en la cual declaramos la profundidad local y el tamaño, es decir, cuántos elementos puede tener antes de hacer overflow. Esta clase consta de ocho métodos las cuales explicaremos a continuación.

    - isFull() : Esta función retornará un booleano que indica si el bucket está lleno o no.

    - insert(Alumno* key) : Introduce un alumno al bucket.

    - search(string dni) :  Busca la posición en la que se encuentra el alumno que posee como llave dni.

    - copy(vector<Alumno*> &temp) : Copia los alumnos del bucket en un vector temporal.

    - display() : Imprime en consola los alumnos insertados en el Bucket.

    - increasedepth() : Aumenta la profundidad del bucket a medida que se divida en el directorio principal.

    - del(string Key) : Borra un alumno que posee la llave dni.

La clase Directory cuenta con cuatro métodos privados:

    - hash(int n) : 

    - split(int bucket_num) : 

    - reinsert(int bucket_num) : 

    - Doubledirectory() : 

### Sequential File



## Resultados experimentales


## Pruebas