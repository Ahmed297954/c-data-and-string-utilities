/*
 * Author: Ahmed BOUAZZA
 * Description: Multi-criteria CLI query engine using tagged unions (struct + enum + union) to filter Tintin characters by year or name substring.
 */

// Utilisation :
// tintin VALUE...
//
// Pour chaque VALUE figurant en paramètre sur la ligne de commande, affiche
// les personnages d’albums de Tintin figurant dans une très petite base de
// données qui satisfont à une requête associée à VALUE.
// Selon que VALUE correspond à l’écriture d’un entier en base 10 et, dans ce
// cas, que cet entier est codable sur le type long int ou que VALUE est une
// chaine de caractères, affiche tous les personnages qui, dans le premier
// cas, ont fait leur première apparition dans un album paru l’année égale à
// l’entier ou dont, dans le deuxième, le nom ou les prénom(s) contiennent la
// chaine.
// Lorsque VALUE est la chaine vide, affiche tous les personnages figurant dans
// la base de données.
//
// Renvoie EXIT_FAILURE à l’environnement d’exécution si le nombre d’arguments
// qui figurent sur la ligne de commande est inférieur ou égal à 1 ou si
// l’une des VALUEs correspond à l’écriture d’un entier en base 10 mais que
// cet entier n’est pas codable sur le type « long int ».
// Renvoie EXIT_SUCCESS dans tous les autres cas.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
//  character : alias d'une structure anonyme permettant de mémoriser le nom et
//    le(s) prénom(s) d'un personnage des albums de Tintin ainsi que l'année de
//    publication de l'album dans lequel il apparait pour la première fois.

#define NAME_LENGTH_MAX 63

typedef struct {
  char lastname[NAME_LENGTH_MAX + 1];   //  nom
  char firstname[NAME_LENGTH_MAX + 1];  //  prénom(s)
  int yearfirstapp;                     //  année de première apparition
} character;

// parameter : alias d’une structure anonyme permettant de mémoriser soit un
// entier de type « long int », champ number, soit un pointeur vers une
// chaine de caractères, champ string. Le champ category mémorise le type de
// valeur mémorisée, NUMBER pour le premier cas, STRING pour le deuxième.
typedef struct {
  enum {
  NUMBER, STRING
} category;
  union {
  long int number;
  const char *string;
};
} parameter;

// parameter_initialize : initialise l’objet pointé par p à partir de la chaine
// de caractères pointée par s. Si la chaine pointée par s correspond à
// l’écriture en base 10 d’un entier mais que cet entier ne peut être codé
// sur le type long int, la fonction ne modifie par l’objet pointé par p et
// renvoie un pointeur vers une chaine de caractères signifiant une erreur.
// La fonction renvoie sinon un pointeur nul.
const char *parameter_initialize(parameter *p, const char *s);
// parameter_execute : affiche sur la sortie standard tous les personnages
// d’une liste qui valident un test associé à l’objet pointé par p. La liste
// consiste en la suite des n premiers composants d’un tableau dont q est
// l’adresse du premier composant. Si l’objet pointé par p correspond à un
// entier, le test est validé lorsque l’année de première apparition du
// personnage est égale à cet entier. Sinon, l’objet pointé par p correspond
// à une chaine de caractères et le test est validé lorsque le nom ou le(s)
// prénom(s) du personnage contiennent cette chaine. L’affichage de tout
// personnage est réalisé par un appel à la fonction character_display_names.
// L’affichage débute par l’envoi sur la sortie standard d’une ligne de texte
// rappelant la teneur du test.
void parameter_execute(parameter *p, const character *q, size_t n);

//  character_display_names : affiche sur la sortie standard les champs
//    firstname et lastname, dans cet ordre, de l'objet de type « character »
//    pointé par q. Les champs affichés sont séparés d'une espace. Le dernier
//    champ affiché est immédiatement suivi d'une fin de ligne.
void character_display_names(const character *q);

int main (int argc, char *argv[]){
    if(argc<2){ // on veut le nom du programmme et au moins un argument
        fprintf(stderr,"%s: At least one parameter is expected\n",argv[0]);
        fprintf(stderr,"Syntax: %s VALUE...\n",argv[0]);
        return EXIT_FAILURE ;
    }

    character tab []={
        {"Ben Kalish Ezab", "Abdallah", 1950},
        {"Ben Salaad", "Omar", 1941},
        {"Alcazar", "Peggy", 1976},
        {"Bolivar", "Hippolyte", 1932},
        {"Haddock", "Archibald", 1941},
        {"Bada", "Ramon", 1937},
        {"Wagner", "Igor", 1939},
        {"Charlet", "Marc", 1948},
        {"Perez", "Alonzo", 1937},
        {"Bazaroff", "Basil", 1937},
        {"Ben Moulfrid", "Youssouf", 1950},
        {"Tortilla", "Rodrigo", 1937},
        {"Cartoffoli", "Arturo Benedetto Giovanni Giuseppe Pietro Archangelo Alfredo", 1956},
        {"Boullu", "Isidore", 1963},
        {"Capone", "Al", 1932},
        {"Sakharine", "Ivan Ivanovitch", 1943},
        {"Topolino", "Alfredo", 1956},
        {"Tournesol", "Tryphon", 1945},
        {"Thompson", "Allan", 1934},
        {"Lampion", "Seraphin", 1956},
        {"Rastapopoulos", "Roberto", 1934},
        {"Castafiore", "Blanca", 1939},
        {"Zarate", "Ramon", 1948},
        {"Bergamotte", "Hippolyte", 1948},
        {"Szprinkoth", "Stephan", 1956},
        {"Ben Mahmoud", "Ali", 1950}
    };
    parameter tab1 [argc];
    for(int k=1; k<argc; ++k){
        const char *message_erreur = parameter_initialize(&tab1[k],argv[k]);
        if(message_erreur != NULL){
            fprintf(stderr,"%s : %s '%s'\n",argv[0],message_erreur,argv[k]);
            return EXIT_FAILURE;
        }
    }
    size_t n = sizeof(tab)/sizeof(tab[0]);
    for(size_t k=1; k<argc; ++k){
        parameter_execute(&tab1[k],tab,n);
     }
    return EXIT_SUCCESS;
}


void character_display_names(const character *q){
    printf("%s %s\n",q->firstname,q->lastname);
  }
const char *parameter_initialize(parameter *p, const char *s){
    char *end;
    errno =0;
    long int valeur = strtol(s, &end, 10);
    if(errno != 0){
      return "Value out of range";
    }
    if(*end!='\0'|| end==s){
        p->category = STRING;
        p->string = s;
    }else{
        p->category = NUMBER;
        p->number = valeur;
      }
      return NULL;
}
void parameter_execute(parameter *p, const character *q, size_t n){
   if(p->category == NUMBER ){
      int flag =0;
      for( size_t k=0; k<n;++k){
      //IB: 0<=k && k<=n  , ts les noms et prenoms de 0 jusqu'a k  qui correspondent à la date d'apparution sont affichés
      //QC: k
        if(p->number == q[k].yearfirstapp){
           if(flag==0){
             printf("--- characters appearing for the first time in %ld \n",p->number);
             flag=1;
           }
         character_display_names(&q[k]);
        }
      }
   }
   if(p->category == STRING ){
   int flag =0;
      for(size_t k=0; k<n; ++k){
      //IB: 0<=k && k<=n  , ts les noms ou prenoms de 0 jusqu'a k  qui contiennent la chaine de caractere lit dans argv[1] sont affichées
      //QC: k
         if(strstr(q[k].firstname,p->string) != NULL  || strstr(q[k].lastname,p->string) != NULL){
            if(flag==0){
                printf("--- characters whose first or last names contain %s \n",p->string);
                flag=1;
            }
            character_display_names(&q[k]);
         }
      }
   }
}


