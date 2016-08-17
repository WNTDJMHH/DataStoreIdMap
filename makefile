All: AllTestProgram AllProgram ObjClean

AllTestProgram:testDataContent testIdService testReverseIndex
AllProgram:LoadFileIndexToShm LoadFileAndContentFromShm shamread shamwrite

AllClean:
	rm testDataContent testIdService testReverseIndex LoadFileIndexToShm LoadFileAndContentFromShm shamwrite  shamread
ObjClean:
	rm -f *.o


shamwrite:shamwrite.cpp DataContent.h IdService.h ReverseIndex.h DataPickle.h
	g++ shamwrite.cpp IdService.cpp DataContent.cpp  ReverseIndex.cpp -o shamwrite
	
shamread:shamread.cpp DataContent.h IdService.h ReverseIndex.h DataPickle.h
	g++ shamread.cpp IdService.cpp DataContent.cpp  ReverseIndex.cpp -o shamread


testDataContent: testDataContent.cpp DataContent.cpp DataContent.h
	g++ -g testDataContent.cpp DataContent.cpp -o testDataContent

testIdService:testIdService.cpp IdService.h IdService.cpp
	g++ testIdService.cpp IdService.cpp -o testIdService
testReverseIndex: testReverseIndex.cpp ReverseIndex.h ReverseIndex.cpp
	g++ testReverseIndex.cpp ReverseIndex.cpp -o testReverseIndex


LoadFileIndexToShm: LoadFileIndexToShm.cpp IdService.cpp IdService.h DataContent.h DataContent.cpp comm.h ReverseIndex.cpp ReverseIndex.h DataPickle.h DataNeiborCache.h DataNeiborCache.cpp 
	g++ -g LoadFileIndexToShm.cpp IdService.cpp DataContent.cpp  ReverseIndex.cpp DataNeiborCache.cpp -o LoadFileIndexToShm

LoadFileAndContentFromShm: LoadFileAndContentFromShm.cpp IdService.cpp IdService.h DataContent.h DataContent.cpp comm.h ReverseIndex.cpp ReverseIndex.h DataPickle.h DataNeiborCache.h DataNeiborCache.cpp
	g++ -g LoadFileAndContentFromShm.cpp IdService.cpp DataContent.cpp  ReverseIndex.cpp DataNeiborCache.cpp -o LoadFileAndContentFromShm
