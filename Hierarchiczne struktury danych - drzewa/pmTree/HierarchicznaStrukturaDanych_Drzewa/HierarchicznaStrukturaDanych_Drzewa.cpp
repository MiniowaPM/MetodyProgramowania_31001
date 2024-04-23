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
    university.addNodes("Wydzia³ Nawigacyjny Dziekan", rootElement);
    university.addNodes("Wydzia³ Mechaniczny Dziekan", rootElement);
    university.addNodes("Wydzai³ In¿ynieryjno-Ekonomiczny Transportu Dziekan", rootElement);
    university.addNodes("Wydzai³ Informatyczny i Telekomunikacyjny Dziekan", rootElement);
    university.addNodes("Wydzai³ Mechatroniki i Elektrotechniki Dziekan", rootElement);

    Faculty* nawigacja = rootElement->departments[0];
    university.addNodes("Dziekanat", nawigacja);
    university.addNodes("Wydza³owe Centrum Kszta³cenia", nawigacja);
    university.addNodes("Katedra In¿ynierii Ruchu Morskiego", nawigacja);
    university.addNodes("Katedra Nawigacji Morskiej", nawigacja);
    university.addNodes("Katedra Symulacji Morskich", nawigacja);
    university.addNodes("Katedra Ratownictwa i Zarz¹dzania Ryzykiem", nawigacja);
    university.addNodes("Katedra Oceanotechniki i Budowy Okrêtów", nawigacja);
    university.addNodes("Katedra Geoinformatyki i Hydrografii", nawigacja);
    university.addNodes("Katedra Geodezji i Pomiarów offshore", nawigacja);

    Faculty* mechanika = rootElement->departments[1];
    university.addNodes("Dziekanat", mechanika);
    university.addNodes("Wydza³owe Centrum Kszta³cenia", mechanika);
    university.addNodes("Centrum Badania Paliw, Cieczy Roboczych i Ochrony Œrodowiska", mechanika);
    university.addNodes("Katedra Podstaw Budowy Maszyn i Materia³oznawstwa", mechanika);
    university.addNodes("Katedra Si³owni Okrêtowych", mechanika);
    university.addNodes("Katedra Diagnostyki i Remontów Maszyn", mechanika);
    university.addNodes("Katedra Energetyki", mechanika);
    university.addNodes("Centrum Badawcze Recyklingu i Diagnostyki Elementów", mechanika);

    Faculty* WCK = mechanika->departments[1];
    university.addNodes("Laboratorium Si³owni Okrêtowych", WCK);
    university.addNodes("Laboratorium Technik Wytwarzania. Warsztaty Mechaniczne", WCK);


    Faculty* inzEkon = rootElement->departments[2];
    university.addNodes("Dziekanat", inzEkon);
    university.addNodes("Wydzia³owe Centrum Kszta³cenia", inzEkon);
    university.addNodes("Katedra Zarz¹dzania i Logistyki", inzEkon);
    university.addNodes("Katedra Gospodarki Morskiej i Systemów Transportowych", inzEkon);
    university.addNodes("Katedra Procesów Technologicznych", inzEkon);
    university.addNodes("Katedra Ochrony Œrodowska i Towaroznastwa", inzEkon);

    Faculty* infTel = rootElement->departments[3];
    university.addNodes("Dziekanat", infTel); // 0
    university.addNodes("Wydzia³owe Centrum Kszta³cenia", infTel); //1
    university.addNodes("Katedra Informatyki", infTel);
    university.addNodes("Katedra Telekomunikacji", infTel);

    Faculty* test = infTel->departments[2];
    university.addNodes("test", test);

    Faculty* MechEkek = rootElement->departments[4];
    university.addNodes("Dziekanat", MechEkek);
    university.addNodes("Wydzia³owe Centrum Kszta³cenia", MechEkek);
    university.addNodes("Katedra Elektrotechniki i Energoelektroniki", MechEkek);
    university.addNodes("Katedra Automatyki Okrêtowej", MechEkek);

    university.rootElement = rootElement;

    university.displayTree();

};