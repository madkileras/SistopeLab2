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

struct prueba{
    int a;
    char b[5];
}prueba;

string passToString(ImageControl im, int width, int heigth){
    string retorno="";
    for(int i=0;i<width;i++){
        for(int j=0;j<heigth;j++){
            retorno+=std::to_string(im.image[i][j][R]);
            retorno+=std::to_string(im.image[i][j][G]);
            retorno+=std::to_string(im.image[i][j][B]);
            retorno+=std::to_string(im.image[i][j][A]);

        }
    }
    cout << retorno << endl;
    return retorno;


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



int  main(int argc, char **argv){

    //SE IMPRIME EL PIPE EN UN DOCUMENTO
    //int pipes[2];
    int umbral, nImages, nUmbral,tag;
    struct prueba p;
    read(100,&nImages,sizeof(nImages));
    read(100,&umbral,sizeof(umbral));
    read(100,&nUmbral,sizeof(nUmbral));
    read(100,&tag,sizeof(tag));
    //read(100,&p,sizeof(p));
    /*
    ofstream myfile;
    myfile.open ("salida.txt");
    myfile <<nImages <<"\n";
    myfile <<umbral <<"\n";
    myfile <<nUmbral <<"\n";
    myfile <<tag <<"\n";
    //myfile <<"prueba: "<<p.b<<"\n";
    myfile.close();*/
    




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
    write(pipes[1],&nImages,sizeof(nImages));
    write(pipes[1],&umbral,sizeof(umbral));
    write(pipes[1],&nUmbral,sizeof(nUmbral));
    write(pipes[1],&tag,sizeof(tag));
    write(pipes[1],&instance,sizeof(instance));

    //pass2tring(instance,instance.imageWidth, instance.imageHeight);


    cout << "Imagen cargada" <<endl;
    dup2(pipes[0],200);
    close(pipes[0]);
    ImageControl doi;
    int umbral2, nImages2, nUmbral2,tag2;
    read(200,&nImages2,sizeof(nImages2));
    read(200,&umbral2,sizeof(umbral2));
    read(200,&nUmbral2,sizeof(nUmbral2));
    read(200,&tag2,sizeof(tag2));
    read(200,&doi,sizeof(doi));
    ofstream myfile2;
    myfile2.open ("salida.txt");
    myfile2 <<"aqui tipo de imagen: "<<doi.type <<"\n";
    myfile2 <<"aqui ofsset de imagen: "<<doi.offset <<"\n";
    myfile2.close();

    ImageControl pruebita;
    cout<<"obteniendo imagen\n";
    pruebita.image=getImage("imageName.txt");
    cout<<"imagen obtenida\n";


    //return data;

    exit(1);
    return 1;
}