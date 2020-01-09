#include "mains.h"


/*
			Partie 1
			Cas -A
Demande d affichage d un header du fichier elf
Arguments:		char* name_file: nom du fichier a ouvrir
*/
int main_load_header(char* name_file ){
	//Recuperation du header
	Elf32_Ehdr res, res_b;
	res=load_header(name_file);
	res_b = res;
	//Passage du header en little endian pour avoir acces au donnees
	swap_header(&res_b);
	//Affichage du header
	print_header(res_b);
		return 0;
}
/*
			Partie 2
			Cas -B
Demande d affichage de la table de section
Arguments:		char* name_file: nom du fichier a ouvrir
*/
int main_read_section_table(char* name_file){
	//Recuperation du header
	Elf32_Ehdr header = load_header(name_file);
	swap_header(&header);//Passage du header en little endian pour avoir acces au donnees
	//Recuperation de la table de string
	int offset = ((header.e_shstrndx*header.e_shentsize)+header.e_shoff);
	char * stringtable = load_stringtable(name_file, offset);
	//Recuperation de la table de section
	Elf32_Shdr sectionheaders[header.e_shnum]; 
	load_tablesection(name_file, header, sectionheaders);
	//Passage de chaque section en little endian pour avoir acces au donnees
	for (int k = 0; k<header.e_shnum;k++){
		swap_header_section(&(sectionheaders[k]));
	}
	//Affichage de la table de section
	print_section_table(sectionheaders, header.e_shnum, stringtable);
	free(stringtable);	
	return 0;
}

/*
			Partie 3
			Cas -C
Demande d affichage d un header de section
Arguments:		char* name_file: nom du fichier a ouvrir
 				char* val: soit - le numero de section
				 				- le nom de la section
*/
int main_section(char* name_file, char* val){
	//Ouverture du fichier
	FILE *f;
	f= fopen(name_file,"rb");
	if( f == NULL){
		printf("erreur, le premier argument n'est pas un fichier\n");
		return -1;
	}
	//test de la présence du second argument
	if(val == NULL){
		printf("erreur, il manque le deuxieme argument\n");
		return -2;
	}
	int index,n;
	//Recuperation du header
	Elf32_Ehdr header = load_header(name_file);
	swap_header(&header);//Passage du header en little endian pour avoir acces au donnees
	//Recuperation de la table de section
	Elf32_Shdr elf_sections[header.e_shnum];
	load_tablesection(name_file,header,elf_sections);
	for (int k = 0; k<header.e_shnum;k++){
		swap_header_section(&(elf_sections[k]));
	}
	//Recuperation de la table de string
	int offset = ((header.e_shstrndx*header.e_shentsize)+header.e_shoff);
	char * nameSec = load_stringtable(name_file,offset);

	//cas ou le second argument est un entier
	if(sscanf(val, "%d", &index)==1){
		if(index<=header.e_shnum && index>=0){
			n = print_section(elf_sections[index], f, nameSec);
			if(n==-1){
				printf("erreur, le premier argument n'est pas un fichier\n");
				return -1;
			}
		}else{
			printf("erreur, section non trouvée\n");
			return -3;
		}
	}else{
	//cas ou la section est une chaine de caracteres
		int i = 0;
		int test =-1;
		char * name;
		//recherche de la chaine dans la table des strings
		while(test!=0 && i<header.e_shnum){
			name = nameSec+elf_sections[i].sh_name;
			if(strcmp(val,name)==0){
				test=0;index=i;
			}
			i++;
		}
		//si on trouve
		if(test==0){
			n = print_section(elf_sections[index], f, nameSec);
			if(n){}
		}
		//si on ne trouve pas
		else{
			printf("erreur, section non trouvée\n");
			return -3;
		}
	}
	free(nameSec);	

	fclose(f);
	return 0;

}

/*
			Partie 4
			Cas -D
Demande d affichage d un header de section
Arguments:		char* name_file: nom du fichier a ouvrir
*/
int main_table_symbole(char* name_file){
	//creation du header
	Elf32_Ehdr header = load_header(name_file);
	swap_header(&header); //Passage du header en little endian pour avoir acces au donnees
	//Recuperation de la table de string
	int offset = ((header.e_shstrndx*header.e_shentsize)+header.e_shoff);
	char * stringtable = load_stringtable(name_file,offset);
	//creation liste symb
	liste_elf32_sym liste_symb;
	creer_liste(&(liste_symb), 5);
	//Recuperation de la table de section
	Elf32_Shdr sectionheaders[header.e_shnum]; 
	load_tablesection(name_file, header, sectionheaders);
	//Passage de chaque section en little endian pour avoir acces au donnees
	for (int k = 0; k<header.e_shnum;k++){
		swap_header_section(&(sectionheaders[k]));
	}
	//Recuperation table de symbole
	load_tablesymbole(name_file,header, sectionheaders, &liste_symb);
	//Passage de chaque symbole en little endian pour avoir acces au donnees
	for (int k = 0; k<liste_symb.n;k++){
		swap_header_symb(&(liste_symb.tab[k]));
	}
	//On recupere la table de string pour les symboles
	offset = load_offset_symb(header, sectionheaders,stringtable);
	free(stringtable);	
	stringtable = load_stringtable(name_file,offset);
	//On affiche la table de symboles
	print_tablesymbol(liste_symb,stringtable);

	free_liste(&liste_symb);	
	free(stringtable);	
	return 0;
}



/*
Partie 1.5
Récupèration et affichage des tables de réimplémentation
arguments:
	char* name_file nom du fichier concerné
*/
int main_relocation_table(char *name_file){
	
//creation liste symb
	liste_elf32_sym liste_symb;
	creer_liste(&(liste_symb), 5);

	//Création et remplissage|chargement de la table de réallocation :
	elf32_section_reloc relocationtable;
	load_relocation_table(name_file, &relocationtable);
	print_relocation_table(name_file, relocationtable);
	return 0;
}



/*Partie Fusion
Fusionnes deux fichiers elf en un fichier elf
*/
int main_fus(int argc, char *argv[]){
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
  sect_head_symb1.sh_size = 0;
  sect_head_symb2.sh_size = 0;
  val = 0;
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


