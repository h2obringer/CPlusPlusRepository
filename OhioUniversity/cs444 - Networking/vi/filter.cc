//***************************************************
// Author: Randal Mark Obringer
// Email: ro120908@ohio.edu
// Coauthor:Professor Osterman (skeleton of program)
//
// Class: CS444
// Projects: vdump, vsend, vbridge
// File: filter.cc
//
// Description: This is the implementation of the filter
//   addresses function and helper functions
//   
//***************************************************
#include <stdio.h>
#include <string.h>
#include "filter.h"
#include <ctype.h>
#include <string>

//***********************************************
// Function: IsAlphaNum() - tests to see whether the 
//   given parameter character is a valid hexadecimal
//   character
//
// Arguments - c - the character being checked
//***********************************************
int IsAlphaNum(char c)  
{
  if (c >= 'A' && c <= 'F') return 1;
  if (c >= 'a' && c <= 'f') return 1;
  if (c >= '0' && c <= '9') return 1;
  return 0;
}

//***********************************************
// Function: is_valid_filter() - checks to see if the
//   given parameter string is suitable as a filter
//   for IP addresses. It supports only hexadecimal
//   values and wildcards "*".
//
// Arguments - checkMe - the string to check 
//***********************************************
int is_valid_filter(char* checkMe){
  int x=0; //1 or 2 before each colon
  int y=0; //as high as the string length 
  std::string findLength=checkMe;
  int len=findLength.length();
  if(len<MIN_ADDR_SIZE) return 0;

  for(y=0;y<len;++y){ 
    if(checkMe[y]==':'){ //if character is a colon
      x=0;//reset number of digits seen before colon
    }else if((IsAlphaNum(checkMe[y]))||(checkMe[y]=='*')){ //else you are a number or a-f or A-F
      ++x;
      if(x>MOST_IN_SECTION) return 0;
    }else{//else not valid
      return 0;
    }
  }
  return 1;
}

//***********************************************
// Function: is_valid_address() - checks if the given
//   parameter string is a valid IP address or not. 
//
// Arguments - checkMe - the string to be checked
//***********************************************
int is_valid_address(char* checkMe){
  int x=0; //1 or 2 before each colon
  int y=0; //as high as the string length 
  std::string findLength=checkMe;
  int len=findLength.length();
  if(len<MIN_ADDR_SIZE) return 0;

  for(y=0;y<len;++y){ 
    if(checkMe[y]==':'){ //if character is a colon
      x=0;//reset number of digits seen before colon
    }else if(IsAlphaNum(checkMe[y])){ //else you are a number or a-f or A-F
      ++x;
      if(x>MOST_IN_SECTION) return 0;
    }else{//else not valid
      return 0;
    }
  }
  return 1;
}

//***********************************************
// Function: convertToStandardForm() - expects the 
//    convertMe string to be a valid IP address. This
//    function will convert the string address
//    into 6 seperate unsigned characters to be held in 
//    our data structure for ethernet packets. 
//
// Arguments - convertMe - the string to be seperated
//    a - f - the positions of the unsigned char array 
//        being held in our ethernet packet structure
//        that will hold the split up address.
//***********************************************
void convertToStandardForm(char* convertMe,unsigned char &a,unsigned char &b,unsigned char &c,unsigned char &d,unsigned char &e,unsigned char &f){
  unsigned char results[6];
  int tempOne=0; //position of character in address
  int tempTwo=0; //position of second character in address
  int howMany=0; //1 or 2 to work with?
  int pos=0; //position on string
  int rPos=0; //position of result
  std::string findLength=convertMe; //convert to string so we can find its length
  int len=findLength.length(); //length of string to convert
  for(pos=0;pos<len;++pos){
    if(convertMe[pos]==':'){
      if(howMany==LEAST_IN_SECTION){
        results[rPos]=standardize((unsigned char)convertMe[tempOne]);
      }else if(howMany==MOST_IN_SECTION){
        results[rPos]=merge((unsigned char)convertMe[tempOne],(unsigned char)convertMe[tempTwo]); 
      }
      ++rPos;
      howMany=0;
    }else{
      if(howMany==LEAST_IN_SECTION-1){
        tempOne=pos;
        ++howMany;
      }else if(howMany==MOST_IN_SECTION-1){
        tempTwo=pos;
        ++howMany;
      }
      if(pos==len-1){ //take care of the last 1 or 2 in the address
        if(howMany==LEAST_IN_SECTION){
          results[rPos]=standardize((unsigned char)convertMe[tempOne]);
        }else if(howMany==MOST_IN_SECTION){
          results[rPos]=merge((unsigned char)convertMe[tempOne],(unsigned char)convertMe[tempTwo]);
        }
      }
    }
  }

  a=results[0];
  b=results[1];
  c=results[2];
  d=results[3];
  e=results[4];
  f=results[5];
}

//***********************************************
// Function: merge() - in the case that we are given
//   part of a filter with 2 characters, this function
//   will merge those characters into 1 character. The
//   merged character will be an ascii number representative
//   of the number symbolized by the 2 characters given in
//   the parameters. This allows for comparison with the 
//   unsigned char hw_addr parameter in our main filtering
//   function on an ascii code basis.
//
// Arguments - one - the first character to be merged
//	two - the second character to be merged
//***********************************************
unsigned char merge(unsigned char one,unsigned char two){
  unsigned char result;
  unsigned char first=one;
  unsigned char second=two;
  if((first>=UPPER_A)&&(first<=UPPER_F)){ //convert lower case to number
    first=(first-HEX_U)*HEX_BASE;
  }else if((first>=LOWER_A)&&(first<=LOWER_F)){ //convert upper case to number
    first=(first-HEX_L)*HEX_BASE;
  }else if((first>=ZERO)&&(first<=NINE)){ //convert ascii number to decimal number
    first=(first-ZERO)*HEX_BASE;
  }

  if((second>=UPPER_A)&&(second<=UPPER_F)){ //convert lower case to number
    second=second-HEX_U;
  }else if((second>=LOWER_A)&&(second<=LOWER_F)){ //convert upper case to number
    second=second-HEX_L;
  }else if((second>=ZERO)&&(second<=NINE)){ //convert ascii number to decimal number
    second=second-ZERO;
  }

  result=first+second;
  return result;

}

//***********************************************
// Function: standardize() - if in the case that we are
//   given part of a filter with only one character,
//   we need to translate it to ascii so that we may
//   compare it to the unsigned char hw_addr parameter 
//   in our main filtering function on an ascii code basis.
//
// Arguments - one - the character to translate into ascii
//***********************************************
unsigned char standardize(unsigned char one){
  if((one>=UPPER_A)&&(one<=UPPER_F)){ //if A-F
    one-=HEX_U;
  }else if((one>=LOWER_A)&&(one<=LOWER_F)){ //convert upper case to number
    one=(one-HEX_L);
  }else if((one>=ZERO)&&(one<=NINE)){ //convert ascii number to decimal number
    one=(one-ZERO);
  }
  return one;
}

//***********************************************
// Function: filter_matches() -
//	filter_matches will return nonzero (true) if the 'filter' argument
//	matches the 'hw_addr' argument, and zero (false) otherwise.
//
// Arguments - filter - char* which contains the string to match.
//	hw_addr - Hardware address being tested against the filter
//
//***********************************************
int filter_matches(char* filter,const unsigned char hw_addr[6]){

  unsigned char compareMe[6];
  convertToStandardForm(filter,compareMe[0],compareMe[1],compareMe[2],compareMe[3],compareMe[4],compareMe[5]);
  if((compareMe[0]==hw_addr[0])||(compareMe[0]==STAR)){
    if((compareMe[1]==hw_addr[1])||(compareMe[1]==STAR)){
      if((compareMe[2]==hw_addr[2])||(compareMe[2]==STAR)){
        if((compareMe[3]==hw_addr[3])||(compareMe[3]==STAR)){
          if((compareMe[4]==hw_addr[4])||(compareMe[4]==STAR)){
            if((compareMe[5]==hw_addr[5])||(compareMe[5]==STAR)){
              return true;
            }
          }
        }
      }
    }
  }

  return false;
  
}

