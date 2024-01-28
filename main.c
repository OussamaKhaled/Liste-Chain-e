#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct data
{
    int num;
    char type[15]; //locomotive | wagon
    char etat[10];
} data;
struct wagon
{
    data D;
    struct wagon *suiv;
};
typedef struct wagon *liste;
void creer_wagon(data *D)
{
    printf("Quel est le numero ? ");
    scanf("%d",&(*D).num);
    do
    {
        printf("Quel est le type ? (locomotive ou wagon) ");
        fflush(stdin);
        scanf("%s",D->type);
    }
    while (strcmp(D->type,"locomotive")!=0 && strcmp(D->type,"wagon")!=0);
    do
    {
        printf("Quel est l etat ? (panne ou OK) ");
        fflush(stdin);
        scanf("%s",D->etat);
    }
    while (strcmp(D->etat,"panne")!=0 && strcmp(D->etat,"OK")!=0);
}
void afficher_train(liste w)
{
    int i=1;
    liste tmp;
    if (w==NULL) printf("Train vide.");
    else
    {
        tmp=w;
        while (tmp!=NULL)
        {
            printf("%d | Numero : %d | Type : %s | Etat : %s\n",i,tmp->D.num,tmp->D.type,tmp->D.etat);
            tmp=tmp->suiv;
            i++;
        }
    }
}
liste ajout_debut(liste w,data valeur)
{
    liste nouv=NULL;
    nouv=(struct wagon*)malloc(sizeof(struct wagon));
    nouv->D=valeur;
    nouv->suiv=NULL;
    if (w==NULL) w=nouv;
    else
    {
        nouv->suiv=w;
        w=nouv;
    }
    return w;
}
liste ajout_fin(liste w,data valeur)
{
    liste tmp=NULL,nouv;
    nouv=(struct wagon*)malloc(sizeof(struct wagon));
    nouv->D=valeur;
    nouv->suiv=NULL;
    if (w==NULL) w=nouv;
    else
    {
        tmp=w;
        while(tmp->suiv!=NULL) tmp=tmp->suiv;
        tmp->suiv=nouv;
    }
    return w;
}
liste rechercher(liste w,int num)
{
    liste tmp=w;
    while(tmp!=NULL && tmp->D.num!=num) tmp=tmp->suiv;
    return tmp;
}
void supprimer_panne(liste w)
{
    liste tmp=w,prec;
    while(tmp!=NULL && strcmp(tmp->D.etat,"panne")!=0)
    {
        prec=tmp;
        tmp=tmp->suiv;
    }
    if (tmp==NULL) printf("Aucun wagon en panne.");
    else
    {
        prec->suiv=tmp->suiv;
        printf("Le wagon supprim%c est:\nNumero : %d | Type : %s | Etat : %s\n",130,tmp->D.num,tmp->D.type,tmp->D.etat);
        free(tmp);
    }
}

liste ajout_milieu(liste w,data A,int pos)
{
    int i=1;
    liste tmp=w,nouv,prec;
    if (pos==1) w=ajout_debut(w,A);
    else
    {
        while(tmp!=NULL && i<pos)
        {
            i++;
            prec=tmp;
            tmp=tmp->suiv;
        }
        if (i<pos) printf("Position invalide, il existe seulement %d wagons dans le train.",i-1);
        else
        {
            if (tmp==NULL) w=ajout_fin(w,A);
            else
            {
                nouv=(liste)malloc(sizeof(struct wagon));
                nouv->D=A;
                nouv->suiv=tmp;
                prec->suiv=nouv;
            }
        }
    }
    return w;
}
void enregistrer(liste l)
{
    liste tmp=l;
    data t;
    FILE *f;
    if (l==NULL) printf("Train vide.");
    else
    {
        f=fopen("Historique","wb");
        while(tmp!=NULL)
        {
            t=tmp->D;
            fwrite( &t, sizeof(struct wagon), 1, f);
            tmp=tmp->suiv;
        }
        fclose(f);
        printf("Train enregistr%c avec succ%cs.",130,130);
    }
}

int main()
{
    data A;
    int choix,num,pos;
    liste w=NULL,x=NULL;
    //w=(struct wagon*)malloc(sizeof(struct wagon));
    strcpy(A.etat,"OK");  //creation de locomotive préalablement avant l'ajout
    A.num=0; //creation de locomotive(le choix de 0 comme numero est arbitraire)
    strcpy(A.type,"locomotive");
    //w->suiv=NULL;
    //w->D=A;
    //w=ajout_debut(w,A); //ajout de locomotive
    do
    {
        printf("\n----------Menu----------");
        printf("\nChoisir une des options suivantes:");
        printf("\n1:Ajouter un wagon.");
        printf("\n2:Rechercher un wagon par son numero et l afficher.");
        printf("\n3:Afficher le train.");
        printf("\n4:Supprimer le premier wagon en panne.");
        printf("\n5:Ajouter un wagon a une position choisie.");
        printf("\n6:Enregistrer le train dans un fichier binaire.");
        printf("\n0:Quitter le menu.");
        printf("\n------------------------");
        scanf("%d",&choix);
        switch(choix)
        {
        case 1:
            creer_wagon(&A);
            w=ajout_fin(w,A);
            break;
        case 2:
            printf("Entrer le numero du wagon a rechercher :");
            scanf("%d",&num);
            x=rechercher(w,num);
            if (x==NULL) printf("Aucun wagon ayant ce numero.");
            else printf("Numero : %d | Type : %s | Etat : %s\n",x->D.num,x->D.type,x->D.etat);
            break;
        case 3:
            afficher_train(w);
            break;
        case 4:
            /*printf("Entrer le numero du wagon a supprimer :");
            scanf("%d",&num);
            x=rechercher(w,num);
            if (x==NULL) printf("Aucun wagon ayant ce numero.");*/
            supprimer_panne(w);
            break;
        case 5:
            printf("Entrer la position a laquelle vous souhaitez ajouter un wagon.");
            scanf("%d",&pos);
            creer_wagon(&A);
            w=ajout_milieu(w,A,pos);
            break;
        case 6:
            enregistrer(w);
            break;
        }
    }
    while (choix!=0);
    return 0;
}
