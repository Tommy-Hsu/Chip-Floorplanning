#include<iostream>
#include<fstream>
#include "src/sa.h"
using namespace std;

int main(int argc, char* argv[]){

    srand(time(NULL));
    ifstream    input(argv[1]);
    ofstream    output(argv[2]);

    if(!input.is_open()){
        cerr << "Error opening file" << endl;
        exit(1);
    }

    SimulationAnnealing sa;
    sa.parse_input(input);
    sa.solve();
    sa.output(output);
    
    return 0;   
}