/*
 * Author: Ahmed BOUAZZA
 * Description: Generates an ASCII terminal chart plotting daily minimum (-) and maximum (+) temperatures from meteorological CSV files.
 */

//  Utilisation :
//     meteocsv_max_tx.c  [FILE...]
//
//  Sans paramètre, rappelle la syntaxe.
//  Sinon, affiche graphe de la temperature
//    pour chacun des fichiers météorologiques
//    (format CSV avec tabulations)
//    dont le nom FILE figure sur la ligne de commande. Pour tout FILE pour
//    lequel le calcul peut être mené à son terme, affiche le résultat du
//    calcul, son unité, une tabulation, FILE, une fin de ligne. Pour les autres
//    FILEs, un message mentionnant FILE est envoyé sur la sortie erreur.
//
//  Renvoie EXIT_SUCCESS à l'environnement d'exécution.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

//--- Déclarations -------------------------------------------------------------

//  report : structure pour les relevés météorologiques.
typedef struct {
  struct {
    int gyear;
    int month;
    int mday;
  } date;
  float tx; // température maximale
  float tn; // température minimale
  float rr; // précipitations
  float w;  // ensoleillement
} report;

//  max_tx:  permette l’affichage du graphe de la  temperature
//    (format CSV avec tabulations)
//    de nom la chaine de caractères pointée par filename.
//  En cas de succès, affecte le résultat du calcul à l'objet pointé par msrptr
//    et renvoie un pointeur nul. En cas d'échec, ne modifie pas la valeur de
//    l'objet pointé par msrptr et renvoie un pointeur vers une chaine de
//    caractères signifiant la cause de l'échec.
const char *max_tx(const char *filename,int TMIN, int TMAX, int Focus, int Graduation);

#define MSR_FORMAT  "%6.1f °C"
#define MAX 30.0f

//--- Fonction principale ------------------------------------------------------

int main(int argc, char *argv[]) {
  if (argc < 6) {
    printf("Syntax: %s TMIN TMAX FOCUS GRADUATION [FILE...]\n", argv[0]);
    return EXIT_SUCCESS;
  }
  errno=0;
  char *end;
  int TMIN = (int)strtol(argv[1],&end,10);
  int TMAX = (int)strtol(argv[2],&end,10);
  int FOCUS = (int)strtol(argv[3],&end,10);
  int GRADUATION = (int)strtol(argv[4],&end,10);

  //  IB : 1 <= k && k <= argc
  //    && les fichiers de nom les chaines pointées par argv[1] à argv[k - 1]
  //        ont été traités
  //  QC : k
  for (int k = 5; k < argc; ++k) {
    const char *r = max_tx(argv[k],TMIN,TMAX,FOCUS,GRADUATION);
    if (r != NULL) {
      fprintf(stderr, "%s: %s '%s'\n", argv[0], r, argv[k]);
    }
  }
  return EXIT_SUCCESS;
}

//--- Développements -----------------------------------------------------------

//  freport_csv_get : lit un relevé sur le flot texte lié au contrôleur pointé
//    par f.
//  En cas de succès, affecte le relevé à l'objet pointé par p et renvoie zéro.
//  En cas d'échec, renvoie une valeur non nulle.
static int freport_csv_get(FILE *f, report *p) {
  //buffer dans lequel on va stocker les chaines à lire
  char buffer[700];
  while(fgets(buffer,sizeof(buffer),f)!=NULL){
    // au cas ou l'ensolleiment n'existera pas , on le met à 0 par défaut dès le début
    p->w=0.0f;
    int var_lus = sscanf(buffer, "%d%d%d%f%f%f%f",
        &p->date.gyear,
        &p->date.month,
        &p->date.mday,
        &p->tx,
        &p->tn,
        &p->rr,
        &p->w);
    if(var_lus == 7 || var_lus ==6){
      return 0;
    }
  }
  return -1;
}

const char *max_tx(const char *filename,int Tmin, int Tmax, int Focus, int Graduation) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    return "Error when opening for reading file";
  }
  report x;
  int rg = freport_csv_get(f, &x);
  printf("%*s",12," ");
  for(int k = Tmin; k<=Tmax; k+=Graduation){
     int n = printf("%d",k);
     printf("%*s",Focus*Graduation-n," ");
  }
  printf("\n");
  printf("%*s",12," ");
  for(int k = Tmin; k<=Tmax; k+=Graduation){
        printf("|");
        printf("%*s",Focus*Graduation-1," ");
  }
  printf("\n");

  //  IB : rg == (valeur renvoyée par le dernier appel freport_csv_get(f, &x))
  //    && x == (dernier relevé lu sur f si rg == 0)
  //    && max_tx  == (  permette l’affichage du graphe. ,
  //              sans compter le dernier relevé lu si rg == 0)
  //  QC : nombre d'appels freport_csv_get(f, &x)
  while (rg == 0) {
    printf("%04d-%02d-%02d  ",x.date.gyear,x.date.month,x.date.mday);
   for(int k = Tmin; k<=Tmax; k+=Focus){
    if(k==0 && x.tx != 0 && x.tn !=0){
       printf("|");
    }else if(k<=x.tn && k+Focus>x.tn && x.tn==x.tx){
       printf("*");
     }else if(k<=x.tn && k+Focus>x.tn){
          printf("-");
     }else if (k<=x.tx && k+Focus>x.tx){
          printf("+");
     }else
          printf(" ");
   }
   printf("\n");
   rg = freport_csv_get(f, &x);
  }
  if (!feof(f)) {
    fclose(f);
    return "Error while processing file";
  }
  if (fclose(f) != 0) {
    return "Error on closing file";
  }
  return NULL;
}
