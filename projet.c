/*****************
 *
 * ORTANCA Baris
 * L2S4
 *
 ****************/


// relations.c�: d�finit le point d'entr�e pour l'application .
//

typedef int bool;
#define false 0
#define true -1

#include "stdlib.h"
#include "memory.h"
#include "stdio.h"
#include "string.h"


bool compar(void *x, void *y) {
    return (x == y) ? true : false;
}

////////////////////////////////////////
// Exercice 1: Classement des Relations

typedef enum {
    FRERE = 2, COUSIN, PARENT, ONCLE, EPOUX, AMI, VIT, CONNAIT,
    CHEF, COLLEGUE, LOCATAIRE, TRAVAILLE, PROPRIETAIRE, SITUE, DECOUVERT
} rtype;

bool est_lien_parente(rtype id) {
    return (id >= FRERE && id <= EPOUX) ? true : false;
}

bool est_lien_professionel(rtype id) {
    return (id == CHEF || id == COLLEGUE) ? true : false;
}

bool est_lien_connaissance(rtype id) {
    return (id >= AMI && id <= CONNAIT) ? true : false;
}


char *toStringRelation(rtype id) {
    static char *tableau_relation[] = {
            "Frère ou soeur de ",
            "Cousin ou cousinne de ",
            "Pere ou mere de ",
            "Oncle ou tante de ",
            "Époux ou epouse de ",
            "Ami de ",
            "Vit avec ",
            "Connaît ",
            "Chef de ",
            "Collègue de ",
            "Locataire de ",
            "Travaille a ",
            "Propriétaire de ",
            "Situé a",
            "Découvert a",
    };

    if ((id - 2) > 14 || (id - 2) < 0)
        return "Relation inconnue";
    else
        return tableau_relation[id - 2];
}


////////////////////////////////////////
// Exercice 2: Liste de pointeurs

typedef struct s_node {
    void *val;  // pointeur vers objet quelconque
    struct s_node *suiv;
} *listeg;

listeg listegnouv() {
    return NULL;      // on renvoie une lst vide
}

listeg adjtete(listeg lst, void *x) {
    listeg new = (listeg) malloc(sizeof(struct s_node));
    new->val = x;
    if (lst == NULL) {
        return new;
    }
    new->suiv = lst;
    return new;     //on renvoie le nouvelle ellement qui devient la tete de liste
}


listeg adjqueue(listeg lst, void *x) {
    listeg tmp = lst;
    listeg new = listegnouv();
    new = adjtete(new, x);
    if (lst == NULL) {
        return new;
    }
    while (tmp->suiv != NULL) {
        tmp = tmp->suiv;
    }
    tmp->suiv = new;
    return lst;
}


listeg suptete(listeg lst) {
    if (lst == NULL) {
        return lst; // si la liste en vide alors on renvoie la liste vide 
    }
    listeg save = lst->suiv;
    free(lst->val);
    free(lst);
    return save;    // sinon on renvoie l'ellement suivant et on libere la tete
}

void *tete(listeg lst) {
    return (lst != NULL) ? lst->val : NULL; // revoie la valeur de tete
}

int longueur(listeg lst) {
    return (lst == NULL) ? 0 : 1 + longueur(
            lst->suiv); // si lst est vide alors ca renvoie 0, sinon il fait une somme avec une appel recursive
}

bool estvide(listeg lst) {
    return (lst == NULL) ? true : false; // si la liste lst==NULL alors elle est vide sinon elle n'est pas vide
}

void detruire(listeg lst) {
    while (lst != NULL)
        lst = suptete(lst);
}

listeg rech(listeg lst, void *x, int(*comp)(void *, void *)) {
    listeg newLst = listegnouv();
    listeg tmp = lst;
    while (tmp != NULL) {
        if (comp(tmp->val, x) == 0)
            newLst = tmp;

        tmp = tmp->suiv;
    }
    return newLst;
}

////////////////////////////////////////
// Exercice 3: Construction du graphe

#define LONG_NOM_MAX 64
typedef enum {
    PERSONNE = 1, OBJET, ADRESSE, VILLE
} etype;
typedef struct s_entite {
    char nom[LONG_NOM_MAX]; // le nom de l�entit� p.ex � Peugeot 106 �
    etype ident; // l�identifiant associ�, p.ex OBJET
} *Entite;
//3.1 les structures de donn�es
typedef struct s_sommet {
    listeg larcs;
    Entite x;
} *Sommet;

typedef struct s_arc {
    rtype t;
    Entite x;
} *Arc;

typedef struct s_relations {
    listeg lsommet;
} *Relations;

//3.2 les constructeurs
Entite creerEntite(char *s, etype e) {
    Entite new = (Entite) malloc(sizeof(struct s_entite));
    new->ident = e;
    strcpy(new->nom, s);
    return new;
}

Sommet nouvSommet(Entite e) {
    Sommet newS = (Sommet) malloc(sizeof(struct s_sommet));
    newS->x = e;
    newS->larcs = NULL;
    return newS;
}

Arc nouvArc(Entite e, rtype type) {
    Arc newA = (Arc) malloc(sizeof(struct s_arc));
    newA->x = e;
    newA->t = type;
    return newA;
}

void relationInit(Relations *g) {
    *g = (Relations) malloc(sizeof(struct s_relations));
    (*g)->lsommet = listegnouv();
}

void relationFree(Relations *g) {
    detruire((*g)->lsommet);
    free(*g);
    *g = NULL;
}

//3.3 les comparaisons
int compEntite(void *e, void *string) {
    Entite entite = (Entite) e;
    char *nom = (char *) string;
    return strcmp(entite->nom, nom);
}

int compSommet(void *s, void *string) {
    Sommet sommet = (Sommet) s;
    char *nom = (char *) string;
    return strcmp(sommet->x->nom, nom);
}

int compArc(void *a, void *string) {
    Arc arc = (Arc) a;
    char *nom = (char *) string;
    return strcmp(arc->x->nom, nom);
}


//3.4 ajout d'entites et de relations
void adjEntite(Relations g, char *nom, etype t) {
    Entite nouvEntiter = creerEntite(nom, t);
    Sommet sommet = nouvSommet(nouvEntiter);
    listeg lsommet = g->lsommet;
    while (lsommet != NULL)         // verification des sommets pour savoir si il y a deja un sommet qui existe
    {
        Sommet sommet2 = (Sommet) lsommet->val;
        if (strcmp(sommet2->x->nom, nom) == 0) {
            return;
        }
        lsommet = lsommet->suiv;
    }
    // si l'entiter n'existe pas au prealable alors il la cree et ajoute a un nouveau sommet
    g->lsommet = adjtete(g->lsommet, sommet);

}

// PRE CONDITION: id doit �tre coh�rent avec les types des sommets correspondants � x et y
//                p.ex si x est de type OBJET, id ne peut pas etre une relation de parente
// PRE CONDITION: strcmp(nom1,nom2)!=0
void adjRelation(Relations g, char *nom1, char *nom2, rtype id) {
    Sommet sommet1, sommet2;
    listeg lsommet = g->lsommet;

    while (lsommet != NULL) {
        Sommet sommet = (Sommet) lsommet->val;
        if (strcmp(sommet->x->nom, nom1) == 0) {
            sommet1 = sommet;
        }
        if (strcmp(sommet->x->nom, nom2) == 0) {
            sommet2 = sommet;
        }
        lsommet = lsommet->suiv;
    }
    Arc arc2 = nouvArc(sommet2->x, id);
    sommet1->larcs = adjqueue(sommet1->larcs, arc2);
    Arc arc1 = nouvArc(sommet1->x, id);
    sommet2->larcs = adjqueue(sommet2->larcs, arc1);

}

////////////////////////////////////////
// Exercice 4: Explorer les relations entre personnes

// 4.1 listes de relations
listeg en_relation(Relations g, char *x) {

    Sommet sommet = (Sommet) rech(g->lsommet, x, compSommet)->val;
    return sommet->larcs;
}

listeg chemin2(Relations g, char *x, char *y) {
    listeg result = listegnouv();
    listeg larcX = en_relation(g, x);
    listeg larcY = en_relation(g, y);


    while (larcX != NULL) {
        while (larcY != NULL) {
            Arc arcX = (Arc) larcX->val;
            Arc arcY = (Arc) larcY->val;

            if (compArc(arcX, arcY->x->nom) == 0 || compArc(arcY,arcX->x->nom) == 0)
                result = adjtete(result, arcY->x);
            larcY = larcY->suiv;
        }
        larcX = larcX->suiv;
    }

    return result;
}

// 4.2 verifier un lien de parente
// PRE CONDITION: strcmp(x,y)!=0
bool ont_lien_parente(Relations g, char *x, char *y) {
    listeg X = rech(g->lsommet, x, compSommet); // on recupere le sommet x
    Sommet sommetX = (Sommet) X->val;
    listeg larcX = sommetX->larcs;
    while (larcX != NULL) {
        Arc arcX = (Arc) larcX->val;
        if (compArc(arcX, y) == 0)
            return est_lien_parente(arcX->t);
        larcX = larcX->suiv;
    }
    return false;
}

// 4.3 tester connaissances
// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent(Relations g, char *x, char *y) {

    listeg X = rech(g->lsommet, x, compSommet); // on recupere le sommet x
    Sommet sommetX = (Sommet) X->val;
    listeg larcX = sommetX->larcs;
    listeg Y = rech(g->lsommet, y, compSommet); // on recupere le sommet x
    Sommet sommetY = (Sommet) Y->val;
    listeg larcY = sommetY->larcs;
    while (larcX != NULL) {
        Arc arcX = (Arc) larcX->val;
        if (compArc(arcX, y) == 0)
            return est_lien_parente(arcX->t) || est_lien_connaissance(arcX->t) || est_lien_professionel(arcX->t);
        larcX = larcX->suiv;
    }
    while (larcY != NULL) {
        Arc arcY = (Arc) larcY->val;
        if (compArc(arcY, y) == 0)
            return est_lien_parente(arcY->t) || est_lien_professionel(arcY->t) || est_lien_connaissance(arcY->t);
        larcY = larcY->suiv;
    }
    return false;
}

// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_proba(Relations g, char *x, char *y) {
    Sommet sX = (Sommet) rech(g->lsommet, x, compSommet);
    Sommet sY = (Sommet) rech(g->lsommet, y, compSommet);
    listeg larcX = en_relation(g, x);
    listeg larcY = en_relation(g, y);
    while (larcX != NULL) {
        while (larcY != NULL) {
            Arc arcX = (Arc) larcX->val;
            Arc arcY = (Arc) larcY->val;
            if (strcmp(arcX->x->nom, arcY->x->nom) == 0) {
                if (ont_lien_parente(g, arcY->x->nom, sX->x->nom) == 0 || ont_lien_parente(g, arcX->x->nom, sY->x->nom))
                    return true;
                else
                    return false;
            }

            larcY = larcY->suiv;
        }
        larcX = larcX->suiv;
    }

    return false;
}

// PRE CONDITION: les sommets correspondants � x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_peutetre(Relations g, char *x, char *y) {

    if (se_connaissent(g, x, y))
        return false;
    if (se_connaissent_proba(g, x, y))
        return true;

    listeg larcX = en_relation(g, x);
    listeg larcY = en_relation(g, y);

    while (larcX != NULL) {
        while (larcY != NULL) {
            Arc arcX = (Arc) larcX->val;
            Arc arcY = (Arc) larcY->val;

            if (strcmp(arcX->x->nom, arcY->x->nom) == 0)
                return true;

            larcY = larcY->suiv;
        }
        larcX = larcX->suiv;
    }
    return false;
}

////////////////////////////////////////
// Exercice 5: Affichages

void affichelg(listeg l, void(*aff)(void *)) {
    if (l == NULL)
        return;
    else {
        aff(l->val);
        affichelg(l->suiv, aff);
    }
}

void afficheEntite(void *x) {
    Entite entite = (Entite) x;
    printf("%s : ", entite->nom);
    if (entite->ident == 1)
        printf("PERSONNE");
    if (entite->ident == 2)
        printf("OBJET");
    if (entite->ident == 3)
        printf("ADRESSE");
    if (entite->ident == 4)
        printf("VILLE");
    printf("\n");
}

void afficheArc(void *x) {
    Arc arc = (Arc) x;
    printf("%s --> ", toStringRelation(arc->t));
    afficheEntite(arc->x);
}

////////////////////////////////////////
// Exercice 6: Parcours


void affiche_degre_relations(Relations r, char *x) {
    listeg X = rech(r->lsommet, x, compSommet);
    Sommet xS = (Sommet) X->val;
    listeg xLArc = xS->larcs;
    printf("%s\n--- 1 \n", x);
    while (xLArc != NULL) {
        printf("%s\n", ((Arc) xLArc->val)->x->nom);
        xLArc = xLArc->suiv;
    }

}


int main() {
    int i, j;
    Relations r;
    relationInit(&r);
    // ajouter les entites de l'exemple
    char *tabe[] = {"KARL", "LUDOVIC", "CELINE", "CHLOE", "GILDAS", "CEDRIC", "SEVERINE",
                    "PEUGEOT 106", "1, RUE DE LA RUE", "STRASBOURG"};
    for (i = 0; i < 7; i++) adjEntite(r, tabe[i], PERSONNE);
    adjEntite(r, tabe[7], OBJET);
    adjEntite(r, tabe[8], ADRESSE);
    adjEntite(r, tabe[9], VILLE);
    // ajouter les relations de l'exemple
    adjRelation(r, tabe[0], tabe[1], FRERE);
    adjRelation(r, tabe[0], tabe[2], AMI);
    adjRelation(r, tabe[0], tabe[3], CONNAIT);
    adjRelation(r, tabe[0], tabe[5], COUSIN);
    adjRelation(r, tabe[0], tabe[7], PROPRIETAIRE);
    adjRelation(r, tabe[0], tabe[8], PROPRIETAIRE);
    adjRelation(r, tabe[3], tabe[4], VIT);
    adjRelation(r, tabe[5], tabe[6], EPOUX);
    adjRelation(r, tabe[5], tabe[8], LOCATAIRE);
    adjRelation(r, tabe[7], tabe[8], DECOUVERT);
    adjRelation(r, tabe[8], tabe[9], SITUE);

    // explorer les relations
    printf("%s est en relation avec:\n", tabe[0]);
    affichelg(en_relation(r, tabe[0]), afficheArc);
    printf("\n");

    for (i = 0; i < 7; i++)
        for (j = i + 1; j < 10; j++) {
            printf("<%s> et <%s> ont les relations communes:\n", tabe[i], tabe[j]);
            affichelg(chemin2(r, tabe[i], tabe[j]), afficheEntite);
            printf("\n");
        }
    printf("\n\n");

    for (i = 0; i < 10; i++)
        for (j = i + 1; j < 10; j++) {
            printf("<%s> et <%s> ont lien de parente: %s\n",
                   tabe[i], tabe[j], ont_lien_parente(r, tabe[i], tabe[j]) ? "vrai" : "faux");
        }
    printf("\n");
    for (i = 0; i < 7; i++) {
        for (j = i + 1; j < 7; j++) {
            printf("<%s> et <%s> se connaissent: %s\n",
                   tabe[i], tabe[j], se_connaissent(r, tabe[i], tabe[j]) ? "vrai" : "faux");
            printf("<%s> et <%s> se connaissent tres probablement: %s\n",
                   tabe[i], tabe[j], se_connaissent_proba(r, tabe[i], tabe[j]) ? "vrai" : "faux");
            printf("<%s> et <%s> se connaissent peut etre: %s\n",
                   tabe[i], tabe[j], se_connaissent_peutetre(r, tabe[i], tabe[j]) ? "vrai" : "faux");
        }
        printf("\n");
    }

    affiche_degre_relations(r, tabe[3]);

    relationFree(&r);

    printf("\nPRESS RETURN\n");
    char buff[64];
    fscanf(stdin, "%s", buff);
    return 0;
}
