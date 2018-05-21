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

void getHeader(char *filename, int offset, ImageControl *im){
    unsigned char retorno;

    //cout << "entraste a get header"<<endl;

    FILE* f = fopen(filename, "rb");
    if(f == NULL)
        throw "Argument Exception";
    //cout << "buscando" << endl;
    fseek(f,0, SEEK_SET);
    //cout << "leyendo" << endl;
    fread(&im->header, sizeof(unsigned char), offset, f);
    //cout << "listo retorno " << endl;
    //cout << retorno << endl;
    fclose(f);
    //cout << "saliendo"<<endl;


    //return retorno;
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

int saveImage(char *filename, int tag,ImageControl * imagen){
   
    FILE *prueba;
    prueba=fopen(filename,"wb");
    fwrite(&imagen->header,1,imagen->offset,prueba);
    if (true){
        //cout << "Height " << imagenHeight << endl;
        //cout << "Width " << imagenWidth << endl;
        for(int i=0; i<imagen->imageHeight ; i++)
        {
            for(int j = 0; j < imagen->imageWidth*4; j += 4)
            {
                if(tag==0){
                    fwrite(&(imagen->image[i][j/4][B]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->image[i][j/4][G]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->image[i][j/4][R]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->image[i][j/4][A]),sizeof(unsigned char),1,prueba);
                }
                else if(tag==1){
                    fwrite(&(imagen->byn[i][j/4][B]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->byn[i][j/4][G]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->byn[i][j/4][R]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->byn[i][j/4][A]),sizeof(unsigned char),1,prueba);
                }
                else{
                    fwrite(&(imagen->escala[i][j/4][B]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->escala[i][j/4][G]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->escala[i][j/4][R]),sizeof(unsigned char),1,prueba);
                    fwrite(&(imagen->escala[i][j/4][A]),sizeof(unsigned char),1,prueba);
                }
            }
        }
    }
    fclose(prueba);
    return 0;
}


int  main(int argc, char **argv){
     //   cout << "Inicia el proceso PRINT"<<endl;
    ///read desde el pipe 200
    
    
    int pipes[2];
    char arguments[30]; 
    char inF[100];
    char outF[100];
    int umbral, nImages, nUmbral,tag;
  
    
    
    ImageControl imagen;
    bool nearly;
    read(500,&nImages,sizeof(nImages));   
    read(500,&umbral,sizeof(umbral));
    read(500,&nUmbral,sizeof(nUmbral));
    read(500,&tag,sizeof(tag));
    read(500,&inF,sizeof(inF));
    read(500,&outF,sizeof(outF));
    read(500,&imagen,sizeof(imagen));
    read(500,&nearly,sizeof(nearly));
    
    
    
    if (nearly==1){
                cout << outF << "  | nearlyBlack: yes   |" << endl;
                
            }
            else{
                cout << outF << "  | nearlyBlack: no    |" << endl;  
    }
    //cout << "imagewidth " << imagen.imageWidth <<endl;
    //cout << "imagen offset " << imagen.offset << endl;
    //ACA SE PEGAAAAA
    char aux[100];
    strcpy(aux,outF);
    //cout<<"getting header "<<inF<<" "<<outF<<endl;
    getHeader(inF,imagen.offset,&imagen);
    //cout<<"getting header2 "<<inF<<" "<<outF<<endl;
    imagen.byn=getImage("imageName.txt",&imagen,imagen.imageWidth,imagen.imageHeight);
  //  cout<<"guardando imagen en "<<aux<<endl;
    saveImage(aux,1,&imagen);
    //cout<<"PROCESO TERMINADO DE IMAGEN"<<endl;
   
    
    
    
 
    return 0;

}