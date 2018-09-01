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
ImageControl *received;

int countWhite;
int countBlack;

pthread_barrier_t mybarrier;
int nhebras;

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
        cout << "|    imageName     |    nearly_black     |"<<endl;
    //Se recorren todas la imagenes, se procesan y se guardan

    nhebras=h-1;
    //pthread_barrier_init(&mybarrier, NULL,nhebras);
    received=new ImageControl();
    //received=(ImageControl*)malloc(sizeof(ImageControl));
    thread t[h];
    pthread_barrier_init(&mybarrier, NULL,nhebras);
    while (i<c){
            
        
        countWhite=0;
        countBlack=0;

        in="imagen_"+std::to_string(i+1)+".bmp";
        out="imagenSalida_"+std::to_string(i+1)+".bmp";
        sprintf(inF,in.c_str());
        sprintf(outF,out.c_str());
        //received->loadImage(inF);
        
        //https://thispointer.com/c-11-multithreading-part-1-three-different-ways-to-create-threads/
        cout << "t0" << endl;
        t[0]=thread(&ImageControl::loadImage,received,inF);
        t[0].join();
        cout << "hebras " << h << " nhebras " <<nhebras<<endl;
        cout << "t1 escala grises" << endl;/*
        for(int p=1;p<h;p++){
            cout << "p " << p << " h " << h << endl; 
            cout << "creando threads para escala de grises" << endl;
            t[p]=thread(&ImageControl::escalaGrises,received,p-1,h-1);
            cout << "LISTO CREADA threads para escala de grises" << endl;
        } 
        cout << "t1 join" << endl;
        for(int p=0;p<h;p++){
             t[p].join();
        }/*    
        //cout << "t2 blanco y negro" << endl;
        for(int p=1;p<h;p++){
   
            t[p]=thread(&ImageControl::blancoYnegro,received,n,p-1,h-1);
        }
        //cout << "t2 join" << endl;        
        for(int p=1;p<h;p++){
             t[p].join();
        }
        //cout << "t3 nearlyblack" << endl;           
        for(int p=1;p<h;p++){
            t[p]=thread(&ImageControl::nearlyBlack,received,u,p-1,h-1);
        }
        //cout << "t3 join" << endl;
        for(int p=1;p<h;p++){
             t[p].join();
        }
        //cout << "save image" << endl;
        received->saveImage(outF,0);  
        
        if(countBlack>countWhite){
             cout << "|   imagen_" << i << "       |   nearlyBlack: yes  |" << endl;
        }else{
             cout << "|   imagen_" << i << "       |   nearlyBlack: no   |" << endl;
        }
        */
        i++;
        //pthread_barrier_destroy(&mybarrier);
        //pthread_exit(NULL);
        //cout << c << endl;
        ready = false;
        processed = false;
        //delete [] t;
        //received->freeImages();
        //free(received);
        cout <<"bai"<<endl;
         
    }
    pthread_barrier_destroy(&mybarrier);
    cout <<"bai2"<<endl;        
  
    return 0;
}

