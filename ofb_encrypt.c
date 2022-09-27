#include<stdio.h>
#include <fcntl.h>
#include <unistd.h>
//initial_permutation
int initial_perm[64] = {  58, 50, 42, 34, 26, 18, 10, 2,
                          60, 52, 44, 36, 28, 20, 12, 4,
                          62, 54, 46, 38, 30, 22, 14, 6,
                          64, 56, 48, 40, 32, 24, 16, 8,
                          57, 49, 41, 33, 25, 17, 9, 1,
                          59, 51, 43, 35, 27, 19, 11, 3,
                          61, 53, 45, 37, 29, 21, 13, 5,
                          63, 55, 47, 39, 31, 23, 15, 7
                        };
//final permutation                            
int final_perm[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                      39, 7, 47, 15, 55, 23, 63, 31,
                      38, 6, 46, 14, 54, 22, 62, 30,
                      37, 5, 45, 13, 53, 21, 61, 29,
                      36, 4, 44, 12, 52, 20, 60, 28,
                      35, 3, 43, 11, 51, 19, 59, 27,
                      34, 2, 42, 10, 50, 18, 58, 26,
                      33, 1, 41, 9, 49, 17, 57, 25
};
// Expansion D-box Table
int expd[48] = { 32, 1, 2, 3, 4, 5, 4, 5,
                  6, 7, 8, 9, 8, 9, 10, 11,
                  12, 13, 12, 13, 14, 15, 16, 17,
                  16, 17, 18, 19, 20, 21, 20, 21,
                  22, 23, 24, 25, 24, 25, 26, 27,
                  28, 29, 28, 29, 30, 31, 32, 1
};
// S-box Table
int sb[8][4][16] = {    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
                        {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                         {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                         {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                         {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
                        {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                         {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                         {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                         {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
                        {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                         {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
                         {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                         {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
                        {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                         {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                         {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                         {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
                        {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                         {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                         {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                         {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
                        {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                         {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                         {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                         {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
                        {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                         {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                         {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                         {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
};
//f-permutation
int per[32] = {   16, 7, 20, 21,
                  29, 12, 28, 17,
                  1, 15, 23, 26,
                  5, 18, 31, 10,
                  2, 8, 24, 14,
                  32, 27, 3, 9,
                  19, 13, 30, 6,
                  22, 11, 4, 25
};
//key perm(pc-1)
int key_perm[56] = {  57, 49, 41, 33, 25, 17, 9,
                      1, 58, 50, 42, 34, 26, 18,
                      10, 2, 59, 51, 43, 35, 27,
                      19, 11, 3, 60, 52, 44, 36,
                      63, 55, 47, 39, 31, 23, 15,
                      7, 62, 54, 46, 38, 30, 22,
                      14, 6, 61, 53, 45, 37, 29,
                      21, 13, 5, 28, 20, 12, 4
};
// Key- Compression Table(round key permutation) pc-2
int key_comp[48] = {  14, 17, 11, 24, 1, 5,
                      3, 28, 15, 6, 21, 10,
                      23, 19, 12, 4, 26, 8,
                      16, 7, 27, 20, 13, 2,
                      41, 52, 31, 37, 47, 55,
                      30, 40, 51, 45, 33, 48,
                      44, 49, 39, 56, 34, 53,
                      46, 42, 50, 36, 29, 32
};
unsigned char round_key[16][6];
void permu (unsigned char *c, unsigned char *b, int *perm, int size);
void print (unsigned char *c, int n);
void left_onebit_shift (unsigned char a[6]);
void left_twobit_shift (unsigned char a[6]);
void key_scheduling (unsigned char *key);
void fiestel(unsigned char b[8] ,int k);
void encryption(unsigned char *c, unsigned char *r);


int main (){
  unsigned char plaintext[8],result[8] ={0} ,a[8] ={0} ,pad_key[8]={0};
  unsigned char key[8] = "Biswajit",IV[8]="SohanDAs";
  key_scheduling (key);
  int fd1=open("sourav",O_RDONLY, 0666),fd2 = open("encryption",O_WRONLY | O_CREAT | O_TRUNC, 0666);
  do{
  	for(int i=0;i<8;i++){
  		plaintext[i]=0;
  		result[i]=0;
  		pad_key[i]=0;
  	}
  	read(fd1,plaintext, 8 * sizeof(unsigned char));
  	printf("initail:");
  	print(IV,8);
  	printf("plaintext:");
  	print(plaintext,8);
  	encryption(IV,pad_key);
  	printf("the ciphertext is\n");
  	for(int i=0;i<8;i++){
  		result[i]=plaintext[i] ^ pad_key[i];
  		printf("%c",result[i]);
  		IV[i]=pad_key[i];
  	}
  	write(fd2,result,8*sizeof(unsigned char));
  	printf("\n");	
  }
  while(plaintext[7]!=0);
  close(fd1);
  close(fd2);
  return 0;
}
void permu (unsigned char *c, unsigned char b[], int *perm, int size)
{
  int i, x, s, y, z;
  for (i = 0; i < size; i++)
    {
      x = i >> 3;		//division by 8
      s = i & 7;
      y = (perm[i] - 1) & 7;	// remainder after dividing by 8
      z = (perm[i] - 1) >> 3;
      b[x] += ((c[z] >> (7 - y)) & 1) << (7 - s);
    }
}
void print (unsigned char *c, int n)
{
  for (int i = 0; i < n; i++)
    printf ("%d,", c[i]);
  printf("\t");
  for (int i = 0; i < n; i++)
    printf ("%c", c[i]);
  printf ("\n");
}
void fiestel(unsigned char b[8] ,int k){
    int i;
    unsigned char l[4] = {0}, r[4] ={0}, compr[8] = {0}, final[4] = {0}, pre[8] ={0};
    //partition the text in left and right 32 bit each
      for (i = 0; i < 4; i++)
	    r[i] = b[i + 4];
      for (i = 0; i < 4; i++)
    	l[i] = b[i];
      //applying expansion on right 
      unsigned char expanded[6] = { 0, 0, 0, 0, 0, 0 };
      permu (r, expanded, expd, 48);
      //xoring with the key
      for (i = 0; i < 6; i++)
	expanded[i] = expanded[i] ^ round_key[15-k][i];
      //making new array of 64bit to apply 8 s-boxes
      compr[0] = expanded[0] >> 2;
      compr[1] = (expanded[0] & 0X3) << 4;
      compr[1] = compr[1] | (expanded[1] >> 4);
      compr[2] = (expanded[1] & 0X0F) << 2;
      compr[2] = compr[2] | (expanded[2] >> 6);
      compr[3] = expanded[2] & 0X3F;

      compr[4] = expanded[3] >> 2;
      compr[5] = (expanded[3] & 0X3) << 4;
      compr[5] = compr[5] | (expanded[4] >> 4);
      compr[6] = (expanded[4] & 0X0F) << 2;
      compr[6] = compr[6] | (expanded[5] >> 6);
      compr[7] = expanded[5] & 0x3F;

      //acessing sbox
      for (i = 0; i < 8; i++)
	{
	  int row, col;
	  row = (compr[i] & 0x1) | ((compr[i] & 0x20) >> 4);
	  col = ((compr[i] >> 1) & 0x0F);
	  pre[i] = sb[i][row][col];
	}

      //returning to 32 bit array
      for (i = 0; i < 8; i = i + 2)
	final[i >> 1] = ((pre[i] << 4) | pre[i + 1]);

      //applying permutation
      unsigned char f[4] = { 0, 0, 0, 0 };
      permu (final, f, per, 32);
      //xoring the output with left
      for (i = 0; i < 4; i++)
	b[i + 4] = f[i] ^ l[i];
      //putting the left as it is on the right
      for (i = 0; i < 4; i++)
	b[i] = r[i];
}
void left_onebit_shift (unsigned char a[7])
{
  unsigned char c, bit;
  int i;
  for (i = 0; i < 7; i++)
    a[i] = a[i] << 1;
  for (i = 6; i > 0; i--)
    a[i - 1] = a[i - 1] | a[i] >> 4;

  a[6] = a[6] | a[0] >> 4;

}
void left_twobit_shift (unsigned char a[7])
{
  unsigned char c, twobit;
  int i;
  for (i = 0; i < 7; i++)
    a[i] = a[i] << 2;
  for (i = 6; i > 0; i--)
    a[i - 1] = a[i - 1] | (a[i] >> 4);
  a[6] = a[6] | a[0] >> 4;
}
//key scheduling
void key_scheduling (unsigned char *key)
{
  unsigned char key_fiftysix[7] = { 0 }, left_key[7], right_key[7];
  permu (key, key_fiftysix, key_perm, 56);
  //printf("Effective 56bit key is:\n");
  //print(key_fiftysix,7);
  for (int r = 0; r < 16; r++)
    {
      //partitioning 56 bit effective key in 28 bit each left and right

      //for the left part
      for (int i = 0; i < 7; i = i + 2)
	left_key[i] = key_fiftysix[i >> 1] >> 4;

      for (int i = 1; i < 7; i = i + 2)
	left_key[i] = key_fiftysix[i >> 1] & 0XF;

      //for the right part
      for (int i = 0; i < 7; i = i + 2)
	right_key[i] = key_fiftysix[(i >> 1) + 3] & 0XF;

      for (int i = 1; i < 7; i = i + 2)
	right_key[i] = key_fiftysix[(i >> 1) + 4] >> 4;

      //rotation on key
      if (r == 0 || r == 1 || r == 8 || r == 15)
	{
	  left_onebit_shift (left_key);
	  left_onebit_shift (right_key);
	}
      else
	{
	  left_twobit_shift (left_key);
	  left_twobit_shift (right_key);
	}
      //merging left and right part
      for (int i = 0; i < 6; i = i + 2)
	key_fiftysix[i >> 1] =
	  ((left_key[i] << 4) | (left_key[i + 1] & 0X0F));

      key_fiftysix[3] = left_key[6] << 4 | right_key[0] & 0X0F;

      for (int i = 1; i < 6; i = i + 2)
	key_fiftysix[4 + (i >> 1)] =
	  ((right_key[i] << 4) | (right_key[i + 1] & 0X0F));

      permu (key_fiftysix, round_key[r], key_comp, 48);
      //printf("round key at %d step:",r+1);
      //print (round_key[r], 6);
    }
}
void encryption(unsigned char *c,unsigned char *result){
   unsigned char  b[8] =
  {
  0, 0, 0, 0, 0, 0, 0, 0}, swap[8];
  int i, k;
  //applyting the innital_permutation
 permu (c, b, initial_perm, 64);
 //printf("after innital_permutation:\n");
 //print(b,8);
//for loop of 16 round fiestel
  for (k = 0; k < 16; k++)
    {
      //printf("at round %d\n",k);
      //print(b,8);
      fiestel(b,k);
     
    }
  //swapping the text in left and right 32 bit each
  for (i = 0; i < 4; i++)
    swap[i] = b[i + 4];
  for (i = 0; i < 4; i++)
    swap[i + 4] = b[i];

//applying final permutaion
  permu (swap, result, final_perm, 64); 
    
}

