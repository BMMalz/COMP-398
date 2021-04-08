This project has two components.

**hasher**
The hasher program will take an input file containing up to one million characters, add a randomly generated six-digit number to the string, and then count from zero to 999,999. Each time it counts, it will add the counted number to the string, and then hash the string using SHA-256. (The SHA-256 code was provided by my professor.) The program will then count how many zeros are at the front of the hashed string's binary representation. If the number of zeros is equal to or greater than a user-inputted amount, the program will terminate and output the string that produced the correct hash to a file. If the number of zeros is invalid, it will continue counting to 999,999. Each increase in the count will produce a new hash. If after one million attempts this still does not produce the desired hash, the program will generate a new random number and count to 999,999 again. This process repeats until the program generates a hash with the desired number of zeros at its beginning.
The program takes 3 command line arguments:
The first argument is the input text file.
The second argument is the output text file. The program will create this if it doesn't exist or overwrite it if it does.
The third argument is the number of zeroes to search for in the hash.
For example, if the user wants to find a string that generates a hash with 25 zeros at the beginning and stores it in a file called "zeroHash.txt" they would input the following:
./hasher input.txt zeroHash.txt 25

**main**
The main program will more efficiently search for a desirable hash by starting a user-specified number of processes that each run the hasher program. Once one of these hasher processes finds an acceptable hash, the program will terminate the other hasher processes and display a message informing the user of the filename containing the string that produced the desirable hash. Main takes 3 command line arguments:
The first is the input text file.
The second is the number of zeroes to search for.
The third is the number of hasher processes to simultaneously run.
For example, if the user wanted 10 hashers to run at once to find a string that produces a 25-zero hash, they would input:
./main input.txt 25 10
