#include<iostream>
#include <fstream>
#include <string>
#include<cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

using namespace::std;

string toBinary(char c) {
    string result="";
    for(int i = 7; i >= 0; i--) (c & (1 << i)) ? result+="1" : result+="0";
    return result;
}

string div(string data, string generator)
{
    int gen_len = int(generator.length());
    string tmp;
    for (int i = 0; i < gen_len; i++) tmp.push_back(data[i]);
  
    while(1) {
        string t;
        string zeros(gen_len, '0');
        tmp[0] == '1' ? t=generator : t=zeros;
        
        string xor_cal = "";
        for (int i = 1; i < tmp.length(); i++)!(t[i] ^ tmp[i]) ? xor_cal+="0" : xor_cal+="1";
        tmp = xor_cal;
        
        if(gen_len != int(data.length())) tmp+=data[gen_len++];
        else break;
        
    }
    return tmp;
}

string encoder(string data, string generator, int data_size)
{
    int gen_len = int(generator.size());
    int code_len =gen_len+data_size-1;
    string codeword="";
    for(int s=0;s<(3-data_size/4);s++){
        string temp="";
        for(int i=0;i<data_size;i++) temp+=data[i+s*data_size];
        for(int i=0;i<gen_len-1;i++) temp +="0";
        string remainder = div(temp, generator);
        //update codeword remainder
        for(int i=data_size;i<code_len;i++) temp[i]=remainder[i-data_size];
        codeword+=temp;
    }
    return codeword;
}

int main(int argc, char *argv[]) {

    //argument number not right
    if (argc != 5){
        cout << "usage: ./crc_encoder input_file output_file generator dataword_size" << endl;
        return 0;
    }
   
    //open input file
    fstream input_file;
    input_file.open(argv[1]);
    int file_size;
    
    if (!input_file.is_open()){ //cannot open error
        cout << "input file open error." << endl;
        return 0;
    }
    
    else{ //get file size
        input_file.seekg (0, ios::end);
        file_size = int(input_file.tellg());
        input_file.seekg (0, ios::beg);
    }
    //cout <<  "file size" << file_size <<endl;
    

    // open output file
    ofstream output_file(argv[2], ios::binary);
    if (!output_file.is_open()){ //cannot open error
        cout << "output file open error." << endl;
        exit(1);
    }
  
    // dataword_size check
    int data_size = atoi(argv[4]);
    if (data_size != 4 && data_size != 8) { //data size not 4 ot 8
        cout << "dataword size must be 4 or 8." << endl;
        return 0;
    }
    
    //define generator
    string generator = argv[3];

    string codeword="";
    
    while (!input_file.eof())
    {
        char c;
        input_file.read(&c,1);
        string data = toBinary(c);
        codeword+= encoder(data,generator, data_size);
       
    }
    
    char byteValue = 0;
    int count = 0;
    
    //zero padding
    
    int len_all = (3-data_size/4)*file_size*(data_size-1+int(generator.length()));
    char pad_len = (8-(len_all%8));
    string zero = toBinary(pad_len);
    for(int i=0;i< 8-(len_all%8) ;i++) zero+="0";
    
    //whole codeword
    codeword=zero+codeword;

    for (int i = 0; i < len_all + 7 + pad_len; i++) {
        byteValue = byteValue << 1;
        byteValue |= codeword[i] - '0';
        count++;
        if (count == 8) {
            count = 0;
            output_file.write(&byteValue, 1);
            byteValue = 0;
           }
       }

    //close file
    input_file.close();
    output_file.close();

    return 0;
}
        
    

