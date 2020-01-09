#ifndef _VERIFICATION_H_

#define _VERIFICATION_H_


#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#define HEADER_NB_HALF 8
#define HEADER_NB_WORD 2
#define HEADER_NB_ADDR 1
#define HEADER_NB_OFF  2

#define SECT_HEADER_NB_WORD 8
#define SECT_HEADER_NB_ADDR 1
#define SECT_HEADER_NB_OFF  1

#define SYMB_NB_WORD  2
#define SYMB_NB_ADDR  1
#define SYMB_NB_HALF  1
#define SYMB_NB_CHAR  2

/*
Verifie si le header est correct
Retour:         0 si ca s'est bien passe, 1 sinon
Arguments:      Elf32_Ehdr header:  le header a verifier
*/
int verif_fread_header(Elf32_Ehdr header);


/*
Verifie si le header de la section est correct
Retour:         0 si ca s'est bien passe, 1 sinon
Arguments:      Elf32_Shdr sectionheader:  le header de section a verifier
*/
int verif_fread_sectionheader(Elf32_Shdr sectionheader);

/*
Verifie si le hsymbole est correct
Retour:         0 si ca s'est bien passe, 1 sinon
Arguments:      Elf32_Sym symbole:  le symbole a verifier
*/
int verif_fread_symbole(Elf32_Sym symbole);
#endif