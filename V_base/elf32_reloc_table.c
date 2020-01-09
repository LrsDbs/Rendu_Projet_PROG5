#include "elf32_reloc_table.h"

void creer_table(elf32_reloc_table * s,int taille){
	s->size=0;
    s->tabrela = malloc(sizeof(Elf32_Rela) * taille);
}

int est_vide_table(elf32_reloc_table s){
	if (s.size==0){
		return 1;
	}
	return 0;
}	

int taille_table(elf32_reloc_table s){
	return s.size;
}

void vider_table(elf32_reloc_table * t){
	t->size=0;
}

void free_table(elf32_reloc_table * t){
	free(&t->header);
	free(t->tabrela);
	t->size=0;
}


void creer_section(elf32_section_reloc * s, int taille){
	s->size = 0;
	s->size_max = taille;
	s->table = malloc(sizeof(elf32_reloc_table) * taille);
}

int est_vide_section(elf32_section_reloc s){
	if(s.size==0){
		return 1;
	}
	return 0;
}

int taille_section(elf32_section_reloc s){
	return s.size;
}

void vider_section(elf32_section_reloc * s){
	s->size=0;
}

void re_alloc_section(elf32_section_reloc * s){
	s->size_max = s->size_max + 2;
	s->table = realloc(s->table, sizeof(elf32_reloc_table) * s->size_max);
}

int ajouter_section(elf32_section_reloc * s, elf32_reloc_table t){
	if (s->size == s->size_max)
		re_alloc_section(s);
	s->table[s->size]=t;
	s->size++;
	return 0;
}

int supprimer_section(elf32_section_reloc * s, int index){
	if(s->size == 0)
		return 0;
	s->table[index] = s->table[s->size - 1];
	s->size--;
	return 0;
}

void free_section(elf32_section_reloc *s){
	for(int i=0;i<s->size;i++){
		free_table(&s->table[i]);
	}
	s->size=0;
}