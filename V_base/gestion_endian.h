#ifndef _GESTION_ENDIAN_H_

#define _GESTION_ENDIAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <string.h>

/*
swap an u_int32_t between little endian and big endian formats
*/
u_int32_t invert_endian_32(u_int32_t val);
/*
swap an u_int16_t between little endian and big endian formats
*/
u_int16_t invert_endian_16(u_int16_t val);
/*
invert all header's attributs in another format (little/big endian)
*/
void swap_header(Elf32_Ehdr* header);
/*
invert all section header's attributs in another format (little/big endian)
*/
void swap_header_section(Elf32_Shdr* header_sect);
/*
invert all symbol table header's attributs in another format (little/big endian)
*/
void swap_header_symb(Elf32_Sym* header_sym);

#endif
