#include <stdio.h>
#include <string.h>   /* for all the new-fangled string functions */
#include <stdlib.h>     /* malloc, free, rand */



int Fsize=50; /*maximum formula length*/
int cases =10;/* number of formulas expected in input.txt*/
int i;/* in case you need it */
int ThSize=100;/* maximum size of set of formulas*/
int TabSize=500; /*maximum length of tableau queue*/
int tableauIsClosed = 1;
int contradiction = 0;
char myTableau[100][50];
int indTab = 0;
void complete();


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
void printFormula(char formula[]){
  printf("\nPrinting formula: ");
  for (int i = 0; i<strlen(formula); i++){
    printf("%c", formula[i]);
  }
  printf("\n");
}
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
  indTab = 0;
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
  return 0;


}

int negations(char *g){
  int negationCount = 0;
  while(*g == '-'){
    negationCount++;
    g++;
  }
  return (negationCount % 2);
}

void contradictionCheck(char *u){
  printf("checking: %s\n", u);
  if (strstr(u, ",p,") != NULL && strstr(u,"-p") != NULL){
    contradiction = 1;
    printf("BOB\n");
  }
  else if (strstr(u, ",q,") != NULL && strstr(u,"-q") != NULL){
    contradiction = 1;
    printf("BOB\n");
  }
  else if (strstr(u, ",r,") != NULL && strstr(u,"-r") != NULL){
    contradiction = 1;
    printf("BOB\n");
  }
  else if (strstr(u,"p") == NULL && strstr(u,"q") == NULL && strstr(u,"r") == NULL){
    contradiction = 1;
    printf("empty set\n");
  }
  else{
    tableauIsClosed = 0;
  }
}

void addAlpha(char formula[], struct tableau *t){
  //z pointer is before the formula position
  printf("I WAS IN ALPHA!!!\nmyTableau\n");
  for (int i = 0; i <= indTab; i++){
    char *z = myTableau[i];
    char store[strlen(z)];
    strcpy(store, "\0");
    printf("Term: %s\n", z);
    char check[strlen(t->S->item) + 3];
    strcpy(check,",");
    strcat(check, t->S->item);
    strcat(check,",");
    if (strstr(myTableau[i], check) != NULL){
      char *formulaPos = strstr(myTableau[i], t->S->item);
      char addcharacter[2];
      addcharacter[1] = '\0';
      while (z != formulaPos){
        addcharacter[0] = *z;
        strcat(store,addcharacter);
        z++;
      }
      strcat(store,formula);
      z += (int)strlen(t->S->item);
      strcat(store,z);
      strcpy(myTableau[i], store);
      //new alpha formula stored.
    }
    else{
      continue;
    }
  }
  printf("Changed Tableau\n");
  for (int i = 0; i <= indTab; i++){
    printf("%s\n", myTableau[i]);
    if(strstr(myTableau[i], "(") == NULL){
      if (tableauIsClosed){
        printf("*****Tableau Complete*****\n\n");
        contradictionCheck(myTableau[i]);
        if (!contradiction){
          printf("*****Formula Satisfied*****\n\n");
          return;
        }
        else{
          printf("**Contradiction. set removed***\n\n");
          strcpy(myTableau[i],"\0");
          contradiction = 0;
        }
      }
      else{
        break;
      }
    }

    else{
      char *formulaPos = strstr(myTableau[i], "(");
      if (*(formulaPos - 1) == '-')
            formulaPos = strstr(myTableau[i], "-(");
      int count =0;
      while (*formulaPos != ','){
        count++;
        formulaPos++;
      }
      formulaPos -= count;
      char newName[count+2];
      strcpy(newName, "\0");
      char addcharacter[2];
      addcharacter[1] = '\0';
      while(*formulaPos != ','){
        addcharacter[0] = *formulaPos;
        strcat(newName, addcharacter);
        formulaPos++;
      }
      printf("\ncount: %i, formula: ", count);
      for (int i = 0; i<strlen(newName); i++){
        printf("%c", newName[i]);
      }
      printf("HI");
      struct set M = {newName, NULL};
      struct tableau n = {&M, NULL};
      complete(&n);
    }
  }
  printf("\n");
}
void addBeta(char formula1[], char formula2[], struct tableau *t){
  printf("ARE YOU BETA???\nmyTableau\n");
        //printFormula(formula1);
  int tab = indTab;
  for (int i = 0; i <= tab; i++){
    printf("Term: %s\n", myTableau[i]);
    char *z = myTableau[i];
    char store1[strlen(z)];
    char store2[strlen(z)];
    strcpy(store1,"\0");
    strcpy(store2,"\0");
    char check[strlen(t->S->item) + 3];
    strcpy(check,",");
    strcat(check, t->S->item);
    strcat(check,",");
    if (strstr(myTableau[i], check) != NULL){
      char *formulaPos = strstr(myTableau[i], t->S->item);
      char addcharacter[2];
      addcharacter[1] = '\0';
      while (z != formulaPos){
        addcharacter[0] = *z;
        printf("z: %s\n", z);
        printf("fpos: %s", formulaPos);
        strcat(store1,addcharacter);
        strcat(store2,addcharacter);
        z++;
      }
      strcat(store1,formula1);
      strcat(store2,formula2);
      z += (int)strlen(t->S->item);
      strcat(store1,z);
      strcat(store2,z);
      strcpy(myTableau[i], store1);
      strcpy(myTableau[indTab+1], store2);
      indTab++;
      //new alpha formula stored.
    }
    else{
      continue;
    }
  }

  printf("Changed Tableau\n");
  for (int i = 0; i <= indTab; i++){
    printf("%s\n", myTableau[i]);
    if (strstr(myTableau[i], "(") == NULL){
      printf("*****Tableau Complete*****\n\n");
      contradictionCheck(myTableau[i]);
      if (!contradiction){
        printf("*****Formula Satisfied*****\n\n");
        return;
      }
      else{
        printf("**Contradiction. set removed***\n\n");
        strcpy(myTableau[i],"\0");
        contradiction = 0;
      }
    }
    else{
      char *formulaPos = strstr(myTableau[i], "(");
      if (*(formulaPos - 1) == '-')
            formulaPos = strstr(myTableau[i], "-(");
      int count =0;
      while (*formulaPos != ','){
        count++;
        formulaPos++;
      }
      formulaPos -= count;
      char newName[count+2];
      strcpy(newName, "\0");
      char addcharacter[2];
      addcharacter[1] = '\0';
      while(*formulaPos != ','){
        addcharacter[0] = *formulaPos;
        strcat(newName, addcharacter);
        formulaPos++;
      }
      printf("\ncount: %i, formula: ", count);
      for (int i = 0; i<strlen(newName); i++){
        printf("%c", newName[i]);
      }
      printf("HI");
      struct set M = {newName, NULL};
      struct tableau n = {&M, NULL};
      complete(&n);
    }
  }
}
void complete(struct tableau *t){
  int outernegation;
  //S is the root of the tableau
  //to add a leaf, use *t->rest->S =
  char *g = t->S->item;
  printf("completing: %s\n", g);
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
    char w[3];
    if (outernegation){
      w[0] = '-';
      w[1] = *g;
      w[2] = '\0';
    }
    else{
      w[0] = *g;
      w[1] = '\0';
    }
    addAlpha(w,t);
  }
  else{
    //inside binary formula ---(pvq)>q)
    g++;
    int negativeX = 0;
    int negativeY = 0;
    while (*g == '-'){
      negativeX++;
      g++;
    }
    if (negativeX % 2 == 1){
      printf("***negative X***\n");
      strcpy(x,"-"); indx++;
    }
    else{
      printf("normal X\n");
      strcpy(x,"\0");
    }
    if (prop(g)){
      printf("MY NAME JEFF: %c\n", *g);
      x[indx] = *g;
      x[indx+1] = '\0';
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
    printFormula(x);
    g++;
    //x complete
    //printf("connective: %c", *g);
    if (*g == 'v')
      connective = 0;
    else if (*g == '^')
      connective = 1;
    else if (*g == '>')
      connective = 2;
    //binary connective determined.
    g++;


    while (*g == '-'){
      negativeY++;
      g++;
    }
    if (negativeY % 2 == 1){
      strcpy(y,"-"); indy++;
    }
    else{
      strcpy(y,"\0");
    }
    while (*g != '\0'){
      y[indy] = *g;
      g++; indy++;
    }
    indy--;
    y[indy] = '\0';

    printf("\nindx = %i\nX = ", indx);
    for (int i = 0; i <= indx; i++){
      printf("%c", x[i]);
    }
    printf("\nconnective: %i\n", connective);
    printf("Y = ");
    for (int i = 0; i <= indy; i++){
      printf("%c", y[i]);
    }
    printf("\n");
    //outernegation and connective known
    //x and y stored
    if (connective == 0){
      if(outernegation){
        //negated disjunction :: alpha
        char w[strlen(x)+strlen(y)+4];
        if (x[0] == '-'){
          strcpy(w,"\0");
          for (int i = 0; i <= indx; i++){
            x[i] = x[i+1];
          }
          x[indx] = '\0';
          printf("\nnew x: ");
          for (int i = 0; i < indx; i++){
            printf("%c", x[i]);
          }
          printf("\n");
        }
        else{
          strcpy(w,"-");
        }
        strcat(w,x);
        if (y[0] == '-'){
          strcat(w,",");
          for (int i = 0; i < indy; i++){
            y[i] = y[i+1];
          }
          y[indy-1] = '\0';
          printf("\nnew Y: ");
          for (int i = 0; i < indy; i++){
            printf("%c", y[i]);
          }
          printf("\n");
        }
        else{
          strcat(w,",-");
        }
        strcat(w,y);
        //store {-x, -y} = {w}
        addAlpha(w, t);
      }
      else{
        //disjunction :: beta
        //store {x} and {y}
        addBeta(x,y,t);
      }
    }
    else if (connective == 1){
      if(outernegation){
        //negated conjunction :: beta
        char w[strlen(x)+2];
        if (x[0] == '-'){
          strcpy(w,"\0");
          for (int i = 0; i <= indx; i++){
            x[i] = x[i+1];
          }
          x[indx] = '\0';
          printf("\nnew x: ");
          for (int i = 0; i < indx; i++){
            printf("%c", x[i]);
          }
          printf("\n");
        }
        else{
          strcpy(w,"-");
        }
        strcat(w,x);
        char v[strlen(y)+2];
        if (y[0] == '-'){
          strcpy(v,"\0");
          for (int i = 0; i < indy; i++){
            y[i] = y[i+1];
          }
          y[indy-1] = '\0';
          printf("\nnew Y: ");
          for (int i = 0; i < indy; i++){
            printf("%c", y[i]);
          }
          printf("\n");
        }
        else{
          strcpy(v,"-");
        }
        strcat(v,y);

        //store {w} and {v}
        addBeta(w,v,t);
      }
      else{
        //conjunction :: alpha
        char w[strlen(x)+strlen(y)+3];
        strcpy(w,x);
        strcat(w,",");
        strcat(w,y);
        //store {x, y}={w}
        addAlpha(w, t);
      }
    }
    else if (connective == 2){
      if(outernegation){
        //negated implication :: alpha
        char w[strlen(x)+strlen(y)+4];
        strcpy(w,x);
        if (y[0] == '-'){
          strcat(w,",");
          for (int i = 0; i < indy; i++){
            y[i] = y[i+1];
          }
          y[indy-1] = '\0';
          printf("\nnew Y: ");
          for (int i = 0; i < indy; i++){
            printf("%c", y[i]);
          }
          printf("\n");
        }
        else{
          strcat(w,",-");
        }
        strcat(w,y);
        //store {x, -y}={w}
        addAlpha(w, t);
      }
      else{
        //implication :: beta
        char w[strlen(x)+2];
        if (x[0] == '-'){
          strcpy(w,"\0");
          for (int i = 0; i < indx; i++){
            printf("%c\n", x[i]);
            x[i] = x[i+1];
          }
          x[indx-1] = '\0';
          printf("indx: %i\nnew x: ", indx);
          for (int i = 0; i <= indx; i++){
            printf("%c", x[i]);
          }
        }
        else{
          strcpy(w,"-");
        }
        strcat(w,x);
        //store {w} and {y}
        addBeta(w,y,t);
      }
    }
  }


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
        tableauIsClosed = 1;
        strcpy(myTableau[0], ",");
        strcat(myTableau[0], name);
        strcat(myTableau[0], ",");
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
