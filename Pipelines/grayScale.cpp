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




int  main(int argc, char **argv){
        cout << "Inicia el proceso GrayScale"<<endl;
    ///read desde el pipe 200
    
    
    int umbral, nImages, nUmbral,tag;
    ImageControl imagen;
    char inF[100];
    char outF[100];
    
    read(200,&nImages,sizeof(nImages));
    read(200,&umbral,sizeof(umbral));
    read(200,&nUmbral,sizeof(nUmbral));
    read(200,&tag,sizeof(tag));
    read(200,&inF,sizeof(inF));
    read(200,&outF,sizeof(outF));
    read(200,&imagen,sizeof(imagen));
    //cout<<"Cargando imagen"<<endl;
    imagen.image=getImage("imageName.txt",&imagen,imagen.imageWidth,imagen.imageHeight);
    
    escalaGrises(&imagen);
    
    int pipes[2];
    if(pipe(pipes)<0){
        cout << "ERROR AL CREAR PIPE EN CARGARIMAGEN.CPP\n";
    }
    cout<<"grayScale " <<inF << " " << outF<<endl;
    write(pipes[1],&nImages,sizeof(nImages));
    write(pipes[1],&umbral,sizeof(umbral));
    write(pipes[1],&nUmbral,sizeof(nUmbral));
    write(pipes[1],&tag,sizeof(tag));
    write(pipes[1],&inF,sizeof(inF));
    write(pipes[1],&outF,sizeof(outF));
    write(pipes[1],&imagen,sizeof(imagen));
    
 
    if ( fork()==0 ){
                dup2(pipes[0],300);
                close(pipes[0]);
                    execl("blackWhite.o","blackWhite",0,0);
                    printf ("Si ves esto, no se pudo ejecutar el proceso blackWhite desde GrayScale\n");
            }
    cout << "Termina el proceso GrayScale"<<endl;

}