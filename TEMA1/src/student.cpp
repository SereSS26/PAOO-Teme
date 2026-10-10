#include <tema/student.hpp>
#include <tema/contor_heap.hpp>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>

namespace tema {

// Constructor Student
Student::Student(int matricol, const std::string& nume, std::size_t capacitateNote)
    : matricol_(matricol)
    , nume_(nume)
    , capacitate_(capacitateNote)
    , numarNote_(0)
    , note_(new int[capacitateNote])
{
    ++ContorHeap::blocuriActive;
    std::cout << "    [constructor]      " << nume_ << ": new int[" << capacitate_ << "] @" << note_ << "\n";
}

// Copy constructor Student
Student::Student(const Student& other)
    : matricol_(other.matricol_)
    , nume_(other.nume_)
    , capacitate_(other.capacitate_)
    , numarNote_(other.numarNote_)
    , note_(new int[other.capacitate_])     // memorie NOUA, proprie copiei
{
    // copiem CONTINUTUL tabloului, nu adresa lui (deep copy)
    std::copy(other.note_, other.note_ + other.numarNote_, note_);
    ++ContorHeap::blocuriActive;
    std::cout << "    [copy constructor] " << nume_ << ": new int[" << capacitate_ << "] @" << note_
              << " (note copiate din @" << other.note_ << ")\n";
}

// Move constructor Student
Student::Student(Student&& other) noexcept
    : matricol_(other.matricol_)
    , nume_(std::move(other.nume_))
    , capacitate_(other.capacitate_)
    , numarNote_(other.numarNote_)
    , note_(other.note_)                    // preluam pointerul, fara new[] si fara copiere
{
    // Sursa nu mai detine memoria. Fara linia de mai jos, destructorul sursei
    // ar elibera tabloul pe care tocmai l-am preluat (dangling pointer + double free).
    other.note_ = nullptr;
    other.capacitate_ = 0;
    other.numarNote_ = 0;
    std::cout << "    [move constructor] " << nume_ << ": preia @" << note_ << " (fara alocare)\n";
}

// Destructor Student
Student::~Student()
{
    if (note_ != nullptr)
    {
        std::cout << "    [destructor]       " << nume_ << ": delete[] @" << note_ << "\n";
        delete[] note_;
        --ContorHeap::blocuriActive;
    }
    else
    {
        std::cout << "    [destructor]       (obiect mutat, nu mai detine memorie)\n";
    }
}

// Aici sunt metodele pentru clasa Student
bool Student::adaugaNota(int nota)
{
    if (!notaValida(nota) || numarNote_ == capacitate_)
    {
        return false;
    }
    note_[numarNote_++] = nota;
    return true;
}

bool Student::corecteazaNota(std::size_t index, int notaNoua)
{
    if (index >= numarNote_ || !notaValida(notaNoua))
    {
        return false;
    }
    note_[index] = notaNoua;
    return true;
}

int Student::matricol() const
{
    return matricol_;
}

const std::string& Student::nume() const
{
    return nume_;
}

std::size_t Student::numarNote() const
{
    return numarNote_;
}

double Student::medie() const
{
    if (numarNote_ == 0)
    {
        return 0.0;
    }
    int suma = 0;
    for (std::size_t i = 0; i < numarNote_; ++i)
    {
        suma += note_[i];
    }
    return static_cast<double>(suma) / numarNote_;
}

void Student::afiseaza() const
{
    std::cout << "    #" << matricol_ << " " << (nume_.empty() ? "(fara nume)" : nume_) << " | note: [";
    for (std::size_t i = 0; i < numarNote_; ++i)
    {
        std::cout << (i ? " " : "") << note_[i];
    }
    std::cout << "] @" << note_ << " | medie " << std::fixed << std::setprecision(2) << medie() << "\n";
}

bool Student::notaValida(int nota)
{
    return nota >= 1 && nota <= 10;
}

} // namespace tema
