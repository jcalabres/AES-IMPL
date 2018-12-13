#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#define ROTL8(x,shift) ((uint8_t ) ((x) << (shift)) | ((x) >> (8 - (shift))))

//AES-128 NK=4;NR=10
//AES-192 NK=6;NR=12
//AES-256 NK=8;NR=14
int Nk=4;
int Nr=10;
int Nb=4;
//rcon hardcoded values
uint8_t rcon[10]={0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x1b,0x36};
//s-box
int sbox[256] =
 {0x63 ,0x7c ,0x77 ,0x7b ,0xf2 ,0x6b ,0x6f ,0xc5 ,0x30 ,0x01 ,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76
 ,0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59 ,0x47 ,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0
 ,0xb7 ,0xfd ,0x93 ,0x26 ,0x36 ,0x3f ,0xf7 ,0xcc ,0x34 ,0xa5 ,0xe5 ,0xf1 ,0x71 ,0xd8 ,0x31 ,0x15
 ,0x04 ,0xc7 ,0x23 ,0xc3 ,0x18 ,0x96 ,0x05 ,0x9a ,0x07 ,0x12 ,0x80 ,0xe2 ,0xeb ,0x27 ,0xb2 ,0x75
 ,0x09 ,0x83 ,0x2c ,0x1a ,0x1b ,0x6e ,0x5a ,0xa0 ,0x52 ,0x3b ,0xd6 ,0xb3 ,0x29 ,0xe3 ,0x2f ,0x84
 ,0x53 ,0xd1 ,0x00 ,0xed ,0x20 ,0xfc ,0xb1 ,0x5b ,0x6a ,0xcb ,0xbe ,0x39 ,0x4a ,0x4c ,0x58 ,0xcf
 ,0xd0 ,0xef ,0xaa ,0xfb ,0x43 ,0x4d ,0x33 ,0x85 ,0x45 ,0xf9 ,0x02 ,0x7f ,0x50 ,0x3c ,0x9f ,0xa8
 ,0x51 ,0xa3 ,0x40 ,0x8f ,0x92 ,0x9d ,0x38 ,0xf5 ,0xbc ,0xb6 ,0xda ,0x21 ,0x10 ,0xff ,0xf3 ,0xd2
 ,0xcd ,0x0c ,0x13 ,0xec ,0x5f ,0x97 ,0x44 ,0x17 ,0xc4 ,0xa7 ,0x7e ,0x3d ,0x64 ,0x5d ,0x19 ,0x73
 ,0x60 ,0x81 ,0x4f ,0xdc ,0x22 ,0x2a ,0x90 ,0x88 ,0x46 ,0xee ,0xb8 ,0x14 ,0xde ,0x5e ,0x0b ,0xdb
 ,0xe0 ,0x32 ,0x3a ,0x0a ,0x49 ,0x06 ,0x24 ,0x5c ,0xc2 ,0xd3 ,0xac ,0x62 ,0x91 ,0x95 ,0xe4 ,0x79
 ,0xe7 ,0xc8 ,0x37 ,0x6d ,0x8d ,0xd5 ,0x4e ,0xa9 ,0x6c ,0x56 ,0xf4 ,0xea ,0x65 ,0x7a ,0xae ,0x08
 ,0xba ,0x78 ,0x25 ,0x2e ,0x1c ,0xa6 ,0xb4 ,0xc6 ,0xe8 ,0xdd ,0x74 ,0x1f ,0x4b ,0xbd ,0x8b ,0x8a
 ,0x70 ,0x3e ,0xb5 ,0x66 ,0x48 ,0x03 ,0xf6 ,0x0e ,0x61 ,0x35 ,0x57 ,0xb9 ,0x86 ,0xc1 ,0x1d ,0x9e
 ,0xe1 ,0xf8 ,0x98 ,0x11 ,0x69 ,0xd9 ,0x8e ,0x94 ,0x9b ,0x1e ,0x87 ,0xe9 ,0xce ,0x55 ,0x28 ,0xdf
 ,0x8c ,0xa1 ,0x89 ,0x0d ,0xbf ,0xe6 ,0x42 ,0x68 ,0x41 ,0x99 ,0x2d ,0x0f ,0xb0 ,0x54 ,0xbb ,0x16};
//inverse s-box
int inv_sbox[256] =
 {0x52 ,0x09 ,0x6a ,0xd5 ,0x30 ,0x36 ,0xa5 ,0x38 ,0xbf ,0x40 ,0xa3 ,0x9e ,0x81 ,0xf3 ,0xd7 ,0xfb
 ,0x7c ,0xe3 ,0x39 ,0x82 ,0x9b ,0x2f ,0xff ,0x87 ,0x34 ,0x8e ,0x43 ,0x44 ,0xc4 ,0xde ,0xe9 ,0xcb
 ,0x54 ,0x7b ,0x94 ,0x32 ,0xa6 ,0xc2 ,0x23 ,0x3d ,0xee ,0x4c ,0x95 ,0x0b ,0x42 ,0xfa ,0xc3 ,0x4e
 ,0x08 ,0x2e ,0xa1 ,0x66 ,0x28 ,0xd9 ,0x24 ,0xb2 ,0x76 ,0x5b ,0xa2 ,0x49 ,0x6d ,0x8b ,0xd1 ,0x25
 ,0x72 ,0xf8 ,0xf6 ,0x64 ,0x86 ,0x68 ,0x98 ,0x16 ,0xd4 ,0xa4 ,0x5c ,0xcc ,0x5d ,0x65 ,0xb6 ,0x92
 ,0x6c ,0x70 ,0x48 ,0x50 ,0xfd ,0xed ,0xb9 ,0xda ,0x5e ,0x15 ,0x46 ,0x57 ,0xa7 ,0x8d ,0x9d ,0x84
 ,0x90 ,0xd8 ,0xab ,0x00 ,0x8c ,0xbc ,0xd3 ,0x0a ,0xf7 ,0xe4 ,0x58 ,0x05 ,0xb8 ,0xb3 ,0x45 ,0x06
 ,0xd0 ,0x2c ,0x1e ,0x8f ,0xca ,0x3f ,0x0f ,0x02 ,0xc1 ,0xaf ,0xbd ,0x03 ,0x01 ,0x13 ,0x8a ,0x6b
 ,0x3a ,0x91 ,0x11 ,0x41 ,0x4f ,0x67 ,0xdc ,0xea ,0x97 ,0xf2 ,0xcf ,0xce ,0xf0 ,0xb4 ,0xe6 ,0x73
 ,0x96 ,0xac ,0x74 ,0x22 ,0xe7 ,0xad ,0x35 ,0x85 ,0xe2 ,0xf9 ,0x37 ,0xe8 ,0x1c ,0x75 ,0xdf ,0x6e
 ,0x47 ,0xf1 ,0x1a ,0x71 ,0x1d ,0x29 ,0xc5 ,0x89 ,0x6f ,0xb7 ,0x62 ,0x0e ,0xaa ,0x18 ,0xbe ,0x1b
 ,0xfc ,0x56 ,0x3e ,0x4b ,0xc6 ,0xd2 ,0x79 ,0x20 ,0x9a ,0xdb ,0xc0 ,0xfe ,0x78 ,0xcd ,0x5a ,0xf4
 ,0x1f ,0xdd ,0xa8 ,0x33 ,0x88 ,0x07 ,0xc7 ,0x31 ,0xb1 ,0x12 ,0x10 ,0x59 ,0x27 ,0x80 ,0xec ,0x5f
 ,0x60 ,0x51 ,0x7f ,0xa9 ,0x19 ,0xb5 ,0x4a ,0x0d ,0x2d ,0xe5 ,0x7a ,0x9f ,0x93 ,0xc9 ,0x9c ,0xef
 ,0xa0 ,0xe0 ,0x3b ,0x4d ,0xae ,0x2a ,0xf5 ,0xb0 ,0xc8 ,0xeb ,0xbb ,0x3c ,0x83 ,0x53 ,0x99 ,0x61
 ,0x17 ,0x2b ,0x04 ,0x7e ,0xba ,0x77 ,0xd6 ,0x26 ,0xe1 ,0x69 ,0x14 ,0x63 ,0x55 ,0x21 ,0x0c ,0x7d};
//inverse multiplicative
int inv_mul[256] =
 {0x00 ,0x01 ,0x8d ,0xf6 ,0xcb ,0x52 ,0x7b ,0xd1 ,0xe8 ,0x4f ,0x29 ,0xc0 ,0xb0 ,0xe1 ,0xe5 ,0xc7
 ,0x74 ,0xb4 ,0xaa ,0x4b ,0x99 ,0x2b ,0x60 ,0x5f ,0x58 ,0x3f ,0xfd ,0xcc ,0x00 ,0x40 ,0xee ,0xb2
 ,0x3a ,0x6e ,0x5a ,0xf1 ,0x55 ,0x4d ,0xa8 ,0xc9 ,0xc1 ,0xa0 ,0x98 ,0x15 ,0x30 ,0x44 ,0xa2 ,0xc2
 ,0x2c ,0x45 ,0x92 ,0x6c ,0xf3 ,0x39 ,0x66 ,0x42 ,0xf2 ,0x35 ,0x20 ,0x6f ,0x77 ,0xbb ,0x59 ,0x19
 ,0x0d ,0xfe ,0x37 ,0x67 ,0x2d ,0x31 ,0xf5 ,0x69 ,0xa7 ,0x64 ,0xab ,0x13 ,0x54 ,0x25 ,0xe9 ,0x09
 ,0xed ,0x5c ,0x05 ,0xca ,0x4c ,0x24 ,0x87 ,0xbf ,0x18 ,0x3e ,0x22 ,0xf0 ,0x51 ,0xec ,0x61 ,0x17
 ,0x16 ,0x5e ,0xaf ,0xd3 ,0x49 ,0xa6 ,0x36 ,0x43 ,0xf4 ,0x47 ,0x91 ,0xdf ,0x33 ,0x93 ,0x21 ,0x3b
 ,0x79 ,0xb7 ,0x97 ,0x85 ,0x10 ,0xb5 ,0xba ,0x3c ,0xb6 ,0x70 ,0xd0 ,0x06 ,0xa1 ,0xfa ,0x81 ,0x82
 ,0x83 ,0x7e ,0x7f ,0x80 ,0x96 ,0x73 ,0xbe ,0x56 ,0x9b ,0x9e ,0x95 ,0xd9 ,0xf7 ,0x02 ,0xb9 ,0xa4
 ,0xde ,0x6a ,0x32 ,0x6d ,0xd8 ,0x8a ,0x84 ,0x72 ,0x2a ,0x14 ,0x9f ,0x88 ,0xf9 ,0xdc ,0x89 ,0x9a
 ,0xfb ,0x7c ,0x2e ,0xc3 ,0x8f ,0xb8 ,0x65 ,0x48 ,0x26 ,0xc8 ,0x12 ,0x4a ,0xce ,0xe7 ,0xd2 ,0x62
 ,0x0c ,0xe0 ,0x1f ,0xef ,0x11 ,0x75 ,0x78 ,0x71 ,0xa5 ,0x8e ,0x76 ,0x3d ,0xbd ,0xbc ,0x86 ,0x57
 ,0x0b ,0x28 ,0x2f ,0xa3 ,0xda ,0xd4 ,0xe4 ,0x0f ,0xa9 ,0x27 ,0x53 ,0x04 ,0x1b ,0xfc ,0xac ,0xe6
 ,0x7a ,0x07 ,0xae ,0x63 ,0xc5 ,0xdb ,0xe2 ,0xea ,0x94 ,0x8b ,0xc4 ,0xd5 ,0x9d ,0xf8 ,0x90 ,0x6b
 ,0xb1 ,0x0d ,0xd6 ,0xeb ,0xc6 ,0x0e ,0xcf ,0xad ,0x08 ,0x4e ,0xd7 ,0xe3 ,0x5d ,0x50 ,0x1e ,0xb3
 ,0x5b ,0x23 ,0x38 ,0x34 ,0x68 ,0x46 ,0x03 ,0x8c ,0xdd ,0x9c ,0x7d ,0xa0 ,0xcd ,0x1a ,0x41 ,0x1c};
 /*
  * Galois field product.
  * @param a: value1.
  * @param b: value2.
  */
 uint8_t GaloisMul(uint8_t a, uint8_t b) {
 	uint8_t p = 0;
 	uint8_t counter;
 	uint8_t hi_bit_set;
 	for(counter = 0; counter < 8; counter++) {
 		if((b & 1) == 1)
 			p ^= a;
 		hi_bit_set = (a & 0x80);
 		a <<= 1;
 		if(hi_bit_set == 0x80)
 			a ^= 0x1b;
 		b >>= 1;
 	}
 	return p;
 }

 /*
  * Print a State matrix like.
  * @param state: State matrix like to print.
  */
 void PrintState(uint8_t state[Nb][4],char *msg){
 	printf("%s\n",msg);
 	for(int i=0;i<Nb;i++){
 		for(int j=0;j<4;j++){
 			printf("%x",state[j][i]);
 		}
 		printf("\n");
 	}
 }

/*
 * Applies circular rotation to a word.
 * @param word: The word to be rotated.
 */
void RotWord(uint8_t word[4]){
  uint8_t tmp[4];
  memcpy(tmp,word,4);
  word[3]=tmp[0];
  word[0]=tmp[1];
  word[1]=tmp[2];
  word[2]=tmp[3];
}

/*
 * Applies the s-box to a word.
 * @param word: A word.
 */
void SubWord(uint8_t word[4]){
  for(int i=0;i<4;i++){
    word[i]=sbox[word[i]];
  }
}

/*
 * Applies the s-box to the state matrix.
 * @param state: The state matrix.
 */
void SubBytes(uint8_t state[Nb][4],uint8_t mask[4][4]){
  srand(time(0));
  uint8_t temp[4][4]={0};
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      temp[i][j]=rand()%255;
    }
  }
  PrintState(temp,"----RANDOM TEMP----");
  for(int i=0;i<Nb;i++){
    for(int j=0;j<4;j++){
      state[i][j]=GaloisMul(state[i][j],temp[i][j]);
      state[i][j]^=GaloisMul(mask[i][j],temp[i][j]);
      state[i][j]=inv_mul[state[i][j]];
      state[i][j]^=GaloisMul(mask[i][j],inv_mul[temp[i[j]]]);
      state[i][j]=GaloisMul(state[i][j],temp[i][j]);
    }
  }
}

/*
 * Applies the inverse s-box to the state matrix.
 * @param state: The state matrix.
 */
void InvSubBytes(uint8_t state[Nb][4]){
  for(int i=0;i<Nb;i++){
    for(int j=0;j<4;j++){
      state[i][j]=inv_sbox[state[i][j]];
    }
  }
}

/*
 * Shift rows process.
 * @param state: The state to shift.
 */
void ShiftRows(uint8_t state[Nb][4]){
	uint8_t tmp[Nb][4];
	memcpy(tmp,state,Nb*4);
	for(int j=1;j<4;j++){
		for(int i=0;i<Nb;i++){
			state[i][j]=tmp[(i+j)%4][j];
		}
	}
}

/*
 * Shift rows inverse process.
 * @param state: The state to shift.
 */
void InvShiftRows(uint8_t state[Nb][4]){
	uint8_t tmp[Nb][4];
	memcpy(tmp,state,Nb*4);
	for(int j=1;j<4;j++){
		for(int i=0;i<Nb;i++){
			state[(i+j)%4][j]=tmp[i][j];
		}
	}
}

 uint8_t MaskInput(uint8_t in[4][4], uint8_t mask[4][4]){
   srand(time(0));
   for(int i=0;i<4;i++){
     for(int j=0;j<4;j++){
       mask[i][j]=rand()%255;
       in[i][j]^=mask[i][j];
     }
   }
 }

/*
 * Mix columns process.
 * @param state: The state.
 */
void MixColumns(uint8_t state[Nb][4]){
	uint8_t temp[4][4]={0};
	memcpy(temp,state,Nb*4);
	for(int i=0;i<4;i++){
		state[i][0]=GaloisMul(temp[i][0],2)^GaloisMul(temp[i][3],1)^GaloisMul(temp[i][2],1)^GaloisMul(temp[i][1],3);
		state[i][1]=GaloisMul(temp[i][1],2)^GaloisMul(temp[i][0],1)^GaloisMul(temp[i][3],1)^GaloisMul(temp[i][2],3);
		state[i][2]=GaloisMul(temp[i][2],2)^GaloisMul(temp[i][1],1)^GaloisMul(temp[i][0],1)^GaloisMul(temp[i][3],3);
		state[i][3]=GaloisMul(temp[i][3],2)^GaloisMul(temp[i][2],1)^GaloisMul(temp[i][1],1)^GaloisMul(temp[i][0],3);
	}
}
/*
 * Mix columns inverse process.
 * @param state: The state.
 */
void InvMixColumns(uint8_t state[Nb][4]){
	uint8_t temp[4][4]={0};
	memcpy(temp,state,4*4);
	for(int i=0;i<4;i++){
		state[i][0]=GaloisMul(temp[i][0],14)^GaloisMul(temp[i][3],9)^GaloisMul(temp[i][2],13)^GaloisMul(temp[i][1],11);
		state[i][1]=GaloisMul(temp[i][1],14)^GaloisMul(temp[i][0],9)^GaloisMul(temp[i][3],13)^GaloisMul(temp[i][2],11);
		state[i][2]=GaloisMul(temp[i][2],14)^GaloisMul(temp[i][1],9)^GaloisMul(temp[i][0],13)^GaloisMul(temp[i][3],11);
		state[i][3]=GaloisMul(temp[i][3],14)^GaloisMul(temp[i][2],9)^GaloisMul(temp[i][1],13)^GaloisMul(temp[i][0],11);
	}
}

/*
 * Add the subkey to the state matrix.
 * @param state: The state matrix.
 * @param expanded: The state applied with the subkey.
 * @rework: adapt to different keys.
 */
void AddRoundKey(uint8_t state[Nb][4],uint8_t expanded[(Nr+1)*Nb][4],int i1){
	for(int i=0;i<Nb;i++){
    for(int j=0;j<4;j++){
			state[i][j]^=expanded[i1+i][j];
		}
  }
}

/*
 * Generate the subkeys for all the rounds.
 * @param expanded: The matrix for all the subkeys.
 * @param Nk: Number of words of the key.
 */
void KeyExpansion(uint8_t key[Nk][4], uint8_t expanded[(Nr+1)*Nb][4], uint8_t Nk){
  uint8_t temp[4]={0};
  memcpy(expanded,key,Nk*4);
  for(int i=Nk;i<Nb*(Nr+1);i++){
    memcpy(temp,expanded[i-1],4);
    if(i%Nk==0){
      RotWord(temp);
      SubWord(temp);
      if(i==4){
        temp[0]^=rcon[0];
      }else if(i==8){
        temp[0]^=rcon[1];
      }else if(i==12){
        temp[0]^=rcon[2];
      }else if(i==16){
        temp[0]^=rcon[3];
      }else if(i==20){
        temp[0]^=rcon[4];
      }else if(i==24){
        temp[0]^=rcon[5];
      }else if(i==28){
        temp[0]^=rcon[6];
      }else if(i==32){
        temp[0]^=rcon[7];
      }else if(i==36){
        temp[0]^=rcon[8];
      }else if(i==40){
        temp[0]^=rcon[9];
      }
    }else if(Nk>6 && i%Nk==4){
      SubWord(temp);
    }
    for(int j=0;j<4;j++){
      expanded[i][j]=expanded[i-Nk][j]^temp[j];
    }
  }
}

void Cipher(uint8_t in[4][4], uint8_t out[4][4], uint8_t expanded[(Nr+1)*Nb][4]){
  PrintState(in,"----INPUT NO MASK----");
  uint8_t mask[4][4]={0};
  MaskInput(in,mask);
  PrintState(mask,"----MASK----");
  PrintState(in,"----INPUT MASK----");

  uint8_t state[Nb][4];
  memcpy(state,in,Nb*4);
  AddRoundKey(state,expanded,0);
	for(int round=1;round<Nr;round++){
		SubBytes(state,mask);
		ShiftRows(state);
		MixColumns(state);
		AddRoundKey(state,expanded,round*4);
	}
	SubBytes(state,mask);
	ShiftRows(state);
	AddRoundKey(state,expanded,Nr*Nb);
	memcpy(out,state,Nb*4);
}

void InvCipher(uint8_t in[4][4], uint8_t out[4][4], uint8_t expanded[(Nr+1)*Nb][4]){
  uint8_t state[Nb][4];
  memcpy(state,in,Nb*4);
  AddRoundKey(state,expanded,Nr*Nb);
	for(int round=Nr;round>1;round--){
		InvShiftRows(state);
		InvSubBytes(state);
		AddRoundKey(state,expanded,(round-1)*Nb);
		InvMixColumns(state);
	}
	InvShiftRows(state);
	InvSubBytes(state);
	AddRoundKey(state,expanded,0);
	memcpy(out,state,Nb*4);
}

int main(){
  uint8_t key[4][4]={
    {0x2b,0x7e,0x15,0x16},
    {0x28,0xae,0xd2,0xa6},
    {0xab,0xf7,0x15,0x88},
    {0x09,0xcf,0x4f,0x3c}};
  uint8_t in[4][4]={
    {0x32,0x43,0xf6,0xa8},
    {0x88,0x5a,0x30,0x8d},
    {0x31,0x31,0x98,0xa2},
    {0xe0,0x37,0x07,0x34}};
  uint8_t out1[4][4]={0};
	uint8_t out2[4][4]={0};

  uint8_t expanded[(Nr+1)*4][4];

  KeyExpansion(key,expanded,Nk);
  //PrintState(in,"----INPUT----");
  Cipher(in,out1,expanded);
  //PrintState(in,"----INPUT MASKED----");
	InvCipher(out1,out2,expanded);
  //PrintState(out1,"----CIPHER----");
	//PrintState(out2,"----DECIPHER----");
}
