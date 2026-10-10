#ifndef TEMA_EXEMPLE_GRESITE_HPP
#define TEMA_EXEMPLE_GRESITE_HPP

#include <cstddef>
#include <string>

namespace tema {

// GRESIT INTENTIONAT: aloca memorie in constructor, dar NU are destructor.
// Destructorul generat de compilator distruge doar pointerul (8 octeti),
// NU si memoria spre care arata -> memory leak la fiecare obiect.
class StudentFaraDestructor
{
public:
    StudentFaraDestructor(const std::string& nume, std::size_t capacitateNote);

    // lipseste: ~StudentFaraDestructor() { delete[] note_; }

private:
    std::string nume_;
    int*        note_;
};

// GRESIT INTENTIONAT: are destructor, dar NU are copy constructor.
// Copy constructor-ul generat de compilator copiaza membru cu membru, deci copiaza
// ADRESA din note_, nu notele (shallow copy). Originalul si copia ajung sa imparta
// acelasi tablou, iar la final ambii destructori fac delete[] pe aceeasi adresa.
class StudentFaraCopyCtor
{
public:
    StudentFaraCopyCtor(const std::string& nume, std::size_t capacitateNote);

    ~StudentFaraCopyCtor();

    // lipseste: StudentFaraCopyCtor(const StudentFaraCopyCtor& other);

public:
    void adaugaNota(int nota);
    void corecteazaNota(std::size_t index, int notaNoua);
    void afiseaza(const char* eticheta) const;

private:
    std::string nume_;
    std::size_t numarNote_;
    int*        note_;
};

} // namespace tema

#endif // TEMA_EXEMPLE_GRESITE_HPP
