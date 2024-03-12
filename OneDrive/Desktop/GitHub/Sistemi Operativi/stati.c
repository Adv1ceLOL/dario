#include <stdio.h>

typedef void (*ActionPtrType) (const unsigned char c);

void s1(const unsigned char c);
void s2(const unsigned char c);
void s3(const unsigned char c);
void s4(const unsigned char c);
void s5(const unsigned char c);

ActionPtrType current_status;

unsigned char packet_size = 0;

void s1(const unsigned char c){
    printf("[s1] %x\n",s1);
    if(c==0xAA)current_status = s2;
}


void s2(const unsigned char c){
    printf("[s2] %x\n",s2);
    if(c==0x55) current_status = s3;
    else current_status = s1;
}

void s3(const unsigned char c){
    printf("[s3] %x\n",s3);
    packet_size = c;
    current_status = s4;
}

void s4(const unsigned char c){
    printf("[s4] %x\n",s4);
    if(packet_size) --packet_size;
    else current_status = s5;
}

void s5(const unsigned char c){
    printf("[s5] %x\n",s5);
    if(c== 0xAA)current_status = s2;
    else current_status = s1;
}

int main(){
    current_status = s1;
    unsigned int c;
    while(1){
        scanf("%x", &c);
        printf("read: %x, current_status: %x\n",c,current_status);
        (*current_status)(c);
        printf("next: %x\n",current_status);
    }
}