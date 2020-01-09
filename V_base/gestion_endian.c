
#include "gestion_endian.h"

uint32_t invert_endian_32(uint32_t val){
	uint32_t result = 0;
	uint8_t temp;
	result = 0;
	for(int i = 1; i <= 4; i++){
		temp = (val << (32-8*i)) >> 24;
		result = (result << 8) + temp;
	}
	return result;
}

uint16_t invert_endian_16(uint16_t val){
	uint16_t result = 0;; 
	uint8_t temp;
	result = 0;
	for(int i = 1; i <= 2; i++){
		temp = (val << (16-8*i)) >> 8;
		result = (result << 8) + temp;
	}
	return result;
}

void swap_header(Elf32_Ehdr* header){

    header->e_type= invert_endian_16(header->e_type);
    header->e_machine= invert_endian_16(header->e_machine);
    header->e_version= invert_endian_32(header->e_version);
    header->e_entry= invert_endian_32(header->e_entry);
    header->e_phoff= invert_endian_32(header->e_phoff);
    header->e_shoff= invert_endian_32(header->e_shoff);
    header->e_flags= invert_endian_32(header->e_flags);
    header->e_ehsize= invert_endian_16(header->e_ehsize);
    header->e_phentsize= invert_endian_16(header->e_phentsize);
    header->e_phnum= invert_endian_16(header->e_phnum);
    header->e_shentsize= invert_endian_16(header->e_shentsize);
    header->e_shnum= invert_endian_16(header->e_shnum);
    header->e_shstrndx= invert_endian_16(header->e_shstrndx);

}

void swap_header_section(Elf32_Shdr* header_sect) {
    header_sect->sh_name= invert_endian_32(header_sect->sh_name);
    header_sect->sh_type= invert_endian_32(header_sect->sh_type);
    header_sect->sh_flags= invert_endian_32(header_sect->sh_flags);
    header_sect->sh_addr= invert_endian_32(header_sect->sh_addr);
    header_sect->sh_offset= invert_endian_32(header_sect->sh_offset);
    header_sect->sh_size= invert_endian_32(header_sect->sh_size);
    header_sect->sh_link= invert_endian_32(header_sect->sh_link);
    header_sect->sh_info= invert_endian_32(header_sect->sh_info);
    header_sect->sh_addralign= invert_endian_32(header_sect->sh_addralign);
    header_sect->sh_entsize= invert_endian_32(header_sect->sh_entsize);

}

void swap_header_symb(Elf32_Sym* header_sym) {
    header_sym->st_name= invert_endian_32(header_sym->st_name);
    header_sym->st_value= invert_endian_32(header_sym->st_value);
    header_sym->st_size= invert_endian_32(header_sym->st_size);
    header_sym->st_shndx= invert_endian_16(header_sym->st_shndx);
}

void swap_rel(Elf32_Rel * relation){
    relation->r_offset = invert_endian_32(relation->r_offset);
    relation->r_info = invert_endian_32(relation->r_info);
}

void swap_rela(Elf32_Rela * relation){
    relation->r_offset = invert_endian_32(relation->r_offset);
    relation->r_info = invert_endian_32(relation->r_info);
    relation->r_addend = invert_endian_32(relation->r_addend);
}