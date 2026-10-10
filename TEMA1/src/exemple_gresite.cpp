#include <tema/exemple_gresite.hpp>
#include <tema/contor_heap.hpp>
#include <iostream>

namespace tema {

// Clasa pentru StudentFaraDestructor
StudentFaraDestructor::StudentFaraDestructor(const std::string& nume, std::size_t capacitateNote)
    : nume_(nume)
    , note_(new int[capacitateNote])
{
    ++ContorHeap::blocuriActive;
    std::cout << "    [constructor]      " << nume_ << ": new int[" << capacitateNote << "] @" << note_
              << "  (nu va fi eliberat niciodata)\n";
}

// Clasa pentru StudentFaraCopyCtor
StudentFaraCopyCtor::StudentFaraCopyCtor(const std::string& nume, std::size_t capacitateNote)
    : nume_(nume)
    , numarNote_(0)
    , note_(new int[capacitateNote])
{
    ++ContorHeap::blocuriActive;
    std::cout << "    [constructor]      " << nume_ << ": new int[" << capacitateNote << "] @" << note_ << "\n";
}

StudentFaraCopyCtor::~StudentFaraCopyCtor()
{
    std::cout << "    [destructor]       " << nume_ << ": delete[] @" << note_ << std::endl;
    delete[] note_;
    --ContorHeap::blocuriActive;
}

void StudentFaraCopyCtor::adaugaNota(int nota)
{
    note_[numarNote_++] = nota;
}

void StudentFaraCopyCtor::corecteazaNota(std::size_t index, int notaNoua)
{
    note_[index] = notaNoua;
}

void StudentFaraCopyCtor::afiseaza(const char* eticheta) const
{
    std::cout << "    " << eticheta << nume_ << " | note: [";
    for (std::size_t i = 0; i < numarNote_; ++i)
    {
        std::cout << (i ? " " : "") << note_[i];
    }
    std::cout << "] @" << note_ << "\n";
}

} // namespace tema
