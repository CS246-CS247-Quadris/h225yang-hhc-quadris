#ifndef ASSIGNMENT_SUBJECT_H
#define ASSIGNMENT_SUBJECT_H

#include <vector>
#include "../display/observer.h"
class Observer;

class Subject {
    std::vector<Observer *> display;
public:
    void attach(Observer *o);
    void notifyAll();
    void subscript();
};

#endif //ASSIGNMENT_SUBJECT_H
