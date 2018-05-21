#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "imageControl.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/mman.h>
 #include <sys/wait.h>
 #include <sys/types.h>
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

void savePixels(ImageControl *im,int b){
    
    
    ofstream myfile ("imageName.txt");
    myfile << im->imageHeight <<"\n"<< im->imageWidth <<"\n";
    if (myfile.is_open())
    {
        for(int i=0;i<im->imageHeight;i++){
            for(int j=0;j<im->imageWidth;j++){
                if(b==1){
                    myfile << im->byn[i][j][R] << "\n" << im->byn[i][j][G] << "\n" << im->byn[i][j][B] << "\n" << im->byn[i][j][A] << "\n";
                }
                else{
                     myfile << im->image[i][j][R] << "\n" << im->image[i][j][G] << "\n" << im->image[i][j][B] << "\n" << im->image[i][j][A] << "\n";
                }
               
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
    //cout<<("imagen blanco y negro procesada\n");
    return;
}



int main(int argc, char **argv){
    //cout << "Inicia el Proceso Black White"<<endl;
     
    
    int umbral, nImages, nUmbral,tag;
    char inF[100];
    char outF[100];
  
    
    
    ImageControl imagen;
    
    read(300,&nImages,sizeof(nImages));   
    read(300,&umbral,sizeof(umbral));
    read(300,&nUmbral,sizeof(nUmbral));
    read(300,&tag,sizeof(tag));
    read(300,&inF,sizeof(inF));
    read(300,&outF,sizeof(outF));
    read(300,&imagen,sizeof(imagen));
    //cout << "carga imagen"<<endl;
    imagen.image=getImage("imageName.txt",&imagen,imagen.imageWidth,imagen.imageHeight);
    //cout << "imagen cargada"<<endl;

    blancoYnegro(umbral,&imagen);
    savePixels(&imagen,1);
    //cout << "imagen byn guardada"<<endl;
    
    int pipes[2];
    if(pipe(pipes)<0){
        cout << "ERROR AL CREAR PIPE EN CARGARIMAGEN.CPP\n";
    }
   // cout<<"blackwhite " <<inF << " " << outF<<endl;
    write(pipes[1],&nImages,sizeof(nImages));
    write(pipes[1],&umbral,sizeof(umbral));
    write(pipes[1],&nUmbral,sizeof(nUmbral));
    write(pipes[1],&tag,sizeof(tag));
    write(pipes[1],&inF,sizeof(inF));
    write(pipes[1],&outF,sizeof(outF));
    write(pipes[1],&imagen,sizeof(imagen));
    //esto lo muestra...
  
   if ( fork()==0 ){
            dup2(pipes[0],400);
                close(pipes[0]);
           	execl("nearlyBlack.o","nearlyBlack","-d","asd",(char*)0);
                
           	printf ("Si ves esto, no se pudo ejecutar el Proceso nearlyBlack desde blackWhite\n");
    }
   
  while (wait( NULL )>0);
  
    
    // cout<<"Termina el proceso blackWhite"<<endl;
     exit(1);
    return 1;

}