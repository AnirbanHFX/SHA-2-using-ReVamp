#include <stdio.h>
#include <stdint.h>

void Rotate(int c, FILE *fp) {  // Used for bit shifting
                                // Rotate(0, fp) signifies no bit shifting
   int t = (63+c)%64;

   for(int i=0; i<64; i++) {

      if (t < 0)
         t+=64;

      fprintf(fp, "1 %d ", t--);

   }

   fprintf(fp, "\n\n");

}

void PIR_RoundConstants(FILE *fp2, int cyc, int i) {

   uint64_t k[] = { 0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538, 
              0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe, 
              0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 
              0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 
              0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab, 
              0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725, 
              0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 
              0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b, 
              0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218, 
              0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 
              0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 
              0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec, 
              0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c, 
              0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6, 
              0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 
              0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817};

   uint64_t h;

   fprintf(fp2, "%d ", cyc);
   for(int c=63; c>=0; c--) {
      h = (k[i]) >> c;
      if (h & 1)
         fprintf(fp2, "1");
      else
         fprintf(fp2, "0");
   }
   fprintf(fp2, "\n\n");

}

void PIR_Words(FILE *fp2, int cyc, int i) {

   uint64_t k[64] = {0};

   uint64_t h;

   fprintf(fp2, "%d ", cyc);
   for(int c=63; c>=0; c--) {
      h = (k[i]) >> c;
      if (h & 1)
         fprintf(fp2, "1");
      else
         fprintf(fp2, "0");
   }
   fprintf(fp2, "\n\n");

}

int Calculate_carrybits(FILE *fp, FILE* fp2, int cyc, int m2, int m1) {

   fprintf(fp, "Read %d\n\n", m2); cyc++;

   fprintf(fp, "Apply %d 1 00 000000 ", m2); cyc++;	 // Calculates and stores c1 in bit 63 (leftmost bit is labelled 0)
   for (int i=0; i<64; i++) {
      if (i == 63)
         fprintf(fp, "1 32 ");
      else if (31-i >=0) 
         fprintf(fp, "0 %d ", 31-i);
      else 
         fprintf(fp, "0 %d ", 64+31-i);
   }
   fprintf(fp, "\n\n");

   for (int i=1; i<32; i++) {

      fprintf(fp, "Read %d\n\n", m2); cyc++;
 
      fprintf(fp, "Apply %d 1 11 %d ", m2, 64-i); cyc++; // Calculates ci and stores in bit 63-i
      for (int j=0; j<64; j++) {
         if (j == 63-i)
            fprintf(fp, "1 %d ", 32+i);
         else if (31-j >=0) 
            fprintf(fp, "0 %d ", 31-j);
         else 
            fprintf(fp, "0 %d ", 64+31-j);
      }
      fprintf(fp, "\n\n");

   }

   // Reset left half of line m2
   // PIR Loads string of 1's

   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;
   for (int i=0; i<64; i++) {
      if (63-i >= 32)
         fprintf(fp, "1 %d ", 63-i);
      else
         fprintf(fp, "0 %d ", 63-i);
   }
   fprintf(fp, "\n\n");

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Read %d\n\n", m2); cyc++;  // Read c32

   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // Copy ~c32 to immediate left
   for (int i=0; i<64; i++) {
      if (62-i == 31)
         fprintf(fp, "1 31 ");
      else if (62-i >= 0)
         fprintf(fp, "0 %d ", 62-i);
      else
         fprintf(fp, "0 %d ", 62-i+63);
   }
   fprintf(fp, "\n\n");   

   fprintf(fp, "Read %d\n\n", m2); cyc++;  // Read ~c31

   fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;  // Copy c31 to the corresponding position in m1
   for (int i=0; i<64; i++) {
      if (64 + 0-i == 32)
         fprintf(fp, "1 32 ");
      else if (0-i >= 0)
         fprintf(fp, "0 %d ", 0-i);
      else
         fprintf(fp, "0 %d ", 0-i+64);
   }
   fprintf(fp, "\n\n");   

   // Calculate remaining carry bits

   fprintf(fp, "Read %d\n\n", m1); cyc++;

   fprintf(fp, "Apply %d 1 11 32 ", m1); cyc++;	 // Calculates and stores c1 in bit 63 (leftmost bit is labelled 0)
   for (int i=0; i<64; i++) {
      if (i == 63)
         fprintf(fp, "1 32 ");
      else if (31-i >= 0) 
         fprintf(fp, "0 %d ", 31-i);
      else 
         fprintf(fp, "0 %d ", 64+31-i);
   }
   fprintf(fp, "\n\n");
   

   for (int i=1; i<31; i++) {  // Does not compute c64

      fprintf(fp, "Read %d\n\n", m1); cyc++;
 
      fprintf(fp, "Apply %d 1 11 %d ", m1, 64-i); cyc++; // Calculates ci and stores in bit 63-i
      for (int j=0; j<64; j++) {
         if (j == 63-i)
            fprintf(fp, "1 %d ", 32+i);
         else if (31-j >=0) 
            fprintf(fp, "0 %d ", 31-j);
         else 
            fprintf(fp, "0 %d ", 64+31-j);
      }
      fprintf(fp, "\n\n");

   }

   // Reset ~c32 in m2
   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;
   for (int i=0; i<64; i++) {
      if (63-i >= 32)
         fprintf(fp, "1 %d ", 63-i);
      else
         fprintf(fp, "0 %d ", 63-i);
   }
   fprintf(fp, "\n\n");

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   // Reset left half of m1
   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;
   for (int i=0; i<64; i++) {
      if (63-i >= 32)
         fprintf(fp, "1 %d ", 63-i);
      else
         fprintf(fp, "0 %d ", 63-i);
   }
   fprintf(fp, "\n\n");

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   // Copy c33 c34 ... c63 to m1 in inverted form

   fprintf(fp, "Read %d\n\n", m1); cyc++;

   fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;
   for (int i=0; i<64; i++) {
      if (31-i >= 0) {
         if (31 - i == 31)
            fprintf(fp, "0 %d ", 31-i);
         else 
            fprintf(fp, "1 %d ", 31-i);
      }
      else 
         fprintf(fp, "0 %d ", 31-i+64);
   }
   fprintf(fp, "\n\n");

   fprintf(fp, "Read %d\n\n", m1); cyc++;  // Read ~c63 ~c62 ... ~c33

   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;   // Copy c63 c62 ... c33 to m2
   for (int i=0; i<64; i++) {
      if (63-i == 63)
         fprintf(fp, "0 %d ", 63-i);
      else if (63-i >= 32)
         fprintf(fp, "1 %d ", 63-i);
      else
         fprintf(fp, "0 %d ", 63-i);
   }
   fprintf(fp, "\n\n");

   // Reset m1
   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   return cyc;

}

int Load(FILE *fp, FILE *fp2, int cyc) {

   uint64_t h[] = { 0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1, 
           0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179 };
   uint64_t k;
   uint64_t m = 0xffffffffffffffff;

   fprintf(fp, "//The following code loads the initial states to the SHA-2 State partition in inverted form\n\n");

   for (int i=0; i<8; i++) {

      fprintf(fp, "Apply %d 0 01 000000 ", i);  cyc++;
      Rotate(0, fp);

      fprintf(fp2, "%d ", cyc);
      for(int c=63; c>=0; c--) {
         k = (h[i]) >> c;
         if (k & 1)
            fprintf(fp2, "1");
         else
            fprintf(fp2, "0");
      }
      fprintf(fp2, "\n\n");

   }

   return cyc;

}

int Ch_EFG(FILE *fp, FILE *fp2, int cyc, int tar, int mem2, int mem1) {  // Computation memory has NOT BEEN PREVIOUSLY CLEARED
			     // Leaves WordLine tar occupied

   fprintf(fp, "//The following code computes Ch(E,F,G) and stores it in wl %d\n\n", tar);

   fprintf(fp, "Read 4\n\n"); cyc++; // Read ~e

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // Store e in wl 9, 10
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++;
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", mem2); cyc++; // Read e  

   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++;  // Store ~e in wl 8
   Rotate(0, fp);

   fprintf(fp, "Read 5\n\n"); cyc++; // Read ~f  

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // e.f
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", mem2); cyc++;  // e.f
   Rotate(0, fp);

   fprintf(fp, "Read 6\n\n"); cyc++; // Read ~g     

   fprintf(fp, "Apply %d 1 00 000000 ", mem1); cyc++; // ~e.g
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", mem1); cyc++; // Read ~e.g    

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // e.f.~(~e.g)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++;  // e.f + ~(~e.g)
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", mem2); cyc++; // Read ~e.f + ~(~e.g)  

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // e.f XOR ~e.g
   Rotate(0, fp);

   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", mem2); cyc++; // RESET wl 8, 9
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; 
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   return cyc;

}

int Sigma1(FILE *fp, FILE *fp2, int cyc, int tar, int mem2, int mem1) {  // Computation memory NOT PREVIOUSLY CLEARED
							  // Leaves wordline tar occupied

   fprintf(fp, "//The following code computes Sigma1 and stores it in wl %d\n\n", tar);

   fprintf(fp, "Read 4\n\n"); cyc++; // Read ~e

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // Write e14
   Rotate(14, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // Write e14
   Rotate(14, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // Write e14
   Rotate(14, fp);

   fprintf(fp, "Read %d\n\n", tar); cyc++; // Read e14

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++;  // e14.~e18
   Rotate(4, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", mem2); cyc++; // e14.~e18
   Rotate(4, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // e14 + ~e18
   Rotate(4, fp);

   fprintf(fp, "Read %d\n\n", mem1); cyc++;  // Read e14 + ~e18

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++;  // e14 XOR e18
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // e14 XOR e18
   Rotate(0, fp);

   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; // Reset wl mem1
   Rotate(0, fp);

   fprintf(fp, "Read 4\n\n"); cyc++;                  // Read ~e

   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // Store e41
   Rotate(41, fp);

   fprintf(fp, "Read %d\n\n", mem1); cyc++;           // Read e41

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++;  // (e14 XOR e18).~e41
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // (e14 XOR e18) + ~e41
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", mem2); cyc++;           // Read (e14 XOR e18) + ~e41

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++;  // (e14 XOR e18) XOR e41
   Rotate(0, fp);

   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", mem2); cyc++; // RESET wl mem1, mem2
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; 
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   return cyc;

}

int Sigma0(FILE *fp, FILE *fp2, int cyc, int tar, int mem2, int mem1) {  // Computation memory NOT PREVIOUSLY CLEARED
							  // Leaves wordline tar occupied

   fprintf(fp, "//The following code computes Sigma0 and stores it in wl %d\n\n", tar);

   fprintf(fp, "Read 0\n\n"); cyc++;                  // Read ~a
 
   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++;  // Write a28
   Rotate(28, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // Write a28
   Rotate(28, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // Write a28
   Rotate(28, fp);

   fprintf(fp, "Read %d\n\n", tar); cyc++;            // Read a28

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++;  // a28.~a34
   Rotate(6, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", mem2); cyc++; // a28.~a34
   Rotate(6, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // a28 + ~a34
   Rotate(6, fp);

   fprintf(fp, "Read %d\n\n", mem1); cyc++;           // Read a28 + ~a34

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++;  // a28 XOR a34
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // a28 XOR a34
   Rotate(0, fp);

   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; // Reset wl mem1
   Rotate(0, fp);

   fprintf(fp, "Read 0\n\n"); cyc++;                  // Read ~a

   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // Store a39
   Rotate(39, fp);

   fprintf(fp, "Read %d\n\n", mem1); cyc++;           // Read a39

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++;  // (a28 XOR a34).~a39
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // (a28 XOR a34) + ~a39
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", mem2); cyc++;           // Read (a28 XOR a34) + ~a39

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++;  // (a28 XOR a34) XOR a39
   Rotate(0, fp);

   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", mem2); cyc++; // RESET wl mem1, mem2
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; 
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   return cyc;

}

int Maj(FILE *fp, FILE *fp2, int cyc, int tar, int m3, int m2, int m1) {

   fprintf(fp, "//The following code computes Maj(A, B, C) and stores it in wl %d\n\n", tar);   

   fprintf(fp, "Read 0\n\n"); cyc++; // Read ~a   

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // Write a
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++;  // Write a
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // Write a
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;  // Write a
   Rotate(0, fp);

   fprintf(fp, "Read 1\n\n"); cyc++;                 // Read ~b  

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // a.b
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", m3); cyc++;  // a.b
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", m2); cyc++;  // a.b
   Rotate(0, fp);

   fprintf(fp, "Read 2\n\n"); cyc++;                 // Read ~c 

   fprintf(fp, "Apply %d 1 00 000000 ", m1); cyc++;  // a.c
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", m1); cyc++;            // Read a.c 

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // (a.b).~(a.c)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", m3); cyc++;  // (a.b).~(a.c)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // (a.b) + ~(a.c)
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", m2); cyc++;            // Read (a.b) + ~(a.c)

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // (a.b)XOR(a.c)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++;  // (a.b)XOR(a.c)
   Rotate(0, fp);

   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;  // RESET wl m2, m1
   Rotate(0, fp);
   fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;  
   Rotate(0, fp);

   fprintf(fp, "Read 1\n\n"); cyc++;                 // Read ~b

   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // Write b
   Rotate(0, fp);

   fprintf(fp, "Read 2\n\n"); cyc++;                 // Read ~c

   fprintf(fp, "Apply %d 1 00 000000 ", m2); cyc++;  // b.c
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", m2); cyc++;            // Read b.c

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // (a.b XOR a.c).~(b.c)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++;  // (a.b XOR a.c) + ~(b.c)
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", m3); cyc++;            // Read (a.b XOR a.c) + ~(b.c)

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // (a.b XOR a.c) XOR (b.c)
   Rotate(0, fp);

   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m3); cyc++;  // RESET wl m3, m2
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;  
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   return cyc;

}

int Sum(int round, FILE *fp, FILE *fp2, int cyc, int a, int b, int m1, int m2, int m3, int m4, int ainverted) {   // Computation memory NOT PREVIOUSLY CLEARED
						                             // Stored A ++ B in m4, ++ is sum

   // Assuming B is initially non-inverted

   if ( ainverted == 0 ) {  // A is non-inverted (from PIR or Computation Memory)

      if (a >= 0) {

         fprintf(fp, "Read %d\n\n", a); cyc++;             // Read A

         fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;   // Write ~A
         Rotate(0, fp);

      }

      else {
         // PIR Loads A
         fprintf(fp, "Apply %d 0 01 000000 ", m1); cyc++;  // Write ~A
         Rotate(0, fp);
         PIR_RoundConstants(fp2, cyc, round);

      }

      fprintf(fp, "Read %d\n\n", m1); cyc++;            // Read ~A

   }

   else {  // A is inverted (from SHA-2 State partition)

      fprintf(fp, "Read %d\n\n", a); cyc++;             // Read ~A

   }

   fprintf(fp, "Apply %d 1 01 000000 ", m4); cyc++;  // Write A
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++;  // Write A
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // Write A
   Rotate(0, fp);

   if (b >= 0) {

      fprintf(fp, "Read %d\n\n", b); cyc++;             // Read B

      fprintf(fp, "Apply %d 1 00 000000 ", m4); cyc++;  // A.~B
      Rotate(0, fp);
      fprintf(fp, "Apply %d 1 00 000000 ", m3); cyc++;  // A.~B
      Rotate(0, fp);
      fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // A + ~B
      Rotate(0, fp); 

   }
    
   else {
      // PIR Loads B
      fprintf(fp, "Apply %d 0 00 000000 ", m4); cyc++;  // A.~B
      Rotate(0, fp);
      PIR_Words(fp2, cyc, round);     

      fprintf(fp, "Apply %d 0 00 000000 ", m3); cyc++;  // A.~B
      Rotate(0, fp);
      PIR_Words(fp2, cyc, round);    

      fprintf(fp, "Apply %d 0 01 000000 ", m2); cyc++;  // A + ~B
      Rotate(0, fp); 
      PIR_Words(fp2, cyc, round);    
 
   }

   fprintf(fp, "Read %d\n\n", m2); cyc++;            // Read A + ~B  

   fprintf(fp, "Apply %d 1 01 000000 ", m4); cyc++;  // A XOR B
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++;  // A XOR B
   Rotate(0, fp);

   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;  // RESET m1, m2
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   if (ainverted == 0) {

      fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;
      Rotate(0, fp);
      fprintf(fp2, "%d ", cyc);
      for(int i=0; i<64; i++)
         fprintf(fp2, "1");
      fprintf(fp2, "\n\n");

   }

   // Copy A in non_inverted form, half to m2 and half to m1

   if (ainverted == 0) {

      if (a >= 0) {

         fprintf(fp, "Read %d\n\n", a); cyc++;             // Read A

         fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;   // Write ~A
         Rotate(0, fp);   

      }   

      else {

         // PIR Loads A
         fprintf(fp, "Apply %d 0 01 000000 ", m1); cyc++;   // Write ~A
         Rotate(0, fp);

         PIR_RoundConstants(fp2, cyc, round);

      }

      fprintf(fp, "Read %d\n\n", m1); cyc++;            // Read ~A

   }

   else {  // A is inverted (from SHA-2 State partition)

      fprintf(fp, "Read %d\n\n", a); cyc++;             // Read ~A

   }

   // PIR Loads String of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;  // Clear m1
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // Copy a31 a30 a29 ... a0 to m2
   for(int i=0; i<32; i++) {
      fprintf(fp, "0 %d ", 63-i);
   }
   for(int i=0; i<32; i++) {
      fprintf(fp, "1 %d ", 31-i);
   }
   fprintf(fp, "\n\n");

   fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;  // Copy 0 a62 a61 ... a32 to m1
   for(int i=0; i<32; i++) {
      fprintf(fp, "0 %d ", 31-i);
   }

   fprintf(fp, "0 63 ");

   for(int i=1; i<32; i++) {
      fprintf(fp, "1 %d ", 63-i);
   }
   fprintf(fp, "\n\n");  

   // Copy B in inverted form, half to m2 and half to m1
   
   if (b >= 0) {

      fprintf(fp, "Read %d\n\n", b); cyc++;             // Read B

      fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // Copy ~b31 ~b30 ~b29 ... ~b0 to m2
      for(int i=0; i<32; i++) {
         fprintf(fp, "1 %d ", 31-i);
      }
      for(int i=0; i<32; i++) {
         fprintf(fp, "0 %d ", 63-i);
      }
      fprintf(fp, "\n\n");

      fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;  // Copy ~b63 ~b62 ~b61 ... ~b32 to m1
      for(int i=0; i<32; i++) {
         fprintf(fp, "1 %d ", 63-i);
      }
      for(int i=0; i<32; i++) {
         fprintf(fp, "0 %d ", 31-i);
      }
      fprintf(fp, "\n\n");   

   }

   else {
//////////////////////////////////////////////
      // PIR Loads B
      fprintf(fp, "Apply %d 0 01 000000 ", m2); cyc++;  // Copy ~b31 ~b30 ~b29 ... ~b0 to m2
      for(int i=0; i<32; i++) {
         fprintf(fp, "1 %d ", 31-i);
      }
      for(int i=0; i<32; i++) {
         fprintf(fp, "0 %d ", 63-i);
      }
      fprintf(fp, "\n\n");

      PIR_Words(fp2, cyc, round);    

      fprintf(fp, "Apply %d 0 01 000000 ", m1); cyc++;  // Copy ~b63 ~b62 ~b61 ... ~b32 to m1
      for(int i=0; i<32; i++) {
         fprintf(fp, "1 %d ", 63-i);
      }
      for(int i=0; i<32; i++) {
         fprintf(fp, "0 %d ", 31-i);
      }
      fprintf(fp, "\n\n");   

      PIR_Words(fp2, cyc, round);    

   }

   cyc = Calculate_carrybits(fp, fp2, cyc, m2, m1);    // Line m2 contains (Carry >>> 1)
                                                  // Carry bits need to be left rotated by 1

   fprintf(fp, "Read %d\n\n", m2); cyc++;         // Read (Carry >>> 1)

   fprintf(fp, "Apply %d 1 00 000000 ", m4); cyc++; // (A XOR B).~C
   Rotate(63, fp); /*
   for(int i=0; i<64; i++) {
      if (62-i >=0 )
         fprintf(fp, "1 %d ", 62-i);
      else
         fprintf(fp, "0 63 ");
   } 
   fprintf(fp, "\n\n"); */

   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++; // (A XOR B) + ~C
   Rotate(63, fp);  /*
   for(int i=0; i<64; i++) {
      if (62-i >=0 )
         fprintf(fp, "1 %d ", 62-i);
      else
         fprintf(fp, "0 63 ");
   }
   fprintf(fp, "\n\n"); */

   fprintf(fp, "Read %d\n\n", m3); cyc++;         // Read (A XOR B) + ~C 

   fprintf(fp, "Apply %d 1 01 000000 ", m4); cyc++; // (A XOR B) XOR C
   Rotate(0, fp);      

   fprintf(fp, "Apply %d 0 00 000000 ", m3); cyc++;   // RESET m3
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;   // RESET m2
   Rotate(0, fp); 

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");  

   return cyc;

}

int TestSum(FILE *, int); 

int Round(FILE *, FILE *, int, int);

int main() {

   FILE *fp, *fp2;

   fp = fopen("SHA-2.ins", "w");
   fp2 = fopen("SHA-2.inp", "w");

   int cyc = 0;

   //cyc = TestSum(fp, cyc);

   cyc = Load(fp, fp2, cyc);

   for (int i=0; i<80; i++) {

      cyc = Round(fp, fp2, cyc, i);

   }

   return 0;

}

int Round(FILE *fp, FILE *fp2, int cyc, int i) {



   uint64_t w[80] = {0};

   // Ki ++ Wi
   cyc = Sum(i, fp, fp2, cyc, -1, -1, 8, 9, 10, 11, 0);  // Correctness of Sum verified

   // Ki ++ Wi ++ H
   cyc = Sum(i, fp, fp2, cyc, 7, 11, 8, 9, 10, 12, 1);

   // Reset wl 11
   fprintf(fp, "Apply 11 0 00 000000 "); cyc++;
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   // Ch(E, F, G)
   cyc = Ch_EFG(fp, fp2, cyc, 10, 9, 8);               // Correctness of Ch not verified

   // G -> H                                     // Correctness of copying verified
   fprintf(fp, "Read 6\n\n"); cyc++;             // Read ~G
   fprintf(fp, "Apply 8 1 01 000000 "); cyc++;   // Write G
   Rotate(0, fp); 
   fprintf(fp, "Read 8\n\n"); cyc++;             // Read G

   fprintf(fp, "Apply 8 0 00 000000 "); cyc++;   // Reset 8
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 7 0 00 000000 "); cyc++;   // Reset H
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 7 1 01 000000 "); cyc++;   // Copy ~G to H
   Rotate(0, fp);

   // F -> G
   fprintf(fp, "Read 5\n\n"); cyc++;             // Read ~F
   fprintf(fp, "Apply 8 1 01 000000 "); cyc++;   // Write F
   Rotate(0, fp);
   fprintf(fp, "Read 8\n\n"); cyc++;             // Read F

   fprintf(fp, "Apply 8 0 00 000000 "); cyc++;   // Reset 8
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 6 0 00 000000 "); cyc++;   // Reset G
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 6 1 01 000000 "); cyc++;   // Copy ~F to G
   Rotate(0, fp);

   // E -> F
   fprintf(fp, "Read 4\n\n"); cyc++;             // Read ~E
   fprintf(fp, "Apply 8 1 01 000000 "); cyc++;   // Write E
   Rotate(0, fp);
   fprintf(fp, "Read 8\n\n"); cyc++;             // Read E

   fprintf(fp, "Apply 8 0 00 000000 "); cyc++;   // Reset 8
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 5 0 00 000000 "); cyc++;   // Reset F
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 5 1 01 000000 "); cyc++;   // Copy ~E to F
   Rotate(0, fp);

   // Computation of Sigma1(E)
   cyc = Sigma1(fp, fp2, cyc, 11, 9, 8);         // Correctness of Sigma1 not verified

   // Ch ++ Sigma1
   cyc = Sum(i, fp, fp2, cyc, 10, 11, 8, 9, 13, 14, 0);

   fprintf(fp, "Apply 10 0 00 000000 "); cyc++;  // Reset 10
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 11 0 00 000000 "); cyc++;  // Reset 11
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   // T1 = (Ch ++ Sigma1) ++ (Ki ++ Wi ++ H)
   cyc = Sum(i, fp, fp2, cyc, 12, 14, 8, 9, 10, 11, 0);

   fprintf(fp, "Apply 14 0 00 000000 "); cyc++;  // Reset 14
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 12 0 00 000000 "); cyc++;  // Reset 12
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   // D ++ T1
   cyc = Sum(i, fp, fp2, cyc, 3, 11, 8, 9, 10, 12, 1);

   // Copy ~(D ++ T1) to ~E
   fprintf(fp, "Read 12\n\n"); cyc++;             // Read (D ++ T1)

   fprintf(fp, "Apply 4 0 00 000000 "); cyc++;    // Reset E
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");   

   fprintf(fp, "Apply 4 1 01 000000 "); cyc++;    // Write ~(D ++ T1) to E
   Rotate(0, fp);

   // Reset (D ++ T1)
   fprintf(fp, "Apply 12 0 00 000000 "); cyc++;   // Reset 12
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");   

   // Maj(A, B, C)
   cyc = Maj(fp, fp2, cyc, 12, 10, 9, 8);              // Correctness of Maj(A,B,C) not verified

   // C -> D                                     // Correctness of copying verified
   fprintf(fp, "Read 2\n\n"); cyc++;             // Read ~C
   fprintf(fp, "Apply 8 1 01 000000 "); cyc++;   // Write C
   Rotate(0, fp); 
   fprintf(fp, "Read 8\n\n"); cyc++;             // Read C

   fprintf(fp, "Apply 8 0 00 000000 "); cyc++;   // Reset 8
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 3 0 00 000000 "); cyc++;   // Reset D
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 3 1 01 000000 "); cyc++;   // Copy ~C to D
   Rotate(0, fp);

   // B -> C
   fprintf(fp, "Read 1\n\n"); cyc++;             // Read ~B
   fprintf(fp, "Apply 8 1 01 000000 "); cyc++;   // Write B
   Rotate(0, fp);
   fprintf(fp, "Read 8\n\n"); cyc++;             // Read B

   fprintf(fp, "Apply 8 0 00 000000 "); cyc++;   // Reset 8
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 2 0 00 000000 "); cyc++;   // Reset C
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 2 1 01 000000 "); cyc++;   // Copy ~B to C
   Rotate(0, fp);

   // A -> B
   fprintf(fp, "Read 0\n\n"); cyc++;             // Read ~A
   fprintf(fp, "Apply 8 1 01 000000 "); cyc++;   // Write A
   Rotate(0, fp);
   fprintf(fp, "Read 8\n\n"); cyc++;             // Read A

   fprintf(fp, "Apply 8 0 00 000000 "); cyc++;   // Reset 8
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 1 0 00 000000 "); cyc++;   // Reset B
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 1 1 01 000000 "); cyc++;   // Copy ~A to B
   Rotate(0, fp);

   // Computation of Sigma0(A)
   cyc = Sigma0(fp, fp2, cyc, 10, 9, 8);               // Correctness of Sigma0 not verified

   // Maj(A,B,C) ++ Sigma0
   cyc = Sum(i, fp, fp2, cyc, 10, 12, 8, 9, 13, 14, 0);

   fprintf(fp, "Apply 10 0 00 000000 "); cyc++;  // Reset 10
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 12 0 00 000000 "); cyc++;  // Reset 12
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   // T1 ++ T2
   cyc = Sum(i, fp, fp2, cyc, 11, 14, 8, 9, 10, 12, 0);

   // Copy ~(T1 ++ T2) to ~A
   fprintf(fp, "Read 12\n\n"); cyc++;             // Read (T2 ++ T1)

   fprintf(fp, "Apply 0 0 00 000000 "); cyc++;    // Reset A
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");   

   fprintf(fp, "Apply 0 1 01 000000 "); cyc++;    // Write ~(D ++ T1) to A
   Rotate(0, fp);

   fprintf(fp, "Apply 11 0 00 000000 "); cyc++;  // Reset 11
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 12 0 00 000000 "); cyc++;  // Reset 12
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   fprintf(fp, "Apply 14 0 00 000000 "); cyc++;  // Reset 14
   Rotate(0, fp);
   fprintf(fp2, "%d ", cyc);
   for (int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   return cyc;

} 

int TestSum(FILE *fp, int cyc) {

   FILE *fp2 = fopen("SHA-2.inp", "w");

   fprintf(fp, "Apply 0 0 01 000000 "); cyc++;
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   //for(int i=0; i<50; i++)
      //fprintf(fp2, "1");
   fprintf(fp2, "0100001010001010001011111001100011010111001010001010111000100010\n\n");

   fprintf(fp, "Apply 1 0 01 000000 "); cyc++;
   Rotate(0, fp);

   fprintf(fp2, "%d ", cyc);
   for(int i=0; i<64; i++)
      fprintf(fp2, "1");
   fprintf(fp2, "\n\n");

   cyc = Sum(0, fp, fp2, cyc, 0, 1, 2, 3, 4, 5, 1);

   return cyc;

}
