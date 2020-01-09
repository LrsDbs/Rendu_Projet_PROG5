#include "fusion_table_symboles.h"


int max(int i1, int i2){
    if (i1 > i2) {
        return i1;
    }
    return i2;
}
/*
return :
the index if the symbol is define in table_symb
-1 if the symbol doesn't exist
*/
// retourne l'index de la section name ou -1 si elle n'existe pas
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
    if(strcmp(symbname, name_symb) == 0 && ndx == symbcurr.st_shndx ){
      if(ELF32_ST_TYPE(symbcurr.st_info) == 0){
        return i;
      }
    }
  }
  return -1;
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
  printf("taillo %d\n", taille(list_symb2));
  for (int i=0; i < taille(list_symb2);i++){
    traiter[i]=0;
  }
  //travel into the fabulous world of table symb 1
  printf("taillo 2 : %d\n", taille(list_symb1));
  for (int i=1; i <  taille(list_symb1);i++){
    get_symbol(list_symb1, i, &symb1);
    swap_header_symb(&symb1);
    printf("%d\n",symb1.st_value);
    symbname = stringtable1 + symb1.st_name;
    printf("%s\n", symbname);
    indexTab2 = is_define_in_table(symbname,symb1.st_shndx, list_symb2, stringtable2);
    //  printf("%d", indexTab2);
    //swap_header_symb(&symb1);
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
      printf("you fool\n");
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


int main(int argc, char *argv[]){
  //Initialisation
  int offsetstring, offsetstring2;
  int v;
  int val;
  char* nameTemp;
  char* res = argv[2];
  char* sources1 = argv[3];
  char* sources2 = argv[4];
  table_string table;
  liste_elf32_Shdr liste_section;
  creer_liste_S(&liste_section, 5);
  Elf32_Ehdr header1;// header fichier source num1
  Elf32_Ehdr header2;// header fichier source num2
  Elf32_Ehdr new_header;// header fichier fusion

  int offset_string_tab_symb1, offset_string_tab_symb2;
  liste_elf32_sym list_symb1, list_symb2, final_list_symb;
  creer_liste(&list_symb1, 5);
  creer_liste(&list_symb2, 5);
  creer_liste(&final_list_symb, 5);

  char * string_table_symb1, * string_table_symb2;
  //table_string final_string_table_symb;
  FILE* fres = fopen(res, "wb");

  //recuperation des headers
  header1 = load_header(sources1);
  header2 = load_header(sources2);
  swap_header(&header1);
  swap_header(&header2);

  //recuperations des tables de sections
  Elf32_Shdr sect_head1[header1.e_shnum];
  Elf32_Shdr sect_head2[header2.e_shnum];
  load_tablesection(sources1, header1, sect_head1);
  load_tablesection(sources2, header2, sect_head2);
  for (int k = 0; k<header1.e_shnum;k++){
    swap_header_section(&(sect_head1[k]));
  }
  for (int k = 0; k<header2.e_shnum;k++){
    swap_header_section(&(sect_head2[k]));
  }

  //creation de la table de string
  table = creat_table_string(header1, header2, sources1, sources2);

  //initialisation du nouveau header
  new_header = header1;
  new_header.e_shnum = 0;
  new_header.e_shoff = header1.e_shoff + header2.e_shoff; // Debut table de section
  int tab_bool[header2.e_shnum]; // Section a traite du deuxieme fichier
  for (int i = 0; i < header2.e_shnum; i++){
    tab_bool[i] = 0;
  }

  //offset courant de chaque section
  int offset = sizeof(new_header);

  printf("duck 1\n");

  offsetstring = ((header1.e_shstrndx*header1.e_shentsize)+header1.e_shoff);
  char * nameSec = load_stringtable(sources1,offsetstring);
  for (int i = 0; i < header1.e_shnum; i++){ // Pour chaque section du premier fichier
    //Si la section est PROGBITS et que le nom de cette section se trouve dans le 2eme fichier
    //Alors on fusionne les deux sections et on met a jour tab_bool
    //Sinon on met juste la section
    if (sect_head1[i].sh_type == SHT_PROGBITS){

      v = search_same_name(nameSec + sect_head1[i].sh_name, sources1);
      if (v != -1){
        tab_bool[v] = 1;
        offset = cont_sect(fres, sources1, sources2, i, v, offset, sect_head1, sect_head2, header1, &new_header, &table, &liste_section);
      } else {
        offset = put_sect(fres, sources1, offset, sect_head1, header1, &new_header, i, &table, &liste_section);
      }
    } else {
      //on ignore la table de string du premier fichier et les symboles
      nameTemp = nameSec + sect_head1[i].sh_name;
      if (strcmp(nameTemp, ".shstrtab") && strcmp(nameTemp, ".strtab") && strcmp(nameTemp, ".symtab")){
        offset = put_sect(fres, sources1, offset, sect_head1, header1, &new_header, i, &table, &liste_section);
      }
    }
  }
  // Pour chaque section non traite du deuxieme fichier,
  // On l ajoute

  offsetstring2 = ((header2.e_shstrndx*header2.e_shentsize)+header2.e_shoff);
  char * nameSec2 = load_stringtable(sources2,offsetstring2);
  for (int i = 0; i < header2.e_shnum; i++){
    if (tab_bool[i] == 0) {
      //on ignore la table de string du deuxieme fichier et les symboles
      nameTemp = nameSec2 + sect_head2[i].sh_name;
      if (strcmp(nameTemp, ".shstrtab") && strcmp(nameTemp, ".strtab") && strcmp(nameTemp, ".symtab")){
        offset = put_sect(fres, sources2, offset, sect_head2, header2, &new_header, i, &table, &liste_section);
      }
    }
  }
  //chargement des tables des symboles
  load_tablesymbole(sources1,header1, sect_head1,&list_symb1);
  load_tablesymbole(sources2,header2, sect_head2,&list_symb2);

  // recupération des offsets des tables des string des symboles

  offset_string_tab_symb1 = load_offset_symb(header1, sect_head1, nameSec);
  offset_string_tab_symb2 = load_offset_symb(header2, sect_head2, nameSec2);


  //fusion des tables des fusion_table_symboles

  Elf32_Shdr sect_head_symb1;
  Elf32_Shdr sect_head_symb2;
  //recherche de la section de la table de string des symboles de la premiere source
  for (int i = 0; i<header1.e_shnum; i++){
    nameTemp = nameSec + sect_head1[i].sh_name;
    if (strcmp(nameTemp, ".strtab")==0){
      sect_head_symb1 = sect_head1[i];
      val = i;
    }
  }
  //recherche de la section de la table de string des symboles de la deuxieme source

  for (int i = 0; i<header2.e_shnum; i++){
    nameTemp = nameSec2 + sect_head2[i].sh_name;
    if (strcmp(nameTemp, ".strtab")==0){
      sect_head_symb2 = sect_head2[i];
    }
  }
  //recuperation des tables des string des symboles
  string_table_symb1 = malloc(sizeof(char) * sect_head_symb1.sh_size);
  string_table_symb2 = malloc(sizeof(char) * sect_head_symb2.sh_size);
  string_table_symb1 = load_stringtable(sources1,offset_string_tab_symb1);
  string_table_symb2 = load_stringtable(sources2,offset_string_tab_symb2);

  //creation de la nouvelle table de string
  table_string final_string_table_symb = creat_table_string_symb(sect_head_symb1.sh_size, sect_head_symb2.sh_size);
  concat_table_symb(list_symb1, string_table_symb1,list_symb2,string_table_symb2, &final_list_symb, &final_string_table_symb);

  //mise a jour de l emplacement de la table des string
  Elf32_Shdr sect_head_tablesymb1;
  for (int i = 0; i<header1.e_shnum; i++){
    if (sect_head1[i].sh_type==SHT_SYMTAB){
      sect_head_tablesymb1 = sect_head1[i];
    }
  }

  //creation de la nouvelle entete de la table des symboles
  Elf32_Shdr new_sect_header = sect_head_tablesymb1;
  new_sect_header.sh_name = table.val;
  new_sect_header.sh_size = sizeof(Elf32_Sym) * final_list_symb.n;
  new_sect_header.sh_offset = offset;
  //ajout dans la liste des entetes de section
  write_table(&table, sect_head_tablesymb1, header1, sources1);
  ajouter_S(&liste_section, new_sect_header);
  new_header.e_shnum++;
  //ecriture du contenu de la section des symboles
  fseek(fres,offset,SEEK_SET);
  for (int i = 0; i<final_list_symb.n;i++){
    fwrite(&(final_list_symb.tab[i]), sizeof(Elf32_Sym), 1, fres);
    offset = offset + sizeof(Elf32_Sym);
}

//ecriture de la nouvelle table de string et ajout dans la liste de la nouvelle entete
ajouter_S(&liste_section, new_string_sect(sources1, header1, header1.e_shoff + (val * header1.e_shentsize), offset, &table, &final_string_table_symb));
offset = write_string_table(fres, final_string_table_symb, offset);

//mise a jour du nouvel header
new_header.e_shnum++;
new_header.e_shstrndx = liste_section.n;
//creation de l entete de la table de string
ajouter_S(&liste_section, new_string_sect(sources1, header1, offsetstring, offset, &table, &table));

//ecriture de la table de string
offset = write_string_table(fres, table, offset);

//ecriture des entete dans le fichier fusion
write_section_table(fres, liste_section, offset);

//On ecrit le header dans le fichier resultat
new_header.e_shoff = offset;
new_header.e_shnum++;
write_new_header(fres, new_header);
return 0;
}
