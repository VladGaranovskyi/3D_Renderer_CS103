#ifndef USERQUERY_H
#define USERQUERY_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string chooseOBJ() {
    int choice;

    cout << "Choose an option:" << endl;
    cout << "1. Use existing object" << endl;
    cout << "2. Upload new .obj file" << endl;
    cout << "Enter choice: ";
    cin >> choice;

    if (choice == 1) {
        int objChoice;

        cout << "Choose an object:" << endl;
        cout << "1. cube.obj" << endl;
        cout << "2. triangle.obj" << endl;
        cout << "3. square.obj" << endl;
        cout << "4. rectangle.obj" << endl;
        cout << "Enter choice: ";
        cin >> objChoice;

        if (objChoice == 1) return "Objects/cube.obj";
        if (objChoice == 2) return "Objects/triangle.obj";
        if (objChoice == 3) return "Objects/square.obj";
        if (objChoice == 4) return "Objects/rectangle.obj";

        cout << "Invalid choice" << endl;
        return "";
    }

    if (choice == 2) {
        string sourcePath;
        string fileName;
        string destinationPath;

        cout << "Enter full path to .obj file: ";
        cin >> sourcePath;

        int i = sourcePath.length() - 1;

        while (i >= 0 && sourcePath[i] != '/' && sourcePath[i] != '\\') {
            i--;
        }

        fileName = sourcePath.substr(i + 1);
        destinationPath = "Objects/" + fileName;

        ifstream sourceFile(sourcePath);
        ofstream destFile(destinationPath);

        if (!sourceFile.is_open()) {
            cout << "Could not open file" << endl;
            return "";
        }

        string line;
        while (getline(sourceFile, line)) {
            destFile << line << endl;
        }

        sourceFile.close();
        destFile.close();

        cout << "File copied to Objects folder" << endl;

        return destinationPath;
    }

    cout << "Invalid option" << endl;
    return "";
}

#endif
