

#include "lecture.h"
//On suppose que chaque fichier lu est en big endian


FILE* ouverture_fich_lecture(char* name){
    FILE* f = fopen(name, "rb");
    if (f == NULL){
        fprintf(stderr, "mauvais nom de fichier : %s\n", name);
        exit(0);
    }
    return f;
}
/*
                Partie 1.1
Recupere le header du fichier elf
Retour :      Elf32_Ehdr : header fichier elf en big endian
Arguments:    char* name : nom du fichier a ouvrir
*/
Elf32_Ehdr load_header(char* name){
    FILE* f;
    Elf32_Ehdr header;
    f=fopen(name, "rb");
    if (f== NULL){
        printf("Erreur ouverture fichier\n");
        exit(0);
    }
    size_t res;
    res=fread(&header, 1, sizeof(header), f);
    fclose(f);
    if (res==0){}
    return header;
}

/*
            Partie 1.2
Recupere une section du fichier elf
Retour :        Elf32_Shdr : header d une section en big endian
Arguments:      char* name : nom du fichier a ouvrir
                int offset : le numero de bit a partir duquel charger le fichier
*/
Elf32_Shdr load_section(char* filename, int offset){
    FILE* file;
    file=fopen(filename, "rb");
    Elf32_Shdr sectionheader;
    if (file){
        int res;
        res=fseek(file, offset, SEEK_SET);
        res=fread(&sectionheader, 1, sizeof(sectionheader), file);
        res++;
    }
    fclose(file);
    return sectionheader;
}
/*
            Partie 1.2
Recupere la table de section
Retour: les sections de sectionheaders seront en big endian
Arguments:      char* filename: nom du fichier
                Elf32_Ehdr header: Header du fichier elf  en little endian
                Elf32_Shdr sectionheaders[]:  tableau de section vide
*/
void load_tablesection(char* filename, Elf32_Ehdr header, Elf32_Shdr sectionheaders[] ){
    int offset = header.e_shoff;
    int sectionsize = header.e_shentsize;
    for(int i=0; i<header.e_shnum;i++){
        Elf32_Shdr sectionheader = load_section(filename, offset);
        sectionheaders[i]=sectionheader;
        offset+=sectionsize;
    }
    return;
}


/*
Recupere une table de string
Retour :        char*: table de string
Arguments:      char* filename: nom du fichier
                int offset : le numero de bit a partir duquel se trouve la table de string
*/
char * load_stringtable(char *name_file, int offset){

    FILE* file = fopen(name_file,"rb");
    int res;
    Elf32_Shdr header;
    header  = load_section(name_file,offset);
    swap_header_section(&header);
    char * stringtable;
    stringtable = malloc(header.sh_size);
    res = fseek(file, header.sh_offset, SEEK_SET);
    res = fread(stringtable, 1, header.sh_size, file);
    res+=0;
    fclose(file);
    return stringtable;
}

/*
            Partie 1.4
Recupere un symbole
Retour :        Elf32_Sym : un Symbole en big endian
Arguments:      char* filename: nom du fichier elf
                int offset: le numero de bit a partir duquel se trouve le symbole
*/
Elf32_Sym load_symb(char* filename, int offset){
    Elf32_Sym symb;
    FILE* file;
    file=fopen(filename, "rb");
    if (file){
        int res;
        res=fseek(file, offset, SEEK_SET);
        res=fread(&symb, 1, sizeof(symb), file);
        res++;
    }
    fclose(file);
    return symb;
}

/*
            Partie 1.4
Recupere la table de symbole
Arguments:      char* file: nom du fichier elf
                Elf32_Ehdr header: header du fichier elf en little endian
                Elf32_Shdr section[]: tableau avec toutes les sections avec les sections en little endians
                liste_elf32_sym* list_symb: liste qui contiendra tous les symboles (qui seront en big endian): represente la table de symbole
*/
void load_tablesymbole(char* file,Elf32_Ehdr header, Elf32_Shdr section[], liste_elf32_sym* list_symb){
    int errore=-1;
    int offs;
    Elf32_Sym symbole;
    for (int i=0; i<header.e_shnum; i++){
        // SHT_SYMTAB ==2
        //On cherche la section qui contient les symboles
        if (section[i].sh_type==SHT_SYMTAB){
            offs = section[i].sh_offset;
            //Quand on l a trouve on charge chaque symbole, on l ajoute a la table de symbole et on prepare la lecture du prochain symbole
            while (offs <section[i].sh_offset+ section[i].sh_size){
                symbole=load_symb(file, offs);
                errore=ajouter(list_symb, symbole);
                offs = sizeof(symbole) + offs;
                if(errore){}
            }
        }
    }
}

/*
            Partie 1.4
Recupere l emplacement de la table de string des symboles
Retour:         le numero de bit a partir duquel se trouve la table de string
Arguments:      Elf32_Ehdr header: header du fichier elf en little endian
                Elf32_Shdr section[]: tableau avec toutes les sections avec chaque section en little endian
                char* stringtable: la table de string // venant du header \\
*/
int load_offset_symb(Elf32_Ehdr header, Elf32_Shdr section[], char* stringtable){
    char* name;
    for (int i=0; i<header.e_shnum; i++){
        //On regarde quel section correpond a la table de string pour les symboles
        if (section[i].sh_type==SHT_STRTAB){
            name = stringtable + section[i].sh_name;
            if (strcmp(name, ".strtab")==0){
                return ((i*header.e_shentsize)+header.e_shoff); // On renvoie son emplacement
            }
        }
    }
    return 0;
}

/*
Partie 1.5
Récupère toutes les sections contenant des réimplémentations et leur réimplémentations
retour:
arguments:
    char* filename le nom du fichier concerné
    elf32_section_reloc* relocationtable la table qui contiendra toutes les sections
*/
/*void load_relocation_table(char *filename, elf32_section_reloc* relocationtable){

    FILE* file = fopen(filename, "rb");
    Elf32_Rela currentrela;
    Elf32_Shdr currentheader;
    Elf32_Ehdr header = load_header(filename);
    Elf32_Shdr sectionheaders[header.e_shnum];

    creer_section(relocationtable, 5);
    load_tablesection(filename, header, sectionheaders);



    for(int i=0;i<header.e_shnum;i++){
        currentheader = sectionheaders[i];
        fseek(file, currentheader.sh_offset, SEEK_SET);

        if(currentheader.sh_type == SHT_RELA){

            int size = currentheader.sh_size / sizeof(Elf64_Rel);

            elf32_reloc_table table;
            creer_table(&table, size);
            table.header=currentheader;

            for(int j=0;j<size;j++){
                int placeholder = fread(&currentrela, 1, sizeof(Elf32_Rela), file);
                if(placeholder){}
                table.tabrela[j]=currentrela;
            }

            ajouter_section(relocationtable, table);
        }
    }
}*/
