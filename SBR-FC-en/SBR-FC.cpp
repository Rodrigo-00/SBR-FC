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

#define REGLAY 0    //This macro helps us to express that a rule contains the operator and 
#define REGLA0 1    //This macro helps us to express that a rule contains the operator or 
#define REGLAC 2     //This macro helps us to express that a rule does not contain any operator and therefore only an antecedent 


using namespace std;

ofstream salida;    //Output file

struct regla{   //Structure of a rule 
    double fc;  //Certainty factor of a rule  
    int tipo = REGLAC;  //By default the type of a rule is REGLACv 
    string consecuente, id;
    list<string> antecedentes;  //Rule Background List 

};

struct hecho{   //Structure of a fact
    double fc;  //Certainty factor of a fact 
    string h;   //Fact identifier 

};

list<hecho> bh; //Fact base in list form 
list<regla> bc; //Knowledge base in list form 
hecho *nuevoh;  //Pointer that helps us to create new facts 
regla *nuevar;  //pointer that helps us to create new rules 
list<hecho>::iterator ith;  //Fact list iterator 



void actualizarbh(string h, double fc){ //Function to update the fact base 
    //We reserve memory for a new fact and add it to the fact base 
    nuevoh = new hecho;
    nuevoh->fc = fc;
    nuevoh->h = h;
    bh.push_back(*nuevoh);  //We add the new fact to the list 

}

double conthb(string objv){     //Function that helps us to know if the objective is contained in the fact base 

    ith = bh.begin();
    for (;ith!=bh.end();ith++){
        if(ith->h == objv) return ith->fc; //We return the factor of certainty of the fact 
    }

    return -2;  //We return -2 if the objective is not in the fact base
}


double mototinf(string objv){   //Function corresponding to the inference engine 

    double v = conthb(objv);

    if(v != -2) return v;   //If the objective is contained in the fact base we return its value 

    list<double> fc;    //In this list we save the certainty factors of the objective for each of the rules 
    string reglas, opera;
    list<regla>::iterator itc;
    list<string>::iterator it;

    for(itc = bc.begin();itc!=bc.end();itc++){  //We evaluate the knowledge base until we find a rule that has the objective as a consequence 
        if (itc->consecuente == objv){  //If it has the objective as a consequence we evaluate the rule 
            it = itc->antecedentes.begin();     //We set a pointer to the beginning of the rule's background 
            if (itc->tipo == REGLA0 || itc->tipo == REGLAY){    //If the rule contains the conjunction "and" or "or" we will apply case 1 
                if(itc->tipo == REGLA0){
                    opera = *it;    //We keep the first antecedent 
                    it++;
                    v = max(mototinf(opera),mototinf(*it));    //We apply case 1 for the ficrst two antecedents  
                    it++;   //We move up the antecedents list 
                    for(;it!=itc->antecedentes.end();it++) v = max(v,mototinf(*it));    //We apply case 1 for the other antecedents, so we recursively call the inference engine with each new antecedent 
                    opera = " or ";     //We save the operator as a string 
                }else{
                    opera = *it;    //We keep the first antecedent 
                    it++;   //We move up the antecedents list 
                    v = min(mototinf(opera),mototinf(*it));     //We apply case 1 for the first two antecedents 
                    it++;
                    for(;it!=itc->antecedentes.end();it++) v = min(v,mototinf(*it));    //We apply case 1 for the other antecedents, so we recursively call the inference engine with each new antecedent 
                    opera = " and ";    //We save the operator as a string 
                }

                //We show in the output file the combination of antecedents that is going to be done 
                it = itc->antecedentes.begin();
                salida << "-Combinaci�n de antecedentes de " << itc->id << " (Caso 1):" << "\nFC(" << *it;

                for(;it!=itc->antecedentes.end();it++) salida << opera << *it;

                salida << ") = " << v << endl;

            }else v = mototinf(*it);  //We apply case 3 and we know that we have only one antecedent in the rule 

        fc.push_back(itc->fc * max((double)0,v)); //We apply case 3 
        salida << "-Combinaci�n de la evidencia con la regla " << itc->id << " (Caso 3): \n" << "FC(" << objv << "_" << itc->id << ") = " << itc->fc << " * " << "max(0, "<< v << ") = " <<  fc.back() << endl;
        reglas += ", " + itc->id;  //We add in the rules string the id of the rules that have as a consequence the objective that we are evaluating 
        }
    }

    if(fc.size() > 1){  //If the if is fulfilled, we have more than one rule for a hypothesis and we will apply case 2 accordingly 

        list<double>::iterator itd;
        itd = fc.begin();
        v = *itd;
        itd++;
        double segun;

        salida << "-Combinaci�n de las reglas" << reglas << " (Caso 2):" << endl;

        //We are applying the rules of case 2 in order to obtain the final certainty factor of the objective 
        for(;itd!=fc.end();itd++){
            segun = *itd;
            if(v >= 0 && segun >= 0)  v = v + segun * (1-v);
            else if (v <= 0 && segun <= 0)  v = v + segun * (1+v);
            else v = (v + segun) / (1-min(fabs(v),fabs(segun)));
        }

    }else v = fc.front();

    salida << "FC(" << objv << ") = " << v << endl;
    actualizarbh(objv, v);  //Update the fact base 
    return v;
}

int main(int argc, char **argv)
{

    if(argc!=3){    //We show an error if we get the wrong number of arguments 
        cout << "N�mero de argumentos incorrecto" << endl;
        exit(1);
    }

    ifstream BC,BH;     //Variables corresponding to the bc and bh files 
    string::iterator it;    //We create a string iterator to obtain the substrings that interest us 

    BC.open(argv[1],ios::in); //The file is opened in read mode 

    if(BC.fail()){  //We show an error if the file cannot be opened 
        cout << "No se pudo abrir el fichero correspondiente a la BC" << endl;
        exit(1);
    }

    BH.open(argv[2],ios::in); //The file is opened in read mode 

    if(BH.fail()){  //We show an error if the file cannot be opened 
        cout << "No se pudo abrir el fichero correspondiente a la BH" << endl;
        exit(1);
    }

    string nombre, cadena, c, nbc; //In nbc we save the name of the knowledge base file 

   //We extract the name (without the extension) of the files that contain the BC and the BH 

    cadena = argv[1];   //We get the name of the BC (knowledge base)
    it = cadena.end();
    it--;
    for(;it!=cadena.begin()-1;it--){
        if(*it == '.') nombre.clear();
        else if(*it == '\\') break;
        else nombre = *it + nombre;
    }

    cadena = argv[2]; //We get the name of the BH (facts base)
    it = cadena.end();
    it--;
    for(;it!=cadena.begin()-1;it--){
        if(*it == '.') c.clear();
        else if(*it == '\\') break;
        else c = *it + c;
    }
    nbc = nombre;
    nombre += c;
    nombre += ".txt";   //We add the extension .txt to the final file 

    //We create the output file if it does not exist 
    salida.open(nombre.c_str(),ios::out);

    salida << "Base de conocimientos que se emplea: " << nbc << "\nBase de hechos que se emplea: " << c << endl;

    int nreglasyhechos;
    BC >> nreglasyhechos;  //We identify the number of rules 
    int i=0;

    nuevar = new regla;
    nuevar->id = "R1";

    while (i<nreglasyhechos){

        BC >> c;

        if(c=="Si" || c=="o" || c=="y"){

            //We set the type of the rule
            if(c=="o") nuevar->tipo = REGLA0;
            else if(c=="y") nuevar->tipo = REGLAY;

            BC >> c;    //We extract the antecedent
            nuevar->antecedentes.push_back(c);  //We add the antecedent in the list of antecedents of the rule 

        }
        else if(c=="Entonces"){

                BC >> c;

                c.erase(c.length()-1);  //We erase the comma 
                nuevar->consecuente = c;    //We assign the consequent to the rule 

                BC >> c;    //We clean the string cadena 

                cadena.clear();
                cadena = c;
                cadena.erase(0,3);  //We keep the certainty factor of the rule 

                nuevar->fc = atof(cadena.c_str());

                bc.push_back(*nuevar);  //We add the new rule to the knowledge base 

                i++;

                nuevar = new regla;    //We reserve memory for a new rule 

                BC >> c;    //We read the identifier of the following rule 
                c.erase(c.length()-1);  //We erase :
                nuevar->id = c;     //We set the id of the rule 
        }
    }


    //We read the BH (Facts base)

    BH >> nreglasyhechos;   //We obtain the number of initial events 
    string objetivo;

    for (i=0;i<nreglasyhechos;i++){

        BH >> c;

        nuevoh = new hecho;

        c.erase(c.length()-1);  //We delete the comma 

        nuevoh->h = c;  //We establish the identifier of the new fact 

        BH >> c;    //We read the certainty factor of the fact 

        cadena.clear();     //We clean the string cadena 
        cadena = c;
        cadena.erase(0,3);  //We keep the certainty factor of the fact 

        nuevoh->fc = atof(cadena.c_str());;   //We convert it to double 

        bh.push_back(*nuevoh);  //We add the new fact to the list of facts 

        if(i == nreglasyhechos-1){  //In the last iteration we read the target 
            BH >> c;
            BH >> objetivo;
        }
    }

    salida << "Objetivo: " << objetivo <<endl<<endl;
    salida << "Comenzamos el proceso de inferencia:" <<endl;
    salida << "\n" << objetivo << " se est� dando con un grado de creencia de " << mototinf(objetivo);

    list<regla>::iterator itc;

    //We empty the reserved memory 

    //We erase the antecedents of each rule and the rule 
     for(itc = bc.begin();itc!=bc.end();itc++){
        while(itc->antecedentes.size()!=0){
            delete &*itc->antecedentes.begin();     //We are freeing the memory of the first element of the rule's antecedents list 
            itc->antecedentes.pop_front();
        }
        delete &*itc; //We delete the rule 
    }

    //We delete the facts 
     for(ith = bh.begin();ith!=bh.end();ith++) delete nuevoh;

    //We close the open files 
    BC.close();
    BH.close();
    salida.close();

    return 0;
}
