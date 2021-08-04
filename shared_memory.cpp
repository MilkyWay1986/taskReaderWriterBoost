#include "shared_memory.h"

//-------------------------------------------------------------------------------------

std::string &SM::SharedMemory::GetSharedMemory() { return static_cast<std::string &>(shared_memory); }

//-------------------------------------------------------------------------------------

void SM::SharedMemory::SetSharedMemory(const char *new_shared_memory, std::size_t len) {
    shared_memory = std::move(std::string{new_shared_memory, len});
}

//-------------------------------------------------------------------------------------

void SM::SharedMemory::PrintSharedMemory() const {
    std::cout << "SM is : " << shared_memory << "\n";
}

//-------------------------------------------------------------------------------------
