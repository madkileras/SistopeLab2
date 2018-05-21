#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "imageControl.hpp"
#include <string>
#include <unistd.h>
#include <iostream>

#define R 0
#define G 1
#define B 2
#define A 3
using namespace std;

int lum(int * pixel){
    return pixel[R]*0.3+pixel[G]*0.59+pixel[B]*0.11;
}

int* getRGBpixel(int i,int j, ImageControl *im){
    return im->image[i][j];
}

bool nearlyBlack(int umbral,ImageControl* image){
    //cout << "Entraste a nearlyBlack" << endl;
    int countBlack=0;
    int countWhite=0;
    for(int i=0; i<image->imageHeight; i++)
    {
        for(int j = 0; j < image->imageWidth; j++)
        {
            if(image->byn[i][j][0]>umbral){
                countWhite++;
            }
            else{
                countBlack++;
            }
        }
    }
    cout<<"Cantidad de Negros: "<<countBlack<<endl;
    cout<<"Cantidad de Blancos: "<<countWhite<<endl;

    if(countBlack>countWhite){
        return true;
    }
    else{
        return false;
    }
    return true;

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





int main(int argc, char **argv){

    cout<<"Inicia el proceso NearlyBlack"<<endl;
    int pipes[2];
    char arguments[30]; 
    char inF[100];
    char outF[100];
    int umbral, nImages, nUmbral,tag;
  
    
    
    ImageControl imagen;
    
    read(400,&nImages,sizeof(nImages));   
    read(400,&umbral,sizeof(umbral));
    read(400,&nUmbral,sizeof(nUmbral));
    read(400,&tag,sizeof(tag));
    read(400,&inF,sizeof(inF));
    read(400,&outF,sizeof(outF));
    read(400,&imagen,sizeof(imagen));
    
    //cout << "carga imagen"<<endl;
    imagen.byn=getImage("imageName.txt",&imagen,imagen.imageWidth,imagen.imageHeight);
    //cout << "imagen cargada"<<endl;

    bool nearly=nearlyBlack(nUmbral,&imagen);
    //cout << "imagen byn guardada"<<endl;
    if(pipe(pipes)<0){
        cout << "ERROR AL CREAR PIPE EN CARGARIMAGEN.CPP\n";
    }
    
    cout<<"nearly black " <<inF << " " << outF<<endl;
    write(pipes[1],&nImages,sizeof(nImages));
    write(pipes[1],&umbral,sizeof(umbral));
    write(pipes[1],&nUmbral,sizeof(nUmbral));
    write(pipes[1],&tag,sizeof(tag));
    write(pipes[1],&inF,sizeof(inF));
    write(pipes[1],&outF,sizeof(outF));
    write(pipes[1],&imagen,sizeof(imagen));
    write(pipes[1],&nearly,sizeof(nearly));
    
    
    cout<< nearly <<endl;
   if ( fork()==0 ){
            dup2(pipes[0],500);
                close(pipes[0]);
           	execl("print.o","print","-d","asd",(char*)0);
                
           	printf ("Si ves esto, no se pudo ejecutar el Proceso Print desde nearlyBlack\n");
    }
   
  
     cout<<"Termina el proceso NearlyBlack"<<endl;
     exit(0);
    return 1;

}