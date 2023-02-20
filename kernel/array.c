#include "include/head.h"
#include  <string.h>


void init_arr(unsigned char *arr){
    int i=0;
    for(i;i<=256;i++){
        arr[i]=0;
    }
}
void del_arr(unsigned char *arr){
    if(arr_size(arr)>5)
    arr[arr_size(arr)-1]=0;
}
int arr_size(unsigned char *arr){
    return strlen(arr);
}

void put_arr(unsigned char *arr,char data){
    arr[arr_size(arr)]=data;
    arr[arr_size(arr)+1]=0x00;
}


