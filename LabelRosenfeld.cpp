#include "LabelRosenfeld.hpp"


/* Constructeur par defaut */
LabelRosenfeld::LabelRosenfeld() {
}

LabelRosenfeld::~LabelRosenfeld() {
}

/* Fonctions communes */
uint32_t LabelRosenfeld::FindRoot(uint32_t* T, uint32_t i) {
    uint32_t r;
    r = i;
    while (T[r]<r) {
        r = T[r];
    }
    return r;
}
void LabelRosenfeld::SetRoot(uint32_t* T, uint32_t i, uint32_t r) {
    uint32_t j;

    while (T[i]<i) {
        j = T[i];
        i = j;
    }
    T[i] = r;
}
void LabelRosenfeld::updateTable(uint32_t* T, uint32_t e, uint32_t epsillon) {
    uint32_t m;

    if ((!e) || (e == epsillon)) return;

    while (T[e] != epsillon) {
        m = T[e];
        T[e] = epsillon;
        e = m;
    }
}

uint32_t LabelRosenfeld::ui32MinNonNul2(uint32_t a, uint32_t b) {
    uint32_t m;

    if (a) {
        if (b) {
            if (a<b) m = a;
            else m =b;
        } else {
            m = a;
        }
    } else {
        m = b;
    }
    return m;
}
uint32_t LabelRosenfeld::ui32MinNonNul3(uint32_t a, uint32_t b, uint32_t c) {
    uint32_t m32 = 0xffffffff;
    uint32_t m = m32;

    if (a) m = a;
    if (b && (b<m)) m = b;
    if (c && (c<m)) m = c;

    if (m==m32) m = 0;
    return m;
}
uint32_t LabelRosenfeld::ui32MinNonNul4(uint32_t a0, uint32_t b0, uint32_t a1, uint32_t b1) {
    uint32_t m, m0,m1;

    m0 = ui32MinNonNul2(a0, b0);
    m1 = ui32MinNonNul2(a1, b1);
    m  = ui32MinNonNul2(m0, m1);

    return m;
}

/* Autres fonctions utiles */
uint32_t LabelRosenfeld::solvePackTable(uint32_t* T, uint32_t ne) {
    uint32_t e;
    uint32_t na; // ancetre packe

    na = 0;
    for (e=1; e<=ne; e++) {
        if (e != T[e]) {
            T[e] = T[T[e]];
        } else {
            T[e] = ++na;
        }
    }
       return na;
}
void LabelRosenfeld::updateLabel(uint32_t **E, int i0, int i1, int j0, int j1, uint32_t* T) {
    int i, j;

    for (i=i0; i<i1; i++) {
        for (j=j0; j<j1; j++) {
            E[i][j] = T[E[i][j]];
        }
    }
}

uint32_t LabelRosenfeld::line0Labeling4C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t e4, r4;

    // Premier point
    x = X[i][0];
    if (x) {
        E[i][0] = ++ne;
    } else {
        E[i][0] = 0;
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e4 = E[i  ][j-1];

            if (e4 == 0) {
                E[i][j] = ++ne;
            } else {
                r4 = FindRoot(T, e4);
                E[i][j] = r4;
            }
        }
    }
    return ne;
}
uint32_t LabelRosenfeld::lineLabeling4C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t epsillon;
    uint32_t e2, e4;
    uint32_t r2, r4;

    // premier point de la ligne
    x = X[i][0];

    if (x) {
        e2 = E[i-1][0];
        if (e2) {
            E[i][0] = e2;
        } else {
            E[i][0] = ++ne;
        }
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e2 = E[i-1][j  ];
            e4 = E[i  ][j-1];

            if ( (e2 == 0) && (e4 == 0) ) {
                E[i][j] = ++ne;
            } else {
                if (e2 == e4) {
                    E[i][j] = e2;
                } else {
                    r2 = FindRoot(T, e2);
                    r4 = FindRoot(T, e4);
                    epsillon = ui32MinNonNul2(r2, r4);

                    if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                    if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);
                    E[i][j] = epsillon;
                }
            }
        }
    }
    return ne;
}

uint32_t LabelRosenfeld::line0Labeling8C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t e4,e;

    // Premier point
    x = X[i][0];
    if (x) {
        E[i][0] = ++ne;
    } else {
        E[i][0] = 0;
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e4 = E[i][j-1];
            if (e4) {
                e = FindRoot(T,e4);
            } else {
                e = ++ne;
            }
            E[i][j] = e;
        }
    }
    return ne;
}
uint32_t LabelRosenfeld::lineLabeling8C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t epsillon;
    uint32_t e1, e2, e3, e4;
    uint32_t r1, r2, r3, r4;

    // premier point de la ligne
    x = X[i][0];
    if (x) {
        e2 = E[i-1][0];
        e3 = E[i-1][1];

        if ( (e2==0) && (e3==0)) {
            E[i][0] = ++ne;
        } else {
            if (e2==e3) {
                E[i][0] = e2;
            } else {
                r2 = FindRoot(T,e2);
                r3 = FindRoot(T,e3);

                epsillon = ui32MinNonNul2(r2, r3);

                if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);

                E[i][0] = epsillon;
            }
        }
    }

    for (j=1; j<largeur-1; j++) {
        x = X[i][j];
        if (x)  {
            e1 = E[i-1][j-1];
            e2 = E[i-1][j  ];
            e3 = E[i-1][j+1];
            e4 = E[i  ][j-1];

            // nouvel element
            if ( (e1==0) && (e2==0) && (e3==0) && (e4==0)) {
                E[i][j] = ++ne;
            } else {
                if ((e1 == e2) && (e1 == e3) && (e1 == e4)) {
                    E[i][j] = e1;
                } else {

                    r1 = FindRoot(T,e1);
                    r2 = FindRoot(T,e2);
                    r3 = FindRoot(T,e3);
                    r4 = FindRoot(T,e4);

                    epsillon = ui32MinNonNul4(r1, r2, r3, r4);

                    if ((e1) && (e1 != epsillon)) SetRoot(T, e1, epsillon);
                    if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                    if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);
                    if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);

                    E[i][j] = epsillon;
                }
            }
        }
    }

    j = largeur-1;
    x = X[i][j];
    if (x)  {
        e1 = E[i-1][j-1];
        e2 = E[i-1][j  ];
        e3 = 0;
        e4 = E[i  ][j-1];

        // nouvel element
        if ( (e1==0) && (e2==0) && (e3==0) && (e4==0)) {
            E[i][j] = ++ne;

        } else {
            if ((e1 == e2) && (e1 == e3) && (e1 == e4)) {
                E[i][j] = e1;
            } else {

                r1 = FindRoot(T,e1);
                r2 = FindRoot(T,e2);
                r3 = FindRoot(T,e3);
                r4 = FindRoot(T,e4);

                epsillon = ui32MinNonNul4(r1, r2, r3, r4);

                if ((e1) && (e1 != epsillon)) SetRoot(T, e1, epsillon);
                if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);
                if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);

                E[i][j] = epsillon;
            }
        }
    }
    return ne;
}



/* --------------------------------- */
/* --------My Code Below------------ */
/* --------------------------------- */

void LabelRosenfeld::labeliseSequetiel4C(Region32& region32) {
    
    /* Declaration des variables */
    int i;
    uint32_t ne;

    int i0 			= 	region32.i0;
    int i1 			= 	region32.i1;
    int j0 			= 	region32.j0;
    int j1 			= 	region32.j1;
    int largeur 	= 	j1-j0;

    /* Netoyage des précédents traitements */
    region32.cleanRegions32();

    /* Premier etiquetage */
    ne = 0;
    ne = line0Labeling4C(region32.X, i0, region32.E, region32.T, largeur, ne);
    for (i=i0+1; i<i1; i++) {
        ne = lineLabeling4C(region32.X, i, region32.E, region32.T, largeur, ne);
    }

    /* Résolution des équivalences */
    region32.neFinal = solvePackTable(region32.T, ne);

    /* Mise à jour sur l'image */
    updateLabel(region32.E, i0, i1, j0, j1, region32.T);

    /* Mémorisation du nombre d'étiquettes */
    region32.ne = ne;
}
void LabelRosenfeld::labeliseSequetiel8C(Region32& region32) {

    /* Declaration des variables */
    int i;
    uint32_t ne;

    int i0 			= 	region32.i0;
    int i1 			= 	region32.i1;
    int j0 			= 	region32.j0;
    int j1 			= 	region32.j1;
    int largeur 	= 	j1-j0;

    /* Netoyage des précédents traitements */
    region32.cleanRegions32();

    /* Premier etiquetage */
    ne = 0;

    ne = line0Labeling8C(region32.X, i0, region32.E, region32.T, largeur, ne);
    for (i=i0+1; i<i1; i++) {
        ne = lineLabeling8C(region32.X, i, region32.E, region32.T, largeur, ne);
    }

    /* Résolution des équivalences */
    region32.neFinal = solvePackTable(region32.T, ne);

    /* Mise à jour sur l'image */
    updateLabel(region32.E, i0, i1, j0, j1, region32.T);

    /* Mémorisation du nombre d'étiquettes */
    region32.ne = ne;
}


/* Labelise en parallèle */
void LabelRosenfeld::labeliseParallele4C(Region32& region32) {
 
  
    int nthread = region32.np; // enregistrement du nombre de threads dŽfinis
    int largeur = region32.j1 - region32.j0;// calcul de la largeur
    
    int i0,i1,j0,j1,ne,ligne_sup,ligne_inf,e1,e2,a1,a2,E; // dŽclarations des variables utilisŽes

    /*
     DŽbut du traitement en parallle 
     Chaque thread possde sa propre copie de ne,i0,i1,j0,j1 : private (ne,i0,i1,j0,j1)
     */
#pragma omp parallel for private (ne,i0,i1,j0,j1)
    for (int t =0 ; t < nthread; t++)
    {
        // enregistrement des donnŽes de la rŽgions pour un accs plus aisŽ
        i0 = region32.Regions[t].i0;
        i1 = region32.Regions[t].i1;
        j0 = region32.Regions[t].j0;
        j1 = region32.Regions[t].j1;
        
        /* Netoyage des précédents traitements */
        region32.Regions[t].cleanRegions32();
        
        /* premier etiquetage de toutes les lignes de la sous rŽgion*/
        ne = 0;
        ne = line0Labeling4C(region32.Regions[t].X, i0, region32.Regions[t].E, region32.Regions[t].T, largeur, ne);
        
        for(int i = i0+1; i<i1;i++)
            ne = lineLabeling4C(region32.Regions[t].X, i, region32.Regions[t].E, region32.Regions[t].T, largeur, ne);
        
        region32.Regions[t].ne = ne; // enregistrement du nombre d'Žtiquette de la sous rŽgion
    }
    
    /*
     Fin du traitement en parallle
     */
    
    /*
      Cette fonction permet de re numŽroter les tables d'Žquivalences des sous rŽgions
     Et de reporter dans la table d'Žquivalence globale
     */

    refactor_tables(region32);
    
    /*
     Gestion de la frontire
     On traite deux lignes pour chaque frontire pour mettre ˆ jour la table d'Žquivalence :
     La dernire ligne de la sous rŽgion du dessus 
     La premire ligne de la sous rŽgion du dessous
    */
    
    for (int t = 1; t < nthread ; t++) // pour chacune des sous rŽgions ˆ partir de la deuxime : la premire n'ayant pas de frontire suppŽrieure avec une autre sous rŽgion
    {
        ligne_inf = region32.Regions[t].i0; // numŽro de la ligne de la sous rŽgion du DESSOUS
        ligne_sup = region32.Regions[t-1].i1-1; // numŽro de la ligne de la sous rŽgion du DESSUS
        
        for ( int k = 0 ; k < largeur; k++){ // pour chacun des cases de la largeur
            
            e1 = region32.Regions[t].E[ligne_inf][k]; // on rŽcupŽre l'etiquette de la rŽgion du DESSOUS dans le tableau des etiquettes de cette rŽgion
            e2 = region32.Regions[t-1].E[ligne_sup][k]; // on rŽcupŽre de mme l'etiquette de la rŽgion du DESSUS
            
            if(e1 != 0 && e2 != 0){ // si il y a une etiquette au dessus et en dessous = si il ne s'agit pas d'une forme isolŽe
                
                a1 = region32.Regions[t].T[e1]; // on rŽcupŽre l'anctre de l'Žtiquette e1 de la region du DESSOUS dans le tableau des Žquivalences de cette rŽgion
                a2 = region32.Regions[t-1].T[e2]; // de mme pour e2 et la srŽgion du DESSUS
                
                E = ui32MinNonNul2(a1,a2); // On dŽtermine l'anctre le plus petit
                
                if(a1 != E) // si l'ancetre a1 n'est pas le plus petit des 2
                    SetRoot(region32.T,a1,E); // on met ˆ jour la table globale des Žquivalences , la fonction Set Root permet de mettre ˆ jour un ancetre tout en gardant l'intŽgritŽ de la table
                /*
                 On utilise l'ancetre a1 et non pas e1 car e1 correspont au numŽro dans la sous rŽgion alors que le numŽro de l'ancetre a ŽtŽ mis ˆ jour gr‰ce ˆ la fonction refactor_tables pour correspondre au numŽro dans le tableau global
                 */
                 
                if(a2 != E) // si c'est a2 qui n'est pas egal ˆ E
                     SetRoot(region32.T,a2,E);
            }
        }
        
        
    }
    /*
     Cette fonction rŽsoud les Žquivalence dans le tableau T des Žquivalences gobales et met ˆ jour les tables d'Žquivalences des sous rŽgions
     */
    super_solvePack(region32);
    
    /*
    on met ˆ jour l'image
     */
    for (int i=0;i<nthread;i++)
        updateLabel(region32.Regions[i].E, region32.Regions[i].i0, region32.Regions[i].i1, region32.Regions[i].j0, region32.Regions[i].j1, region32.Regions[i].T);
    
    
   
 }

/*
    mme fonction que pour la connexite 4
 */

void LabelRosenfeld::labeliseParallele8C(Region32& region32) {
    
    int nthread = region32.np;
    int largeur = region32.j1 - region32.j0;
    
    int i0,i1,j0,j1,ne,ligne_sup,ligne_inf,e1,e2,a1,a2,E;
    
#pragma omp parallel for private (ne,i0,i1,j0,j1)
    for (int t =0 ; t < nthread; t++)
    {
        i0 = region32.Regions[t].i0;
        i1 = region32.Regions[t].i1;
        j0 = region32.Regions[t].j0;
        j1 = region32.Regions[t].j1;
        
        /* Netoyage des précédents traitements */
        region32.Regions[t].cleanRegions32();
        
        /* premier etiquetage */
        ne = 0;
        ne = line0Labeling8C(region32.Regions[t].X, i0, region32.Regions[t].E, region32.Regions[t].T, largeur, ne);
        
        for(int i = i0+1; i<i1;i++)
            ne = lineLabeling8C(region32.Regions[t].X, i, region32.Regions[t].E, region32.Regions[t].T, largeur, ne);
        
        region32.Regions[t].ne = ne;
    }
    
    
    refactor_tables(region32);
    
    for (int t = 1; t < nthread ; t++)
    {
        ligne_inf = region32.Regions[t].i0;
        ligne_sup = region32.Regions[t-1].i1-1;
        
        for ( int k = 0 ; k < largeur; k++){
            
            e1 = region32.Regions[t].E[ligne_inf][k];
            e2 = region32.Regions[t-1].E[ligne_sup][k];
            
            if(e1 != 0 && e2 != 0){
                
                a1 = region32.Regions[t].T[e1];
                a2 = region32.Regions[t-1].T[e2];
                
                E = ui32MinNonNul2(a1,a2);
                
                if(a1 != E)
                    SetRoot(region32.T,a1,E);
                if(a2 != E)
                    SetRoot(region32.T,a2,E);
            }
        }
        
        
    }
    
    super_solvePack(region32);
    
    for (int i=0;i<nthread;i++)
        updateLabel(region32.Regions[i].E, region32.Regions[i].i0, region32.Regions[i].i1, region32.Regions[i].j0, region32.Regions[i].j1, region32.Regions[i].T);
    
    


}
/*
 Cette fonction permet de re numŽroter les tables d'Žquivalences des sous rŽgions
 Et de reporter dans la table d'Žquivalence globale
 */
void LabelRosenfeld::refactor_tables(Region32& r32){
    int i,sne=0,j,b;
    
    
    r32.initialiseTables(r32.ne);
    
    for(i=0;i<r32.np;i++)//for each thread
    {
        for(j=1;j<=r32.Regions[i].ne;j++){//Pour chaque etiquette
            r32.Regions[i].T[j] += sne; // on dŽcale chaque etiquette de la somme cummulŽe des etiquettes des regions prŽcedentes : etiquette region 2 = etiquette region 2 + nb etiquette r1
            r32.T[j+sne] =r32.Regions[i].T[j]; // on enregistre l'antŽcŽdant dans le tab total ˆ son nouveau numŽro
            
        }
        sne += r32.Regions[i].ne; // on incrŽmente l'offset du nombre d'Žtiquette de la sous rŽgion que l'on vient de traiter
     }
    
    r32.ne = sne; // on sauvegarde le nombre total d'Žtiquette
}
/*
 Cette fonction rŽsoud les Žquivalence dans le tableau T des Žquivalences gobales et met ˆ jour les tables d'Žquivalences des sous rŽgions
 */
void LabelRosenfeld::super_solvePack(Region32& r32){
    r32.neFinal = solvePackTable(r32.T, r32.ne); // rŽsolution des Žquivalences
    
    int sne =0;
    // mise ˆ jour des tables Žquivalences locales avec la table T globale
    for(int i=0;i<r32.np;i++){//for each thread
        for(int j = 1; j <= r32.Regions[i].ne;j++)
            r32.Regions[i].T[j] = r32.T[sne+j]; 
        sne+=r32.Regions[i].ne;
        }
}
