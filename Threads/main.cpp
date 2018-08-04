#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include  <thread>
#include "imageControl.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
 #include <sys/wait.h>
 #include <sys/types.h>
#include <mutex>
#include <condition_variable>

using namespace std;
#define R 0
#define G 1
#define B 2
#define A 3
#define READ 0
#define WRITE 1

std::mutex m;
std::condition_variable cv;
bool ready = false;
bool processed = false;

int main(int argc, char **argv){
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

    //CREACION DEL PRIMER PIPE

    //SE IMPRIMEN VALORES PARA VERIFICAR PIPE
   // cout << "c "<<c<<" u "<<u<<" n "<<n<<" b "<<b<<endl;
    //SE ESCRIBE EL PIPE
    
    //write(pipes[1],&p,sizeof(p));

    // -----> HASTA ACA EL PROCESO MAIN
    int lock=0;
       cout << endl << "Resultados:" << endl;
        cout << "|    image          |    nearly_black    |"<<endl;
    //Se recorren todas la imagenes, se procesan y se guardan
    while (i<c){
        thread t[h];
        in="imagen_"+std::to_string(i+1)+".bmp";
        out="imagenSalida_"+std::to_string(i+1)+".bmp";
        sprintf(inF,in.c_str());
        sprintf(outF,out.c_str());
        ImageControl *received=new ImageControl();
        cout << inF << endl;
        //received->loadImage(inF);
       
       //https://thispointer.com/c-11-multithreading-part-1-three-different-ways-to-create-threads/
        t[0]=thread(&ImageControl::loadImage,received,inF);
        
        t[1]=thread(&ImageControl::blancoYnegro,received,n,0,1);
        t[2]=thread(&ImageControl::blancoYnegro,received,n,1,2);
        //t[2]=thread(&ImageControl::escalaGrises,received,1,2);
        //t[2]=thread(&ImageControl::blancoYnegro,received,n);
       
        
         t[0].join();
         t[1].join();
         t[2].join();
       // t[1].join();
         cout << received->getRGBpixel(250,250)[1]<<endl;
        i++;
    }
        
        
        
  
    return 0;
}

