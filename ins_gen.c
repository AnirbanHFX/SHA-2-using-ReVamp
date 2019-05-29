#include <stdio.h>

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

int Calculate_carrybits(FILE *fp, int cyc, int m2, int m1) {

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
      if (63-i >= 32)
         fprintf(fp, "1 %d ", 63-i);
      else
         fprintf(fp, "0 %d ", 63-i);
   }
   fprintf(fp, "\n\n");

   // Reset m1
   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;
   Rotate(0, fp);

   return cyc;

}

int Load(FILE *fp, int cyc) {

   fprintf(fp, "//The following code loads the initial states to the SHA-2 State partition in inverted form\n\n");

   for (int i=0; i<8; i++) {

      fprintf(fp, "Apply %d 0 01 000000 ", i);  cyc++;
      Rotate(0, fp);

   }

   return cyc;

}

int Ch_EFG(FILE *fp, int cyc, int tar, int mem2, int mem1) {  // Computation memory has NOT BEEN PREVIOUSLY CLEARED
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
   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; 
   Rotate(0, fp);

   return cyc;

}

int Sigma1(FILE *fp, int cyc, int tar, int mem2, int mem1) {  // Computation memory NOT PREVIOUSLY CLEARED
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
   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; 
   Rotate(0, fp);

   return cyc;

}

int Sigma0(FILE *fp, int cyc, int tar, int mem2, int mem1) {  // Computation memory NOT PREVIOUSLY CLEARED
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
   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; 
   Rotate(0, fp);

   return cyc;

}

int Maj(FILE *fp, int cyc, int tar, int m3, int m2, int m1) {

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
   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;  
   Rotate(0, fp);

   return cyc;

}

int Sum(FILE *fp, int cyc, int a, int b, int m1, int m2, int m3, int m4) {   // Computation memory NOT PREVIOUSLY CLEARED
						                             // Stored A ++ B in m4, ++ is sum

   // Assuming A and B are initially non-inverted

   if (a >= 0) {

      fprintf(fp, "Read %d\n\n", a); cyc++;             // Read A

      fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;   // Write ~A

   }

   else {
      // PIR Loads A
      fprintf(fp, "Apply %d 0 01 000000 ", m1); cyc++;  // Write ~A

   }

   fprintf(fp, "Read %d\n\n", m1); cyc++;            // Read ~A

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
      fprintf(fp, "Apply %d 0 00 000000 ", m3); cyc++;  // A.~B
      Rotate(0, fp);
      fprintf(fp, "Apply %d 0 01 000000 ", m2); cyc++;  // A + ~B
      Rotate(0, fp); 
 
   }

   fprintf(fp, "Read %d\n\n", m2); cyc++;            // Read A + ~B  

   fprintf(fp, "Apply %d 1 01 000000 ", m4); cyc++;  // A XOR B
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++;  // A XOR B
   Rotate(0, fp);

   // PIR Loads string of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;  // RESET m1, m2
   Rotate(0, fp);
   fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;
   Rotate(0, fp);

   // Copy A in non_inverted form, half to m2 and half to m1

   if (a >= 0) {

      fprintf(fp, "Read %d\n\n", a); cyc++;             // Read A

      fprintf(fp, "Apply %d 1 01 000000 ", m1); cyc++;   // Write ~A
      Rotate(0, fp);   

   }   

   else {

      // PIR Loads A
      fprintf(fp, "Apply %d 0 01 000000 ", m1); cyc++;   // Write ~A
      Rotate(0, fp);

   }

   fprintf(fp, "Read %d\n\n", m1); cyc++;            // Read ~A

   // PIR Loads String of 1's
   fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;  // Clear m1
   Rotate(0, fp);

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

      // PIR Loads B
      fprintf(fp, "Apply %d 0 01 000000 ", m2); cyc++;  // Copy ~b31 ~b30 ~b29 ... ~b0 to m2
      for(int i=0; i<32; i++) {
         fprintf(fp, "1 %d ", 31-i);
      }
      for(int i=0; i<32; i++) {
         fprintf(fp, "0 %d ", 63-i);
      }
      fprintf(fp, "\n\n");

      fprintf(fp, "Apply %d 0 01 000000 ", m1); cyc++;  // Copy ~b63 ~b62 ~b61 ... ~b32 to m1
      for(int i=0; i<32; i++) {
         fprintf(fp, "1 %d ", 63-i);
      }
      for(int i=0; i<32; i++) {
         fprintf(fp, "0 %d ", 31-i);
      }
      fprintf(fp, "\n\n");   

   }

   cyc = Calculate_carrybits(fp, cyc, m2, m1);    // Line m2 contains (Carry >>> 1)
                                                  // Carry bits need to be left rotated by 1
   fprintf(fp, "Read %d\n\n", m2); cyc++;         // Read (Carry >>> 1)

   fprintf(fp, "Apply %d 1 00 000000 ", m4); cyc++; // (A XOR B).~C
   Rotate(63, fp);                                  // Is equivalent to a 1 bit leftrotate

   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++; // (A XOR B) + ~C
   Rotate(63, fp);

   fprintf(fp, "Read %d\n\n", m3); cyc++;         // Read (A XOR B) + ~C 

   fprintf(fp, "Apply %d 1 01 000000 ", m4); cyc++; // (A XOR B) XOR C
   Rotate(0, fp);         

   return cyc;

}

//int TestSum(FILE *, int); 

int main() {

   FILE *fp, *fp2;

   fp = fopen("SHA-2.ins", "w");

   int cyc = 0;

   //cyc = TestSum(fp, cyc);

   return 0;

}

