#include <uC++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <iostream>

using namespace std;

_Monitor BoundedBuffer {
    int front, back, count;
    int elements[20];
    public:
    BoundedBuffer() : front(0), back(0), count(0) {}
    _Nomutex int query() { return count; }
    void insert(int elem) {
    if (count == 20) _Accept( remove );
        elements[back] = elem;
        back = (back+1)% 20;
        count += 1;
    }
    int remove() {
        if (count == 0) _Accept( insert );
        int elem = elements[front];
        front = (front+1)%20;
        count -= 1;
        return elem;
    }
};


void uMain::main(){
    cout<<"Prueba"<<endl;
}