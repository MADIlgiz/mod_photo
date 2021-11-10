#include <iostream>
#include "Programm.hpp"

using namespace std;
using namespace retk;
using namespace slp;


int main(int argc, char* argv[]) {
    Program app;

  setlinebuf(stdout);
  try {
    if (app.setup(argc,argv)) return 0;
    return app.run();
  }
  catch (exception& ex) {
    cerr << "Error: " << ex.what() << endl;
    return -1;
  }
}
