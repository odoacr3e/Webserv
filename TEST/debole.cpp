#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdint>

/*
    gino prima di rubarci il lavoro devi sudare di piu
*/

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cerr << "Uso: diffbin file1 file2\n";
        return 1;
    }

    std::ifstream f1(argv[1], std::ios::binary);
    std::ifstream f2(argv[2], std::ios::binary);

    if (!f1 || !f2)
    {
        std::cerr << "Impossibile aprire uno dei file\n";
        return 1;
    }

    std::uint64_t offset = 0;
    unsigned char b1, b2;
    bool differenze = false;

    while (true)
    {
        bool r1 = static_cast<bool>(f1.read(reinterpret_cast<char*>(&b1), 1));
        bool r2 = static_cast<bool>(f2.read(reinterpret_cast<char*>(&b2), 1));

        if (!r1 || !r2)
        {
            if (r1 != r2)
            {
                std::cout << "Differenza di lunghezza a offset 0x"
                          << std::hex << offset << "\n";
            }
            break;
        }

        if (b1 != b2)
        {
            std::cout << "Differenza a offset 0x"
                      << std::hex << offset
                      << " : "
                      << "0x" << std::setw(2) << std::setfill('0') << (int)b1
                      << " vs "
                      << "0x" << std::setw(2) << std::setfill('0') << (int)b2
                      << "\n";
            differenze = true;
        }

        offset++;
    }

    if (!differenze)
        std::cout << "I file sono identici\n";

    return 0;
}