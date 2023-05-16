```c
monitor pairbuf{
	queue<T> buf = [];	// buffer FIFO
	queue<T> waiting_p ; // waiting put
	queue<T> waiting_g ; // waiting get
	condition ok2put, ok2get;

	procedure entry void put(T x) {
		if (waiting_g > waiting_p) {
			waiting_p;
			ok2put.wait();
			waiting_p--;
		}
	}
	procedure entry T get(void) {
	}
}
```