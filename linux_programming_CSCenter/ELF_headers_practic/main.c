#include <stdio.h>
#define ELF_NIDENT	16

// program header-ы такого типа нужно загрузить в
// память при загрузке приложения
#define PT_LOAD		1

// структура заголовка ELF файла
struct elf_hdr {
	std::uint8_t e_ident[ELF_NIDENT];
	std::uint16_t e_type;
	std::uint16_t e_machine;
	std::uint32_t e_version;
	std::uint64_t e_entry;
	std::uint64_t e_phoff;
	std::uint64_t e_shoff;
	std::uint32_t e_flags;
	std::uint16_t e_ehsize;
	std::uint16_t e_phentsize;
	std::uint16_t e_phnum;
	std::uint16_t e_shentsize;
	std::uint16_t e_shnum;
	std::uint16_t e_shstrndx;
} __attribute__((packed));

// структура записи в таблице program header-ов
struct elf_phdr {
	std::uint32_t p_type;
	std::uint32_t p_flags;
	std::uint64_t p_offset;
	std::uint64_t p_vaddr;
	std::uint64_t p_paddr;
	std::uint64_t p_filesz;
	std::uint64_t p_memsz;
	std::uint64_t p_align;
} __attribute__((packed));


std::size_t space(const char *name)
{
    struct elf_hdr header;
    struct elf_phdr buffer_ph[32];
    std::uint64_t need_size = 0;

    FILE *fp = fopen(name, "r");
    fread(&header, sizeof(struct elf_hdr), 1, fp);
    fseek(fp, header.e_phoff, SEEK_SET);
    fread(buffer_ph, header.e_phentsize, header.e_phnum, fp);
    fclose(fp);

    for(int i = 0; i < header.e_phnum; i++)
        if(buffer_ph[i].p_type == PT_LOAD)
            need_size += buffer_ph[i].p_memsz;

    return need_size;
}
