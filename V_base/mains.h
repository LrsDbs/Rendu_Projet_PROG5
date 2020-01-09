#ifndef _MAINS_H_

#define _MAINS_H_

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "type_liste_symb.h"
#include "lecture.h"
#include "affichage.h"
#include "gestion_endian.h"
/*
			Partie 1
			Cas -A
Demande d affichage d un header du fichier elf
Arguments:		char* name_file: nom du fichier a ouvrir
*/
int main_load_header(char* name_file );

/*
			Partie 2
			Cas -B
Demande d affichage de la table de section
Arguments:		char* name_file: nom du fichier a ouvrir
*/
int main_read_section_table(char* name_file);

/*
			Partie 3
			Cas -C
Demande d affichage d un header de section
Arguments:		char* name_file: nom du fichier a ouvrir
 				char* val: soit - le numero de section
				 				- le nom de la section
retourne : -1 si le fichier n'a pas pu être ouvert
           -2 si le second argument est NULL
           -3 si la section n'existe pas
            0 sinon
*/
int main_section(char* name_file, char* val);


/*
			Partie 4
			Cas -D
Demande d affichage d un header de section
Arguments:		char* name_file: nom du fichier a ouvrir
*/
int main_table_symbole(char* name_file);

/*
Partie 1.5
récupèration et affichage des tables de réimplémentation
arguments:
	char* name_file nom du fichier concerné
*/
int main_relocation_table(char *name_file);

#endif
