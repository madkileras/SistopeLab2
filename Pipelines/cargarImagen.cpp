#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "imageControl.hpp"
#include <string>
#include <unistd.h>
#include <fstream>

 #include <sys/wait.h>
 #include <sys/types.h>
#define R 0
#define G 1
#define B 2
#define A 3
using namespace std;

struct prueba{
    int a;
    char b[5];
}prueba;



void savePixels(ImageControl *im){
    ofstream myfile ("imageName.txt");
    myfile << im->imageHeight <<"\n"<< im->imageWidth <<"\n";
    if (myfile.is_open())
    {
        for(int i=0;i<im->imageHeight;i++){
            for(int j=0;j<im->imageWidth;j++){
                myfile << im->image[i][j][R] << "\n" << im->image[i][j][G] << "\n" << im->image[i][j][B] << "\n" << im->image[i][j][A] << "\n";
            }

        }
        myfile.close();
    }

}

int*** getImage(string filename,ImageControl *im,int width,int height){
    string line;
    ifstream file (filename);
    int count=0;
    int count2=0;

  
    int ***retorno=(int***)malloc(sizeof(int**)*height);
    for(int y=0;y<height;y++){
        retorno[y]=(int**)malloc(sizeof(int**)*width);
        for(int u=0;u<width;u++){
            retorno[y][u]=(int*)malloc(sizeof(int)*4);
        }
    }
    int i,j;
    i=j=0;
    if (file.is_open())
    {
        while ( getline (file,line) )
        {
            //cout << "linea: " << count2 << " valor: " << line << endl;
            //count2++;
            if(count==0){
                width=stoi(line);
                count++;
            }
            else if(count==1){
                count++;
                height=stoi(line);
                
            }
            else if (count==2){
                //cout << "aquí entré" << endl;
               
                //cout << "aquí estoy" << endl;
                retorno[i][j][R]=stoi(line);
                //cout << "ya voy saliendo" << endl;
                count++;
               
                 
            }
            else if(count==3){
                retorno[i][j][G]=stoi(line);
                count++;
            }
            else if(count==4){
                retorno[i][j][B]=stoi(line);
                count++;
                
            }
            else{
                retorno[i][j][A]=stoi(line);
                count=2;
                j++;
               
                
                if(j==height){
                    i=i+1;
                    j=0;
                    
                    
                }
               
            }
        }
        file.close();
    }
    
    
    
    return retorno;

}


int  main(int argc, char **argv){

    //SE IMPRIME EL PIPE EN UN DOCUMENTO

  // cout<<"\nInicia Proceso CargarImagen"<<endl;

    //int pipes[2];
    int umbral, nImages, nUmbral,tag;
    char inF[100];
    char outF[100];
    read(100,&nImages,sizeof(nImages));
    read(100,&umbral,sizeof(umbral));
    read(100,&nUmbral,sizeof(nUmbral));
    read(100,&tag,sizeof(tag));
    read(100,&inF,sizeof(inF));
    read(100,&outF,sizeof(outF));
    char aux1[100], aux2[100];
    strcpy(aux1,inF);
    strcpy(aux2,outF);
    //cout<<"inf "<<inF << " outF " << outF <<endl;


    ImageControl instance;
     std::string in;
        std::string out;
        /*char inF[100];
        char outF[100];*/
        in="imagen_1.bmp";
        out="imagenSalida_1.bmp";
        /*sprintf(inF,in.c_str());
        sprintf(outF,out.c_str());*/
    FILE* f = fopen(inF, "rb");
    if(f == NULL)
        throw "Argument Exception";

    fseek(f,0, SEEK_SET);
    fread(instance.type, sizeof(char), 2, f);
    strcpy(instance.type,"BM");
    //cout << "type " << type << endl;
    fread(&instance.tam, sizeof(int), 1, f);
    //cout << "tam " << tam << endl;
    //reservado=*(int*)&info[6];
    fread(&instance.reservado, sizeof(int), 1, f);
    //cout << "reservado " << reservado << endl;
    fread(&instance.offset, sizeof(int), 1, f);
    //cout << "offset " << offset << endl;
    //fread(tam, sizeof(int), 1, f);
    //reservado=aux2;
    fread(&instance.tamMet, sizeof(int), 1, f);
    //cout << "imageWidth " << imageWidth << endl;
    fread(&instance.imageHeight,sizeof(int),1, f);
    fread(&instance.imageWidth,sizeof(int),1, f);/*
    //cout << "imageHeight " << imageHeight << endl;
    fread(&numeroP, sizeof(short), 1, f);
    fread(&prof, sizeof(short), 1, f);
    fread(&compresion, sizeof(int), 1, f);
    fread(&estructura, sizeof(int), 1, f);
    fread(&pxmh, sizeof(int), 1, f);
    fread(&pxmv, sizeof(int), 1, f);
    fread(&coloresUsados, sizeof(int), 1, f);
    fread(&coloresImportantes, sizeof(int), 1, f);*/
    fseek(f,0, SEEK_SET);
    //offset=121;
    fread(&instance.header, sizeof(unsigned char), instance.offset, f);

    //cout << type << endl;


    instance.image = (int***)malloc(sizeof(int**)*instance.imageHeight);
    int row_padded = (instance.imageWidth*4 + 4) & (~4);
    //cout << "row row_padded " << row_padded << endl;
    unsigned char* data = new unsigned char[row_padded];
    
    int j=0;
    //cout << (imageWidth-1)*3;
    for(int i = 0; i < instance.imageHeight; i++)
    {
        //cout << "tengo un i: "<<i<<endl;
       //cout << "Se cayo aquí"<<endl;
        instance.image[i]=(int**)malloc(sizeof(int*)*instance.imageWidth);
        fread(data, sizeof(unsigned char), row_padded, f);
          
        for(j = 0; j < instance.imageWidth*4; j += 4)
        {
            //cout << "Se cayo acá"<<endl;
            //cout << j << endl;;
          // Convert (B, G, R) to (R, G, B)

            instance.image[i][j/4]=(int*)malloc(sizeof(int)*4);
            //cout << "aqui: " << image[i][j/3] << " i: " << i <<" j: " << j << endl;
            instance.image[i][j/4][B]=(int)data[j];
            
            //cout << "image2 " << image[i][j/3][R] << endl;
            instance.image[i][j/4][G]=(int)data[j+1];
            instance.image[i][j/4][R]=(int)data[j+2];
            instance.image[i][j/4][A]=(int)data[j+3];
            //cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
        }
        //cout<<"sali del ciclo "<<j<<" con i "<<i<<endl;

    }
    fclose(f);
    
    savePixels(&instance);


    int pipes[2];
    if(pipe(pipes)<0){
        cout << "ERROR AL CREAR PIPE EN CARGARIMAGEN.CPP\n";
    }
    //cout<<"inf "<<aux1 << " outF " << aux2 <<endl;
    
    //cout<<"cargarImagen " <<inF << " " << outF<<endl;
    write(pipes[1],&nImages,sizeof(nImages));
    write(pipes[1],&umbral,sizeof(umbral));
    write(pipes[1],&nUmbral,sizeof(nUmbral));
    write(pipes[1],&tag,sizeof(tag));
    write(pipes[1],&aux1,sizeof(aux1));
    write(pipes[1],&aux2,sizeof(aux2));
    write(pipes[1],&instance,sizeof(instance));

   
    
    //cout << imagenstring<<endl;

    //cout << "Imagen cargada" <<endl;

    
    
    if ( fork()==0 ){
            dup2(pipes[0],200);
                close(pipes[0]);
           	execl("grayScale.o","grayScale","-d","asd",(char*)0);
                
           	printf ("Si ves esto, no se pudo ejecutar el Proceso grayScale en cargarImagen\n");
    }
    while (wait( NULL )>0);
  
    
    //cout<<"Termina el proceso cargarImagen"<<endl;
    
   





    //return data;
    
    exit(0);
    return 1;
}

 