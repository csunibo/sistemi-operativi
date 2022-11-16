//https://github.com/csunibo/sistemi-operativi/blob/main/prove/scritto/scritto-2022-07-20-testo.pdf
//esercizio c1:
/*In un porto con una sola banchina utilizzabile occorre caricare cereali sulle navi. I camion portano i cereali al porto. Una sola nave alla volta può essere attraccata al molo, un solo camion alla volta scarica i cereali nella nave.
Il codice eseguito da ogni nave è:
nave[i] process:
	porto.attracca(capacità)
	porto.salpa()
	...naviga verso la destinazione
il codice di ogni camion è:
camion[j] process:
	while(true):
	quantità = carica_cereali()
	porto.scarica(quantità)
I camion fanno la spola dai depositi alla nave. La nave arriva vuota e può salpare solo se è stata completamente riempita (la somma delle quantità scaricate dai camion raggiunge la capacità indicata come parametro della funzione attracca). Se un camion può scaricare solo parzialmente il suo carico rimane in porto e aspetta di completare l'operazione con la prossima nave che attraccherà al molo.
Scrivere il monitor porto.
*/


//soluzione es c.1 del prof Davoli

/*
monitor porto(){
  boolean is_molo_free = true //per la nave
  boolean is_park_free = true //per il camion
  int dariempire=0 //per la nave, quanto è ancora da riempire?
  condition ok2attracca;
  condition ok2salpa;
  condition ok2scarica;
  condition ok2ancora;

  porto attracca(capacita){
    if (is_molo_free == false)
      ok2attracca.wait() //se il molo è occupato fermati ad aspettare
    is_molo_free = False //attracca.
    dariempire = capacita
    ok2ancora.signal()

  porto.salpa(){
    if (dariempire > 0)
      ok2salpa.wait() //posso salpare solo se ho saturato la capacità della nave
    is_molo_free = True //salpa.

  porto scarica(quantita){
    if (is_park_free == false)
      ok2scarica.wait() //se il parcheggio è occupato fermati ad aspettare
    is_park_free = False //parcheggia.
    while (quantita > dariempire){ //la quantità non può essere versata completamente
      quantita -= dariempire //metti quello che ci sta sulla nave
      dariempire = 0 //satura la nave
      ok2salpa.signal() //lascia andare la nave
      ok2ancora . wait() //il camion deve aspettare la prossima nave
    }
    dariempire -= quantita //metti tutta la capacità del camion sulla nave
    if (dariempire == 0) //ora la nave è riempita?
      ok2salpa.signal()
    is_park_free = True //il camion se ne va
    ok2scarica.signal;
}
*/


//riscritta in linguaggio C per l'esecuzione da Longo Libera

#include<stdio.h>
#include<pthread.h>
#include<stdbool.h>
#include<monitor.h>
#include<stdint.h>

#define HUMAN_READABLE_ATTESA 10000
#define RANDOM_ATTESA 2000000
#define DORMITINA_RANDOM() (usleep( HUMAN_READABLE_ATTESA + (random() % RANDOM_ATTESA) ))	

volatile bool is_molo_free = true; //per la nave
volatile bool is_park_free = true; //per il camion
volatile int dariempire = 0; //per la nave, quanto è ancora da riempire?

monitor porto;
condition ok2attracca;
condition ok2salpa;
condition ok2scarica;
condition ok2ancora;

void porto_create(void) {
	porto = monitor_create();
	ok2attracca = condition_create(porto);
	ok2salpa = condition_create(porto);
	ok2scarica = condition_create(porto);
	ok2ancora = condition_create(porto);
}

void porto_attracca(int capacita) {
	monitor_enter(porto);
	if (is_molo_free == false)
		condition_wait(ok2attracca);//se il molo è occupato fermati ad aspettare
	is_molo_free = false;//attracca.
	dariempire = capacita;
	condition_signal(ok2ancora);
	monitor_exit(porto);
}

void porto_salpa() {
	monitor_enter(porto);
	if(dariempire > 0)
		condition_wait(ok2salpa); //posso salpare solo se ho saturato la capacità della nave
	is_molo_free = true; //salpa.
	monitor_exit(porto);
}

void porto_scarica(int quantita) {
	monitor_enter(porto);
	if(is_park_free == false)
		condition_wait(ok2scarica); //se il parcheggio è occupato fermati ad aspettare
	is_park_free = false; //parcheggia
    while (quantita > dariempire){ //la quantità non può essere versata completamente
    	quantita -= dariempire; //metti quello che ci sta sulla nave
    	dariempire = 0; //satura la nave
		condition_signal(ok2salpa); //lascia andare la nave
		condition_wait(ok2ancora); //il camion deve aspettare la prossima nave
    }
    dariempire -= quantita; //metti tutta la capacità del camion sulla nave
    if (dariempire == 0) //ora la nave è riempita?
		condition_signal(ok2salpa);
	is_park_free = true; //il camion se ne va
	condition_signal(ok2scarica);
	monitor_exit(porto);
}


void *nave(void *arg) {
	int i = (uintptr_t)arg;
	while (1) {
		printf("\t\t\tnave %d ready\n", i);
		int nave_capienza = random() % 300;
		printf("\t\t\tcapienza_nave %d = %d\n", i, nave_capienza);
		DORMITINA_RANDOM();
		porto_attracca(nave_capienza);
		printf("\t\t\tattracca %d\n", i);
		DORMITINA_RANDOM();
		porto_salpa();
		printf("\t\t\tsalpa %d\n", i);
		printf("\t\t\t...%d naviga verso la destinazione...\n", i);
		DORMITINA_RANDOM();
	}
}

void *camion(void *arg) {
	int i = (uintptr_t)arg;	
	while (1) {
		printf("camion %d ready\n", i);
		DORMITINA_RANDOM();	
		int cereali_sul_camion = random() % 300;
		printf("sul_camion %d = %d\n", i, cereali_sul_camion);
		DORMITINA_RANDOM();
		porto_scarica(cereali_sul_camion);
		printf("scarica %d\n", i);
		DORMITINA_RANDOM();
	}
}


int main(int argc, char *argv[]) {

	const int NAVI = 3;   //quante navi ci sono? puoi cambiare questo numero purchè sia >= 1
	const int CAMION = 5; //quanti camion ci sono? puoi cambiare questo numero purchè sia >= 1

	int i;
	pthread_t nave_t[NAVI];
	pthread_t camion_t[CAMION];
	porto_create();
	srandom(time(NULL));
	for (i=0; i<NAVI; i++)
		pthread_create(&nave_t[i], NULL, nave, (void *)(uintptr_t) i);
	for (i=0; i<CAMION; i++)
		pthread_create(&camion_t[i], NULL, camion, (void *)(uintptr_t) i);
	for (i=0; i<NAVI; i++)
		pthread_join(nave_t[i], NULL);
	for (i=0; i<CAMION; i++)
		pthread_join(camion_t[i], NULL);
}

