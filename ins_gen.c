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

   fprintf(fp, "Read 4\n\n"); cyc++; // Read ~e

   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // Store e41
   Rotate(41, fp);

   fprintf(fp, "Read %d\n\n", mem1); cyc++; // Read e41

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // (e14 XOR e18).~e41
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // (e14 XOR e18) + ~e41
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", mem2); cyc++; // Read (e14 XOR e18) + ~e41

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // (e14 XOR e18) XOR e41
   Rotate(0, fp);

   fprintf(fp, "Apply %d 0 00 000000 ", mem2); cyc++; // RESET wl mem1, mem2
   Rotate(0, fp);
   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; 
   Rotate(0, fp);

   return cyc;

}

int Sigma0(FILE *fp, int cyc, int tar, int mem2, int mem1) {  // Computation memory NOT PREVIOUSLY CLEARED
							  // Leaves wordline tar occupied

   fprintf(fp, "//The following code computes Sigma0 and stores it in wl %d\n\n", tar);

   fprintf(fp, "Read 0\n\n"); cyc++; // Read ~a

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // Write a28
   Rotate(28, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // Write a28
   Rotate(28, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // Write a28
   Rotate(28, fp);

   fprintf(fp, "Read %d\n\n", tar); cyc++; // Read a28

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++;  // a28.~a34
   Rotate(6, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", mem2); cyc++; // a28.~a34
   Rotate(6, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // a28 + ~a34
   Rotate(6, fp);

   fprintf(fp, "Read %d\n\n", mem1); cyc++;  // Read a28 + ~a34

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++;  // a28 XOR a34
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // a28 XOR a34
   Rotate(0, fp);

   fprintf(fp, "Apply %d 0 00 000000 ", mem1); cyc++; // Reset wl mem1
   Rotate(0, fp);

   fprintf(fp, "Read 0\n\n"); cyc++; // Read ~a

   fprintf(fp, "Apply %d 1 01 000000 ", mem1); cyc++; // Store a39
   Rotate(39, fp);

   fprintf(fp, "Read %d\n\n", mem1); cyc++; // Read a39

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // (a28 XOR a34).~a39
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", mem2); cyc++; // (a28 XOR a34) + ~a39
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", mem2); cyc++; // Read (a28 XOR a34) + ~a39

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // (a28 XOR a34) XOR a39
   Rotate(0, fp);

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

   fprintf(fp, "Read 1\n\n"); cyc++; // Read ~b  

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // a.b
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", m3); cyc++;  // a.b
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", m2); cyc++;  // a.b
   Rotate(0, fp);

   fprintf(fp, "Read 2\n\n"); cyc++; // Read ~c 

   fprintf(fp, "Apply %d 1 00 000000 ", m1); cyc++;  // a.c
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", m1); cyc++; // Read a.c 

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // (a.b).~(a.c)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 00 000000 ", m3); cyc++;  // (a.b).~(a.c)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // (a.b) + ~(a.c)
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", m2); cyc++; // Read (a.b) + ~(a.c)

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // (a.b)XOR(a.c)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++;  // (a.b)XOR(a.c)
   Rotate(0, fp);

   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;  // RESET wl m2, m1
   Rotate(0, fp);
   fprintf(fp, "Apply %d 0 00 000000 ", m1); cyc++;  
   Rotate(0, fp);

   fprintf(fp, "Read 1\n\n"); cyc++; // Read ~b

   fprintf(fp, "Apply %d 1 01 000000 ", m2); cyc++;  // Write b
   Rotate(0, fp);

   fprintf(fp, "Read 2\n\n"); cyc++; // Read ~c

   fprintf(fp, "Apply %d 1 00 000000 ", m2); cyc++;  // b.c
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", m2); cyc++; // Read b.c

   fprintf(fp, "Apply %d 1 00 000000 ", tar); cyc++; // (a.b XOR a.c).~(b.c)
   Rotate(0, fp);
   fprintf(fp, "Apply %d 1 01 000000 ", m3); cyc++;  // (a.b XOR a.c) + ~(b.c)
   Rotate(0, fp);

   fprintf(fp, "Read %d\n\n", m3); cyc++; // Read (a.b XOR a.c) + ~(b.c)

   fprintf(fp, "Apply %d 1 01 000000 ", tar); cyc++; // (a.b XOR a.c) XOR (b.c)
   Rotate(0, fp);

   fprintf(fp, "Apply %d 0 00 000000 ", m3); cyc++;  // RESET wl m3, m2
   Rotate(0, fp);
   fprintf(fp, "Apply %d 0 00 000000 ", m2); cyc++;  
   Rotate(0, fp);

   return cyc;

}


int main() {

   FILE *fp, *fp2;

   fp = fopen("SHA-2.ins", "w");

   int cyc = 0;

   cyc = Load(fp, cyc);

   return 0;

}
