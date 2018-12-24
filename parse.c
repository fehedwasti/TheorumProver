#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */

int Fsize=50;
int inputs=6;

//issue lies with forming x and y.

int i;
int j;


/*put all your functions here.  You will need
1.
int parse(char *g) which returns 1 if a proposition, 2 if neg, 3 if binary, ow 0
Of course you will almost certainly need other functions.

Your program should read 6 formulas in a file called "input.txt" and output in the way indicated below to a file called "output.txt".
*/

int prop(char *g){
  if(*g == 'p' || *g == 'q' || *g == 'r'){
      return 1;
  }
  else
    return 0;
}
int bfunction(char *g){
  if (*g == 'v' || *g == '^' || *g == '>')
    return 1;
  else
    return 0;
}

int checkformula(char *g){
  printf("running on %s\n", g);
  int brackets = 0;
  char x[100];
  char y[100];
  int indx = 0;
  int indy = 0;

  if (*g == '('){
    if (*(g+1) == '\0'){
      return 0;
    }
    g++; // removes first bracket. eg (p^r) -> p^r)
  }
  else if(prop(g)){ // eg checkformula on p
    if (strlen(g) == 1)
       return 1;
    else
       return 0;
  }

  if (prop(g) && prop(g+1)){
    //eg (pq)
    return 0;
  }

  else if (*g == '-'){
      while (*g == '-'){
        g++;
      }
      if (bfunction(g) || *g == ')'){
        return 0;
      }
  }

  if (*g == '(' || prop(g)){
    if (prop(g) && bfunction(g+1)){
      x[indx] = *g; g++;
    }
    else if (*g == '('){

      brackets += 1;
      x[indx] = *g;
      while (brackets != 0){
        g++;
        indx++;
        if (*g == '('){
          brackets += 1;
        }
        else if (*g == ')'){
          brackets -= 1;
        }
        else if (*g == '\0'){
          return 0;
        }
        x[indx] = *g;
      }
      g++;
    }

    if (!bfunction(g)){
      return 0;
    }
    else{
      if (bfunction(g+1) || *(g+1) == ')'){
        return 0;
      }
      g++;

      if (*g == '-'){
        while (*g == '-'){
          g++;
        }
        if (*g == ')' || bfunction(g)){
          return 0;
        }
      }

      if (prop(g) && *(g+1)==')'){
        y[indy] = *g;
      }
      else{
        while (*g != '\0'){

          y[indy] = *g;
          g++; indy++;
        }
        y[indy-1] = '\0';
      }
    }

    }
    printf("x = ");
    for (int i = 0; i<= indx; i++){
	printf("%c", x[i]);
    }
    printf("\n");

    printf("y = ");
    for (int i = 0; i<= indy; i++){
	printf("%c", y[i]);
    }
    printf("\n");

    char *x1, *y1;
    x1 = &x[0];
    y1 = &y[0];
    if (!indx && !indy){
      return 1;
    }
    else if (!indx && indy != 0){
      return (checkformula(y1));
    }
    else if (indx != 0 && !indy){
      return (checkformula(x1));
    }
    return (checkformula(x1) && checkformula(y1));
}



int parse(char *g){
  int isFormula;
  int negation = 0;

  //first check for negation and remove - signs from string.
  if (*g == '-'){ //negation
    while (*g == '-')
      g++;
    negation = 1;
  }


  if (prop(g) && strlen(g) == 1){ //proposition
    if (negation){
      return 2;
    }
    else if (strlen(g) == 1 && prop(g)){
      return 1;
    }
  }

  else if (*g == '('){
    isFormula = checkformula(g);

    if (isFormula && negation){
        return 2;
    }
    else if(isFormula){
      return 3;
    }
    else{
      return 0;
    }
  }


  else{

    return 0;
  }


}

int main()
{ /*input 6 strings from "input.txt" */
  char *names[inputs];/*to store each of the input strings*/

  for (i=0;i<inputs;i++) names[i]=malloc(Fsize);/*create enough space*/



  FILE *fp, *fpout, *fopen();

  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file input.txt\n");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file\n");exit(1);}/*ouputs to be sent to "output.txt"*/

  fscanf(fp,"%s %s %s %s %s %s",names[0],names[1], names[2], names[3],names[4],names[5]);/*read input strings from "input.txt"*/

  /*lets check your parser*/
  for(i=0;i<inputs;i++)
    {j=parse(names[i]);
      switch(j)
	{
	case(0):fprintf(fpout,"%s is not a formula\n", names[i]);break;
	case(1):fprintf(fpout,"%s is a proposition\n",names[i]);break;
	case(2):fprintf(fpout,"%s is a negation\n",names[i]);break;
	case(3):fprintf(fpout,"%s is a binary formula\n",names[i]);break;
	default:fprintf(fpout,"%s is not a formula\n",names[i]);break;
	}
    }


  fclose(fp);
  fclose(fpout);

  return(0);
}
