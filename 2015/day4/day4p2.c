#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <bcrypt.h>

#define ERRLOG(msg) { puts(msg); exit(1); }

#define NUM_DIGITS 7
#define START_NUM 1000000
#define END_NUM (START_NUM*10)-1
#define INPUT_LEN 8
#define WORD_LEN 8 + NUM_DIGITS


int main(){
    char *input = "<secret word>";

    BCRYPT_ALG_HANDLE cah;
    BCRYPT_HASH_HANDLE hHash;
    if(BCryptOpenAlgorithmProvider(&cah,BCRYPT_MD5_ALGORITHM,NULL,BCRYPT_HASH_REUSABLE_FLAG) < 0) ERRLOG("Failed to create algorithm provider")

    unsigned long hashObjlen;
    unsigned long hashlen;
    unsigned char* hashObj;
    unsigned char* hash;
    unsigned long extra;
    long i;

    if(BCryptGetProperty(cah,BCRYPT_OBJECT_LENGTH,(unsigned char*)&hashObjlen,sizeof(unsigned long),&extra,0) < 0) ERRLOG("Failed to get bcrypt object property")
    hashObj = (unsigned char*) calloc(1,hashObjlen);

    if(BCryptGetProperty(cah,BCRYPT_HASH_LENGTH,(unsigned char*)&hashlen,sizeof(unsigned long),&extra,0) < 0) ERRLOG("Failed to get bcrypt object property")
    hash = (unsigned char*) calloc(1,hashlen);

    if(BCryptCreateHash(cah,&hHash,hashObj,hashObjlen,NULL,0,BCRYPT_HASH_REUSABLE_FLAG) < 0) ERRLOG("Failed to create hash")

    for(i = START_NUM; i < END_NUM; i++){
        char msg[WORD_LEN];
        sprintf(msg,"%s%ld",input,i);
        if(BCryptHashData(hHash,(unsigned char*)msg,WORD_LEN,0) < 0) ERRLOG("Failed to hash data")
        if(BCryptFinishHash(hHash,(unsigned char*)hash,hashlen,0) < 0) ERRLOG("Failed to finish the hash")
        if(!hash[0] && !hash[1] && !(hash[2])){
            break;
        }
    }
    printf("HASH: ");
    for(int h = 0; h < 16; h++){
        printf("%02x",hash[h]);
    }
    puts("");
    printf("%ld\n",i);
    if(BCryptCloseAlgorithmProvider(cah,0) < 0) ERRLOG("Failed to close algorithm provider")
    if(BCryptDestroyHash(hHash) < 0) ERRLOG("Failed to free hash object")
    free(hashObj);
    free(hash);
}