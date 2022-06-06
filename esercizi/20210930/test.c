int main() {
	volatile int i;
	for (i = 0; i < 1000000000; ++i) {
	}
	return 42;
}
