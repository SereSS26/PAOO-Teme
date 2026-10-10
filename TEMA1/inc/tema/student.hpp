#ifndef TEMA_STUDENT_HPP
#define TEMA_STUDENT_HPP

#include <cstddef>
#include <string>

namespace tema {

class Student
{
public:
    Student(int matricol, const std::string& nume, std::size_t capacitateNote);
    Student(const Student& other);          
    Student(Student&& other) noexcept;      
    ~Student();

    // Atribuirile nu fac parte din tema. Le interzicem explicit: altfel compilatorul ar genera
    // un operator= care copiaza doar pointerul (aceeasi problema ca la lipsa copy constructor-ului).
    // Oricum, membrul const matricol_ face ca atribuirea sa nu aiba sens.
    Student& operator=(const Student&) = delete;
    Student& operator=(Student&&) = delete;

public:
    bool adaugaNota(int nota);
    bool corecteazaNota(std::size_t index, int notaNoua);
    int matricol() const;
    const std::string& nume() const;
    std::size_t numarNote() const;
    double medie() const;
    void afiseaza() const;

private:
    static bool notaValida(int nota);

private:
    // Membrii se initializeaza in ORDINEA DECLARARII (nu in ordinea din lista de initializare),
    // de aceea note_ (care foloseste capacitate_) este declarat dupa capacitate_.
    const int   matricol_;      // const -> poate primi valoare DOAR in lista de initializare
    std::string nume_;
    std::size_t capacitate_;
    std::size_t numarNote_;
    int*        note_;          // resursa de pe heap detinuta de obiect
};

} // namespace tema

#endif // TEMA_STUDENT_HPP
