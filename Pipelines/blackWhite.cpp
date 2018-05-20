#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "imageControl.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>

#define R 0
#define G 1
#define B 2
#define A 3
using namespace std;

int lum2(int * pixel){
    return pixel[R]*0.3+pixel[G]*0.59+pixel[B]*0.11;
}

int* getRGBpixel2(int i,int j, ImageControl *im){
    return im->image[i][j];
}

void blancoYnegro(int umbral, ImageControl *image){
    image->byn= (int***)malloc(sizeof(int**)*image->imageHeight);
    for (int i=0;i< image->imageHeight;i++){
        image->byn[i]=(int**)malloc(sizeof(int*)*image->imageWidth);
        for(int j=0;j<image->imageWidth;j++){
            image->byn[i][j]=(int*)malloc(sizeof(int)*4);
            if(lum2(getRGBpixel2(i,j,image))>umbral){
                image->byn[i][j][B]=255;
                image->byn[i][j][R]=255;
                image->byn[i][j][G]=255;
                image->byn[i][j][A]=255;
            }
            else{
                image->byn[i][j][B]=0;
                image->byn[i][j][R]=0;
                image->byn[i][j][G]=0;
                image->byn[i][j][A]=255;
            }
        }
    }
    cout<<("imagen blanco y negro procesada\n");
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

int*** getImage(string filename){
    string line;
    ifstream file (filename);
    int count=0;
    int count2=0;
    int width,height;
    width=height=0;
    int ***retorno;
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
                retorno=(int***)malloc(sizeof(int**)*height);
            }
            else if (count==2){
                //cout << "aquí entré" << endl;
                retorno[i]=(int**)malloc(sizeof(int*)*width);
                retorno[i][j]=(int*)malloc(sizeof(int)*4);
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
                    i++;
                    j=0;
                }
            }
        }
        file.close();
    }

    return retorno;

}


int main(int argc, char **argv){
    cout << "Inicia el Proceso Black White"<<endl;
     
    int pipes[2];
    char arguments[30]; 
    int umbral, nImages, nUmbral,tag;
  
    
    
    ImageControl instance;
    
    read(300,&nImages,sizeof(nImages));   
    read(300,&umbral,sizeof(umbral));
    read(300,&nUmbral,sizeof(nUmbral));
    read(300,&tag,sizeof(tag));
   
       
    
    //esto lo muestra...
  
   if ( fork()==0 ){
            dup2(pipes[0],400);
                close(pipes[0]);
           	execl("nearlyBlack.o","nearlyBlack","-d","asd",(char*)0);
                
           	printf ("Si ves esto, no se pudo ejecutar el Proceso BlackWhite\n");
    }
   
  
    
     cout<<"Termina el proceso blackWhite"<<endl;
     exit(1);
    return 1;

}