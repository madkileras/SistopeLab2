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
int  main(int argc, char **argv){

    int pipes[2];
    int umbral, nImages, nUmbral,tag;
    read(STDIN_FILENO,&nImages,sizeof(nImages));
    read(STDIN_FILENO,&umbral,sizeof(umbral));
    read(STDIN_FILENO,&nUmbral,sizeof(nUmbral));
    read(STDIN_FILENO,&tag,sizeof(tag));
    FILE *archivo;




    ImageControl instance;
     std::string in;
        std::string out;
        char inF[100];
        char outF[100];
        in="imagen_1.bmp";
        out="imagenSalida_1.bmp";
        sprintf(inF,in.c_str());
        sprintf(outF,out.c_str());
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
    /*fread(&imageHeight,sizeof(int),1, f);
    fread(&imageWidth,sizeof(int),1, f);
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
    cout << "Imagen cargada" <<endl;

    //return data;

    exit(1);
    return 1;
}