/*
 * Author: Ahmed BOUAZZA
 * Description: Converts meteorological TSV/CSV files into raw binary files (.bin) using fwrite and IEEE 754 NAN for missing values.
 */

//  Utilisation :
//    meteocsv_create_bin [FILE...]
//
//  Sans paramètre, rappelle la syntaxe.
//  Sinon, crée pour chacun des fichiers météorologiques (format CSV avec
//    tabulations) dont le nom FILE figure sur la ligne de commande le fichier
//    météorologique binaire associé.
//  Chaque FILE doit avoir pour suffixe la chaine désignée par la macroconstante
//    SUFF ci-après définie. Ce suffixe est remplacé par la chaine désignée par
//    la macroconstante REPL ci-après définie pour le nom des fichiers binaires.
//  Pour tout FILE pour lequel la production du fichier binaire peut être menée
//    à son terme, affiche FILE, une tabulation, un message de succès et une fin
//    de ligne. Pour tout FILE pour lequel SUFF n'est pas un suffixe, un message
//    d'échec mentionnant FILE est envoyé sur la sortie erreur. Pour tout
//    fichier binaire qui ne peut être créé puis produit dans son intégralité,
//    un message d'échec mentionnant son nom ou FILE est envoyé sur la sortie
//    erreur.
//
//  Renvoie EXIT_SUCCESS à l'environnement d'exécution.

#define SUFF ".csv"
#define REPL ".bin"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

//  create_bin : crée le fichier météorologique binaire de nom la chaine de
//    caractères pointée par destfn associé au fichier météorologique au format
//    CSV avec tabulations de nom la chaine de caractères pointée par srcfn.
//  En cas de succès, affecte à l'objet pointé par errptr un pointeur nul et
//    renvoie CREATE_BIN_SUCCESS (qui vaut zéro).
//  En cas d'échec, affecte à l'objet pointé par errptr l'adresse d'une chaine
//    de caractères signifiant la cause de l'échec et renvoie, selon que l'échec
//    est imputable au fichier srcfn ou destfn, CREATE_BIN_FAILURE_SRC ou
//    CREATE_BIN_FAILURE_DEST (qui ne valent pas zéro).
//  Les constantes entières CREATE_BIN_SUCCESS, CREATE_BIN_FAILURE_SRC et
//    CREATE_BIN_FAILURE_DEST sont définies ci-après.

enum {
  CREATE_BIN_SUCCESS,
  CREATE_BIN_FAILURE_SRC,
  CREATE_BIN_FAILURE_DEST,
};

int create_bin(const char *destfn, const char *srcfn, const char **errptr);

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
    /*
     *  Déclarer ici une zone mémoire suffisamment grande pour qu'il puisse y
     *    être copié à terme la chaine obtenue de argv[k] en remplaçant son
     *    suffixe SUFF par REPL, quels que soient SUFF et REPL.
     */
    /*
     *  Si SUFF est un suffixe de argv[k], appeler create_bin.
     */
    /*
     *  Selon les cas, afficher un message de succès ou d'erreur.
     */
    size_t len_arg = strlen(argv[k]);
    size_t len_suff = strlen(SUFF);
    size_t len_repl = strlen(REPL);
    size_t n = len_arg -len_suff+ len_repl+1;
    char tab[n];
    if (len_suff > len_arg || strcmp(SUFF, (argv[k] + len_arg - len_suff))!=0) {
      fprintf(stderr, "%s: Invalid suffix for file '%s'\n", argv[0], argv[k]);
    } else {
      memcpy(tab, argv[k], len_arg - len_suff);
      memcpy(tab + len_arg - len_suff, REPL, len_repl);
      tab[n-1]='\0';
    const char *errptr = NULL;
    int resultat = create_bin(tab,argv[k],&errptr);
    if(resultat == CREATE_BIN_SUCCESS){
        printf("%s Successfuly created \n ",argv[k]);
    }else if(resultat == CREATE_BIN_FAILURE_SRC){
        fprintf(stderr,"%s : %s \n ",argv[k],errptr);
    }else if(resultat == CREATE_BIN_FAILURE_DEST){
        fprintf(stderr,"%s : %s \n ",argv[k],errptr);
    }
    }
  }
  return EXIT_SUCCESS;
}

//--- Développements -----------------------------------------------------------

//  freport_csv_get : lit un relevé sur le flot texte lié au contrôleur pointé
//    par f. Lorsque l'ensoleillement est absent, la valeur NAN est affectée au
//    champ w du relevé.
//  En cas de succès, affecte le relevé à l'objet pointé par p et renvoie zéro.
//  En cas d'échec, renvoie une valeur non nulle.
static int freport_csv_get(FILE *f, report *p) {
  if (fscanf(f, "%d%d%d%f%f%f",
        &p->date.gyear,
        &p->date.month,
        &p->date.mday,
        &p->tx,
        &p->tn,
        &p->rr) != 6) {
    return -1;
  }
  int c = fgetc(f);
  if(c == '\n' || c == EOF){
    p->w = NAN;
  }else if(c == '\t'){
    if(fscanf(f,"%f",&p->w)!=1){
        return -1;
    }
   }else{
      return -1;
   }
  return 0;
}

/*
 *  Définir ici create_bin, laquelle fonction appelle freport_csv_get.
 */
int create_bin(const char *destfn, const char *srcfn, const char **errptr){
    FILE *src = fopen(srcfn,"r");
    if(src == NULL){
        *errptr= "Impossible d'ouvrir le fichier";
        return CREATE_BIN_FAILURE_SRC;
    }
    FILE *dst = fopen(destfn,"wb");
    if(dst == NULL){
        *errptr="Impossible de creer le fichier binaire en destination";
        fclose(src);
        return CREATE_BIN_FAILURE_DEST;
    }
    report buffer;

    while(freport_csv_get(src,&buffer)==0){
        size_t written = fwrite(&buffer,sizeof(report),1,dst);
        if(written != 1){
            *errptr = "Erreur pendant l ecriture sur le fichier binaire";
            fclose(src);
            fclose(dst);
            return CREATE_BIN_FAILURE_DEST;
        }
    }
    fclose(src);
    fclose(dst);
    *errptr = NULL;
    return CREATE_BIN_SUCCESS;

}
