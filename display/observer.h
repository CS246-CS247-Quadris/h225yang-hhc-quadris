#ifndef ASSIGNMENT_OBSERVER_H
#define ASSIGNMENT_OBSERVER_H

class Subject;
class Board;

class Observer {
public:
    virtual void notify(Subject *whoFrom) = 0; // pass the Subject that called the notify method
    virtual ~Observer() = default;
};

#endif //ASSIGNMENT_OBSERVER_H
