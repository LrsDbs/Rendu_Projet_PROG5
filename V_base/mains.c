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
retourne : -1 si le fichier n'a pas pu être ouvert
					 -2 si le second argument est NULL
					 -3 si la section n'existe pas
					  0 sinon
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
		}
		else{
			printf("erreur, section non trouvée\n");
			return -3;
		}
		}
	else{
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
récupèration et affichage des tables de réimplémentation
arguments:
	char* name_file nom du fichier concerné
*/
/*int main_relocation_table(char *name_file){
//	Elf32_Ehdr header = load_header(name_file);
	elf32_section_reloc relocationtable;
	load_relocation_table(name_file, &relocationtable);
	print_relocation_table(name_file, relocationtable);
	return 0;
}*/
