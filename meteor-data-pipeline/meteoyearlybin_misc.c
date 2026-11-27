/*
 * Author: Ahmed BOUAZZA
 * Description: Analyzes annual binary weather records (365-366 days) using generic array queries to report frost nights, first no-thaw day, and minimum Tx.
 */

// meteoyearlybin_misc.c
//  Utilisation :
//    meteoyearlybin_misc [FILE]...

//  Sans paramètre, affiche la syntaxe.
//  Sinon, effectue divers calculs effectués sur chacun des fichiers
//    météorologiques (format binaire) annuels dont le nom FILE figure sur la
//    ligne de commande. Pour chaque FILE pour lequel le calcul peut être mené à
//    son terme, affiche l'année, l'objet du calcul, le résultat, éventuellement
//    une information complémentaire, avec la tabulation comme séparateur et la
//    fin de ligne comme terminateur. Pour les autres FILEs, un message
//    mentionnant FILE est envoyé sur la sortie erreur.
//
//  Renvoie EXIT_SUCCESS à l'environnement d'exécution.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

//--- Déclarations -------------------------------------------------------------

//  date : structure.
typedef struct {
  int gyear;  //  année grégorienne
  int month;  //  numéro du mois dans l'année, [1, 12]
  int mday;   //  quantième, [1, 31]
} date;

//  date_print_full, date_print_year : affiche les quantième, mois et année
//    séparés par des tirets ou l'année de la date pointée par p.
void date_print_full(const date *p);
void date_print_year(const date *p);

//  report : structure.
typedef struct {
  date d;
  float tx; //  température maximale
  float tn; //  température minimale
  float rr; //  précipitations
  float w;  //  ensoleillement
} report;

//  report_frostnight_cond : teste si l'objet pointé par p correspond à une nuit
//    avec gelée (Tn <= 0) ou non.
bool report_frostnight_cond(const report *p) {
  return p->tn <= 0.0;
}

//  report_frost_cond : teste si l'objet pointé par p correspond à un jour sans
//    dégel (Tx, Tn <= 0) ou non.
bool report_frost_cond(const report *p) {
  return p->tx <= 0.0 && p->tn <= 0.0;
}

//  report_mintx_left_search : recherche la première occurrence d'un jour de
//    température maximale (Tx) minimum dans le tableau de composants de type
//    report, d'adresse de premier composant base et de longueur nmemb. Renvoie
//    un pointeur nul si nmemb vaut zéro, l'adresse du composant trouvé sinon.
report *report_mintx_left_search(const report *base, size_t nmemb);

//  yearlyreport : structure.

#define NDAYS_PER_YEAR_MIN  365
#define NDAYS_PER_YEAR_MAX  366

typedef struct {
  report array[NDAYS_PER_YEAR_MAX];
  size_t length;
} yearlyreport;

//  yearlyreport_fload : charge le contenu du fichier météorologique (format
//    binaire) annuel de nom la chaine pointée par filename à partir de
//    l'adresse du champ array de l'objet pointé par p. La longueur du fichier
//    doit être comprise entre NDAYS_PER_YEAR_MIN et NDAYS_PER_YEAR_MAX.
//  En cas de succès, affecte au champ length de l'objet pointé par p cette
//    longueur et renvoie zéro.
//  En cas d'échec, renvoie une valeur non nulle.
int yearlyreport_fload(const char *filename, yearlyreport *p);

//  Formats d'affichage.

#define MSR_FORMAT    "%0.1f"
#define LABEL_FORMAT  "%-20s"

//--- Fonction principale ------------------------------------------------------

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Syntax: %s [FILE]...\n", argv[0]);
    return EXIT_SUCCESS;
  }
  //  IB : 1 <= k && k <= argc
  //    && les fichiers de nom les chaines pointées par argv[1] à argv[k - 1]
  //        ont été traités
  //  QC : k
  for (int k = 1; k < argc; ++k) {
    const char *a = argv[k];
    yearlyreport yrpt;
    int r = yearlyreport_fload(a, &yrpt);
    if (r != 0) {
      fprintf(stderr, "%s: %s '%s'\n", argv[0], "Error on file", a);
    } else {
      {
        size_t n = cond_count(yrpt.array, yrpt.length, sizeof(report), 
                              (bool (*)(const void *))report_frostnight_cond);
        date_print_year(&yrpt.array[0].d);
        printf("\t" LABEL_FORMAT "\t", "no. days of frost");
        printf("%zu\n", n);
      }
      {
        report *p = (report *)cond_left_search(yrpt.array, yrpt.length, sizeof(report), 
                                               (bool (*)(const void *))report_frost_cond);
        date_print_year(&yrpt.array[0].d);
        printf("\t" LABEL_FORMAT "\t", "first no-thaw day");
        if (p == nullptr) {
          printf("***\n");
        } else {
          date_print_full(&p->d);
          printf("\tTx/Tn = " MSR_FORMAT "/" MSR_FORMAT "\n", p->tx, p->tn);
        }
      }
      {
        report *p = report_mintx_left_search(yrpt.array, yrpt.length);
        date_print_year(&yrpt.array[0].d);
        printf("\t" LABEL_FORMAT "\t", "first min. Tx");
        if (p == nullptr) {
          printf("***\n");
        } else {
          date_print_full(&p->d);
          printf("\tTx = " MSR_FORMAT "\n", p->tx);
        }
      }
    }
  }
  return EXIT_SUCCESS;
}

//--- Développements -----------------------------------------------------------

void date_print_full(const date *p) {
  printf("%.2d-%.2d-%.4d", p->mday, p->month, p->gyear);
}

void date_print_year(const date *p) {
  printf("%.4d", p->gyear);
}

int yearlyreport_fload(const char *filename, yearlyreport *p) {
  FILE *f = fopen(filename, "rb");
  if (f == nullptr) {
    return -1;
  }
  int r = 0;
  size_t n = fread(p->array, sizeof(report), NDAYS_PER_YEAR_MAX, f);
  if (n < NDAYS_PER_YEAR_MIN
      || (n == NDAYS_PER_YEAR_MAX && fgetc(f) != EOF)
      || !feof(f)) {
    r = -1;
  }
  if (fclose(f) != 0) {
    r = -1;
  }
  if (r == 0) {
    p->length = n;
  }
  return r;
}

report *report_mintx_left_search(const report *base, size_t nmemb) {
  if (nmemb == 0) {
    return nullptr;
  }
  size_t j = 0;
  //  IB : 1 <= k && k <= nmemb
  //    && j == (indice de la première occurrence d'un composant du tableau base
  //              dont l'indice est strictement inférieur à k et dont le champ
  //              tx est minimum)
  //  QC : k
  for (size_t k = 1; k < nmemb; ++k) {
    if (base[j].tx > base[k].tx) {
      j = k;
    }
  }
  return (report *) &base[j];
}
