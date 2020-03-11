#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>
using namespace std;

double leaveOneOutValidation(vector< vector<double> >, vector<int>, int, int);
void normalize(vector<vector<double> > &);
void Forward_Selection(vector< vector<double> >);
void Backward_Selection(vector< vector<double> >);
void print(vector<int>);
double euclidean_distance(vector<double>, vector<double>, vector<int>, int, int);
bool intersect(vector<int>, int );
vector<int> remove_feature(vector<int>, int);

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
	double nn_4_feature = 0;
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
	cout << "Please wait while I normalize the data... Done!" << endl;
	vector<int> all2(data.at(0).size() - 1);
	for (size_t i = 1; i < data.at(0).size(); i++) {//fills vector with all features;
		all2.at(i - 1) = i;
	}
	nn_4_feature = leaveOneOutValidation(data, all2, data.at(0).size(), 1);
	cout << "Running nearest neighbor with all 4 features, using “leaving-one-out” evaluation, I get an accuracy of ";
	cout << setprecision(2) << fixed << nn_4_feature * 100 << "% \n";
	cout << "Beginning search\n";
	normalize(data);
	if (num == 1) {
		Forward_Selection(data);
	}
	else{
		Backward_Selection(data);
	}

    return 0; 
}


//normalize 
void normalize(vector<vector<double> > &data) {
    for (size_t i = 1; i < data.at(0).size(); i++) {
        vector<double> temp;
        double mean = 0;
        double dev = 0;
        double sum = 0;
        double count = 0;
        double mid = 0;
        for (size_t j = 0; j < data.size(); j++) {
            sum += data.at(j).at(i);
            count++;
        }
        mean = sum / count;
        for (size_t j = 0; j < data.size(); j++) {
            mid += pow((data.at(j).at(i)) - mean,2);
        }
        dev = sqrt(mid/count);

        if (dev != 0) {
            for (size_t y = 0; y < data.size(); y++) {
                data.at(y).at(i) = (data.at(y).at(i) - mean) / dev;
            }
        }
    }
}

bool intersect(vector<int> data, int n) {
	for (size_t i = 0; i < data.size(); i++) {
		if (data.at(i) == n) return true;
	}
	return false;
}

double leaveOneOutValidation(vector<vector<double>> data, vector<int>currset, int n, int choice) {
	double correct = 0;

	for (size_t i = 0; i < data.size(); i++) {
		vector<double> temp = data.at(i);
		vector<double> temp2;
		double nearestNeighbor = DBL_MAX;

		for (size_t j = 0; j < data.size(); j++) {
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

	for (size_t i = 0; i < currset.size(); i++) {
		dist += (pow(x.at(currset.at(i)) - y.at(currset.at(i)), 2));
	}
	if (choice == 1) {
		dist += (pow(x.at(n) - y.at(n), 2));
	}

	return sqrt(dist);
}

void Forward_Selection(vector< vector<double> > data) {
	vector<int> curr_set;
	vector<int> best_set;
	double max_accuracy_global = DBL_MIN;

	for (size_t i = 1; i < data.at(0).size(); i++) {
		int feature_to_add = 0;
		double max_accuracy_local = DBL_MIN;

		for (size_t j = 1; j < data.at(0).size(); j++) {
			//check not the same feature
			if (!intersect(curr_set, j)) {
				double accuracy_temp = 0;
				//find accuracy
				accuracy_temp = leaveOneOutValidation(data, curr_set, j, 1);

				cout << "\tUsing feature(s) {";
				for (size_t x = 0; x < curr_set.size(); x++) {
					cout << curr_set.at(x) << ",";
				}
				cout << j << "} accuracy is " << accuracy_temp * 100 << "%" << endl;

				//comp and update the max accuracy
				if (accuracy_temp > max_accuracy_local) {
					max_accuracy_local = accuracy_temp;
					feature_to_add = j;
				}
			}
		}

		curr_set.push_back(feature_to_add);
		if (max_accuracy_local > max_accuracy_global) {
			max_accuracy_global = max_accuracy_local;
			best_set = curr_set;
		}
		else {
			cout << "\n(Warning, Accuracy, has decreased! Continuing search in case of local maxima)\n";
		}

		cout << "Feature set {";
		for (size_t x = 0; x < curr_set.size() - 1; x++) {
			cout << curr_set.at(x) << ",";
		}
		cout << curr_set.at(curr_set.size()-1) 
			 << "} was best, accuracy is " << max_accuracy_local * 100 << "%\n\n" ;
	}
	cout << "Finished Search!! The best feature subset is {";
	for (size_t x = 0; x < best_set.size() - 1; x++) {
		cout << best_set.at(x) << ",";
	}
	cout << best_set.at(best_set.size()-1) << "}, which has an accuracy of " 
		 << max_accuracy_global * 100 << "%" << endl;
}

void Backward_Selection(vector< vector<double> > data) {
	vector<int> curr_set;
	vector<int> best_set;
	double max_accuracy_global = DBL_MIN;

	for (size_t i = 1; i < data.at(0).size(); i++) {
		curr_set.push_back(i);
	}

	for (size_t i = 1; i < data.at(0).size(); i++) {
		int feature_to_remove = 0;
		double max_accuracy_local = DBL_MIN;

		for (size_t j = 1; j < data.at(0).size(); j++) {
			//check the same feature
			if (intersect(curr_set, j)) {
				double accuracy_temp = 0;
				vector<int> feature_temp = remove_feature(curr_set, j);
				//find accuracy
				accuracy_temp = leaveOneOutValidation(data, feature_temp, j, 2);

				cout << "\tUsing feature(s) {";
				for (size_t x = 0; x < feature_temp.size() -1 ; x++) {
					cout << feature_temp.at(x) << ",";
				}
				cout << feature_temp.at(feature_temp.size()-1) 
					 << "} accuracy is " << accuracy_temp * 100 << "%" << endl;

				//comp and update the max accuracy
				if (accuracy_temp > max_accuracy_local) {
					max_accuracy_local = accuracy_temp;
					feature_to_remove = j;
				}
			}
		}

		curr_set = remove_feature(curr_set, feature_to_remove);
		if (max_accuracy_local > max_accuracy_global) {
			max_accuracy_global = max_accuracy_local;
			best_set = curr_set;
		}
		else {
			cout << "\n(Warning, Accuracy, has decreased! Continuing search in case of local maxima)\n";
		}

		cout << "Feature set {";
		for (size_t x = 0; x < curr_set.size() - 1; x++) {
			cout << curr_set.at(x) << ",";
		}
		cout << curr_set.at(curr_set.size() - 1)
			<< "} was best, accuracy is " << max_accuracy_local * 100 << "%\n\n";
	}
	cout << "Finished Search!! The best feature subset is {";
	for (size_t x = 0; x < best_set.size() - 1; x++) {
		cout << best_set.at(x) << ",";
	}
	cout << best_set.at(best_set.size() - 1) << "}, which has an accuracy of "
		<< max_accuracy_global * 100 << "%" << endl;
}

vector<int> remove_feature(vector<int> data, int n) {
	for (size_t i = 0; i < data.size(); i++) {
		if (data.at(i) == n) {
			data.erase(data.begin() + i);
			return data;
		}
	}

	return data;
}