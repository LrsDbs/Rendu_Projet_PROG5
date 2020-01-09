#include "fusion.h"

int max(int i1, int i2){
    if (i1 > i2) {
        return i1;
    }
    return i2;
}

/*                  Partie 6
Indice de la section contenant le nom donnee
Retour:         Retourne l'index de la section name ou -1 si elle n'existe pas
Arguments:      char * name: nom a retrouve
                char * name_file: fichier elf ouvert au prealable
*/
int search_same_name(char * name, char * name_file){
    Elf32_Ehdr header = load_header(name_file);
    swap_header(&header);
      Elf32_Shdr elf_sections[header.e_shnum];
    load_tablesection(name_file,header,elf_sections);
    for (int k = 0; k<header.e_shnum;k++){
        swap_header_section(&(elf_sections[k]));
    }
    int offset = ((header.e_shstrndx*header.e_shentsize)+header.e_shoff);
      char * nameSec = load_stringtable(name_file,offset);

    int index, i = 0;
    int test =-1;
    char * nameTemp;

    // On regarde le nom de chaque section du fichier name_file
    //Si le nom est egal a name, on renvoie son indice 
    //Sinon on renvoie -1
    while(test!=0 && i<header.e_shnum){
        nameTemp = nameSec+elf_sections[i].sh_name;
        if(strcmp(name,nameTemp)==0){
            test=0;index=i;
        }
        i++;
        }
    if(test==0){
        return index;
    }
    else{
        return -1;
    }
}

int is_define_in_table(char * name_symb,Elf32_Half ndx, liste_elf32_sym table_symb, char  * stringtable){
  Elf32_Sym symbcurr;
  for(int i=0;i<taille(table_symb);i++){
    get_symbol(table_symb, i, &symbcurr);
    swap_header_symb(&symbcurr);
    char * symbname = stringtable + symbcurr.st_name;
    if((strcmp(symbname, name_symb) != 0)&& ndx == symbcurr.st_shndx ){
      if(ELF32_ST_TYPE(symbcurr.st_info) == 0){
        return i;
      }
    }
  }
  return -1;
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
                table_string* table: table de string
                liste_elf32_Shdr* liste_section : tous les headers de section
*/
int cont_sect(FILE* res, char* s1, char* s2, int v1, int v2, int offset, Elf32_Shdr sectheader1[], Elf32_Shdr sectheader2[], Elf32_Ehdr header_s1, Elf32_Ehdr* header, table_string* table, liste_elf32_Shdr* liste_section){
    Elf32_Shdr sect_header_res;

    //fusion de l entete de la section
    sect_header_res = sectheader1[v1];
    sect_header_res.sh_offset = offset;
    sect_header_res.sh_size = sectheader1[v1].sh_size + sectheader2[v2].sh_size;
    sect_header_res.sh_addralign = max(sectheader1[v1].sh_addralign, sectheader2[v2].sh_addralign);
    sect_header_res.sh_name = table->val;

    //on met la nouvelle entete dans la liste d'entete
    write_table(table,sectheader1[v1], header_s1, s1);
    ajouter_S(liste_section, sect_header_res);

    //ecriture des contenus de sections dans le fichier fusion
    cont_sect_e(res, s1, s2, v1, v2, offset, sectheader1,sectheader2,header,sect_header_res);

    //On ajoute une nouvelle section dans le header
    header->e_shnum++;
    
    //Mise a jour de l offset
    offset = offset + sect_header_res.sh_size;
    
    //parce que compilateurwrite
    return offset;
}

/*
Creer la nouvelle table de string
Retour:         l entete de la nouvelle section de string
Arguments:      char* source: nom du fichier source
                Elf32_Ehdr header : le header du fichier elf
                int offsetstring : offset de la section
                int offset: indice courant
                table_string* table: tableau contenant tout les strings
                table_string* table_source : tableau contenant des strings
*/
Elf32_Shdr new_string_sect(char* source, Elf32_Ehdr header,int offsetstring, int offset, table_string* table, table_string* table_source) {
    //Initialisation
    Elf32_Shdr sect;
    Elf32_Shdr new_sect;
    int res;

    //On se place dans le fichier la ou se trouve la table de string
    FILE* f = ouverture_fich_lecture(source);
    res = fseek(f, offsetstring, SEEK_SET);
    //Recuperation du header de la table de string
    res = fread(&sect, 1, sizeof(new_sect), f);
    swap_header_section(&sect);

    //creation du nouvel header de la table de string
    new_sect = sect;
    new_sect.sh_offset = offset;
    new_sect.sh_name = table->val;
    //On ecrit la nouvelle table de string
    write_table(table, sect, header, source);
    new_sect.sh_size = table_source->val;
    res++;
    return new_sect;
}

//param : list_symb1, list_symb2 : les listes de symboles à fusionner
//        stringtable1, stringtable2 : Les stringtable des deux listes à fusionner
//        final_stringtable : La stringtable de la nouvelle table des symboles
//       final_list_symb : La nouvelle liste des symboles
void concat_table_symb(liste_elf32_sym list_symb1,char* stringtable1, liste_elf32_sym list_symb2, char * stringtable2, liste_elf32_sym * final_list_symb, table_string* final_stringtable){

  Elf32_Sym symb1;
  int indexTab2;
  char * symbname;
  //Boolean table for test
  int traiter[taille(list_symb2)];
  for (int i=0; i < taille(list_symb2);i++){
    traiter[i]=0;
  }
  //travel into the fabulous world of table symb 1
  for (int i=1; i <  taille(list_symb1);i++){
    get_symbol(list_symb1, i, &symb1);
    swap_header_symb(&symb1);
    symbname = stringtable1 + symb1.st_name;
    indexTab2 = is_define_in_table(symbname,symb1.st_shndx, list_symb2, stringtable2);
    switch(indexTab2){

      case -1:
      //nv entrée
      symb1.st_name = final_stringtable->val;
      swap_header_symb(&symb1);
      ajouter(final_list_symb,symb1);
      //ecrire table chaine
      write_table_Symb(final_stringtable, symbname);
      //write_in_string_table(stringtable3, symbname);

      break;

      // define in 1 and 2
      case 0:
      //Erreur
      printf("erreur: impossible de fusionner les fichiers\n");
      return;

      //symb not present in 2
      default:
      //nv entrée
      symb1.st_name = final_stringtable->val;
      swap_header_symb(&symb1);
      ajouter(final_list_symb,symb1);
      //ecrire table chaine
      //write_in_string_table(stringtable3, symbname);
      write_table_Symb(final_stringtable, symbname);
      //maj tab
      traiter[indexTab2]=1;
      break;

    }
  }
  //travel into the fabulous world of table symb 2
  Elf32_Sym symb2;
  for(int i = 0; i<taille(list_symb2) ; i++) {
    if(traiter[i] == 0) {
      get_symbol(list_symb2, i, &symb2);
      swap_header_symb(&symb2);
      symbname = stringtable2 + symb2.st_name;
      symb2
.st_name = final_stringtable->val;
      swap_header_symb(&symb2);
      ajouter(final_list_symb,symb2);
      write_table_Symb(final_stringtable, symbname);
      //write_in_string_table(stringtable3, symbname);
    }
  }
}

































































































    //mdr st martin d heres (ca rime en fait si t as pas vu et du coup c est rigolo toi comprendre?)
    //je pense que tu n as pas compris, en fait ca rime genre quand on le dit et bah ca termine pareil mageul. POINT holala tavu ma blague avec le point genre c est drole. POINT encore une blague. j avais pas envie de faire la 
    //Je pense que l'auteur dans son ouvrage à voulu exprimer son opinion sur son etat d'esprit à se stade du projet. On peut voir cela notamment à l'utilisation de rimes de type AA mais aussi grace à l'accumulation de blague tres tres drole a la ligne 2