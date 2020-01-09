#include "ecriture.h"



/*                  Partie 6
Ecrit le header dans le fichier resultat
Arguments:      FILE* res: Fichier ouvert resultat (en ecriture)
                Elf32_Ehdr* new_header: header du nouveau fichier
*/
void write_new_header(FILE* res, Elf32_Ehdr new_header){
    int resval;
    resval=fseek(res, 0, SEEK_SET);
    swap_header(&new_header);
    fwrite(&new_header, sizeof(new_header), 1, res);
    resval++;
}



/*                  Partie 6
Mets la section dans le fichier resultat
Retour:         Retourne l'offset courant
Arguments:      FILE* res: Fichier ouvert resultat (en ecriture)
                char* source: nom du fichier source
                int offset: indice courant
                Elf32_Shdr sectheader[]: table des sections de source
                Elf32_Ehdr* header: header du nouveau fichier
                int v: L'indice de la section source a ajouter au fichier
                table_string* table: table de string
                liste_elf32_Shdr* liste_section: liste des entetes de sections
*/
int put_sect(FILE* res, char* source, int offset, Elf32_Shdr sectheader[], Elf32_Ehdr header_s1, Elf32_Ehdr* header, int v, table_string* table, liste_elf32_Shdr* liste_section){
    int resval;

    FILE* fs = fopen(source, "rb");

    //On se place pour la lecture
    resval=fseek(res, header->e_shoff + header->e_shnum * sizeof(sectheader[0]), SEEK_SET);
    resval=fseek(fs, sectheader[v].sh_offset, SEEK_SET);

    //Preparation du nouveau entete de section
    Elf32_Shdr sect_header_res = sectheader[v];

    //On ajoute une nouvelle section dans le header
    header->e_shnum++;

    if (sectheader[v].sh_size != 0){
        //change_indstringtable(source ,header,sectheader, v);
        sect_header_res.sh_offset = offset;
    }
    sect_header_res.sh_name = table->val;
    write_table(table,sectheader[v], header_s1, source);
    ajouter_S(liste_section, sect_header_res);
    //Mise a jour de l emplacement courant
    offset = offset + sect_header_res.sh_size;


    //On se place sur le contenu de la section et on l ecrit
    resval=fseek(res, sect_header_res.sh_offset, SEEK_SET);
    char c;
    for (int i = 0; i<sectheader[v].sh_size; i++){
        resval = fscanf(fs, "%c", &c);
        fprintf(res, "%c", c);
    }
    resval++;
    return offset;

}

/*                  Partie 6
ecrit la liste des entetes de section dans le fichier fusion
    FILE* fres: fichier resultat
    liste_elf32_Shdr liste_section: liste des entetes de section
    int offset: indice courant
*/
void write_section_table(FILE* fres, liste_elf32_Shdr liste_section, int offset){
    Elf32_Shdr courant;

    //on se place a la fin du fichier
    fseek(fres, offset, SEEK_SET);

    //pour chaque entete de section dans la liste
    //on l'ecrit dans le fichier fusion
    for (int i = 0; i<liste_section.n;i++){
        get_symbol_S(liste_section, i, &courant);
        swap_header_section(&courant);
        fwrite(&courant, sizeof(courant), 1, fres);
    }
}

/*                  Partie 6
ecrit dans le fichier fusion la table de string
    FILE* fres: fichier resutat
    table_string table: table de string
    int offset: offset courant
*/
int write_string_table(FILE* fres, table_string table, int offset){
    int resval;

    //on se place a la fin du fichier
    resval = fseek(fres, offset, SEEK_SET);
    //on recopie la table de string dans le fichier
    for (int i = 0; i<table.val; i++){
        fprintf(fres, "%c", table.string[i]);
        offset++;
    }
    return offset;
}
/*                  Partie 6
Fusionne 2 sections
Retour:         Retourne l'offset courant
Arguments:      FILE* res: Fichier ouvert resultat (en ecriture)
                char* s1: nom du fichier source 1
                char* s2: nom du fichier source 2
                int v1: L'indice de la section source 1 a fusionner
                int v2: L'indice de la section source 2 a fusionner
                int offset: indice courant
                Elf32_Shdr sectheader1[]: table des sections de source 1
                Elf32_Shdr sectheader2[]: table des sections de source 2
                Elf32_Ehdr header_s1 : header du premier fichier a fusioner
                Elf32_Ehdr* header: header du nouveau fichier
*/
void cont_sect_e(FILE* res, char* s1, char* s2, int v1, int v2, int offset, Elf32_Shdr sectheader1[], Elf32_Shdr sectheader2[], Elf32_Ehdr* header, Elf32_Shdr sect_header_res){
    // Ouverture des 2 fichiers sources en lecture
    FILE* fs1 = fopen(s1, "rb");
    FILE* fs2 = fopen(s2, "rb");
    int resval;

    //On se place pour la lecture
    resval=fseek(fs1, sectheader1[v1].sh_offset, SEEK_SET);
    resval=fseek(fs2, sectheader2[v2].sh_offset, SEEK_SET);

    //On se place sur le contenu de la section et on l ecrit
    resval=fseek(res, sect_header_res.sh_offset, SEEK_SET);
    char c;
    for (int i = 0; i<sectheader1[v1].sh_size; i++){
        resval = fscanf(fs1, "%c", &c);
        fprintf(res, "%c", c);
    }
    for (int i = 0; i<sectheader2[v2].sh_size; i++){
        resval = fscanf(fs1, "%c", &c);
        fprintf(res, "%c", c);
    }
    //parce que compilateurwrite
    resval++;
}
