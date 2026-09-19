#include "cart.h"
#include "cart_type.h"
#include <iostream>
#include <fstream>
#include <Windows.h>

char runtime_path_buffer[MAX_PATH];
uint8_t cartridge_data[MAX_CART_SIZE];
bool cartridge_loaded = false;
cart_header_struct* cartridge_header = (cart_header_struct*)(cartridge_data + 0x100);

bool get_runtime_path()
{
	if (GetCurrentDirectory(MAX_PATH, runtime_path_buffer) == 0)
	{
		return false;
	}
	return true;
}

bool cart_open_file()
{
	if (!get_runtime_path())
	{
		return false;
	}

	char filename[MAX_PATH];

	OPENFILENAMEA ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "GB ROM Files\0*.gb\0AnyFile\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.lpstrInitialDir = runtime_path_buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a ROM file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn))
	{
		cart_load(filename);
		return true;
	}
	return false;
}

void cart_print_info()
{
	printf("Entry point: %.2X%.2X%.2X%.2X\n",
		cartridge_header->entry_point[0],
		cartridge_header->entry_point[1],
		cartridge_header->entry_point[2],
		cartridge_header->entry_point[3]);
	printf("Title: %s\n", cartridge_header->title);
	printf("CGB Flag: %.2X\n", cartridge_header->cgb_flag);
	printf("New Licensee code: %.2X%.2X\n", cartridge_header->new_licensee_code[0], cartridge_header->new_licensee_code[1]);
	printf("SGB Flag: %.2X\n", cartridge_header->sgb_flag);
	printf("Type: %.2X (%s)\n", cartridge_header->cartridge_type, cart_type_data[cartridge_header->cartridge_type].readable_name);
	printf("ROM Size: %.2X\n", cartridge_header->rom_size);
	printf("RAM Size: %.2X\n", cartridge_header->ram_size);
	printf("Destination code: %.2X\n", cartridge_header->destination_code);
	printf("Old Licensee code: %.2X\n", cartridge_header->old_licensee_code);
	printf("Version: %.2X\n", cartridge_header->mask_rom_version_number);
	printf("Header Checksum: %.2X\n", cartridge_header->header_checksum);
	printf("Global Checksum: %.2X%.2X\n", cartridge_header->global_checksum_hi, cartridge_header->global_checksum_lo);
}

bool cart_load(const char* filename)
{
	std::streampos size;
	std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		file.seekg(0, std::ios::beg);
		file.read((char*)cartridge_data, MAX_CART_SIZE);
		file.close();
		printf("ROM %s loaded, size: %lli bytes\n", filename, std::streamoff(size));
		return true;
	}
	printf("Failed to load file %s\n", filename);
	return false;
}
