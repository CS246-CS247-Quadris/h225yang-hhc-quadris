#include "subject.h"

void Subject::attach(Observer *o) {
    display.push_back(o);
}

void Subject::notifyAll() {
    for (auto &ob : display) ob->notify(&(*this));
}

void Subject::subscript() {
    display.clear();
}
