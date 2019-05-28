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

int Load(FILE *fp, cyc) {

   fprintf(fp, "//The following code loads the initial states to the SHA-2 State partition in inverted form\n\n");

   for (int i=0; i<8; i++) {

      fprintf(fp, "Apply %d 0 01 000000 ", i);  cyc++;
      Rotate(0, fp);

   }

   return cyc;

}

int Ch_EFG(FILE *fp, cyc) {  // Computation memory has NOT BEEN PREVIOUSLY CLEARED

   fprintf(fp, "//The following code computed Ch(E,F,G) and stores it in wl 10\n\n");

   fprintf(fp, "Read 4\n\n"); cyc++; // Read ~e

   fprintf(fp, "Apply 10 1 01 000000 "); cyc++; // Store e in wl 9, 10
   Rotate(0, fp);
   fprintf(fp, "Apply 9 1 01 000000 "); cyc++;
   Rotate(0, fp);

   fprintf(fp, "Read 9\n\n"); cyc++; // Read e  

   fprintf(fp, "Apply 8 1 01 000000 "); cyc++;  // Store ~e in wl 8
   Rotate(0, fp);

   fprintf(fp, "Read 5\n\n"); cyc++; // Read ~f  

   fprintf(fp, "Apply 10 1 00 000000 "); cyc++; // e.f
   Rotate(0, fp);
   fprintf(fp, "Apply 9 1 00 000000 "); cyc++;  // e.f
   Rotate(0, fp);

   fprintf(fp, "Read 6\n\n"); cyc++; // Read ~g     

   fprintf(fp, "Apply 8 1 00 000000 "); cyc++; // ~e.g
   Rotate(0, fp);

   fprintf(fp, "Read 8\n\n"); cyc++; // Read ~e.g    

   fprintf(fp, "Apply 10 1 00 000000 "); cyc++; // e.f.~(~e.g)
   Rotate(0, fp);
   fprintf(fp, "Apply 9 1 01 000000 "); cyc++;  // e.f + ~(~e.g)
   Rotate(0, fp);

   fprintf(fp, "Read 9\n\n"); cyc++; // Read ~e.f + ~(~e.g)  

   fprintf(fp, "Apply 10 1 01 000000 "); cyc++; // e.f XOR ~e.g
   Rotate(0, fp);

   fprintf(fp, "Apply 9 0 00 000000 "); cyc++; // RESET wl 8, 9
   Rotate(0, fp);
   fprintf(fp, "Apply 8 0 00 000000 "); cyc++; 
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
