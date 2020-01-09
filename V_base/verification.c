#include "verification.h"

/*
Verifie si le header est correct
Retour:         0 si ca s'est bien passe, 1 sinon
Arguments:      Elf32_Ehdr header:  le header a verifier
*/
int verif_fread_header(Elf32_Ehdr header){
    int s =0;
    s= EI_NIDENT;
    s+= HEADER_NB_HALF * sizeof(header.e_type) ;
    s+= HEADER_NB_WORD * sizeof(header.e_flags);
    s+= HEADER_NB_ADDR * sizeof(header.e_entry);
    s+=  HEADER_NB_OFF * sizeof(header.e_shoff);
    if (sizeof(header)== s){
        return 0;
    }

    return 1;
}

/*
Verifie si le header de la section est correct
Retour:         0 si ca s'est bien passe, 1 sinon
Arguments:      Elf32_Shdr sectionheader:  le header de section a verifier
*/
int verif_fread_sectionheader(Elf32_Shdr sectionheader){
    int s =0;
    s+= SECT_HEADER_NB_WORD * sizeof(sectionheader.sh_flags);
    s+= SECT_HEADER_NB_ADDR * sizeof(sectionheader.sh_addr);
    s+= SECT_HEADER_NB_OFF  * sizeof(sectionheader.sh_offset);
    if (sizeof(sectionheader)== s){
        return 0;
    }

    return 1;
}

/*
Verifie si le hsymbole est correct
Retour:         0 si ca s'est bien passe, 1 sinon
Arguments:      Elf32_Sym symbole:  le symbole a verifier
*/
int verif_fread_symbole(Elf32_Sym symbole){
    int s =0;
    s+= SYMB_NB_WORD * sizeof(symbole.st_name);
    s+= SYMB_NB_ADDR * sizeof(symbole.st_value);
    s+= SYMB_NB_HALF * sizeof(symbole.st_shndx);
    s+= SYMB_NB_CHAR * sizeof(symbole.st_info);
    if (sizeof(symbole)== s){
        return 0;
    }

    return 1;
}
