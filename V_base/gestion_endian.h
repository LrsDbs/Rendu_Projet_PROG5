#ifndef _GESTION_ENDIAN_H_

#define _GESTION_ENDIAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <string.h>

u_int32_t invert_endian_32(u_int32_t val);

u_int16_t invert_endian_16(u_int16_t val);

void swap_header(Elf32_Ehdr* header);

void swap_header_section(Elf32_Shdr* header_sect);

void swap_header_symb(Elf32_Sym* header_sym);

void swap_rel(Elf32_Rel * relation);

void swap_rela(Elf32_Rela * relation);

#endif