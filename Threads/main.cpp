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
using namespace std;
#define R 0
#define G 1
#define B 2
#define A 3
#define READ 0
#define WRITE 1

void intento(int n){
    cout << "esta es la hebra one " << n << endl;
}



void loadImage(ImageControl image,char* filename){
    FILE* f = fopen(filename, "rb");
    if(f == NULL)
        throw "Argument Exception";
  
    fseek(f,0, SEEK_SET);
    fread(image.type, sizeof(char), 2, f);
    strcpy(image.type,"BM");
    //cout << "type " << type << endl;
    fread(&image.tam, sizeof(int), 1, f);
    //cout << "tam " << tam << endl;
    //reservado=*(int*)&info[6];
    fread(&image.reservado, sizeof(int), 1, f);
    //cout << "reservado " << reservado << endl;
    fread(&image.offset, sizeof(int), 1, f);
    //cout << "offset " << offset << endl;
    //fread(tam, sizeof(int), 1, f);
    //reservado=aux2;
    fread(&image.tamMet, sizeof(int), 1, f);
    //cout << "imageWidth " << imageWidth << endl;
    fread(&image.imageHeight,sizeof(int),1, f);
    fread(&image.imageWidth,sizeof(int),1, f);
    //cout << "imageHeight " << imageHeight << endl;
    fread(&image.numeroP, sizeof(short), 1, f);
    fread(&image.prof, sizeof(short), 1, f);
    fread(&image.compresion, sizeof(int), 1, f);
    fread(&image.estructura, sizeof(int), 1, f);
    fread(&image.pxmh, sizeof(int), 1, f);
    fread(&image.pxmv, sizeof(int), 1, f);
    fread(&image.coloresUsados, sizeof(int), 1, f);
    fread(&image.coloresImportantes, sizeof(int), 1, f);
    fseek(f,0, SEEK_SET);
    //offset=121;
    fread(&image.header, sizeof(unsigned char), image.offset, f);

    //cout << type << endl;


    image.image = (int***)malloc(sizeof(int**)*image.imageHeight);
    int row_padded = (image.imageWidth*4 + 4) & (~4);
    //cout << "row row_padded " << row_padded << endl;
    unsigned char* data = new unsigned char[row_padded];
   
    int j=0;
    //cout << (imageWidth-1)*3;
    for(int i = 0; i < image.imageHeight; i++)
    {
        //cout << "tengo un i: "<<i<<endl;
       //cout << "Se cayo aquí"<<endl;
        image.image[i]=(int**)malloc(sizeof(int*)*image.imageWidth);
        fread(data, sizeof(unsigned char), row_padded, f);

        for(j = 0; j < image.imageWidth*4; j += 4)
        {
            //cout << "Se cayo acá"<<endl;
            //cout << j << endl;;
          // Convert (B, G, R) to (R, G, B)

            image.image[i][j/4]=(int*)malloc(sizeof(int)*4);
            //cout << "aqui: " << image[i][j/3] << " i: " << i <<" j: " << j << endl;
            image.image[i][j/4][B]=(int)data[j];
            //cout << "image2 " << image[i][j/3][R] << endl;
            image.image[i][j/4][G]=(int)data[j+1];
            image.image[i][j/4][R]=(int)data[j+2];
            image.image[i][j/4][A]=(int)data[j+3];
            //cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
        }
        //cout<<"sali del ciclo "<<j<<" con i "<<i<<endl;

    }
    cout <<"fin de lectura hebra 1"<<endl;
    fclose(f);

}





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
        thread t[3];
        in="imagen_"+std::to_string(i+1)+".bmp";
        out="imagenSalida_"+std::to_string(i+1)+".bmp";
        sprintf(inF,in.c_str());
        sprintf(outF,out.c_str());
        ImageControl *received=new ImageControl();
        cout << inF << endl;
       /* received.loadBMP(inF);
        cout << "asd" <<endl;*/
       //https://thispointer.com/c-11-multithreading-part-1-three-different-ways-to-create-threads/
        t[0]=thread(&ImageControl::loadImage,received,inF);
        //thread one= loadImage(received,"imagen_1.bmp");
        t[0].join();
        i++;
    }
        
        
        
  
    return 0;
}

