#include <iostream>
#include <string>
#include <sstream>
#include <gflags/gflags.h>


#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/rng.hpp"

using namespace caffe;
using namespace std;
using std::pair;
using boost::scoped_ptr;

/* argv[1] the input file, argv[2] the val_set output file*/
/* argv[3] the train_set output file, argv[4] the size of val_set */

DEFINE_string(backend, "lmdb", "The backend for storing the result");  // get Flags_backend == lmdb

int main(int argc, char **argv){
	::google::InitGoogleLogging(argv[0]);

	#ifndef GFLAGS_GFLAGS_H_
	  namespace gflags = google;
	#endif

	if(argc < 5){
		LOG(ERROR)<< "please check the input arguments!";
		LOG(ERROR)<< argc<<"\n";
		return 1;
	}
	ifstream infile(argv[1]);
	if(!infile){
		LOG(ERROR)<< "please check the input arguments!";
		return 1;
	}
	
	int val_size=atoi(argv[4]);

	string str;
	int count = 0;
	int rows = 28;
	int cols = 28;
	unsigned char *buffer = new  unsigned char[rows*cols];//store an image
	stringstream ss;

	Datum datum;             // this data structure store the data and label
	datum.set_channels(1);    // the channels
	datum.set_height(rows);    // rows
	datum.set_width(cols);     // cols

	scoped_ptr<db::DB> db(db::GetDB(FLAGS_backend));         // new DB object
	db->Open(argv[2], db::NEW);                    // open the lmdb file to store the data
	scoped_ptr<db::Transaction> txn(db->NewTransaction());   // new Transaction object to put and commit the data

	const int kMaxKeyLength = 256;           // set the max length of the string to be 256
	char key_cstr[kMaxKeyLength];

	bool lflag=false;

	bool flag= false;
	while(getline(infile, str)){
		if(flag == false){
			flag = true;
			continue;
		}                                                    //skip the first line
		int beg = 0;
		int end = 0;
		int str_index = 0;
		//test  need to add this
		//datum.set_label(0);

		while((end = str.find_first_of(',', beg)) != string::npos){    
			//cout << end << endl;
			string dig_str = str.substr(beg, end - beg);                      //extract each pixel value of each line
			int pixes;
			ss.clear();
			ss << dig_str;
			ss >> pixes;
			// test need to delete this
			if(beg == 0){
				datum.set_label(pixes);
				beg = ++ end;
				continue;
			}                                                                                //store the label
			buffer[str_index++] = (unsigned char)pixes;
			beg = ++end;                                                            //the pixel value of each image
		}
		
		string dig_str = str.substr(beg);                                       //extract the last value
		int pixes;
		ss.clear();
		ss << dig_str;
		ss >> pixes;
		buffer[str_index++] = (unsigned char)pixes;
		datum.set_data(buffer, rows*cols);                                //store the image data
		
		
		int length = snprintf(key_cstr, kMaxKeyLength, "%08d", count);      //length>=8

		    // Put in db
		string out;
		/*if(lflag)
			LOG(ERROR) << "error1 " ;*/
		CHECK(datum.SerializeToString(&out));              // serialize to string
		/*if(lflag)
			LOG(ERROR) << "error3 " ;*/
		txn->Put(string(key_cstr, length), out);        // put it, both the key and value
		
		/*if(lflag)
			LOG(ERROR) << "error2 " ;*/

		if (++count % 1000 == 0) {       // to commit every 1000 iteration
		  // Commit db
		  txn->Commit();
		  txn.reset(db->NewTransaction());
		  LOG(ERROR) << "Processed " << count << " files.";
		}

		if((!lflag)&&(count==val_size)){                                     //val_set end
			
			// write the last batch
			if (count % 1000 != 0) {            // commit the last batch
			txn->Commit();
			LOG(ERROR) << "Processed " << count << " files.";
			}
			count=0;	
			db->Close(); 
			db->Open(argv[3], db::NEW);    
			txn.reset(db->NewTransaction()); 
			lflag=true;
		}

	}
	// write the last batch
	  if (count % 1000 != 0) {            // commit the last batch
		txn->Commit();
		LOG(ERROR) << "Processed " << count << " files.";
	  }
	  db->Close(); 
	return 0;
}
