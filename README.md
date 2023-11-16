# Cyclic_Redundancy_Check_Program

crc_encoder:  data word is converted into a codeword using a CRC.

crc_decoder: restores codeword to dataword, and detects if there is an error.

3.1. crc_encoder
(1) To execute crc_encoder, use the following command:
```
./crc_encoder input_file output_file generator dataword_size
```
The program takes four arguments:
- `input_file`: the file to be transmitted
- `output_file`: the file encoded with CRC for transmission
- `generator`: the CRC generator
- `dataword_size`: the size of the dataword in bits

If the number of arguments is incorrect, the program will display the following message and exit:
```
usage: ./crc_encoder input_file output_file generator dataword_size
```

(2) If the input_file cannot be opened, the program will display the following message and exit:
```
input file open error.
```

(3) If the output_file cannot be opened, the program will display the following message and exit:
```
output file open error.
```

(4) The `dataword_size` is supported only for 4 or 8. If it is not 4 or 8, the program will display the following message and exit:
```
dataword size must be 4 or 8.
```

(5) The crc_encoder divides the data read from the input file into datawords of the specified size. For example, if the input file contains the letter 'A,' the ASCII code for 'A' is 65, represented in binary as 01000001. The first dataword is '0100,' and the second dataword is '0001.'

(6) Each dataword is converted to a codeword using modulo-2 division. Assuming the generator given as a program argument is '1101':
   - The first dataword is '0100.'
   - Since the generator has four digits, '000' is appended to the dataword.
   - Modulo-2 division is performed on '0100000' divided by '1101,' resulting in a remainder of '011.'
   - The codeword for dataword '0100' becomes '0100011.'
   - The remaining datawords are transformed into codewords in the same manner.

(7) The codewords transformed using CRC need to be written to the output file. However, as codewords may not be multiples of 8, padding is applied to make the file size in bytes. Padding is added at the beginning, and the number of padding bits is specified in the first byte of the output file.

3.2. crc_decoder
(1) To execute crc_decoder, use the following command:
```
./crc_decoder input_file output_file result_file generator dataword_size
```
The program takes five arguments:
- `input_file`: the file encoded with CRC
- `output_file`: the file with CRC removed, containing the original data
- `result_file`: a file displaying the total number of frames and the number of frames with errors
- `generator`: the CRC generator
- `dataword_size`: the size of the dataword in bits

If the number of arguments is incorrect, the program will display the following message and exit:
```
usage: ./crc_decoder input_file output_file result_file generator dataword_size
```

(2) If the input_file cannot be opened, the program will display the following message and exit:
```
input file open error.
```

(3) If the output_file cannot be opened, the program will display the following message and exit:
```
output file open error.
```

(4) If the result_file cannot be opened, the program will display the following message and exit:
```
result file open error.
```

(5) The `dataword_size` is supported only for 4 or 8. If it is not 4 or 8, the program will display the following message and exit:
```
dataword size must be 4 or 8.
```

(6) The crc_decoder reads the first byte of the input file to determine the size of padding.

(7) The padding in the second byte is removed.

(8) The remaining bits are divided into codewords.

(9) For each codeword, error checking is performed using modulo-2 division with the given generator. The program records the total number of codewords and the number of codewords with errors.

(10) Whether there is an error or not, the codeword is decoded to a dataword and written to the output file.

(11) The result file contains a single line with the total number of codewords and the number of codewords with errors. For example, if there are 23 total codewords and 5 have errors, the result file contains:
```
23 5
```
