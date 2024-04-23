#include <iostream>
#include <string>
#include <vector>

struct Faculty
{
    std::string name;
    std::vector<Faculty*> departments;

    Faculty(std::string name) : name(name) {};
};

class University {
public:
    Faculty* rootElement;

    void displayElements(Faculty* departments, int depth = 0) {
        if (departments == nullptr) {
            return;
        };
        for (int i = 0; i < depth; i++) {
            std::cout << "\t";
        };
        std::cout << "- " << departments->name << "\n";
        for (Faculty* departments : departments->departments) {
            displayElements(departments, depth + 1);
        };
    };

    void addNodes(const std::string name, Faculty* node) {
        Faculty* newDepartment = new Faculty(name);
        if (node == nullptr) {
            rootElement = newDepartment;
        }
        else {
            node->departments.push_back(newDepartment);
        };
    };

    void displayTree() {
        if (rootElement == nullptr) {
            std::cout << "University is empty." << std::endl;
        }
        else {
            std::cout << "University Structure:" << std::endl;
            displayElements(rootElement);
        };
    };

    ~University() {
        deleteTree(rootElement);
    };

    void deleteTree(Faculty* departments) {
        if (departments == nullptr) {
            return;
        };
        for (Faculty* departments : departments->departments) {
            deleteTree(departments);
        };
        delete departments;
    };
};

int main() {
    setlocale(LC_CTYPE, "Polish");

    University university;

    Faculty* rootElement = new Faculty("Rektor");
    university.addNodes("Wydzia� Nawigacyjny Dziekan", rootElement);
    university.addNodes("Wydzia� Mechaniczny Dziekan", rootElement);
    university.addNodes("Wydzai� In�ynieryjno-Ekonomiczny Transportu Dziekan", rootElement);
    university.addNodes("Wydzai� Informatyczny i Telekomunikacyjny Dziekan", rootElement);
    university.addNodes("Wydzai� Mechatroniki i Elektrotechniki Dziekan", rootElement);

    Faculty* nawigacja = rootElement->departments[0];
    university.addNodes("Dziekanat", nawigacja);
    university.addNodes("Wydza�owe Centrum Kszta�cenia", nawigacja);
    university.addNodes("Katedra In�ynierii Ruchu Morskiego", nawigacja);
    university.addNodes("Katedra Nawigacji Morskiej", nawigacja);
    university.addNodes("Katedra Symulacji Morskich", nawigacja);
    university.addNodes("Katedra Ratownictwa i Zarz�dzania Ryzykiem", nawigacja);
    university.addNodes("Katedra Oceanotechniki i Budowy Okr�t�w", nawigacja);
    university.addNodes("Katedra Geoinformatyki i Hydrografii", nawigacja);
    university.addNodes("Katedra Geodezji i Pomiar�w offshore", nawigacja);

    Faculty* mechanika = rootElement->departments[1];
    university.addNodes("Dziekanat", mechanika);
    university.addNodes("Wydza�owe Centrum Kszta�cenia", mechanika);
    university.addNodes("Centrum Badania Paliw, Cieczy Roboczych i Ochrony �rodowiska", mechanika);
    university.addNodes("Katedra Podstaw Budowy Maszyn i Materia�oznawstwa", mechanika);
    university.addNodes("Katedra Si�owni Okr�towych", mechanika);
    university.addNodes("Katedra Diagnostyki i Remont�w Maszyn", mechanika);
    university.addNodes("Katedra Energetyki", mechanika);
    university.addNodes("Centrum Badawcze Recyklingu i Diagnostyki Element�w", mechanika);

    Faculty* WCK = mechanika->departments[1];
    university.addNodes("Laboratorium Si�owni Okr�towych", WCK);
    university.addNodes("Laboratorium Technik Wytwarzania. Warsztaty Mechaniczne", WCK);


    Faculty* inzEkon = rootElement->departments[2];
    university.addNodes("Dziekanat", inzEkon);
    university.addNodes("Wydzia�owe Centrum Kszta�cenia", inzEkon);
    university.addNodes("Katedra Zarz�dzania i Logistyki", inzEkon);
    university.addNodes("Katedra Gospodarki Morskiej i System�w Transportowych", inzEkon);
    university.addNodes("Katedra Proces�w Technologicznych", inzEkon);
    university.addNodes("Katedra Ochrony �rodowska i Towaroznastwa", inzEkon);

    Faculty* infTel = rootElement->departments[3];
    university.addNodes("Dziekanat", infTel); // 0
    university.addNodes("Wydzia�owe Centrum Kszta�cenia", infTel); //1
    university.addNodes("Katedra Informatyki", infTel);
    university.addNodes("Katedra Telekomunikacji", infTel);

    Faculty* test = infTel->departments[2];
    university.addNodes("test", test);

    Faculty* MechEkek = rootElement->departments[4];
    university.addNodes("Dziekanat", MechEkek);
    university.addNodes("Wydzia�owe Centrum Kszta�cenia", MechEkek);
    university.addNodes("Katedra Elektrotechniki i Energoelektroniki", MechEkek);
    university.addNodes("Katedra Automatyki Okr�towej", MechEkek);

    university.rootElement = rootElement;

    university.displayTree();

};