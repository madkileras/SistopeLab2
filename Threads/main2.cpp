#include <uC++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <iostream>
#include "buffer.hpp"

using namespace std;

#define R 0
#define G 1
#define B 2
#define A 3
#define READ 0
#define WRITE 1

Buffer monitores[5];



void uMain::main(){

    pid_t my_pid=getpid(), parent_pid=getppid(),child_pids;

    
    
    //DECLARACION DE VARIABLES PARA ARGUMENTOS RECIBIDOS
    //c:cantidad de imágenes ; u: umbral; n: umbral para clasificacion; b: bandera(?)
    int c,u,n,h,b=0;
    int opt;
    while((opt=getopt(argc, argv,"c:u:n:h:b"))!=-1){
        //SE ASIGNAN LOS VALORES DE LA CONSOLA A LAS VARIABLES DEFINIDAS
        switch(opt){
            case 'c':
                c=atoi(optarg);
                break;
            case 'u':
                u=atoi(optarg);
                break;
            case 'n':
                n=atoi(optarg);
                break;
             case 'h':
                h=atoi(optarg);
                break;
            case 'b':
                b++;
        }

    }
    int i=0;
    //Se reserva memoria para guardar los valores de nearlyBlack de cada imagen
    int *nbValues=(int*)malloc(sizeof(int)*c);
    std::string in;
    std::string out;
    char  inF[100];
    char  outF[100];

    cout << "c " << c << endl;
    cout << "u " << u << endl;

    //CREACION DEL PRIMER PIPE

    //SE IMPRIMEN VALORES PARA VERIFICAR PIPE
   // cout << "c "<<c<<" u "<<u<<" n "<<n<<" b "<<b<<endl;
    //SE ESCRIBE EL PIPE
    
    //write(pipes[1],&p,sizeof(p));

    // -----> HASTA ACA EL PROCESO MAIN
    int lock=0;
       cout << endl << "Resultados:" << endl;
        cout << "|    imageName     |    nearly_black     |"<<endl;
    //Se recorren todas la imagenes, se procesan y se guardan
    
    //received=new ImageControl();
    while (i<c){ 

        i++;

    }
  
    cout << "Working" << endl ;
}
