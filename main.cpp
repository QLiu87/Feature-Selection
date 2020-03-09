#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

double leaveOneOutValidation(vector<int> data, vector<int>currset, int data, int choice);
void featureSearch(vector<int> data, int choice);
void const print(vector<int> data);
bool const intersect(vector<int> data, int n);

int main(){
    vector< vector <double> > data;
    string file;
    int num;

    cout << "Welcome to Qi Liu's Feature Selection Algorithm." << endl;
    cout << "Type in the name of the file to test: ";
    cin >> file;

    cout << endl << "Type the number of the algorithm you want to run." << endl << endl;
    cout << "\t1)  Forward Selection" << endl << "\t2)  Backward Elimination" << endl << "\t3)  Better Search Algorithm" << endl << "\t\t\t";
    cin >> num;

    double t;
    string input_data;
    ifstream myfile(file);

    if(myfile.is_open()){
        while(!myfile.eof()){
            vector<double> temp;
            getline(myfile, input_data);    
            istringstream ss(input_data);
            while(ss >> t){
                temp.push_back(t);
            }
            data.push_back(temp);
        }
        myfile.close();
    }
    else{
        cout << "Error opening file." << endl;
        return 1;
    }
    cout << "This dataset has " << data[0].size()-1 << " features(not including the class atribute), with " << data.size()-1 << " instances." << endl << endl;
   featureSearch(data,num);

    return 0; 
}

void print(vector
