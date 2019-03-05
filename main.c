   #include <stdio.h>
   #include <stdlib.h>
   
   int main()
   {
      char ch, file_name[9];
      FILE *fp;
   
   
     fp = fopen("input.txt", "r"); // read mode
  
     if (fp == NULL)
     {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
     }
  
  //   printf("%s\n", file_name);
  
     while((ch = fgetc(fp)) != EOF)
        printf("%c", ch);
  
     fclose(fp);
     return 0;
  }
       
