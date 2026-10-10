# Tema 3 – Catalog de studenți (PAOO)

Aplicație C++ (proof of life, temă individuală) care ilustrează:

| # | Cerință | Unde în cod | Punctaj |
|---|---------|-------------|---------|
| 1 | Build cu CMake (structura din laborator) | `CMakeLists.txt`, `inc/`, `src/` | 3p |
| 2 | Încapsulare | `Student` – membri `private`, metode care validează | 1p |
| 3 | Inițializarea membrilor din constructor | lista de inițializare din `Student`, `demoListaInitializare()` | 1p |
| 4 | Eliberarea heap-ului în destructor | `Student::~Student()` vs `StudentFaraDestructor` | 1p |
| 5 | Copy constructor | `Student(const Student&)` vs `StudentFaraCopyCtor` | 2p |
| 6 | Move constructor | `Student(Student&&) noexcept` | 2p |

Fiecare constructor și destructor afișează un mesaj și adresa memoriei de pe heap, ca să se vadă clar ce se alocă, ce se copiază și ce se eliberează.

---

## Structura proiectului

```
LAB3/
├── CMakeLists.txt              # proiectul: biblioteca statica + executabilul
├── inc/tema/                   # headere (incluse cu prefix: #include <tema/student.hpp>)
│   ├── student.hpp             # clasa CORECTA
│   ├── exemple_gresite.hpp     # clase GRESITE intentionat (contra-exemple)
│   ├── contor_heap.hpp         # contor de blocuri new[] neeliberate
│   └── demo.hpp                # cate o functie demonstrativa pe cerinta
├── src/
│   ├── student.cpp
│   ├── exemple_gresite.cpp
│   ├── contor_heap.cpp
│   ├── demo.cpp
│   └── main.cpp
└── build/                      # generat de CMake (nu se preda)
```

## Compilare și rulare

```bash
cd LAB3
mkdir build && cd build
cmake ..
make
./tema3          # toate demonstratiile corecte
./tema3 crash    # ce se intampla FARA copy constructor (programul se opreste)
```

---

## Pas 1 – CMake (3p)

Am urmat structura din `LAB2/cmake/02` și `myfolder`, cum e descrisă în `Cmake.docx`:

1. **`cmake_minimum_required` + `project`**. Am folosit versiunea 3.10 în loc de 3.0, pentru că CMake 3.29 dă avertisment pentru versiuni < 3.5, iar CMake 4.x dă eroare.
2. **Standardul C++14**: `CMAKE_CXX_STANDARD 14`, `..._REQUIRED ON`, `..._EXTENSIONS OFF`. Move constructor-ul există doar de la C++11.
3. **Separare `inc/` și `src/`**. Headerele stau în `inc/tema/` și se includ cu prefix: `#include <tema/student.hpp>`, ca în exemplul `02`.
4. **Bibliotecă statică**: `add_library(tema_lib STATIC ${lib_files})` conține toate clasele.
5. **`target_include_directories(tema_lib PUBLIC $<BUILD_INTERFACE:.../inc> $<INSTALL_INTERFACE:inc>)`**. `PUBLIC` înseamnă că folderul `inc/` e folosit atât la compilarea bibliotecii, cât și de oricine se leagă de ea. Din această cauză `main.cpp` nu are nevoie de `#include "../inc/..."`.
6. **Executabil**: `add_executable(tema3 src/main.cpp)` + `target_link_libraries(tema3 PRIVATE tema_lib)`.
7. **Build out-of-source**: totul se generează în `build/`, iar sursele rămân curate.

## Pas 2 – Încapsulare (1p)

Datele `Student`-ului (`note_`, `numarNote_`, `capacitate_`) sunt `private`. Se pot modifica numai prin `adaugaNota()` și `corecteazaNota()`, care verifică:
- nota să fie între 1 și 10;
- să mai fie loc în tablou, altfel s-ar scrie în afara memoriei alocate (buffer overflow);
- indexul notei corectate să existe.

```
>> ana.adaugaNota(15);   ->  RESPINS - nota trebuie sa fie intre 1 si 10
>> ana.adaugaNota(6);    ->  RESPINS - catalogul e plin
```

Accesul direct nu compilează: `ana.note_[7] = 100;` → `error: 'note_' is a private member of 'tema::Student'`.

**De ce e nevoie:** dacă `note_` ar fi public, oricine ar putea scrie `note_[100] = 5` și ar corupe memoria. Încapsularea garantează că obiectul e mereu într-o stare validă.

## Pas 3 – Inițializarea membrilor în constructor (1p)

```cpp
Student::Student(int matricol, const std::string& nume, std::size_t capacitateNote)
    : matricol_(matricol)            // lista de initializare
    , nume_(nume)
    , capacitate_(capacitateNote)
    , numarNote_(0)
    , note_(new int[capacitateNote])
{ ... }
```

**De ce e nevoie:** membrii unui obiect sunt construiți **înainte** de a se intra în corpul `{ }` al constructorului.

- **Uneori lista e obligatorie.** `matricol_` e `const int`, deci nu i se poate atribui o valoare în corp:
  `error: cannot assign to non-static data member 'matricol_' with const-qualified type 'const int'`.
  Același lucru e valabil pentru referințe și pentru membri care nu au constructor implicit.
- **Altfel se face muncă dublă.** Demonstrația folosește clasa `Eticheta`, care afișează fiecare operație:
  ```
  MaterieAtribuireInCorp m1("PAOO");     MaterieListaInitializare m2("PAOO");
    Eticheta()   implicit                  Eticheta("PAOO")  direct
    Eticheta::operator=  suprascrie
  ```
- **Ordinea contează.** Membrii se inițializează în ordinea în care sunt **declarați** în clasă, nu în ordinea din listă. De aceea `note_` (care folosește `capacitate_`) e declarat după `capacitate_`.

## Pas 4 – Eliberarea heap-ului în destructor (1p)

```cpp
Student::~Student()
{
    if (note_ != nullptr) { delete[] note_; ... }
}
```

**De ce e nevoie:** destructorul generat automat distruge doar **pointerul** (8 octeți), nu și memoria spre care arată. Comparația din program:

```
a) Student (CU destructor):              blocuri heap pierdute: 0
b) StudentFaraDestructor (FARA):         blocuri heap pierdute: 3
```

Confirmat și cu unealta sistemului (`leaks --atExit -- ./build/tema3` pe macOS, `valgrind --leak-check=full ./build/tema3` pe Linux): **3 leaks**, exact cele 3 obiecte `StudentFaraDestructor`.

## Pas 5 – Copy constructor (2p)

```cpp
Student::Student(const Student& other)
    : ..., note_(new int[other.capacitate_])               // memorie NOUA
{
    std::copy(other.note_, other.note_ + other.numarNote_, note_);   // copiem CONTINUTUL
}
```

**De ce e nevoie:** copy constructor-ul generat de compilator copiază membru cu membru. Pentru `int* note_` se copiază doar **adresa** (shallow copy). Rulează `./tema3 crash` ca să vezi efectul:

```
>> StudentFaraCopyCtor copie = original;
>> copie.corecteazaNota(0, 10);           // modificam DOAR copia
   original: note: [10 9] @0x...6040      <- s-a stricat si originalul!
   copie:    note: [10 9] @0x...6040      <- ACEEASI adresa
   [destructor] delete[] @0x...6040
   [destructor] delete[] @0x...6040       <- double free -> abort (exit code 134)
```

Cu copy constructor-ul propriu (`./tema3`), cele două obiecte au adrese diferite și sunt independente:

```
original: note: [8 9]  @0x...4b50
copie:    note: [10 9] @0x...4a40
```

Demonstrația arată și că copy constructor-ul se apelează **implicit** când un obiect e transmis prin valoare (`raportPrinValoare(original)`).

## Pas 6 – Move constructor (2p)

```cpp
Student::Student(Student&& other) noexcept
    : ..., note_(other.note_)       // preluam pointerul - fara new[], fara copiere
{
    other.note_ = nullptr;          // sursa nu mai detine memoria
}
```

**De ce e nevoie:** când sursa este un obiect temporar sau unul marcat cu `std::move`, e inutil să alocăm memorie nouă și să copiem tot. Putem pur și simplu „fura” tabloul sursei.

```
Student copie = sursa;              -> alocari noi: 1  (new int[1000] + copiere)
Student mutat = std::move(sursa);   -> alocari noi: 0  (doar se preia pointerul)
   sursa: note: [] @0               <- sursa a ramas goala
   mutat: note: [10 9] @<aceeasi adresa pe care o avea sursa>
```

În `std::vector`:
- la `push_back(Student(...))`, obiectul temporar e **mutat** în vector, nu copiat;
- când vectorul se mărește, elementele vechi sunt **mutate** în noua zonă de memorie.

Detalii importante:
- **`other.note_ = nullptr`**: fără această linie, destructorul sursei ar elibera tabloul preluat, iar destinația ar rămâne cu un pointer invalid (și ar urma un double free).
- **`noexcept`**: `std::vector` folosește move constructor-ul la redimensionare doar dacă e marcat `noexcept`. Altfel, din motive de siguranță la excepții, copiază.
- Dacă o clasă are copy constructor sau destructor declarat de noi, compilatorul **nu** mai generează automat un move constructor. Atunci `std::move` ar apela tot copy constructor-ul, adică o copie completă.

---

## Întrebări posibile la prezentare

- **Regula celor 3/5.** Dacă o clasă are nevoie de destructor, are nevoie și de copy constructor și de `operator=`. De la C++11 se adaugă move constructor și move assignment. Atribuirile nu erau cerute în temă, așa că le-am **interzis explicit** (`= delete`). Altfel compilatorul ar fi generat un `operator=` care copiază doar pointerul. În plus, `matricol_` e `const`, deci atribuirea nu are sens.
- **De ce `if (note_ != nullptr)` în destructor?** `delete[] nullptr` e sigur, dar verificarea separă cazul „obiect mutat”: nu mai are ce elibera și nu trebuie scăzut din contor.
- **De ce `Student s = creeaza();` nu apelează move constructor-ul?** Din cauza *copy elision* / RVO: compilatorul construiește obiectul direct la destinație. De aceea demonstrația folosește `std::move` explicit și `std::vector`.
