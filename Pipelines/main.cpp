#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "imageControl.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
 #include <sys/wait.h>
 #include <sys/types.h>
using namespace std;
#define R 0
#define G 1
#define B 2
#define A 3
#define READ 0
#define WRITE 1


int main(int argc, char **argv){
    pid_t my_pid=getpid(), parent_pid=getppid(),child_pids;
    

    //DECLARACION DE VARIABLES PARA ARGUMENTOS RECIBIDOS
    //c:cantidad de imágenes ; u: umbral; n: umbral para clasificacion; b: bandera(?)
    int c,u,n,b=0;
    int opt;
    while((opt=getopt(argc, argv,"c:u:n:b"))!=-1){
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
            case 'b':
                b++;
        }

    }
    int i=0;
    //Se reserva memoria para guardar los valores de nearlyBlack de cada imagen
    int *nbValues=(int*)malloc(sizeof(int)*c);
    std::string in;
    std::string out;
    char inF[100];
    char outF[100];

    //CREACION DEL PRIMER PIPE
    int pipes[2];
    if(pipe(pipes)<0){
        cout << "ERROR AL CREAR PIPE EN MAIN.CPP\n";
    }
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
        in="imagen_"+std::to_string(i+1)+".bmp";
        out="imagenSalida_"+std::to_string(i+1)+".bmp";
        sprintf(inF,in.c_str());
        sprintf(outF,out.c_str());
       
        ImageControl received;
        write(pipes[1],&c,sizeof(c));
        write(pipes[1],&u,sizeof(u));
        write(pipes[1],&n,sizeof(n));
        write(pipes[1],&b,sizeof(b));
        write(pipes[1],&inF,sizeof(inF));
        write(pipes[1],&outF,sizeof(outF));
        pid_t child_pid, wpid;
        int status = 0;
        //received.loadBMP(inF);
    
            if ( child_pid=fork()==0 ){
                    dup2(pipes[0],100);
                    close(pipes[0]);
                    execl("cargarImagen.o","cargarImagen",0,0);                   
                    printf ("Si ves esto, no se pudo ejecutar el proceso cargar imagen\n");
                    exit(0);
                   
            }
            while (wait( NULL )>0); // this way, the father waits for all the child processes 

            i=i+1;
        
        
    }

        //cout << "salimoh " <<endl;
        /*received.blancoYnegro(u);
        received.escalaGrises();
        sprintf(outF,out.c_str());
        received.saveImage(outF,1);
        if(received.nearlyBlack(n)){
            nbValues[i+1]=1;
        }
        else{
            nbValues[i+1]=0;
        }
        //received.freeImages();
        i++;
        
        
    }

    if(b){ //Se imprimen los resultados de nearlyBlack de cada imágen
        cout << endl << "Resultados:" << endl;
        cout << "|    image    |    nearly_black    |"<<endl;
        cout << "|-------------|--------------------|"<<endl;
        
        for(i=1;i<c+1;i++){
            if (nbValues[i]==1){
                cout << "|   imagen_" << i << "  | nearlyBlack: yes   |" << endl;
                
            }
            else{
                cout << "|   imagen_" << i << "  | nearlyBlack: no    |" << endl;  
            }
        }
    }
    
   */
  remove("imageName.txt");

  
    return 0;
}

