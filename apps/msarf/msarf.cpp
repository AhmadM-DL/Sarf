/**
  * @file   msarf.cpp
  * @author Ahmad Mustapha
  * @brief  TODO.
  */

#include <iostream>
#include <QFile>
#include "EfficientStemmer.h"
#include <sarf.h>
#include <myprogressifc.h>

using namespace std;


/**
  * This method runs an instance of the class defined and triggers the pracket operator
  * in it to start the tool
  * @param input This is a string representing the input string to be processed
  */
void run_process(QString &input) {
    QStringList list = input.split(' ', QString::SkipEmptyParts);

    for (int i = 0; i < list.size(); i++) {
        QString *inString = &(list[i]);
        EfficientStemmer stemmer(inString);
        stemmer();
    }
}

/**
  * TODO
  * @return TODO
  */
int test() {
    QFile Ofile("output.txt");
    QFile Efile("error.txt");
    Ofile.open(QIODevice::WriteOnly);
    Efile.open(QIODevice::WriteOnly);
    MyProgressIFC *pIFC = new MyProgressIFC();

    Sarf srf;
    bool all_set = srf.start(&Ofile, &Efile, pIFC);
    Sarf::use(&srf);

    if (!all_set) {
        _error << "Can't Set up Project";
    } else {
        cout << "All Set" << endl;
    }

    char filename[100];
    cout << "please enter a file name: " << endl;
    cin >> filename;

    QFile Ifile(filename);

    if (!Ifile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cerr << "error opening file." << endl;
        return -1;
    }

    QTextStream in(&Ifile);

    while (!in.atEnd()) {
        QString line = in.readLine();
        run_process(line);
    }

    srf.exit();

    return 0;
}

int main(int /*argc*/, char ** /*argv[]*/) {

    if (!test() ) {
        cout << "The example is successful\n";
    } else {
        cout << "The example failed\n";
    }

    return 0;
}
