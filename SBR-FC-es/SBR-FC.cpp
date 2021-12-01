/**
*   @author rodebox00
*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <string>
#include <math.h>
#include <list>

#define REGLAY 0    //Esta macro nos sirve para expresar que una regla contiene el operador y
#define REGLA0 1    //Esta macro nos sirve para expresar que una regla contiene el operador o
#define REGLAC 2     //Esta macro nos sirve para expresar que una regla no contiene ning�n operador y por tanto solo un antecedente


using namespace std;

ofstream salida;    //fichero de salida

struct regla{   //Estructura de un hecho
    double fc;  //Factor de certeza de la regla
    int tipo = REGLAC;  //Por defecto el tipo de una regla es REGLAC
    string consecuente, id;
    list<string> antecedentes;  //Lista de antecedentes de la regla

};

struct hecho{   //Estructura de un hecho
    double fc;  //Factor de certeza de un hecho
    string h;   //Identificador del hecho

};

list<hecho> bh; //Base de hechos en forma de lista
list<regla> bc; //Base de conocimiento en forma de lista
hecho *nuevoh;  //puntero que nos sirve para crear nuevos hechos
regla *nuevar;  //puntero que nos sirve para crear nuevas reglas
list<hecho>::iterator ith;  //Iterador de la lista de hechos



void actualizarbh(string h, double fc){ //Funcion para actualizar la base de hechos
    //Reservamos memoria para un nuevo hecho y lo a�adimos a la base de hechos
    nuevoh = new hecho;
    nuevoh->fc = fc;
    nuevoh->h = h;
    bh.push_back(*nuevoh);  //A�adimos el nuevo hecho a la lista

}

double conthb(string objv){     //Funcion que nos ayuda a saber si el objetivo esta contenido en la base de hechos

    ith = bh.begin();
    for (;ith!=bh.end();ith++){
        if(ith->h == objv) return ith->fc; //retornamos el factor de certeza del hecho
    }

    return -2;  //Retornamos -2 si el objetivo no se encuentra en la base de hechos
}


double mototinf(string objv){   //Funci�n correspondiente al motor de inferencia

    double v = conthb(objv);

    if(v != -2) return v;   //Si el objetivo est� contenido en la base de hechos devolvemos su valor

    list<double> fc;    //En esta lista guardamos los factores de certeza del objetivo para cada una de las reglas
    string reglas, opera;
    list<regla>::iterator itc;
    list<string>::iterator it;

    for(itc = bc.begin();itc!=bc.end();itc++){  //Vamos evaluando la base de conocimientos hasta que encontramos una regla que tiene como consecuente el objetivo
        if (itc->consecuente == objv){  //Si tiene el objetivo como consecuente evaluamos la regla
            it = itc->antecedentes.begin();     //Establecemos un puntero al principio de antecedentes de la regla
            if (itc->tipo == REGLA0 || itc->tipo == REGLAY){    //Si la regla contiene la conjunci�n y u o aplicaremos el caso 1
                if(itc->tipo == REGLA0){
                    opera = *it;    //Guardamos el primer antecedente
                    it++;
                    v = max(mototinf(opera),mototinf(*it));    //Aplicamos el caso 1 para los dos primeros antecedentes
                    it++;   //Avanzamos en la lista de antecedentes
                    for(;it!=itc->antecedentes.end();it++) v = max(v,mototinf(*it));    //Aplicamos el caso 1 para los dem�s antecedentes, por lo que llamamos recursivamente al motor de inferencia con cada nuevo antecedente
                    opera = " or ";     //Guardamos el operador en forma de string
                }else{
                    opera = *it;    //Guardamos el primer antecedente
                    it++;   //Avanzamos en la lista de antecedentes
                    v = min(mototinf(opera),mototinf(*it));     //Aplicamos el caso 1 para los dos primeros antecedentes
                    it++;
                    for(;it!=itc->antecedentes.end();it++) v = min(v,mototinf(*it));    //Aplicamos el caso 1 para los dem�s antecedentes, por lo que llamamos recursivamente al motor de inferencia con cada nuevo antecedente
                    opera = " and ";    //Guardamos el operador en forma de string
                }

                //Mostramos en el fichero salida la combinaci�n de antecedentes que se va a hacer
                it = itc->antecedentes.begin();
                salida << "-Combinaci�n de antecedentes de " << itc->id << " (Caso 1):" << "\nFC(" << *it;

                for(;it!=itc->antecedentes.end();it++) salida << opera << *it;

                salida << ") = " << v << endl;

            }else v = mototinf(*it);  //Aplicamos el caso 3 y sabemos que tenemos solo un antecedente en la regla

        fc.push_back(itc->fc * max((double)0,v)); //Aplicamos el caso 3
        salida << "-Combinaci�n de la evidencia con la regla " << itc->id << " (Caso 3): \n" << "FC(" << objv << "_" << itc->id << ") = " << itc->fc << " * " << "max(0, "<< v << ") = " <<  fc.back() << endl;
        reglas += ", " + itc->id;  //A�adimos en el string reglas el id de las reglas que tienen como consecuente el objetivo que estamos evaluando
        }
    }

    if(fc.size() > 1){  //Si se cumple el if tenemos m�s de una regla para una hip�tesis y apllicaremos el caso 2 en consecuencia

        list<double>::iterator itd;
        itd = fc.begin();
        v = *itd;
        itd++;
        double segun;

        salida << "-Combinaci�n de las reglas" << reglas << " (Caso 2):" << endl;

        //Vamos aplicando las reglas del caso 2 con el fin de obtener el factor de certeza final del objetivo
        for(;itd!=fc.end();itd++){
            segun = *itd;
            if(v >= 0 && segun >= 0)  v = v + segun * (1-v);
            else if (v <= 0 && segun <= 0)  v = v + segun * (1+v);
            else v = (v + segun) / (1-min(fabs(v),fabs(segun)));
        }

    }else v = fc.front();

    salida << "FC(" << objv << ") = " << v << endl;
    actualizarbh(objv, v);  //Actualizamos la base de hechos
    return v;
}

int main(int argc, char **argv)
{

    if(argc!=3){    //Mostramos un error si obtenemos un numero incorrecto de argumentos
        cout << "N�mero de argumentos incorrecto" << endl;
        exit(1);
    }

    ifstream BC,BH;     //Variables correspondientes a los ficheros de la bc y la bh
    string::iterator it;    //Creamos un iterador de strings para obtener las subcandenas que nos interesan

    BC.open(argv[1],ios::in); //Se abre el fichero en modo lectura

    if(BC.fail()){  //Mostramos un error si no se puede abrir el fichero
        cout << "No se pudo abrir el fichero correspondiente a la BC" << endl;
        exit(1);
    }

    BH.open(argv[2],ios::in); //Se abre el fichero en modo lectura

    if(BH.fail()){  //Mostramos un error si no se puede abrir el fichero
        cout << "No se pudo abrir el fichero correspondiente a la BH" << endl;
        exit(1);
    }

    string nombre, cadena, c, nbc; //En nbc guardamos el nombre del fichero de la base de conocimientos

   //Extraemos el nombre (sin la extensi�n) de los ficheros que contienen la BC y la BH

    cadena = argv[1];   //Obtenemos el nombre de la BC
    it = cadena.end();
    it--;
    for(;it!=cadena.begin()-1;it--){
        if(*it == '.') nombre.clear();
        else if(*it == '\\') break;
        else nombre = *it + nombre;
    }

    cadena = argv[2]; //Obtenemos el nombre de la BH
    it = cadena.end();
    it--;
    for(;it!=cadena.begin()-1;it--){
        if(*it == '.') c.clear();
        else if(*it == '\\') break;
        else c = *it + c;
    }
    nbc = nombre;
    nombre += c;
    nombre += ".txt";   //A�adimos la extensi�n .txt al fichero final

    //Creamos el fichero de salida si no existe
    salida.open(nombre.c_str(),ios::out);

    salida << "Base de conocimientos que se emplea: " << nbc << "\nBase de hechos que se emplea: " << c << endl;

    int nreglasyhechos;
    BC >> nreglasyhechos;  //Identificamos el numero de reglas
    int i=0;

    nuevar = new regla;
    nuevar->id = "R1";

    while (i<nreglasyhechos){

        BC >> c;

        if(c=="Si" || c=="o" || c=="y"){

            //Extablecemos el tipo de la regla
            if(c=="o") nuevar->tipo = REGLA0;
            else if(c=="y") nuevar->tipo = REGLAY;

            BC >> c;    //Extraemos el antecedente
            nuevar->antecedentes.push_back(c);  //A�adimos el antecedente en la lista de antecedentes de la regla

        }
        else if(c=="Entonces"){

                BC >> c;

                c.erase(c.length()-1);  //Borramos la coma
                nuevar->consecuente = c;    //Asignamos el consecuente a la regla

                BC >> c;    //Limpiamos el string cadena

                cadena.clear();
                cadena = c;
                cadena.erase(0,3);  //Nos quedamos con el factor de certeza de la regla

                nuevar->fc = atof(cadena.c_str());

                bc.push_back(*nuevar);  //A�adimos la nueva regla a la base de conocimientos

                i++;

                nuevar = new regla;    //Reservamos memoria para una nueva regla

                BC >> c;    //Leemos el identificador de la siguiente regla
                c.erase(c.length()-1);  //Borramos los dos puntos
                nuevar->id = c;     //Establecemos el id de la regla
        }
    }


    //LEEMOS LA BH

    BH >> nreglasyhechos;   //Obtenemos el numero de hechos iniciales
    string objetivo;

    for (i=0;i<nreglasyhechos;i++){

        BH >> c;

        nuevoh = new hecho;

        c.erase(c.length()-1);  //Borramos la coma

        nuevoh->h = c;  //Establecemos el identificador del nuevo hecho

        BH >> c;    //Leemos el factor de certeza del hecho

        cadena.clear();     //Limpiamos el string cadena
        cadena = c;
        cadena.erase(0,3);  //Nos quedamos con el factor de certeza del hecho

        nuevoh->fc = atof(cadena.c_str());;   //convertimos a double

        bh.push_back(*nuevoh);  //A�adimoms el nuevo hecho a la lista de hechos

        if(i == nreglasyhechos-1){  //En la �ltima iteraci�n leemos el objetivo
            BH >> c;
            BH >> objetivo;
        }
    }

    salida << "Objetivo: " << objetivo <<endl<<endl;
    salida << "Comenzamos el proceso de inferencia:" <<endl;
    salida << "\n" << objetivo << " se est� dando con un grado de creencia de " << mototinf(objetivo);

    list<regla>::iterator itc;

    //Vaciamos la memoria reservada

    //Borramos los antecedentes de cada regla y la regla
     for(itc = bc.begin();itc!=bc.end();itc++){
        while(itc->antecedentes.size()!=0){
            delete &*itc->antecedentes.begin();     //Vamos liberando la memoria del primer elemento de la lista de antecedentes de la regla
            itc->antecedentes.pop_front();
        }
        delete &*itc; //Borramos la regla
    }

    //Borramos los hechos
     for(ith = bh.begin();ith!=bh.end();ith++){
        //nuevoh = &*ith;
        delete nuevoh;
    }

    //Cerramos los ficheros abiertos
    BC.close();
    BH.close();
    salida.close();

    return 0;
}
