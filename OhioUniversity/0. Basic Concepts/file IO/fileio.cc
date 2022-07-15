#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main(){
	ifstream input;
	ofstream output("output.dat");
	/*ofstream test ("test.txt");
	test << "My text here!" << endl;
	test.close();*/
	
	input.open("infile.dat");
	if(input.fail()){
		cout << "Input file opening failed" << endl;
		exit(1);
	}
	/*output.open("outfile.dat");
	if(output.fail()){
		cout << "Output file opening failed" << endl;
		cout << "Lets create it instead" << endl;
		output ("outfile.txt");
		exit(1);
	}*/

	int first, second, third;
	input >> first >> second >> third;
	output << "The sum of the first 3" << endl;
	output << "numbers in infile.dat" << endl;
	output << "is " << (first+second+third) << endl;

	input.close();
	output.close();
}
