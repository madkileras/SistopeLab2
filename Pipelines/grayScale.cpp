#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "imageControl.hpp"
#include <string>
#include <unistd.h>
#include <fstream>

#define R 0
#define G 1
#define B 2
#define A 3
using namespace std;

int* getRGBpixel(int i,int j, ImageControl *im){
    return im->image[i][j];
}
int lum(int * pixel){
    return pixel[R]*0.3+pixel[G]*0.59+pixel[B]*0.11;
}

void escalaGrises(ImageControl *image){
    image->escala= (int***)malloc(sizeof(int**)*image->imageHeight);
    for (int i=0;i< image->imageHeight;i++){
        image->escala[i]=(int**)malloc(sizeof(int*)*image->imageWidth);
        for(int j=0;j<image->imageWidth;j++){
                image->escala[i][j]=(int*)malloc(sizeof(int)*4);
                //cout << lum(getRGBpixel(i,j))<<endl;
                image->escala[i][j][B]=lum(getRGBpixel(i,j,image));
                image->escala[i][j][R]=lum(getRGBpixel(i,j,image));
                image->escala[i][j][G]=lum(getRGBpixel(i,j,image));
                image->escala[i][j][A]=image->image[i][j][A];
        }
    }
    return;
}

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

int*** getImage(char *filename){
    string line;
    ifstream file (filename);
    int count=0;
    int width,height;
    width=height=0;
    int ***retorno;
    int i,j;
    i=j=0;
    if (file.is_open())
    {
        while ( getline (file,line) )
        {
            if(count==0){
                width=stoi(line);
                count++;
            }
            else if(count==1){
                count++;
                height=stoi(line);
                retorno=(int***)malloc(sizeof(int**)*height);
            }
            else if (count==2){
                retorno[i]=(int**)malloc(sizeof(int*)*width);
                retorno[i][j][R]=stoi(line);
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
                    i++;
                    j=0;
                }
            }
        }
        file.close();
    }

    return retorno;

}


int  main(int argc, char **argv){
        cout << "Inicia el proceso GrayScale"<<endl;


    int umbral, nImages, nUmbral,tag;
    ImageControl imagen;
    read(100,&nImages,sizeof(nImages));
    read(100,&umbral,sizeof(umbral));
    read(100,&nUmbral,sizeof(nUmbral));
    read(100,&tag,sizeof(tag));
    read(100,&imagen,sizeof(imagen));

    escalaGrises(&imagen);


    int pipes[2];
    if(pipe(pipes)<0){
        cout << "ERROR AL CREAR PIPE EN CARGARIMAGEN.CPP\n";
    }
    write(pipes[1],&nImages,sizeof(nImages));
    write(pipes[1],&umbral,sizeof(umbral));
    write(pipes[1],&nUmbral,sizeof(nUmbral));
    write(pipes[1],&tag,sizeof(tag));
    write(pipes[1],&imagen,sizeof(imagen));

    pid_t pid;
    if ( fork()==0 ){
                dup2(pipes[0],300);
                close(pipes[0]);
                    execl("blackWhite.o","blackWhite",0,0);
                    printf ("Si ves esto, no se pudo ejecutar el asdasdasdasd\n");
            }
    cout << "Termina el proceso GrayScale"<<endl;

}