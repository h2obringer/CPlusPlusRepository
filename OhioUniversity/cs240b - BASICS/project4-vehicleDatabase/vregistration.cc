#include "vregistration.h"
using namespace std;

vregistration::vregistration(){  //constructor
  owner=license_plate=make=model=" ";
  year=0;
}

istream& operator>>(istream& in,vregistration& temp){  //overloaded input operator makes for 
  temp.input_data(in);	//ease of input
  return in;
}

istream& vregistration::input_data(istream& ins){ //Needed for overloaded operator and inputing
if(ins.peek()=='\n'){		//data for vehicle registrations
  ins.ignore();
  }
if(ins==cin){cout << "Owner of Vehicle:\n";}
getline(ins,owner);
if(ins==cin){cout << "License Plate Number:\n";}
getline(ins,license_plate);
if(ins==cin){cout << "Make:\n";}
getline(ins,make);
if(ins==cin){cout << "Model:\n";}
getline(ins,model);
if(ins==cin){cout << "Year:\n";}
ins >> year;
return ins;
}

ostream& operator<<(ostream& out,vregistration& temp){  //overloaded output operator
  temp.output_data(out);
  return out;
}

ostream& vregistration::output_data(ostream& outs){  //Needed for overloaded operator and
if(outs==cout){outs << "Owner of Vehicle:\n";}  //outputing data for vehicle registrations
outs << owner << endl;
if(outs==cout){outs << "License Plate Number:\n";}
outs << license_plate << endl;
if(outs==cout){outs << "Make:\n";}
outs << make << endl;
if(outs==cout){outs << "Model:\n";}
outs << model << endl;
if(outs==cout){outs << "Year:\n";}
outs << year << endl;
if(outs==cout){outs << endl;}
return outs;
}


