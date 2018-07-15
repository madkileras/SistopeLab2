#include "imageControl.hpp"

// funcion para leer una imagen con hebras
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
