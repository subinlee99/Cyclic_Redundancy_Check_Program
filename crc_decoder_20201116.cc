#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include<cstdlib>


using namespace::std;

string toBinary(char c) {
    string result="";
    for(int i = 7; i >= 0; i--) (c & (1 << i)) ? result+="1" : result+="0";
    return result;
}

string toAscii(string binary)
{
    int N = int(binary.size());
    string ascii_num = "";
    for (int i = 0; i < N; i += 8) {
        string tmp=binary.substr(i, 8);
        int num=0; int base=1;
        for(int j = 7; j >= 0; j--){
            if (tmp[j] == '1') num += base;
            base *= 2;
        }
        ascii_num += char(num);
    }
    return ascii_num;
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

bool decode(string data, string generator)
{
    int gen_len=int(generator.length());
    string temp=data.substr(0, gen_len);
    string cur = div(temp, generator);
    
    while (1) {
        if(gen_len == data.length()) {
            cur = div(cur, generator);
            break;
        }
        if (cur.length() == generator.length()) cur = div(cur, generator);
        else cur.push_back(data[gen_len++]);
    }
    
    for(int i=0;i<cur.length();i++) if(cur[i]=='1') return false;
    return true;
}

int main(int argc, const char * argv[]) {
    //argument number not right
    if (argc != 6){
        cout << "usage: ./crc_encoder input_file output_file generator dataword_size" << endl;
        return 0;
    }
   
    //open input file
    fstream input_file;
    input_file.open(argv[1]);
    if (!input_file.is_open()){ //cannot open error
        cout << "input file open error." << endl;
        return 0;
    }
    // open output file
    ofstream output_file(argv[2], ios::binary);
    //output_file.open(argv[2]);
    if (!output_file.is_open()){ //cannot open error
        cout << "output file open error." << endl;
        exit(1);
    }
 
    ofstream result_file(argv[3]);
    if (!result_file.is_open()){ //cannot open error
        cout << "result file open error." << endl;
        exit(1);
    }
    
    // dataword_size check
    int data_size = atoi(argv[5]);
    if (data_size != 4 && data_size != 8) { //data size not 4 ot 8
        cout << "dataword size must be 4 or 8." << endl;
        return 0;
    }
    
    //define generator
    string generator = argv[4];
    
    char c;
    //get size of padding
    input_file.read(&c,1);
    int padding = c;
    //cout << " padding " << padding<<endl;
    int codeword_num=0;
    int error_num=0; //count number of error
    string input="";
    while (!input_file.eof())
    {
        input_file.read(&c,1);
        string data = toBinary(c);
        input+=data;
    }
    string output="";
    int len = (int(generator.length())+data_size-1);
    int idx=padding;
    //cout << "input size : " << input.size() << "  padding: " << idx<< endl;
    
    if(data_size==8){
        while(idx < input.size()-len){
            codeword_num++;
            string data = input.substr(idx, len);
            string out=input.substr(idx, 8);
            output+=toAscii(out);
            //cout << "decoding " << data <<endl;
            !decode(data, generator) ? error_num++ : 1;
            idx+=len;
        }
        codeword_num++;
    }
    else{
        int flag=0; string out="";
        while(idx < input.size()-2*len){//
            codeword_num++;
            string data = input.substr(idx, len);
            if(!flag) {
                out=input.substr(idx, 4);
                flag=1;
            }
            else{
                out+=input.substr(idx, 4);
                output+=toAscii(out);
                flag=0;
            }
            //cout << "decoding " << data <<endl;
            idx+=len;
            !decode(data, generator) ? error_num++ : 1;
        }
        codeword_num+=2;
        
    }
    output_file << output <<endl;
    output_file.close();
    //cout << codeword_num << " " << error_num << endl;
    result_file << codeword_num << " " << error_num << endl;
    
    input_file.close();
    output_file.close();
    result_file.close();

    return 0;
}
