#include <tema/demo.hpp>
#include <cstring>

int main(int argc, char* argv[])
{
    // ./tema3 crash  -> arata ce se intampla fara copy constructor 
    if (argc > 1 && std::strcmp(argv[1], "crash") == 0)
    {
        tema::demoFaraCopyConstructor();
        return 0;
    }
    // Am testat e ok tot, comentarile mele auxiliare pot sa le stergi sau cum vrei, Artur nu se supara
    // Daca ai intrebari ma intrebi ca sunt mai bun ca Claude
    // Sunt cam trist de ceva zile si chiar nu stiu cum sa fac sa fie bine

    tema::demoIncapsulare();

    //tema::demoListaInitializare();

    //tema::demoDestructor();

    //tema::demoCopyConstructor();

    //tema::demoMoveConstructor();

    //tema::raportHeap();

    return 0;
}
