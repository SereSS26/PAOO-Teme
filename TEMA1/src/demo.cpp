#include <tema/demo.hpp>
#include <tema/contor_heap.hpp>
#include <tema/exemple_gresite.hpp>
#include <tema/student.hpp>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace tema {
namespace {
//Functii ajutatoare ca sa fie afisarea mai wow
void titlu(const char* text)
{
    std::cout << "\n=== " << text << " ===\n";
}
void pas(const char* cod)
{
    std::cout << "  >> " << cod << "\n";
}
void explica(const char* text)
{
    std::cout << "  => " << text << "\n";
}
const char* rezultat(bool acceptat)
{
    return acceptat ? "acceptat" : "RESPINS";
}
// Clase ajutatoare pentru lista de initializare
// Eticheta este un membru al clasei MaterieAtribuireInCorp si MaterieListaInitializare
class Eticheta
{
public:
    // Constructor cu valoare implicita (fara parametri) si constructor cu parametru
    Eticheta()
        : text_("(gol)")
    {
        std::cout << "    Eticheta()              -> construit cu valoarea implicita \"" << text_ << "\"\n";
    }
    // Constructor cu parametru
    explicit Eticheta(const std::string& text)
        : text_(text)
    {
        std::cout << "    Eticheta(\"" << text_ << "\") -> construit direct cu valoarea finala\n";
    }
    // Operator de atribuire
    Eticheta& operator=(const std::string& text)
    {
        std::cout << "    Eticheta::operator=     -> suprascrie \"" << text_ << "\" cu \"" << text << "\"\n";
        text_ = text;
        return *this;
    }
private:
    std::string text_;
};
// Clase care demonstreaza diferenta intre initializare in lista si atribuire in corp
class MaterieAtribuireInCorp
{
public:
    explicit MaterieAtribuireInCorp(const std::string& titlu)
    {                       // aici titlu_ a fost DEJA construit cu Eticheta()
        titlu_ = titlu;     // a doua operatie: atribuire
    }

private:
    Eticheta titlu_;
};

// Clase care demonstreaza diferenta intre initializare in lista si atribuire in corp
class MaterieListaInitializare
{
public:
    explicit MaterieListaInitializare(const std::string& titlu)
        : titlu_(titlu)     // o singura operatie: construire directa
    {
    }
private:
    Eticheta titlu_;
};
// Functie care primeste un Student prin valoare (se apeleaza copy constructor-ul)
void raportPrinValoare(Student s)
{
    std::cout << "    (in functie) ";
    s.afiseaza();
}
} // namespace

// Demo1 pentru incapsulare
void demoIncapsulare()
{
    titlu("2. INCAPSULARE");
    explica("datele (note_, numarNote_) sunt private; se modifica doar prin metode care le valideaza");
    pas("Student ana(1001, \"Ana Popescu\", 3);   // catalog cu loc pentru 3 note");

    Student ana(1001, "Ana Popescu", 3);

    pas("ana.adaugaNota(9);");
    std::cout << "    " << rezultat(ana.adaugaNota(9)) << "\n";

    pas("ana.adaugaNota(15);");
    std::cout << "    " << rezultat(ana.adaugaNota(15)) << " - nota trebuie sa fie intre 1 si 10\n";

    pas("ana.adaugaNota(7); ana.adaugaNota(10);");
    std::cout << "    " << rezultat(ana.adaugaNota(7)) << ", " << rezultat(ana.adaugaNota(10)) << "\n";

    pas("ana.adaugaNota(6);");
    std::cout << "    " << rezultat(ana.adaugaNota(6))
              << " - catalogul e plin; fara verificare s-ar scrie in afara tabloului (buffer overflow)\n";

    pas("ana.corecteazaNota(5, 10);");
    std::cout << "    " << rezultat(ana.corecteazaNota(5, 10)) << " - nu exista nota cu indexul 5\n";

    pas("ana.afiseaza();");
    ana.afiseaza();

    explica("ana.note_[7] = 100;  NU compileaza: 'note_' is a private member of 'tema::Student'");
    explica("starea obiectului nu poate fi stricata din exterior");
}

// Demo2 pentru lista de initializare
void demoListaInitializare()
{
    titlu("3. INITIALIZAREA MEMBRILOR IN CONSTRUCTOR");
    explica("un membru este construit INAINTE sa se intre in corpul constructorului");

    pas("MaterieAtribuireInCorp m1(\"PAOO\");    // titlu_ = titlu; in corp");
    MaterieAtribuireInCorp m1("PAOO");
    explica("2 operatii: construire implicita + atribuire (munca facuta de doua ori)");

    pas("MaterieListaInitializare m2(\"PAOO\");  // : titlu_(titlu)");
    MaterieListaInitializare m2("PAOO");
    explica("1 operatie: membrul este construit direct cu valoarea corecta");

    std::cout << "\n";
    explica("pentru unii membri lista de initializare este OBLIGATORIE:");
    std::cout << "     - membri const (Student::matricol_), referinte, membri fara constructor implicit\n";
    std::cout << "     - matricol_ = matricol; in corp -> eroare: cannot assign to non-static data member\n";
    std::cout << "       'matricol_' with const-qualified type 'const int'\n";

    pas("Student ion(1002, \"Ion Ionescu\", 2);  // matricol_(matricol), note_(new int[...]) in lista");
    Student ion(1002, "Ion Ionescu", 2);
    std::cout << "    matricol = " << ion.matricol() << " (fixat la creare, nu se mai poate schimba)\n";
}

// Demo3 pentru destructor
void demoDestructor()
{
    titlu("4. ELIBERAREA HEAP-ULUI IN DESTRUCTOR");

    std::cout << "  a) Student - ARE destructor cu delete[]\n";
    int inainte = ContorHeap::blocuriActive;
    pas("for (i = 1..3) { Student s(...); }   // s este distrus la fiecare '}'");
    for (int i = 1; i <= 3; ++i)
    {
        Student s(2000 + i, "Student " + std::to_string(i), 4);
    }
    std::cout << "    blocuri heap pierdute: " << ContorHeap::blocuriActive - inainte << "\n";

    std::cout << "\n  b) StudentFaraDestructor - NU are destructor\n";
    inainte = ContorHeap::blocuriActive;
    pas("for (i = 1..3) { StudentFaraDestructor s(...); }");
    for (int i = 1; i <= 3; ++i)
    {
        StudentFaraDestructor s("Fantoma " + std::to_string(i), 4);
    }
    std::cout << "    blocuri heap pierdute: " << ContorHeap::blocuriActive - inainte << "\n";
    explica("obiectele au disparut, dar memoria lor a ramas ocupata si nimeni nu mai are adresa ei");
    explica("(memory leak) - intr-un program care ruleaza mult, memoria se consuma treptat");
}

// Demo4 pentru copy constructor
void demoCopyConstructor()
{
    titlu("5. COPY CONSTRUCTOR (copie in adancime)");

    pas("Student original(3001, \"Maria Ionescu\", 5); + note 8 si 9");
    Student original(3001, "Maria Ionescu", 5);
    original.adaugaNota(8);
    original.adaugaNota(9);

    pas("Student copie = original;");
    Student copie = original;

    pas("copie.corecteazaNota(0, 10);   // modificam DOAR copia");
    copie.corecteazaNota(0, 10);

    std::cout << "    original:"; original.afiseaza();
    std::cout << "    copie:   "; copie.afiseaza();
    explica("adrese diferite -> fiecare obiect are propriul tablou; originalul a ramas cu nota 8");

    std::cout << "\n";
    explica("copy constructor-ul se apeleaza si implicit, la transmiterea prin valoare:");
    pas("raportPrinValoare(original);");
    raportPrinValoare(original);

    std::cout << "\n";
    explica("cum arata FARA copy constructor propriu: ruleaza  ./build/tema3 crash");
}

// Demo5 pentru move constructor
void demoMoveConstructor()
{
    titlu("6. MOVE CONSTRUCTOR (transfer fara copiere)");

    std::cout << "  a) copiere vs mutare\n";
    pas("Student sursa(4001, \"Andrei Pop\", 1000); + note 10 si 9");
    Student sursa(4001, "Andrei Pop", 1000);
    sursa.adaugaNota(10);
    sursa.adaugaNota(9);

    int inainte = ContorHeap::blocuriActive;
    pas("Student copie = sursa;                 // copy");
    Student copie = sursa;
    std::cout << "    alocari noi: " << ContorHeap::blocuriActive - inainte
              << " (new int[1000] + copierea notelor)\n";

    inainte = ContorHeap::blocuriActive;
    pas("Student mutat = std::move(sursa);      // move");
    Student mutat = std::move(sursa);
    std::cout << "    alocari noi: " << ContorHeap::blocuriActive - inainte << " (doar se preia pointerul)\n";

    std::cout << "    sursa: "; sursa.afiseaza();
    std::cout << "    mutat: "; mutat.afiseaza();
    explica("'mutat' are exact adresa pe care o avea 'sursa'; sursa a ramas goala (note_ = nullptr)");

    std::cout << "\n  b) obiecte temporare si std::vector\n";
    explica("un temporar oricum dispare -> e mai ieftin sa ii 'furam' memoria decat sa o copiem");
    std::vector<Student> grupa;

    pas("grupa.push_back(Student(4002, \"Elena\", 5));");
    grupa.push_back(Student(4002, "Elena", 5));

    pas("grupa.push_back(Student(4003, \"Mihai\", 5));   // vectorul se mareste");
    grupa.push_back(Student(4003, "Mihai", 5));

    pas("grupa.push_back(Student(4004, \"Ioana\", 5));   // vectorul se mareste");
    grupa.push_back(Student(4004, "Ioana", 5));

    explica("la marire, vectorul MUTA elementele vechi (move ctor e noexcept), nu le copiaza");
    explica("fara move constructor, fiecare pas de mai sus ar fi facut new[] + copiere completa");

    std::cout << "\n  -- sfarsitul functiei: se distrug toate obiectele locale --\n";
}

// Demo extra exemplu fara copy constructor
void demoFaraCopyConstructor()
{
    titlu("FARA COPY CONSTRUCTOR (copie superficiala)");

    pas("StudentFaraCopyCtor original(\"Maria Ionescu\", 5); + note 8 si 9");
    StudentFaraCopyCtor original("Maria Ionescu", 5);
    original.adaugaNota(8);
    original.adaugaNota(9);

    pas("StudentFaraCopyCtor copie = original;   // copy ctor generat de compilator");
    StudentFaraCopyCtor copie = original;
    explica("niciun mesaj de constructor si nicio alocare: s-a copiat doar valoarea pointerului");

    pas("copie.corecteazaNota(0, 10);   // modificam DOAR copia");
    copie.corecteazaNota(0, 10);

    original.afiseaza("original: ");
    copie.afiseaza("copie:    ");
    explica("ACEEASI adresa -> modificarea copiei a stricat si originalul (8 a devenit 10)");

    std::cout << "\n";
    explica("la iesirea din functie ambii destructori fac delete[] pe ACEEASI adresa");
    explica("-> double free -> programul este oprit de sistem");
    std::cout << std::endl;
}

void raportHeap()
{
    std::cout << "\n=== FINAL ===\n";
    std::cout << "  blocuri heap neeliberate: " << ContorHeap::blocuriActive
              << " (toate provin de la StudentFaraDestructor)\n";
}

} // namespace tema
