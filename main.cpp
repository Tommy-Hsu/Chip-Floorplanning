#include<iostream>
#include<fstream>
#include "src/sa.h"
using namespace std;

int main(int argc, char* argv[]){

    ifstream    input(argv[1]);
    ofstream    output(argv[2]);

    if(!input.is_open()){
        cerr << "Error opening file" << endl;
        exit(1);
    }

    SimulationAnnealing sa;
    sa.parse_input(input);
    sa.randomize_initial_bstree();
    // sa.solve();
    sa.packing_bstree();
    sa.output(output);

    return 0;   
}