#include "table_string.h"

/* creation de la table
Arguments:      Elf32_Ehdr header1: header du premier fichier a fusioner
                Elf32_Ehdr header2: header du deuxieme fichier a fusioner
                char* source1: nom du premier fichier a fusioner
                char* source2: nom du deuxieme fichier a fusioner
*/
table_string creat_table_string(Elf32_Ehdr header1, Elf32_Ehdr header2, char* source1, char* source2) {
    //On recupere l entete de la table de string du premier fichier
    Elf32_Shdr sect1 = load_section(source1, (header1.e_shstrndx*header1.e_shentsize)+header1.e_shoff);
    swap_header_section(&sect1);

    //On recupere l entete de la table de string du deuxieme fichier
    Elf32_Shdr sect2 = load_section(source2, (header2.e_shstrndx*header2.e_shentsize)+header2.e_shoff);
    swap_header_section(&sect2);

    //On initialise la table de string
    table_string table;
    init_table(&table, sect1.sh_size + sect2.sh_size);
    return table;
}

table_string creat_table_string_symb(Elf32_Word size1, Elf32_Word size2) {
    //On recupere l entete de la table de string du premier fichier
    //On initialise la table de string
    table_string table;
    init_table(&table, size1 + size2);
    return table;
}
/* initialisation de la table
Arguments:      table_string *table: table de string
                int size: taille de la table
*/
void init_table(table_string *table, int size){
    table->val = 0;
    table->string = malloc(sizeof(char) * size);
}


/* ecriture dans la table
Arguments:      table_string *table: table de string
                Elf32_Shdr section: entete de la section en modification
                Elf32_Ehdr header: header du fichier source
                char* source: fichier source
*/
void write_table(table_string* table, Elf32_Shdr section, Elf32_Ehdr header, char* source){
    //recupere table de string du fichier source
    int offset = ((header.e_shstrndx*header.e_shentsize)+header.e_shoff);
	char * stringtable = load_stringtable(source,offset);
    //Recupere le nom de l entete de la section
    stringtable = stringtable + section.sh_name;

    //On copie dans le nom de l entete de section dans la nouvelle table de string
    int i = 0;
    while(stringtable[i] != '\0'){
        table->string[table->val] = stringtable[i];
        table->val++;
        i++;
    }
    table->string[table->val] = stringtable[i];
    table->val++;
}

void write_table_Symb(table_string* table, char* stringtable){
    //Recupere le nom de l entete de la section

    //On copie dans le nom de l entete de section dans la nouvelle table de string
    int i = 0;
    while(stringtable[i] != '\0'){
        table->string[table->val] = stringtable[i];
        table->val++;
        i++;
    }
    table->string[table->val] = stringtable[i];
    table->val++;
}
