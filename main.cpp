#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <cfloat>
using namespace std;
using namespace std::chrono;
float leaveOneOutValidation(vector< vector<float> >, vector<int>, int, int);
void normalize(vector<vector<float> > &);
void Forward_Selection(vector< vector<float> >);
void Backward_Selection(vector< vector<float> >);
void print(vector<int>);
float euclidean_distance(vector<float>, vector<float>, vector<int>, int, int);
bool intersect(vector<int>, int );
vector<int> remove_feature(vector<int>, int);
///////////////////
//float find_mean(vector<float> x);
//float find_standard_dev(vector<float> x);

int main(){
    vector< vector <float> > data;
    string file = "cs_170_small47.txt";
    int num;

    cout << "Welcome to Qi Liu's Feature Selection Algorithm." << endl;
    cout << "Type in the name of the file to test: ";
    //cin >> file;

    float t;
	float nn_4_feature = 0;
    string input_data;
    ifstream myfile(file);
	cout << "File name is: " << file << endl;
    if(myfile.is_open()){
        while(!myfile.eof()){
            vector<float> temp;
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

	normalize(data);

	cout << endl << "Type the number of the algorithm you want to run." << endl << endl;
	cout << "\t1)  Forward Selection" << endl << "\t2)  Backward Elimination" << endl << "\t3)  Better Search Algorithm" << endl << "\t\t\t";
	cin >> num;
    cout << "This has " << data[0].size()-1 << " features(not including the class atribute), with " << data.size()-1 << " instances." << endl << endl;
	cout << "Please wait while I normalize the data... Done!" << endl;

	/*vector<int> all_feature(data.at(0).size() - 1);
	for (size_t i = 1; i < data.at(0).size(); i++) {//fills vector with all features;
		all_feature.at(i - 1) = i;
	}
	nn_4_feature = leaveOneOutValidation(data, all_feature, 9, 1);

	cout << "Running nearest neighbor with all 10 features, using \"leaving-one-out\" evaluation, I get an accuracy of ";
	cout << setprecision(2) << fixed << nn_4_feature * 100 << "% \n";
	*/cout << "Beginning search\n";
	
	auto start = high_resolution_clock::now();
	if (num == 1) {
		Forward_Selection(data);
	}
	else{
		cout << "In backward\n";
		Backward_Selection(data);
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Time taken by algorithm: " << duration.count() << " milliseconds" << endl;

    return 0; 
}

//normalize 
void normalize(vector<vector<float> > &data) {
	float mean = 0;
	float dev = 0;
	float sum = 0;
	float count = 0;
	float mid = 0;

    for (size_t i = 1; i < data.at(0).size(); i++) {
        vector<float> temp;
        //there's an empty line at the end of file, thus size()-1
        for (size_t j = 0; j < data.size()-1; j++) {

            sum += data.at(j).at(i);
            count++;
        }
        mean = sum / count;
        for (size_t j = 0; j < data.size()-1; j++) {
            mid += pow((data.at(j).at(i)) - mean,2);
        }
        dev = sqrt(mid/count);

        if (dev != 0) {
            for (size_t y = 0; y < data.size()-1; y++) {
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

float leaveOneOutValidation(vector<vector<float>> data, vector<int>currset, int n, int choice) {
	float correct = 0;

	for (size_t i = 0; i < data.size()-1; i++) {
		vector<float> temp = data.at(i);
		vector<float> temp2;
		float nearestNeighbor = DBL_MAX;

		for (size_t j = 0; j < data.size()-1; j++) {
			//not checking same row
			if (j != i) {
				float dist = euclidean_distance(temp, data.at(j), currset, n, choice);
				if (dist < nearestNeighbor) {
					nearestNeighbor = dist;
					temp2 = data.at(j);
				}
			}
		} 
		if(temp2.at(0) == temp.at(0)){
			correct++;
		}
	}
	return correct / static_cast<float>(data.size()-1);
}

float euclidean_distance(vector<float> x, vector<float> y, vector<int> currset, int n, int choice) {
	float dist = 0;
	for (size_t i = 0; i < currset.size(); i++) {
		//dist += (pow(x.at(currset.at(i)) - y.at(currset.at(i)), 2));
		dist += (pow(x.at(currset.at(i)) - y.at(currset.at(i)), 2));
	}
	
	if (choice == 1) {
		dist += (pow(x.at(n) - y.at(n), 2));
	}
	
	return sqrt(dist);
}

void Forward_Selection(vector< vector<float> > data) {
	vector<int> curr_set;
	vector<int> best_set;
	float max_accuracy_global = 0;

	for (size_t i = 1; i < data.at(0).size(); i++) {
		int feature_to_add = 0;
		float max_accuracy_local = 0;

		for (size_t j = 1; j < data.at(0).size(); j++) {
			//check not the same feature
			if (!intersect(curr_set, j)) {
				float accuracy_temp = 0;
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

void Backward_Selection(vector< vector<float> > data) {
	vector<int> curr_set;
	vector<int> best_set;
	float max_accuracy_global = 0;

	for (size_t i = 1; i < data.at(0).size(); i++) {
		curr_set.push_back(i);
	}

	for (size_t i = 1; i < data.at(0).size()-1; i++) {
		int feature_to_remove = 0;
		float max_accuracy_local = 0;

		for (size_t j = 1; j < data.at(0).size(); j++) {
			//check the same feature
			if (intersect(curr_set, j)) {
				float accuracy_temp = 0;
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