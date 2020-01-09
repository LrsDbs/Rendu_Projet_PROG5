#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

Elf64_Ehdr load_header(FILE *f){

  Elf64_Ehdr converted_file;;
  if(f) {
    int n = fread(&converted_file, 1, sizeof(converted_file), f);
    if(n == 0){
      return converted_file;
    }
  }
  return converted_file;

  }

char * get_string_table(char *filename, int offset){
    FILE* file;
    file=fopen(filename, "rb");

    int res;
    Elf64_Shdr header;
    char * stringtable;

    res = fseek(file, offset, SEEK_SET);
    res = fread(&header, 1, sizeof(header), file);
    stringtable = malloc(header.sh_size);
    res = fseek(file, header.sh_offset, SEEK_SET);
    res = fread(stringtable, 1, header.sh_size, file);

    res+=0;
    fclose(file);
    return stringtable;
}

Elf64_Shdr load_section(char* filename, int offset){
    FILE* file;
    file=fopen(filename, "rb");
    Elf64_Shdr sectionheader;
    if (file){
        int res;
        res=fseek(file, offset, SEEK_SET);
        res=fread(&sectionheader, 1, sizeof(sectionheader), file);
        res++;
    }
    fclose(file);
    return sectionheader;
}

void get_section_table(char* filename, Elf64_Ehdr header, Elf64_Shdr sectionheaders[]){
    int offset = header.e_shoff;
    int sectionsize = header.e_shentsize;

    for(int i=0; i<header.e_shnum;i++){
        Elf64_Shdr sectionheader = load_section(filename, offset);
        sectionheaders[i]=sectionheader;
        offset+=sectionsize;
    }
    return;
}


int getSectionHeader(FILE *f, Elf64_Shdr section, char *sectionHeader) {
  fseek(f, section.sh_offset, SEEK_SET);
  int n;
  n = fread(sectionHeader, 1, section.sh_entsize, f);
  if(n == 0) {
    return -1;
  }
  return 0;
}

//nameSec = StringTable addr
int printSection(Elf64_Shdr section, FILE *f, char * nameSec) {

  fseek(f, section.sh_offset, SEEK_SET);
  char sectionString[section.sh_size];
  for (int i = 0; i<section.sh_size; i++){
    if(fscanf(f, "%c", &(sectionString[i])) == 0) {
      printf("Erreur lecture fichier");
      return -1;
    }
  }

  printf("\nVidange hexadécimale de la section << %s >> :\n",nameSec+section.sh_name);
    for (int y=0;y<section.sh_size;y+=16){
      //affichage de la ligne d'adresse
    printf("0x%.8lx ",section.sh_addr+y);

    //affichage version hexa
    for(int i=0;i<16;i++){
      if(i%4==0)
        printf(" ");
      if(i+y<section.sh_size){
        if(sectionString[i+y]=='\x0'){
          printf("00");
        }
        else if(sectionString[i+y]>>16=='\xff')
          printf("%x",(sectionString[i+y])&(0xff) );
        else
          printf("%x",sectionString[i+y]);
        }
      else
        printf("  ");

   }
     printf("\t");
    //affichage version string
   for(int z=0;z<16;z++){
     if(z+y<section.sh_size){
     if(sectionString[z+y]<32){
       printf(".");
     }
     else
      printf("%c",sectionString[z+y]);
   }
 }
   printf("\n");
  }
  printf("\n");
  return 0;

}

/*
  Return Value:
  - -1 premier argument n'est pas un fichier
  - -2 second argument manquant
  - -3 section non trouvée par numéro
  - -4 section non trouvée par nom
*/
int main(int argc, char *argv[]){
  char * fic = argv[1];
  FILE *f;
  f = fopen(fic,"rb");
  if(f == NULL){
    printf("erreur, le premier argument n'est pas un fichier\n");
    return -1;
  }
  if(argv[2] == NULL){
    printf("erreur, il manque le deuxieme argument\n");
    return -2;
  }
  int index, n;
  Elf64_Ehdr header = load_header(f);
  Elf64_Shdr elf_sections[header.e_shnum];
  int offset = ((header.e_shstrndx*header.e_shentsize)+header.e_shoff);
  char * nameSec = get_string_table(fic,offset);
  get_section_table(fic,header,elf_sections);

  if(sscanf(argv[2], "%d", &index) == 1){
    if(index <= header.e_shnum && index >= 0){
      n = printSection(elf_sections[index], f, nameSec);
      if(n == -1){
          printf("erreur, le premier argument n'est pas un fichier\n");
          return -1;
      }
    }
    else {
      printf("erreur, section non trouvée\n");
      return -3;
    }
  }
  else{
    int i = 0;
    int test =-1;
    char * name;
    while(test != 0 && i < header.e_shnum){
       name = nameSec + elf_sections[i].sh_name;
      if(strcmp(argv[2],name) == 0){
        test = 0; index = i;
      }
      i++;
    }
    if(test == 0){
      n = printSection(elf_sections[index], f, nameSec);
      if(n){}
    }
    else {
      printf("erreur, section non trouvée\n");
      return -3;  
    }
  }


  fclose(f);
  return 0;
}
