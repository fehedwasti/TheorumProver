#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */



int Fsize=50; /*maximum formula length*/
int cases =10;/* number of formulas expected in input.txt*/
int i;/* in case you need it */
int ThSize=100;/* maximum size of set of formulas*/
int TabSize=500; /*maximum length of tableau queue*/
int tableauIsClosed = 1;



/* A set will contain a list of words. Use NULL for emptyset.  */
  struct set{
    char *item;/* pointer to first word of non-empty set*/
    struct set *tail;/*remaining words in the set*/
  };

/* A tableau will contain a list of pointers to sets (of words).  Use NULL for empty list.*/
struct tableau {
  struct set * S; /* pointer to first set in non-empty list */
  struct tableau *rest; /*list of pointers to other sets*/
};


/*You need to change this next bit and include functions for parse, closed and complete.*/

//from my parse code
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
    /*printf("x = ");
    for (int i = 0; i<= indx; i++){
	printf("%c", x[i]);
    }
    printf("\n");

    printf("y = ");
    for (int i = 0; i<= indy; i++){
	printf("%c", y[i]);
    }
    printf("\n");*/

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
  printf("name = %s\n", g);
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

int negations(char *g){
  int negationCount = 0;
  while(*g == '-'){
    negationCount++;
    g++;
  }
  return (negationCount % 2);
}

void complete(struct tableau *t){
  int outernegation;
  //S is the root of the tableau
  //to add a leaf, use *t->rest->S =
  char *g = t->S->item;
  outernegation = negations(g);
  while (*g == '-')
    g++;

  int brackets = 0;
  char x[100];
  char y[100];
  int indx = 0;
  int indy = 0;
  int connective; // 0='v'  1='^'  2='>'

  if (prop(g)){
    tableauIsClosed = 0;
    return;
  }
  else{
    //inside binary formula ---(pvq)>q)
    g++;
    while (*g == '-'){
      x[indx] = *g;
      g++;
    }
    if (prop(g)){
      x[indx] = *g;
      indx++;
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
        x[indx] = *g;
      }
    }
    g++;
    //x complete
    printf("connective: %c", *g);
    if (*g == 'v')
      connective = 0;
    else if (*g == '^')
      connective = 1;
    else if (*g == '>')
      connective = 2;
    //binary connective determined.
    g++;


    while (*g != '\0'){

      y[indy] = *g;
      g++; indy++;
    }
    indy--;
    y[indy] = '\0';

  }
  printf("\nX = ");
  for (int i = 0; i <= indx; i++){
    printf("%c", x[i]);
  }
  printf("\nconnective: %i\n", connective);
  printf("Y = ");
  for (int i = 0; i <= indy; i++){
    printf("%c", y[i]);
  }
  printf("\n");

  printf("current pointer value = %c\n", *g);
  //outernegation and connective known
  //x and y stored
  if (connective == 0){
    if(outernegation){
      //negated disjunction :: alpha
      char w[strlen(x)+strlen(y)+4];
      strcpy(w,"-");
      strcat(w,x);
      strcat(w,",-");
      strcat(w,y);
      //store {-x, -y} = {w}
    }
    else{
      //disjunction :: beta
      //store {x} and {y}
    }
  }
  else if (connective == 1){
    if(outernegation){
      //negated conjunction :: beta
      char w[strlen(x)+2];
      strcpy(w,"-");
      strcat(w,x);
      char v[strlen(y)+2];
      strcpy(v,"-");
      strcat(v,y);

      //store {w} and {v}
    }
    else{
      //conjunction :: alpha
      char w[strlen(x)+strlen(y)+3];
      strcpy(w,x);
      strcat(w,",");
      strcat(w,y);
      //store {x, y}={w}
    }
  }
  else if (connective == 2){
    if(outernegation){
      //negated implication :: alpha
      char w[strlen(x)+strlen(y)+4];
      strcpy(w,x);
      strcat(w,",-");
      strcat(w,y);
      //store {x, -y}={w}
    }
    else{
      //implication :: beta
      char w[strlen(x)+2];
      strcpy(w,"-");
      strcat(w,x);
      //store {w} and {y}
    }
  }

}

void addAlpha(char *alphaString, struct tableau *t){
  //find the set containing the formula
  //extract all terms except the formula and add them to the string
  //remove the formula string from the set
  //add in the new string.
}

int closed(struct tableau *t) {

  if (tableauIsClosed){
    return 1;
  }
  else{
    return 0;
  }
}
/*You should not need to alter the program below.*/
int main() {
  /*input a string and check if its a propositional formula */
  char *name = malloc(Fsize);
  FILE *fp, *fpout;


  /* reads from input.txt, writes to output.txt*/
  if ((  fp=fopen("input.txt","r"))==NULL){printf("Error opening file");exit(1);}
  if ((  fpout=fopen("output.txt","w"))==NULL){printf("Error opening file");exit(1);}

  int j;
  for(j=0;j<cases;j++){
      fscanf(fp, "%s",name);/*read formula*/
      switch (parse(name)){
         case(0): fprintf(fpout, "%s is not a formula.  ", name);break;
	       case(1): fprintf(fpout, "%s is a proposition.  ", name);break;
	       case(2): fprintf(fpout, "%s is a negation.  ", name);break;
	       case(3):fprintf(fpout, "%s is a binary.  ", name);break;
	       default:fprintf(fpout, "What the f***!  ");
	   }


      struct set S = {name, NULL};
      struct tableau t = {&S, NULL};
      if (parse(name)!=0){
        complete(&t);
	      if (closed(&t))
            fprintf(fpout, "%s is not satisfiable.\n", name);
	      else
            fprintf(fpout, "%s is satisfiable.\n", name);
	   }
      else  fprintf(fpout, "I told you, %s is not a formula.\n", name);
  }


  fclose(fp);
  fclose(fpout);
  free(name);

  return(0);
}


//
