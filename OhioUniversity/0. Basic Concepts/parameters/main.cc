#include <iostream>
#include <string>

void get_input(int& f_variable); //pass by reference

int main(){
	int fahrenheit;
	get_input(fahrenheit);
	std::cout << fahrenheit << std::endl;
}

void get_input(int& f_variable){
	using namespace std;
	cout << "Enter number: " << endl;
	cin >> f_variable;
}
