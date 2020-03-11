#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>

using namespace std;

double leaveOneOutValidation(vector< vector<double> >, vector<int>, int, int);
void forward_Selection(vector< vector<double> >);
void print(vector<int>);
double euclidean_distance(vector<double>, vector<double>, vector<int>, int, int);
bool intersect(vector<int>, int );

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
    cout << "This  has " << data[0].size()-1 << " features(not including the class atribute), with " << data.size()-1 << " instances." << endl << endl;
   featureSearch(data,num);

    return 0; 
}


//normalize 
void normalize(vector<vector<double> > &data) {
    for (int i = 1; i < data.at(0).size(); i++) {
        vector<double> temp;
        double mean = 0;
        double dev = 0;
        double sum = 0;
        double count = 0;
        double mid = 0;
        for (int j = 0; j < data.size(); j++) {
            sum += data.at(j).at(i);
            count++;
        }
        mean = sum / count;
        for (int j = 0; j < data.size(); j++) {
            mid += pow((data.at(j).at(i)) - mean,2);
        }
        dev = sqrt(mid/count);

        if (dev != 0) {
            for (int y = 0; y < data.size(); y++) {
                data.at(y).at(i) = (data.at(y).at(i) - mean) / dev;
            }
        }
    }
}

bool intersect(vector<int> data, int n) {
	for (int i = 0; i < data.size(); i++) {
		if (data.at(i) == n) return true;
	}
	return false;
}

double leaveOneOutValidation(vector<vector<double>> data, vector<int>currset, int n, int choice) {
	double correct = 0;

	for (int i = 0; i < data.size(); i++) {
		vector<double> temp = data.at(i);
		vector<double> temp2;
		double nearestNeighbor = DBL_MAX;

		for (int j = 0; j < data.size(); j++) {
			//not checking same row
			if (j != i) {
				double dist = euclidean_distance(temp, data.at(j), currset, n, choice);
				if (dist < nearestNeighbor) {
					nearestNeighbor = dist;
					temp2 = data.at(j);
				}
			}
		}

		if (temp.at(0) == temp2.at(0)) {
			correct++;
		}
	}
	return correct / data.size();
}

double euclidean_distance(vector<double> x, vector<double> y, vector<int> currset, int n, int choice) {
	double dist = 0;

	for (int i = 0; i < currset.size(); i++) {
		dist += (pow(x.at(currset.at(i)) - y.at(currset.at(i)), 2));
	}
	if (choice == 1) {
		dist += (pow(x.at(n) - y.at(n), 2));
	}

	return sqrt(dist);
}

