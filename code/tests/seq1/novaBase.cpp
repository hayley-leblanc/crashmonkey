#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <dirent.h>
#include <cstring>
#include <errno.h>
// #include <attr/xattr.h>

#include "../BaseTestCase.h"
#include "../../user_tools/api/workload.h"
#include "../../user_tools/api/actions.h"
#include "novaCheckpoint.h"

using fs_testing::tests::DataTestResult;
using fs_testing::user_tools::api::WriteData;
using fs_testing::user_tools::api::WriteDataMmap;
using fs_testing::user_tools::api::Checkpoint;
using std::string;

int main() {
	
	// crashmonkey/ace must define these elsewhere but I don't think they're 
	// particularly important for us
	string mnt_dir_ = "/mnt/pmem";
	int checkpoint = -1;
	
	// private
	
	// setup 
	
	// run 
	
	// check_test
	
	return 0;
}
