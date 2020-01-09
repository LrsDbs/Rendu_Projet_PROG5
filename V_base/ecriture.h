#ifndef _ECRITURE_H_

#define _ECRITURE_H_

#include "type_liste_section.h"
#include "table_string.h"
#include "gestion_endian.h"
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

/*                  Partie 6
Ecrit le header dans le fichier resultat
Arguments:      FILE* res: Fichier ouvert resultat (en ecriture)
                Elf32_Ehdr* new_header: header du nouveau fichier
*/
void write_new_header(FILE* res, Elf32_Ehdr new_header);

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
int put_sect(FILE* res, char* source, int offset, Elf32_Shdr sectheader[], Elf32_Ehdr header_s1, Elf32_Ehdr* header, int v, table_string* table, liste_elf32_Shdr* liste_section);

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
void cont_sect_e(FILE* res, char* s1, char* s2, int v1, int v2, int offset, Elf32_Shdr sectheader1[], Elf32_Shdr sectheader2[], Elf32_Ehdr* header, Elf32_Shdr sect_header_res);

/*                  Partie 6
ecrit la liste des entetes de section dans le fichier fusion
    FILE* fres: fichier resultat
    liste_elf32_Shdr liste_section: liste des entetes de section
    int offset: indice courant
*/
void write_section_table(FILE* fres,liste_elf32_Shdr liste_section, int offset);

/*                  Partie 6
ecrit dans le fichier fusion la table de string
    FILE* fres: fichier resutat
    table_string table: table de string
    int offset: offset courant
*/
int write_string_table(FILE* fres, table_string table, int offset);

#endif
