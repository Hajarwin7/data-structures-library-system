all:
	g++ -std=c++11 -o library_system \
		main.cpp \
		libraries/BookManager.cpp \
		libraries/CommentSystem.cpp \
		libraries/RequestQueue.cpp \
		libraries/UserCollection.cpp \
		libraries/CircularList.cpp \
		libraries/UserStack.cpp \
		libraries/RatingHashTable.cpp \
		-Iheaders

clean:
	rm -f library_system