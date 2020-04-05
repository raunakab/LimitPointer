SOURCE="../src/reference_counter.cpp"

if [ -f "rc" ]; then
	rm rc
	clear
fi

g++ -std=c++17 test.cpp $SOURCE -o rc

if [ -f "rc" ]; then
	clear
	./rc
	rm rc
fi
