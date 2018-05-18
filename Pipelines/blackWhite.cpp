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


int main(int argc, char **argv){

    int paip[2];
    char arguments[30]; 
    int umbral, nImages, nUmbral,tag;
    pid_t pid;

    ImageControl *image;

    if(pipe(paip)==-1){
        cout << "ERROR EN PIPE PROCESO BLACKWHITE\n";
    }
    if(pid=fork()<0){
        cout << "ERROR EN FORK PROCESO BLACKWHITE\n";
    }
    else if(pid==0){
        close(paip[0]);
        //blancoYnegro(umbral,image);
        //read(paip[1]);
        //write(paip[1],image,sizeof(image));

    }
    else{
        close(paip[1]);
        read(STDIN_FILENO,&nImages,sizeof(nImages));
        read(STDIN_FILENO,&umbral,sizeof(umbral));
        read(STDIN_FILENO,&nUmbral,sizeof(nUmbral));
        read(STDIN_FILENO,&tag,sizeof(tag));
        read(STDIN_FILENO,image,sizeof(image));
        blancoYnegro(umbral,image);

    }
    return 0;

}