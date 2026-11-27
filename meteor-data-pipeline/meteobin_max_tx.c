/*
 * Author: Ahmed BOUAZZA
 * Description: Reads binary meteorological files (.bin) via fread and computes the maximum recorded temperature (Tx).
 */

//  Utilisation :
//     meteocsv_max_tx.c  [FILE...]
//
//  Sans paramètre, rappelle la syntaxe.
//  Sinon, calcule la chaleur maximale.
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
#include <math.h>

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

//  max_tx:  permette l’affichage du maximum des températures maximales Tx
//    pour le fichier météorologique
//    (format CSV avec tabulations)
//    de nom la chaine de caractères pointée par filename.
//  En cas de succès, affecte le résultat du calcul à l'objet pointé par msrptr
//    et renvoie un pointeur nul. En cas d'échec, ne modifie pas la valeur de
//    l'objet pointé par msrptr et renvoie un pointeur vers une chaine de
//    caractères signifiant la cause de l'échec.
const char *max_tx(const char *filename, float *msrptr);

#define MSR_FORMAT  "%6.1f °C"
#define MAX 30.0f

//--- Fonction principale ------------------------------------------------------

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Syntax: %s [FILE...]\n", argv[0]);
    return EXIT_SUCCESS;
  }
  //  IB : 1 <= k && k <= argc
  //    && les fichiers de nom les chaines pointées par argv[1] à argv[k - 1]
  //        ont été traités
  //  QC : k
  for (int k = 1; k < argc; ++k) {
    float msr = -1000;
    const char *r = max_tx(argv[k], &msr);
    if (r != NULL) {
      fprintf(stderr, "%s: %s '%s'\n", argv[0], r, argv[k]);
    } else {
      printf(MSR_FORMAT "\t%s\n", msr, argv[k]);
    }
  }
  return EXIT_SUCCESS;
}

//--- Développements -----------------------------------------------------------

const char *max_tx(const char *filename, float *msrptr) {
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    return "Error when opening for reading file";
  }

  float max = -100.0f;
  int nbr_jrs_lu=0;
  report x;
  while (fread(&x, sizeof(report), 1,f) == 1) {
    if(x.tx>max){
      max = x.tx;
    }
      nbr_jrs_lu+=1;
   }
  if (!feof(f)) {
    fclose(f);
    return "Error while processing file";
  }
  if (fclose(f) != 0) {
    return "Error on closing file";
  }
    if(nbr_jrs_lu == 0){
    return "Empty file";
   }else{
       *msrptr = max;
   }
  return NULL;
}
