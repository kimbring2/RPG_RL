#include "ViZGolfGamePython.h"

void GolfGamePython::print() {
    cout << "id: " << this->id << endl;
    this->setId(111);
    cout << "id: " << this->id << endl;
}