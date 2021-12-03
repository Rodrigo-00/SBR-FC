# 1. Introducción:
Este repositorio contiene un SBR (sistema basado en reglas) que incorpora el conocimiento sobre 
la incertidumbre expresado por factores de certeza (FC), por tanto se denomina SBR-FC. Este sistema 
SBR-FC permite tomar decisiones en distintas situaciones expresadas. Por lo tanto, se diseña 
un motor de inferencia con encadenamiento hacia atrás adaptado al nuevo conocimiento disponible.

# 2. Manual de uso:
Para la ejecución del programa se debe de introducir por una terminal la dirección donde se encuentra el ejecutable SBR-FC.exe dentro del dispositivo. Como parámetros seguidamente separado por un espacio se debe indicar la dirección 
del fichero que contiene la base de conocimientos. Por último y al igual que con la base de conocimientos se debe indicar la ruta del fichero portador de la base de hechos. Cabe destacar que cualquiera de los ficheros o el ejecutable
se pueden indicar sin ruta si ejecutamos el programa desde el directorio en el que se encuentra. Los ficheros correspondientes a la base de conocimientos y la base de hechos deben ser ficheros de texto, el factor de certeza asociado 
tanto a los hechos como a las reglas debe estar ente [-1,1]. Ambas bases se componen del siguiente formato:

	-La base de conocimientos está compuesta por una primera línea que indica el número de reglas que contiene seguido de las reglas. Cada una de las reglas se encuentra en una línea distinta y contiene el formato
	<<Identificador: Regla, FC=factor>> donde Regla contiene la palabra reservada Entonces para indicar el consecuente de la regla y las conjunciones reservadas <<y, o>> que se usan entre los antecedentes. En relación 
	al Identificador este debe ser del tipo Ri donde i será el número de la regla.

	-La base de hechos, al igual que la base de conocimientos contiene como primera línea el número de hechos que contiene seguido de los hechos en cada línea. El formato de cada línea queda de la forma <<hecho, FC=factor>> .
	Para indicar el objetivo del programa se debe poner en la penúltima línea la palabra <<Objetivo>> y en la siguiente línea el hecho meta del que se quiere obtener el grado de certeza.
	
El programa genera un fichero salida cuyo nombre será la unión de los nombres de la base de conocimientos y la base de hechos con extensión .txt. Este fichero contendrá en las dos primeras líneas el nombre de la base de conocimientos
y la base de hechos empleada para el programa y en la siguiente línea el hecho objetivo. A continuación se mostrará el proceso de inferencia seguido hasta la obtención del factor de certeza del hecho meta que se indicará por último.

------------------------------------------------------------------------------------

# 1.Introduction:
This repository contains an SBR (rule-based system) that incorporates knowledge about
the uncertainty expressed by certainty factors (FC), therefore it is called SBR-FC. This system
SBR-FC allows decisions to be made in different expressed situations. Therefore, it is designed
a backward chaining inference engine adapted to new available knowledge.

# 2. User manual:
To execute the program, the address where the SBR-FC.exe executable is located inside the device must be entered through a terminal. As parameters then separated by a space, the address must be indicated
of the file that contains the knowledge base. Finally, and as with the knowledge base, the path of the file that carries the fact base must be indicated. It should be noted that any of the files or the executable
They can be indicated without a path if we execute the program from the directory in which it is located. The files corresponding to the knowledge base and the fact base must be text files, the associated certainty factor
both the facts and the rules must be between [-1,1]. Both bases are composed of the following format:

	-The knowledge base is made up of a first line that indicates the number of rules it contains followed by the rules. Each of the rules is on a different line and contains the format
	<<Identificador: Regla, FC=factor>> where Rule contains the reserved word Then to indicate the consequent of the rule and the reserved conjunctions <<y, o>> that are used between the antecedents. In relation
	al Identifier this must be of the type Ri where i will be the number of the rule.

	-The fact base, like the knowledge base, contains as the first line the number of facts it contains followed by the facts in each line. The format of each line is in the form <<hecho, FC=factor>> .
	To indicate the objective of the program, put the word <<Objetivo>> in the penultimate line and in the next line the target fact from which the degree of certainty is to be obtained.
        
The program generates an output file whose name will be the union of the names of the knowledge base and the fact base with a .txt extension. This file will contain in the first two lines the name of the knowledge base
and the fact base used for the program and in the next line the objective fact. Next, the inference process followed until obtaining the certainty factor of the target event that will be indicated last will be shown. 

