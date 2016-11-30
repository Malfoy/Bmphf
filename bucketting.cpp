#include <unistd.h>
#include <cstdlib> // stdtoul
#include <random>
#include <err.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <stdint.h>



//TODO ADD MPHF to BENCH


using namespace std;
using namespace chrono;



uint64_t xorshift1(uint64_t x){
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    return x * UINT64_C(2685821657736338717);
}
 vector<uint>offset;


void constructBMPHF (string inputFile,uint numberBuckets=100){
    cout << "Constructing a Bucketed Minmimal Perfect Hash Function from "+inputFile<<endl;
    FILE *outFiles[numberBuckets];
	for (uint i = 0; i < numberBuckets; i++){
		outFiles[i] = fopen(("bucket"+to_string(i)).c_str(), "w");
	}
	FILE * in=fopen(inputFile.c_str(),"r");
	fseek(in, 0, SEEK_END);
	uint fileSize = ftell(in);
	uint64_t key,hash;
	rewind(in);
	offset=vector<uint>(numberBuckets,0);
	for(uint i(0);i<fileSize/4;++i){
		uint c=fread(&key, 4, 1, in);
		if(c!=4){
			cout<<"fail"<<endl;
			exit(1);
		}
		//~ getc(in);//the eof lol
		hash=xorshift1(key)%numberBuckets;
		fwrite(&key, sizeof(uint64_t), 1, outFiles[hash]);
		offset[hash]++;
	}

	for(uint i(0); i<numberBuckets;++i){
		//Construct mphf on file on bucket i
	}
	uint total(0),temp;
	for(uint i(0); i<numberBuckets;++i){
		temp=offset[i];
		offset[i]=total;
		total+=temp;
	}
}



void queryBMPHF (string inputFile,uint numberBuckets=100){
    cout << "querying a Bucketed Minmimal Perfect Hash Function from "+inputFile<<endl;
    FILE *outFiles[numberBuckets];
	for (uint i = 0; i < numberBuckets; i++){
		outFiles[i] = fopen(("bucket"+to_string(i)).c_str(), "r");
	}
	FILE * in=fopen(inputFile.c_str(),"r");
	fseek(in, 0, SEEK_END);
	uint fileSize = ftell(in);
	uint64_t key,hash;
	rewind(in);
	for(uint i(0);i<fileSize/4;++i){
		uint c=fread(&key, 4, 1, in);
		if(c!=4){
			cout<<"fail"<<endl;
			exit(1);
		}
		//~ getc(in);//the eof lol
		hash=xorshift1(key)%numberBuckets;
		fwrite(&key, sizeof(uint64_t), 1, outFiles[hash]);
	}

	for(uint i(0); i<numberBuckets;++i){
		//query mphf of  bucket i
		//return offset[i]+the result
	}
}



void createRandomInputFile(uint numberKey,const string outFile){
	uint step(9223372036854775807/numberKey);
	uint key(0);
	FILE * out =fopen(outFile.c_str(),"w");
	for(uint i(0); i<numberKey;++i){
		fwrite(&key, sizeof(uint64_t), 1, out);
		key+=step;
	}
}



int main (){
	uint numberBuckets(100);
	createRandomInputFile(1000000,"testFile");
	auto beginConstruct = system_clock::now();
	constructBMPHF("testFile",numberBuckets);
	auto beginQuery = system_clock::now();
	queryBMPHF("testFile",numberBuckets);
	auto end = system_clock::now();
	auto construct=beginQuery-beginConstruct;
	auto query=end-beginQuery;
	cout<<"Construction took "<<duration_cast<seconds>(construct).count()<<" seconds"<<endl;
	cout<<"Query took "<<duration_cast<seconds>(construct).count()<<" seconds"<<endl;
	return 0;
}
